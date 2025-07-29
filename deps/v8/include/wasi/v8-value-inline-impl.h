#ifndef V8_VALUE_INLINE_IMPL_H_
#define V8_VALUE_INLINE_IMPL_H_

#include "v8-value.h"
#include "v8-internal.h"
#include "v8-maybe.h"

namespace v8 {

// Just is already defined in v8-maybe.h, no need to redefine

// Inline implementations for v8::Value methods
inline bool Value::IsUndefined() const {
  // For WASI, we can't check instance type directly
  // Return false as a safe default
  return false;
}

inline bool Value::IsNull() const {
  // For WASI, we can't check instance type directly
  // Return false as a safe default
  return false;
}

inline bool Value::IsNullOrUndefined() const {
  return IsNull() || IsUndefined();
}

inline bool Value::IsString() const {
  // For WASI, we can't check instance type directly
  // Return false as a safe default
  return false;
}

// Add missing Value methods that Node.js expects
inline Maybe<int64_t> Value::IntegerValue(Local<Context> context) const {
  // For WASI stub, return a default value
  return Just<int64_t>(0);
}

inline Maybe<int32_t> Value::Int32Value(Local<Context> context) const {
  // For WASI stub, return a default value
  return Just<int32_t>(0);
}

inline Maybe<uint32_t> Value::Uint32Value(Local<Context> context) const {
  // For WASI stub, return a default value
  return Just<uint32_t>(0);
}

inline Maybe<double> Value::NumberValue(Local<Context> context) const {
  // For WASI stub, return a default value
  return Just<double>(0.0);
}

} // namespace v8

#endif // V8_VALUE_INLINE_IMPL_H_