#ifndef V8_LOCAL_HANDLE_FIXES_H_
#define V8_LOCAL_HANDLE_FIXES_H_

#if defined(__wasi__) && !defined(WASI_V8_MISSING_TYPES_H_)

// Add ptr() method to v8::Local for WASI compatibility
namespace v8 {

template <typename T>
class Local;
template <typename T> class ReturnValue;

namespace internal {
// Helper to extract address from Local handle
template <typename T>
inline Address* GetAddress(const Local<T>& handle) {
  return reinterpret_cast<Address*>(const_cast<T*>(*handle));
}
}  // namespace internal

// Extension methods for Local<T>
template <typename T>
inline auto Local<T>::ptr() const
    -> decltype(internal::GetAddress(*this)) {
  return internal::GetAddress(*this);
}

// Declare Cast as a static member function by injecting it into Local class
// This works because Local is incomplete at this point
template <typename T>
class LocalCastHelper {
 public:
  template <typename S>
  static Local<T> Cast(Local<S> that) {
    // For WASI stub, just return an empty Local of the target type
    return Local<T>();
  }
};

// Extension methods for Local<T> - add Cast as a static method
template <typename T>
template <typename S>
inline Local<T> Local<T>::Cast(Local<S> that) {
  return LocalCastHelper<T>::template Cast<S>(that);
}

// Fix for ReturnValue::Set methods
template <typename T>
template <typename S>
void ReturnValue<T>::Set(const Local<S> handle) {
  static_assert(std::is_void<S>::value || std::is_base_of<T, S>::value,
                "type check");
  if (!handle.IsEmpty()) {
    SetInternal(handle.ptr());
  }
}

// Specialization for uint32_t
template <typename T>
void ReturnValue<T>::Set(uint32_t i) {
  SetNonEmpty(Integer::NewFromUnsigned(GetIsolate(), i));
}

}  // namespace v8

#endif  // defined(__wasi__) && !defined(WASI_V8_MISSING_TYPES_H_)

#endif  // V8_LOCAL_HANDLE_FIXES_H_
