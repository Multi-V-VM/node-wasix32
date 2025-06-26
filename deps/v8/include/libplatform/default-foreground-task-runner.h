#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_LIBPLATFORM_DEFAULT_FOREGROUND_TASK_RUNNER_H_
#define V8_LIBPLATFORM_DEFAULT_FOREGROUND_TASK_RUNNER_H_

#include <memory>
#include <queue>

#include "include/libplatform/libplatform-export.h"
#include "include/v8-platform.h"
#include "src/base/platform/condition-variable.h"
#include "src/base/platform/mutex.h"

#ifdef __wasi__

namespace v8 {
namespace platform {

class V8_PLATFORM_EXPORT DefaultForegroundTaskRunner
    : public v8::TaskRunner,
      public NON_EXPORTED_BASE(std::enable_shared_from_this<
                               DefaultForegroundTaskRunner>) {
 public:
  DefaultForegroundTaskRunner();
  ~DefaultForegroundTaskRunner();

  void PostTask(std::unique_ptr<Task> task) override;
  void PostDelayedTask(std::unique_ptr<Task> task,
                       double delay_in_seconds) override;
  void PostIdleTask(std::unique_ptr<IdleTask> task) override;
  bool IdleTasksEnabled() override;
  bool NonNestableTasksEnabled() const override;
  bool NonNestableDelayedTasksEnabled() const override;

  std::unique_ptr<Task> PopTaskFromQueue(MessageLoopBehavior wait_for_work);

 private:
  base::Mutex queue_mutex_;
  std::queue<std::unique_ptr<Task>> task_queue_;
  base::ConditionVariable event_loop_control_;
};

} // namespace platform
} // namespace v8

#endif // __wasi__

#endif  // V8_LIBPLATFORM_DEFAULT_FOREGROUND_TASK_RUNNER_H_
