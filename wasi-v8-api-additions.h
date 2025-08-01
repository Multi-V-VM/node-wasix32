#ifndef WASI_V8_API_ADDITIONS_H
#define WASI_V8_API_ADDITIONS_H

// This file provides additional V8 API definitions for WASI builds
// It should be included AFTER v8.h to extend the V8 namespace

#include <vector>
#include <initializer_list>
#include <cstddef>

// Include necessary V8 headers first
#include "deps/v8/include/v8-value.h"
#include "deps/v8/include/v8-object.h"
#include "deps/v8/include/v8-local-handle.h"
#include "deps/v8/include/v8-primitive.h"
#include "deps/v8/include/v8-tracing-base.h"

// This header includes our comprehensive V8 API stubs
#include "wasi-v8-api-stubs.h"

namespace v8 {

// Note: HeapSpaceStatistics is already defined in v8-statistics.h
// using HeapSpaceStatistics = Isolate::HeapSpaceStatistics;

// Note: ResourceConstraints is already defined in wasi/nuclear-fix.h
// We only define additional helper functions below if needed
#if 0
class ResourceConstraints {
 public:
  ResourceConstraints() = default;
  
  void set_stack_limit(uint32_t* value) { 
    // Note: This would need to be stored and applied when used with CreateParams
    stack_limit_ = value;
  }
  
  void set_max_young_generation_size_in_bytes(size_t value) {
    max_young_generation_size_in_bytes_ = value;
  }
  
  size_t max_young_generation_size_in_bytes() const {
    return max_young_generation_size_in_bytes_;
  }
  
  void set_max_old_generation_size_in_bytes(size_t value) {
    max_old_generation_size_in_bytes_ = value;
  }
  
  size_t max_old_generation_size_in_bytes() const {
    return max_old_generation_size_in_bytes_;
  }
  
  void set_code_range_size_in_bytes(size_t value) {
    code_range_size_in_bytes_ = value;
  }
  
  size_t code_range_size_in_bytes() const {
    return code_range_size_in_bytes_;
  }
  
  void set_code_range_size(size_t value) {
    code_range_size_in_bytes_ = value;
  }
  
 private:
  uint32_t* stack_limit_ = nullptr;
  size_t max_young_generation_size_in_bytes_ = 0;
  size_t max_old_generation_size_in_bytes_ = 0;
  size_t code_range_size_in_bytes_ = 0;
};
#endif // 0

// Fix for LocalVector constructor issues
template<typename T>
inline LocalVector<T> MakeLocalVector(Isolate* isolate, 
                                     std::initializer_list<Local<T>> init) {
    // Create LocalVector and populate it
    LocalVector<T> result(isolate);
    result.reserve(init.size());
    for (const auto& item : init) {
        result.push_back(item);
    }
    return result;
}

// Value inline methods are now defined in wasi/v8-value-inline-impl.h
// Note: QuickIsString is not implemented for WASI - it should be defined in v8-value.h

// Fix for Object::New overload issues
inline Local<Object> ObjectNewWithArrays(
    Isolate* isolate,
    Local<Value> prototype_or_null,
    Local<Name>* names,
    Local<Value>* values,
    size_t length) {
    // Return empty object for WASI builds
    return Local<Object>();
}

// WASI stub for FixedArray
#ifndef V8_FIXED_ARRAY_DEFINED
#define V8_FIXED_ARRAY_DEFINED
class FixedArray : public Data {
 public:
  int Length() const { return 0; }
  Local<Value> Get(Local<Context> context, int index) const {
    return Local<Value>();
  }
  V8_INLINE static FixedArray* Cast(Data* data) {
    return static_cast<FixedArray*>(data);
  }
};
#endif // V8_FIXED_ARRAY_DEFINED

// WASI extension for String::Equals method
// This method appears to have been removed from newer V8 versions
// but is still used in Node.js code
inline Maybe<bool> StringEquals(Local<String> str1, Local<Context> context, Local<String> str2) {
  // For WASI builds, we'll use StrictEquals which should be available
  return Just(str1.As<Value>()->StrictEquals(str2.As<Value>()));
}

// SealHandleScope stub for WASI builds
#ifndef V8_SEAL_HANDLE_SCOPE_DEFINED
class SealHandleScope {
 public:
  explicit SealHandleScope(Isolate* isolate) {}
  ~SealHandleScope() = default;
  
  SealHandleScope(const SealHandleScope&) = delete;
  SealHandleScope& operator=(const SealHandleScope&) = delete;
};
#endif

// ValueToBoolean helper function for WASI builds
// Converts a Value to a Boolean, used in Node.js code
inline Local<Boolean> ValueToBoolean(Local<Value> value, Isolate* isolate) {
  if (value->IsBoolean()) {
    return value.As<Boolean>();
  }
  // For non-boolean values, convert to boolean based on JS truthiness
  bool is_truthy = !value->IsNullOrUndefined() && 
                   !(value->IsNumber() && value.As<Number>()->Value() == 0) &&
                   !(value->IsString() && value.As<String>()->Length() == 0);
  return Boolean::New(isolate, is_truthy);
}

// Note: TracingController is already defined in v8-tracing.h for WASI builds
// We provide a default implementation here
class DefaultTracingController : public TracingController {
 public:
  DefaultTracingController() = default;
  ~DefaultTracingController() override = default;
  
  const uint8_t* GetCategoryGroupEnabled(const char* name) override {
    static uint8_t enabled = 0;
    return &enabled;
  }
  
  uint64_t AddTraceEvent(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<v8::ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags) override {
    return 0;
  }
  
  uint64_t AddTraceEventWithTimestamp(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<v8::ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags, int64_t timestamp) override {
    return 0;
  }
  
  void UpdateTraceEventDuration(
      const uint8_t* category_enabled_flag, const char* name, uint64_t handle) override {
    // No-op for WASI
  }
  
  void AddTraceStateObserver(TracingController::TraceStateObserver* observer) override {
    // No-op for WASI
  }
  
  void RemoveTraceStateObserver(TracingController::TraceStateObserver* observer) override {
    // No-op for WASI
  }
};

} // namespace v8

// Note: V8 Value methods are now implemented directly in v8-value.h with __wasi__ guards

#endif // WASI_V8_API_ADDITIONS_H