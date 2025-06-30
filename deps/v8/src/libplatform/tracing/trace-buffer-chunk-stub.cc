// TraceBufferChunk implementation for WASI
#include "src/libplatform/tracing/trace-buffer.h"
#include "v8-tracing-definitions.h"

namespace v8 {
namespace platform {
namespace tracing {

TraceBufferChunk::TraceBufferChunk(uint32_t seq) 
    : seq_(seq), chunk_(new TraceObject[kTraceBufferChunkSize]) {}

void TraceBufferChunk::Reset(uint32_t new_seq) {
  next_free_ = 0;
  seq_ = new_seq;
}

bool TraceBufferChunk::IsFull() const {
  return next_free_ == kTraceBufferChunkSize;
}

TraceObject* TraceBufferChunk::AddTraceEvent(size_t* event_index) {
  if (IsFull()) return nullptr;
  *event_index = next_free_++;
  return &chunk_[*event_index];
}

TraceObject* TraceBufferChunk::GetEventAt(size_t index) {
  if (index >= next_free_) return nullptr;
  return &chunk_[index];
}

// TraceObject implementation
void TraceObject::Initialize(
    char phase, const char* category_group_name, const char* name,
    const char* scope, uint64_t id, uint64_t bind_id, int num_args,
    const char** arg_names, const uint8_t* arg_types,
    const uint64_t* arg_values, unsigned int flags,
    int64_t timestamp, int64_t cpu_timestamp) {
  phase_ = phase;
  category_group_name_ = category_group_name;
  name_ = name;
  scope_ = scope;
  id_ = id;
  bind_id_ = bind_id;
  flags_ = flags;
  ts_ = timestamp;
  tts_ = cpu_timestamp;
  pid_ = 1;  // WASI doesn't have process IDs
  tid_ = 1;  // WASI doesn't have thread IDs
  
  num_args_ = num_args;
  for (int i = 0; i < num_args && i < kMaxNumArgs; ++i) {
    arg_names_[i] = arg_names[i];
    arg_types_[i] = arg_types[i];
    arg_values_[i].as_uint = arg_values[i];
  }
}

}  // namespace tracing
}  // namespace platform
}  // namespace v8
