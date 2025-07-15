#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_LOCAL_HANDLE_H_
#define INCLUDE_V8_LOCAL_HANDLE_H_

#ifdef __wasi__
// WASI stub for v8-local-handle
#include "wasi/v8-wasi-compat.h"  // For internal::Address

namespace v8 {

// Forward declarations
class Isolate;
class Context;
class Data;
class Value;
class Primitive;
class Boolean;
class String;
class Private;

template<typename T> class Local {
public:
  Local() : ptr_(nullptr) {}
  T* operator->() const { return ptr_; }
  T& operator*() const { return *ptr_; }
  bool IsEmpty() const { return ptr_ == nullptr; }
  
  // Factory method needed by Eternal::Get and others
  static Local<T> FromSlot(::v8::internal::Address* slot) {
    Local<T> local;
    if (slot) {
      local.ptr_ = reinterpret_cast<T*>(*slot);
    }
    return local;
  }
  
  // Constructor from pointer
  explicit Local(T* ptr) : ptr_(ptr) {}
  
  // Template method for unsafe cast
  template<typename S>
  Local<S> UnsafeAs() const {
    return Local<S>(reinterpret_cast<S*>(ptr_));
  }
  
  // Template method for safe cast (same as unsafe in WASI)
  template<typename S>
  Local<S> As() const {
    return Local<S>(reinterpret_cast<S*>(ptr_));
  }
  
  // Method to get value
  template<typename S>
  S* value() const {
    return reinterpret_cast<S*>(ptr_);
  }
  
  // Constructor for New method
  static Local<T> New(Isolate* isolate, const T* that) {
    return Local<T>(const_cast<T*>(that));
  }
  
  // New method with Address
  static Local<T> New(Isolate* isolate, ::v8::internal::Address addr) {
    return Local<T>(reinterpret_cast<T*>(addr));
  }
  
  // Constructor for casting from compatible types
  template<typename S>
  Local(const Local<S>& that) : ptr_(reinterpret_cast<T*>(that.ptr_)) {}
  
  // Friend declaration to allow access between different Local instantiations
  template<typename S> friend class Local;
  
private:
  T* ptr_;
};

} // namespace v8

#else
// Include real v8-local-handle.h for non-WASI
#endif

#endif // INCLUDE_V8_LOCAL_HANDLE_H_
