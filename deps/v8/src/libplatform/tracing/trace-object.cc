#ifdef V8_TARGET_ARCH_WASM32
#include "../../include/libplatform/libplatform-wasi-fix.h"
#endif
// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef __wasi__
// WASI stub implementation
#include <memory>
#include <cstdint>
#include <cstring>

namespace v8 {

class ConvertableToTraceFormat {
 public:
  virtual ~ConvertableToTraceFormat() = default;
};

namespace platform {
namespace tracing {

constexpr int kTraceMaxNumArgs = 2;

class TraceObject {
 public:
  union ArgValue {
    uint64_t as_uint;
    int64_t as_int;
    double as_double;
    const void* as_pointer;
    const char* as_string;
  };

  TraceObject() : pid_(0), tid_(0), phase_(0), category_enabled_flag_(nullptr),
                  name_(nullptr), scope_(nullptr), id_(0), bind_id_(0),
                  num_args_(0), flags_(0), ts_(0), tts_(0), duration_(0),
                  cpu_duration_(0), parameter_copy_storage_(nullptr) {}
  
  ~TraceObject() { delete[] parameter_copy_storage_; }

  void Initialize(char phase, const uint8_t* category_enabled_flag,
                  const char* name, const char* scope, uint64_t id,
                  uint64_t bind_id, int num_args, const char** arg_names,
                  const uint8_t* arg_types, const uint64_t* arg_values,
                  std::unique_ptr<v8::ConvertableToTraceFormat>* arg_convertables,
                  unsigned int flags, int64_t timestamp, int64_t cpu_timestamp) {
    // Stub implementation - just store basic values
    phase_ = phase;
    category_enabled_flag_ = category_enabled_flag;
    name_ = name;
    scope_ = scope;
    id_ = id;
    bind_id_ = bind_id;
    num_args_ = (num_args > kTraceMaxNumArgs) ? kTraceMaxNumArgs : num_args;
    flags_ = flags;
    ts_ = timestamp;
    tts_ = cpu_timestamp;
  }

  void UpdateDuration(int64_t timestamp, int64_t cpu_timestamp) {
    duration_ = timestamp - ts_;
    cpu_duration_ = cpu_timestamp - tts_;
  }

  void InitializeForTesting(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<v8::ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags, int pid, int tid, int64_t ts, int64_t tts,
      uint64_t duration, uint64_t cpu_duration) {
    pid_ = pid;
    tid_ = tid;
    phase_ = phase;
    category_enabled_flag_ = category_enabled_flag;
    name_ = name;
    scope_ = scope;
    id_ = id;
    bind_id_ = bind_id;
    num_args_ = num_args;
    flags_ = flags;
    ts_ = ts;
    tts_ = tts;
    duration_ = duration;
    cpu_duration_ = cpu_duration;
  }

  // Getters
  int pid() const { return pid_; }
  int tid() const { return tid_; }
  char phase() const { return phase_; }
  const uint8_t* category_enabled_flag() const { return category_enabled_flag_; }
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
  int64_t ts() const { return ts_; }
  int64_t tts() const { return tts_; }
  int64_t duration() const { return duration_; }
  int64_t cpu_duration() const { return cpu_duration_; }

 private:
  int pid_;
  int tid_;
  char phase_;
  const uint8_t* category_enabled_flag_;
  const char* name_;
  const char* scope_;
  uint64_t id_;
  uint64_t bind_id_;
  int num_args_;
  const char* arg_names_[kTraceMaxNumArgs];
  uint8_t arg_types_[kTraceMaxNumArgs];
  ArgValue arg_values_[kTraceMaxNumArgs];
  std::unique_ptr<v8::ConvertableToTraceFormat> arg_convertables_[kTraceMaxNumArgs];
  char* parameter_copy_storage_;
  unsigned int flags_;
  int64_t ts_;
  int64_t tts_;
  int64_t duration_;
  int64_t cpu_duration_;
};

}  // namespace tracing
}  // namespace platform
}  // namespace v8

#else

#include "include/libplatform/v8-tracing.h"
#include "include/v8-platform.h"
#include "src/base/platform/platform.h"
#include "src/base/platform/time.h"
#include "src/tracing/trace-event-no-perfetto.h"

namespace v8 {
namespace platform {
namespace tracing {

// We perform checks for nullptr strings since it is possible that a string arg
// value is nullptr.
V8_INLINE static size_t GetAllocLength(const char* str) {
  return str ? strlen(str) + 1 : 0;
}

// Copies |*member| into |*buffer|, sets |*member| to point to this new
// location, and then advances |*buffer| by the amount written.
V8_INLINE static void CopyTraceObjectParameter(char** buffer,
                                               const char** member) {
  if (*member == nullptr) return;
  size_t length = strlen(*member) + 1;
  memcpy(*buffer, *member, length);
  *member = *buffer;
  *buffer += length;
}

void TraceObject::Initialize(
    char phase, const uint8_t* category_enabled_flag, const char* name,
    const char* scope, uint64_t id, uint64_t bind_id, int num_args,
    const char** arg_names, const uint8_t* arg_types,
    const uint64_t* arg_values,
    std::unique_ptr<v8::ConvertableToTraceFormat>* arg_convertables,
    unsigned int flags, int64_t timestamp, int64_t cpu_timestamp) {
  pid_ = base::OS::GetCurrentProcessId();
  tid_ = base::OS::GetCurrentThreadId();
  phase_ = phase;
  category_enabled_flag_ = category_enabled_flag;
  name_ = name;
  scope_ = scope;
  id_ = id;
  bind_id_ = bind_id;
  flags_ = flags;
  ts_ = timestamp;
  tts_ = cpu_timestamp;
  duration_ = 0;
  cpu_duration_ = 0;

  // Clamp num_args since it may have been set by a third-party library.
  num_args_ = (num_args > kTraceMaxNumArgs) ? kTraceMaxNumArgs : num_args;
  for (int i = 0; i < num_args_; ++i) {
    arg_names_[i] = arg_names[i];
    arg_values_[i].as_uint = arg_values[i];
    arg_types_[i] = arg_types[i];
    if (arg_types[i] == TRACE_VALUE_TYPE_CONVERTABLE)
      arg_convertables_[i] = std::move(arg_convertables[i]);
  }

  bool copy = !!(flags & TRACE_EVENT_FLAG_COPY);
  // Allocate a long string to fit all string copies.
  size_t alloc_size = 0;
  if (copy) {
    alloc_size += GetAllocLength(name) + GetAllocLength(scope);
    for (int i = 0; i < num_args_; ++i) {
      alloc_size += GetAllocLength(arg_names_[i]);
      if (arg_types_[i] == TRACE_VALUE_TYPE_STRING)
        arg_types_[i] = TRACE_VALUE_TYPE_COPY_STRING;
    }
  }

  bool arg_is_copy[kTraceMaxNumArgs];
  for (int i = 0; i < num_args_; ++i) {
    // We only take a copy of arg_vals if they are of type COPY_STRING.
    arg_is_copy[i] = (arg_types_[i] == TRACE_VALUE_TYPE_COPY_STRING);
    if (arg_is_copy[i]) alloc_size += GetAllocLength(arg_values_[i].as_string);
  }

  if (alloc_size) {
    // Since TraceObject can be initialized multiple times, we might need
    // to free old memory.
    delete[] parameter_copy_storage_;
    char* ptr = parameter_copy_storage_ = new char[alloc_size];
    if (copy) {
      CopyTraceObjectParameter(&ptr, &name_);
      CopyTraceObjectParameter(&ptr, &scope_);
      for (int i = 0; i < num_args_; ++i) {
        CopyTraceObjectParameter(&ptr, &arg_names_[i]);
      }
    }
    for (int i = 0; i < num_args_; ++i) {
      if (arg_is_copy[i]) {
        CopyTraceObjectParameter(&ptr, &arg_values_[i].as_string);
      }
    }
  }
}

TraceObject::~TraceObject() { delete[] parameter_copy_storage_; }

void TraceObject::UpdateDuration(int64_t timestamp, int64_t cpu_timestamp) {
  duration_ = timestamp - ts_;
  cpu_duration_ = cpu_timestamp - tts_;
}

void TraceObject::InitializeForTesting(
    char phase, const uint8_t* category_enabled_flag, const char* name,
    const char* scope, uint64_t id, uint64_t bind_id, int num_args,
    const char** arg_names, const uint8_t* arg_types,
    const uint64_t* arg_values,
    std::unique_ptr<v8::ConvertableToTraceFormat>* arg_convertables,
    unsigned int flags, int pid, int tid, int64_t ts, int64_t tts,
    uint64_t duration, uint64_t cpu_duration) {
  pid_ = pid;
  tid_ = tid;
  phase_ = phase;
  category_enabled_flag_ = category_enabled_flag;
  name_ = name;
  scope_ = scope;
  id_ = id;
  bind_id_ = bind_id;
  num_args_ = num_args;
  flags_ = flags;
  ts_ = ts;
  tts_ = tts;
  duration_ = duration;
  cpu_duration_ = cpu_duration;
}

}  // namespace tracing
}  // namespace platform
}  // namespace v8

#endif // __wasi__
