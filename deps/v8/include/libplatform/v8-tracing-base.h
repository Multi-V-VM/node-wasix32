#ifndef V8_LIBPLATFORM_V8_TRACING_BASE_H_
#define V8_LIBPLATFORM_V8_TRACING_BASE_H_

#include <memory>
#include <cstdint>
#include "v8-platform.h"

namespace v8 {
namespace platform {
namespace tracing {

const int kTraceMaxNumArgs = 2;

class TraceObject {
 public:
  union ArgValue {
    uint64_t as_uint;
    int64_t as_int;
    double as_double;
    const void* as_pointer;
    const char* as_string;
  };

  TraceObject() = default;
  ~TraceObject() = default;
  
  void Initialize(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<v8::ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags, int64_t timestamp, int64_t cpu_timestamp);
  void UpdateDuration(int64_t timestamp, int64_t cpu_timestamp);
  
  int pid() const { return pid_; }
  int tid() const { return tid_; }
  char phase() const { return phase_; }
  const uint8_t* category_enabled_flag() const {
    return category_enabled_flag_;
  }
  const char* name() const { return name_; }
  const char* scope() const { return scope_; }
  uint64_t id() const { return id_; }
  uint64_t bind_id() const { return bind_id_; }
  int num_args() const { return num_args_; }
  const char** arg_names() { return arg_names_; }
  uint8_t* arg_types() { return arg_types_; }
  ArgValue* arg_values() { return arg_values_; }
  std::unique_ptr<v8::ConvertableToTraceFormat>* arg_convertables() {
    return arg_convertables_;
  }
  unsigned int flags() const { return flags_; }
  int64_t ts() { return ts_; }
  int64_t tts() { return tts_; }
  uint64_t duration() { return duration_; }
  uint64_t cpu_duration() { return cpu_duration_; }

 private:
  int pid_ = 0;
  int tid_ = 0;
  char phase_ = 0;
  const char* name_ = nullptr;
  const char* scope_ = nullptr;
  const uint8_t* category_enabled_flag_ = nullptr;
  uint64_t id_ = 0;
  uint64_t bind_id_ = 0;
  int num_args_ = 0;
  const char* arg_names_[kTraceMaxNumArgs];
  uint8_t arg_types_[kTraceMaxNumArgs];
  ArgValue arg_values_[kTraceMaxNumArgs];
  std::unique_ptr<v8::ConvertableToTraceFormat>
      arg_convertables_[kTraceMaxNumArgs];
  char* parameter_copy_storage_ = nullptr;
  unsigned int flags_ = 0;
  int64_t ts_ = 0;
  int64_t tts_ = 0;
  uint64_t duration_ = 0;
  uint64_t cpu_duration_ = 0;
  
  TraceObject(const TraceObject&) = delete;
  void operator=(const TraceObject&) = delete;
};

class TraceWriter {
 public:
  TraceWriter() = default;
  virtual ~TraceWriter() = default;
  virtual void AppendTraceEvent(TraceObject* trace_event) = 0;
  virtual void Flush() = 0;
  
  static TraceWriter* CreateJSONTraceWriter(std::ostream& stream);
  static TraceWriter* CreateJSONTraceWriter(std::ostream& stream,
                                            const std::string& tag);

 private:
  TraceWriter(const TraceWriter&) = delete;
  void operator=(const TraceWriter&) = delete;
};

class TraceBufferChunk {
 public:
  explicit TraceBufferChunk(uint32_t seq);
  void Reset(uint32_t new_seq);
  bool IsFull() const;
  TraceObject* AddTraceEvent(size_t* event_index);
  TraceObject* GetEventAt(size_t index);
  uint32_t seq() const { return seq_; }
  size_t size() const { return next_free_; }
  static constexpr size_t kTraceBufferChunkSize = 64;

 private:
  size_t next_free_ = 0;
  TraceObject chunk_[kTraceBufferChunkSize];
  uint32_t seq_;
};

} // namespace tracing
} // namespace platform
} // namespace v8

#endif  // V8_LIBPLATFORM_V8_TRACING_BASE_H_