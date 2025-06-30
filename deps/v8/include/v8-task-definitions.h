#ifndef V8_TASK_DEFINITIONS_H_
#define V8_TASK_DEFINITIONS_H_

namespace v8 {

// Base Task class
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

}  // namespace v8

#endif  // V8_TASK_DEFINITIONS_H_
