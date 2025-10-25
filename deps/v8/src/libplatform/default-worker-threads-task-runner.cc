#include "src/libplatform/default-worker-threads-task-runner.h"
#ifdef __wasi__
#include "../../include/libplatform/libplatform-wasi-fix.h"
// WASI stub: no background threads. Methods forward to DelayedTaskQueue and
// run tasks synchronously when queried by consumers.
namespace v8 { namespace platform {

DefaultWorkerThreadsTaskRunner::DefaultWorkerThreadsTaskRunner(
    uint32_t /*thread_pool_size*/, TimeFunction time_function,
    base::Thread::Priority /*priority*/)
    : queue_(time_function), time_function_(time_function) {}

DefaultWorkerThreadsTaskRunner::~DefaultWorkerThreadsTaskRunner() = default;

double DefaultWorkerThreadsTaskRunner::MonotonicallyIncreasingTime() {
  return time_function_();
}

void DefaultWorkerThreadsTaskRunner::Terminate() {
  ::v8::base::MutexGuard guard(&lock_);
  terminated_ = true;
  queue_.Terminate();
}

void DefaultWorkerThreadsTaskRunner::PostTaskImpl(
    ::std::unique_ptr<Task> task, const SourceLocation&) {
  ::v8::base::MutexGuard guard(&lock_);
  if (!terminated_) queue_.Append(::std::move(task));
}

void DefaultWorkerThreadsTaskRunner::PostDelayedTaskImpl(
    ::std::unique_ptr<Task> task, double delay_in_seconds,
    const SourceLocation&) {
  ::v8::base::MutexGuard guard(&lock_);
  if (!terminated_) queue_.AppendDelayed(::std::move(task), delay_in_seconds);
}

void DefaultWorkerThreadsTaskRunner::PostIdleTaskImpl(
    ::std::unique_ptr<IdleTask> /*task*/, const SourceLocation&) {
  // No idle worker tasks on WASI.
}

bool DefaultWorkerThreadsTaskRunner::IdleTasksEnabled() { return false; }

}}  // namespace v8::platform
#else
// Non-WASI implementation with worker threads.
#include "src/base/platform/time.h"
#include "src/libplatform/delayed-task-queue.h"

namespace v8 { namespace platform {

DefaultWorkerThreadsTaskRunner::DefaultWorkerThreadsTaskRunner(
    uint32_t thread_pool_size, TimeFunction time_function,
    base::Thread::Priority priority)
    : queue_(time_function), time_function_(time_function) {
  for (uint32_t i = 0; i < thread_pool_size; ++i) {
    thread_pool_.push_back(::std::make_unique<WorkerThread>(this, priority));
  }
}

DefaultWorkerThreadsTaskRunner::~DefaultWorkerThreadsTaskRunner() = default;

double DefaultWorkerThreadsTaskRunner::MonotonicallyIncreasingTime() {
  return time_function_();
}

void DefaultWorkerThreadsTaskRunner::Terminate() {
  {
    ::v8::base::MutexGuard guard(&lock_);
    terminated_ = true;
    queue_.Terminate();
    idle_threads_.clear();
  }
  thread_pool_.clear();
}

void DefaultWorkerThreadsTaskRunner::PostTaskImpl(
    ::std::unique_ptr<Task> task, const SourceLocation&) {
  ::v8::base::MutexGuard guard(&lock_);
  if (terminated_) return;
  queue_.Append(::std::move(task));
  if (!idle_threads_.empty()) { idle_threads_.back()->Notify(); idle_threads_.pop_back(); }
}

void DefaultWorkerThreadsTaskRunner::PostDelayedTaskImpl(
    ::std::unique_ptr<Task> task, double delay_in_seconds, const SourceLocation&) {
  ::v8::base::MutexGuard guard(&lock_);
  if (terminated_) return;
  queue_.AppendDelayed(::std::move(task), delay_in_seconds);
  if (!idle_threads_.empty()) { idle_threads_.back()->Notify(); idle_threads_.pop_back(); }
}

void DefaultWorkerThreadsTaskRunner::PostIdleTaskImpl(
    std::unique_ptr<IdleTask> /*task*/, const SourceLocation&) { UNREACHABLE(); }

bool DefaultWorkerThreadsTaskRunner::IdleTasksEnabled() { return false; }

DefaultWorkerThreadsTaskRunner::WorkerThread::WorkerThread(
    DefaultWorkerThreadsTaskRunner* runner, base::Thread::Priority priority)
    : Thread(Options("V8 DefaultWorkerThreadsTaskRunner WorkerThread", priority)),
      runner_(runner) { CHECK(Start()); }

DefaultWorkerThreadsTaskRunner::WorkerThread::~WorkerThread() { condition_var_.NotifyAll(); Join(); }

void DefaultWorkerThreadsTaskRunner::WorkerThread::Run() {
  ::v8::base::MutexGuard guard(&runner_->lock_);
  while (true) {
    DelayedTaskQueue::MaybeNextTask next_task = runner_->queue_.TryGetNext();
    switch (next_task.state) {
      case DelayedTaskQueue::MaybeNextTask::kTask:
        runner_->lock_.Unlock(); next_task.task->Run(); runner_->lock_.Lock(); continue;
      case DelayedTaskQueue::MaybeNextTask::kTerminated: return;
      case DelayedTaskQueue::MaybeNextTask::kWaitIndefinite:
        runner_->idle_threads_.push_back(this); condition_var_.Wait(&runner_->lock_); continue;
      case DelayedTaskQueue::MaybeNextTask::kWaitDelayed:
        runner_->idle_threads_.push_back(this);
        { bool notified = condition_var_.WaitFor(&runner_->lock_, next_task.wait_time); USE(notified); }
        continue;
    }
  }
}

void DefaultWorkerThreadsTaskRunner::WorkerThread::Notify() { condition_var_.NotifyAll(); }

}}  // namespace v8::platform
#endif
