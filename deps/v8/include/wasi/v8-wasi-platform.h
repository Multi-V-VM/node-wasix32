#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_PLATFORM_H_
#define V8_WASI_PLATFORM_H_

#ifdef __wasi__

#include <cstdint>
#include <functional>
#include <memory>

namespace v8 {

// Forward declarations
class Task;
class IdleTask;

// Platform enums and types
enum class MessageLoopBehavior { kDoNotWait, kWaitForWork };

enum class TaskPriority { kBestEffort, kUserVisible, kUserBlocking };

// Tracing related types
class ConvertableToTraceFormat {
 public:
  virtual ~ConvertableToTraceFormat() = default;
};

class TraceObject {
 public:
  TraceObject() = default;
  ~TraceObject() = default;

  void Initialize(char phase, const uint8_t* category_enabled, const char* name,
                  const char* scope, uint64_t id, uint64_t bind_id,
                  int32_t num_args, const char** arg_names,
                  const uint8_t* arg_types, const uint64_t* arg_values,
                  std::unique_ptr<ConvertableToTraceFormat>* arg_convertables,
                  unsigned int flags, int64_t timestamp,
                  int64_t cpu_timestamp) {}

  void UpdateDuration(int64_t timestamp, int64_t cpu_timestamp) {}
};

class TraceBufferChunk {
 public:
  TraceBufferChunk(unsigned int seq) : seq_(seq) {}
  ~TraceBufferChunk() = default;

  TraceObject* AddTraceEvent(size_t* event_index) {
    *event_index = 0;
    return &trace_object_;
  }

  static const size_t kTraceBufferChunkSize = 1024;

 private:
  unsigned int seq_;
  TraceObject trace_object_;
};

class TraceBuffer {
 public:
  virtual ~TraceBuffer() = default;
  virtual TraceObject* AddTraceEvent(uint64_t* handle) = 0;
  virtual TraceObject* GetEventByHandle(uint64_t handle) = 0;
  virtual bool IsFull() const = 0;
  virtual void Flush() = 0;
};

class TracingController {
 public:
  TracingController() = default;
  ~TracingController() = default;

  void Initialize(void* trace_buffer) {}
  int64_t CurrentTimestampMicroseconds() { return 0; }
  int64_t CurrentCpuTimestampMicroseconds() { return 0; }

  uint64_t AddTraceEvent(
      char phase, const uint8_t* category_enabled, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags) {
    return 0;
  }

  void UpdateTraceEventDuration(const uint8_t* category_enabled,
                                const char* name, uint64_t handle) {}

  void SetTraceBufferChunk(const uint8_t* chunk_start, size_t chunk_size) {}
};

// Platform enums
enum class TaskPriority { kLowPriority, kNormalPriority, kHighPriority };

enum class MessageLoopBehavior { kDoNotWait, kWaitForWork };

enum class IdleTaskSupport { kEnabled, kDisabled };

enum class InProcessStackDumping { kEnabled, kDisabled };

// TraceBuffer related
class TraceBufferChunk {
 public:
  static const uint32_t kChunkSize = 65536;
  static const uint32_t kMaxChunkSize = kChunkSize;
};

class TraceObject {
 public:
  TraceObject() = default;
  ~TraceObject() = default;
};

class TraceBuffer {
 public:
  TraceBuffer() = default;
  virtual ~TraceBuffer() = default;
};

class TraceWriter {
 public:
  TraceWriter() = default;
  virtual ~TraceWriter() = default;
};

class TraceConfig {
 public:
  static TraceConfig* CreateDefaultTraceConfig();
  virtual ~TraceConfig() = default;
  virtual bool IsCategoryGroupEnabled(const char* category_group) const {
    return false;
  }
  virtual void AddIncludedCategory(const char* included_category) {}
};

inline TraceConfig* TraceConfig::CreateDefaultTraceConfig() {
  return new TraceConfig();
}

}  // namespace v8

namespace v8 {
namespace internal {

// Task and JobHandle interfaces
class Task {
 public:
  virtual ~Task() = default;
  virtual void Run() = 0;
};

class IdleTask {
 public:
  virtual ~IdleTask() = default;
  virtual void Run(double deadline_in_seconds) = 0;
};

class JobDelegate {
 public:
  virtual ~JobDelegate() = default;
  virtual bool ShouldYield() { return false; }
  virtual void NotifyConcurrencyIncrease() {}
  virtual bool IsJoiningThread() const { return false; }
  virtual uint8_t GetTaskId() { return 0; }
};

class JobTask {
 public:
  virtual ~JobTask() = default;
  virtual void Run(JobDelegate* delegate) = 0;
  virtual size_t GetMaxConcurrency(size_t worker_count) const = 0;
};

class JobHandle {
 public:
  virtual ~JobHandle() = default;
  virtual void Join() {}
  virtual void Cancel() {}
  virtual void CancelAndDetach() {}
  virtual bool IsActive() { return false; }
  virtual bool IsValid() { return false; }
  virtual bool UpdatePriorityEnabled() const { return false; }
  virtual void UpdatePriority(TaskPriority priority) {}
};

// Platform interface
class Platform {
 public:
  virtual ~Platform() = default;
  virtual void PostTaskOnWorkerThread(TaskPriority priority,
                                      std::unique_ptr<Task> task) {}
  virtual double MonotonicallyIncreasingTime() { return 0.0; }
  virtual double CurrentClockTimeMillis() { return 0.0; }
};

}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // V8_WASI_PLATFORM_H_