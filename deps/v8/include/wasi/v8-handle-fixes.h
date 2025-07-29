#ifndef V8_HANDLE_FIXES_H_
#define V8_HANDLE_FIXES_H_

#include "v8-internal.h"

namespace v8 {

// Forward declarations
template <typename T> class Local;
template <typename T> class Global;
template <typename T> class BasicTracedReference;
class Value;
class BigInt;
class Array;
class Integer;

// Extension methods for Local handle to support ptr() returning Address
template <typename T>
class LocalHandleHelper {
 public:
  static internal::Address GetAddress(const Local<T>& handle) {
    if (handle.IsEmpty()) return 0;
    return reinterpret_cast<internal::Address>(handle.ptr());
  }
};

// Extension methods for handle types to get internal address
template <typename T>
inline internal::Address HandleToAddress(const Local<T>& handle) {
  return LocalHandleHelper<T>::GetAddress(handle);
}

// Extension for Global handles
template <typename T>
inline internal::Address HandleToAddress(const Global<T>& handle) {
  if (handle.IsEmpty()) return 0;
  return reinterpret_cast<internal::Address>(handle.ptr());
}

// Extension for BasicTracedReference handles
template <typename T>
inline internal::Address HandleToAddress(const BasicTracedReference<T>& handle) {
  if (handle.IsEmpty()) return 0;
  return reinterpret_cast<internal::Address>(handle.ptr());
}

// Helper to convert various V8 types to Address for SetInternal
template <typename T>
inline internal::Address ToInternalAddress(const T& value) {
  // For V8 handle types, get their internal address
  return HandleToAddress(value);
}

// Specializations for primitive types
template <>
inline internal::Address ToInternalAddress(const v8::Local<v8::Value>& value) {
  return HandleToAddress(value);
}

template <>
inline internal::Address ToInternalAddress(const v8::Local<v8::BigInt>& value) {
  return HandleToAddress(value);
}

template <>
inline internal::Address ToInternalAddress(const v8::Local<v8::Array>& value) {
  return HandleToAddress(value);
}

template <>
inline internal::Address ToInternalAddress(const v8::Local<v8::Integer>& value) {
  return HandleToAddress(value);
}

} // namespace v8

#endif // V8_HANDLE_FIXES_H_