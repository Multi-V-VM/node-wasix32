#ifndef V8_VALUE_INLINE_IMPL_H_
#define V8_VALUE_INLINE_IMPL_H_

#include "v8-value.h"
#include "v8-internal.h"

namespace v8 {

// Inline implementations for v8::Value methods
inline bool Value::IsUndefined() const {
  using I = internal::Internals;
  // For WASI, we can't check instance type directly
  // Return false as a safe default
  return false;
}

inline bool Value::IsNull() const {
  using I = internal::Internals;
  // For WASI, we can't check instance type directly
  // Return false as a safe default
  return false;
}

inline bool Value::IsNullOrUndefined() const {
  return IsNull() || IsUndefined();
}

inline bool Value::IsString() const {
  using I = internal::Internals;
  // For WASI, we can't check instance type directly
  // Return false as a safe default
  return false;
}

} // namespace v8

#endif // V8_VALUE_INLINE_IMPL_H_