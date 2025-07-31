#ifndef WASI_V8_VALUE_METHODS_H_
#define WASI_V8_VALUE_METHODS_H_

#ifdef __wasi__

#include "v8.h"

namespace v8 {

// Stub implementations for missing V8 methods in WASI builds
// These are provided as free functions to avoid modifying V8 class definitions

inline bool DataIsValue(const Data* data) {
  return true;  // All Data objects can be values
}

inline MaybeLocal<Boolean> ValueToBoolean(Local<Value> value, Local<Context> context) {
  Isolate* isolate = context->GetIsolate();
  return Boolean::New(isolate, true);  // Stub implementation
}

inline MaybeLocal<Number> ValueToNumber(Local<Value> value, Local<Context> context) {
  Isolate* isolate = context->GetIsolate();
  return Number::New(isolate, 0.0);  // Stub implementation
}

inline MaybeLocal<String> ValueToDetailString(Local<Value> value, Local<Context> context) {
  return value->ToString(context);  // Use ToString as fallback
}

inline Maybe<bool> ValueInstanceOf(Local<Value> value, Local<Context> context, Local<Object> constructor) {
  return Just(false);  // Stub implementation
}

inline bool ValueSameValue(Local<Value> lhs, Local<Value> rhs) {
  return lhs == rhs;  // Simple comparison
}

inline bool StringEquals(Local<String> lhs, Local<Context> context, Local<String> rhs) {
  return lhs == rhs;  // Simple comparison
}

// Add missing Isolate methods as free functions
inline int64_t IsolateAdjustAmountOfExternalAllocatedMemory(Isolate* isolate, int64_t change_in_bytes) {
  return 0;  // Stub implementation
}

// FixedArray stub
class FixedArray : public Data {
 public:
  int Length() const { return 0; }
  Local<Data> Get(Local<Context> context, int index) const { 
    return Local<Data>(); 
  }
};

// Note: Eternal class is provided by V8 headers

} // namespace v8

#endif // __wasi__

#endif // WASI_V8_VALUE_METHODS_H_