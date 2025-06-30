#ifndef V8_TRACING_DEFINITIONS_H_
#define V8_TRACING_DEFINITIONS_H_

#include <memory>
#include <vector>

namespace v8 {
namespace platform {
namespace tracing {

// Forward declarations
class TraceObject;

// TraceBufferChunk class
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
  std::unique_ptr<TraceObject[]> chunk_;
  uint32_t seq_;
};

// TraceBuffer base class
class TraceBuffer {
 public:
  virtual ~TraceBuffer() = default;
  
  virtual TraceObject* AddTraceEvent(uint64_t* handle) = 0;
  virtual TraceObject* GetEventByHandle(uint64_t handle) = 0;
  virtual bool Flush() = 0;
};

// TraceWriter base class  
class TraceWriter {
 public:
  virtual ~TraceWriter() = default;
  virtual void AppendTraceEvent(TraceObject* trace_event) = 0;
  virtual void Flush() = 0;
};

// TraceObject class
class TraceObject {
 public:
  union ArgValue {
    bool as_bool;
    uint64_t as_uint;
    int64_t as_int;
    double as_double;
    const void* as_pointer;
    const char* as_string;
  };
  
  char phase() const { return phase_; }
  const char* name() const { return name_; }
  const char* category_group_name() const { return category_group_name_; }
  uint64_t id() const { return id_; }
  int pid() const { return pid_; }
  int tid() const { return tid_; }
  int64_t ts() const { return ts_; }
  int64_t tts() const { return tts_; }
  uint32_t flags() const { return flags_; }
  
  void Initialize(
      char phase, const char* category_group_name, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values, unsigned int flags,
      int64_t timestamp, int64_t cpu_timestamp);
      
 private:
  char phase_;
  const char* name_;
  const char* category_group_name_;
  const char* scope_;
  uint64_t id_;
  uint64_t bind_id_;
  int pid_;
  int tid_;
  int64_t ts_;
  int64_t tts_;
  uint32_t flags_;
  
  static constexpr int kMaxNumArgs = 2;
  int num_args_;
  const char* arg_names_[kMaxNumArgs];
  uint8_t arg_types_[kMaxNumArgs];
  ArgValue arg_values_[kMaxNumArgs];
};

}  // namespace tracing
}  // namespace platform
}  // namespace v8

#endif  // V8_TRACING_DEFINITIONS_H_
