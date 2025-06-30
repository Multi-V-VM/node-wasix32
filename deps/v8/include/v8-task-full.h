#ifndef V8_TASK_FULL_H_
#define V8_TASK_FULL_H_

#include <memory>

namespace v8 {

// Forward declarations
class Isolate;

// Task base class
class Task {
 public:
  virtual ~Task() = default;
  virtual void Run() = 0;
};

// IdleTask class
class IdleTask {
 public:
  virtual ~IdleTask() = default;
  virtual void Run(double deadline_in_seconds) = 0;
};

// TaskRunner interface
class TaskRunner {
 public:
  virtual ~TaskRunner() = default;
  virtual void PostTask(std::unique_ptr<Task> task) = 0;
  virtual void PostDelayedTask(std::unique_ptr<Task> task, double delay_in_seconds) = 0;
  virtual void PostIdleTask(std::unique_ptr<IdleTask> task) = 0;
  virtual bool IdleTasksEnabled() = 0;
  virtual bool NonNestableTasksEnabled() const { return false; }
  virtual bool NonNestableDelayedTasksEnabled() const { return false; }
};

}  // namespace v8

#endif  // V8_TASK_FULL_H_
