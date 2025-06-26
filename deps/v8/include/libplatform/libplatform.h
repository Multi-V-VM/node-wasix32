#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_LIBPLATFORM_LIBPLATFORM_H_
#define V8_LIBPLATFORM_LIBPLATFORM_H_

#ifdef __wasi__

#include <memory>

#include "v8-platform.h"
#include "v8-tracing.h"

namespace v8 {
namespace platform {
#if defined(__wasi__) && !defined(std)
// ensure std namespace resolution works properly in WASI

#endif
// Simple WASI platform implementation
class DefaultPlatform : public Platform {
 public:
  DefaultPlatform() = default;
  ~DefaultPlatform() override = default;

  PageAllocator* GetPageAllocator() override { return nullptr; }
  int NumberOfWorkerThreads() override { return 1; }
  std::shared_ptr<TaskRunner> GetForegroundTaskRunner(
      Isolate* isolate) override {
    return nullptr;
  }
  void CallOnWorkerThread(std::unique_ptr<Task> task) override {}
  void CallDelayedOnWorkerThread(std::unique_ptr<Task> task,
                                 double delay_in_seconds) override {}
  void CallOnForegroundThread(Isolate* isolate,
                              std::unique_ptr<Task> task) override {}
  void CallDelayedOnForegroundThread(Isolate* isolate,
                                     std::unique_ptr<Task> task,
                                     double delay_in_seconds) override {}
  void CallIdleOnForegroundThread(Isolate* isolate,
                                  std::unique_ptr<IdleTask> task) override {}
  bool IdleTasksEnabled(Isolate* isolate) override { return false; }
  std::unique_ptr<JobHandle> PostJob(
      TaskPriority priority, std::unique_ptr<JobTask> job_task) override {
    return nullptr;
  }
  double MonotonicallyIncreasingTime() override { return 0.0; }
  double CurrentClockTimeMillis() override { return 0.0; }
  TracingController* GetTracingController() override {
    return &tracing_controller_;
  }
  StackTracePrinter GetStackTracePrinter() override {
    return []() {};
  }

 private:
  TracingController tracing_controller_;
};

}  // namespace platform

// Factory functions
std::unique_ptr<Platform> NewDefaultPlatform(
    int thread_pool_size = 0,
    IdleTaskSupport idle_task_support = IdleTaskSupport::kDisabled,
    InProcessStackDumping in_process_stack_dumping =
        InProcessStackDumping::kDisabled,
    std::unique_ptr<TracingController> tracing_controller = {});

enum class IdleTaskSupport { kDisabled, kEnabled };
enum class InProcessStackDumping { kDisabled, kEnabled };

}  // namespace v8

#else
// Include original header for non-WASI
#include "libplatform-original.h"
#endif  // __wasi__

#endif  // V8_LIBPLATFORM_LIBPLATFORM_H_
