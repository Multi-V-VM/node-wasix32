#ifndef WASI_V8_MISSING_TYPES_H_
#define WASI_V8_MISSING_TYPES_H_

#include <memory>
#include <string>
#include <cstdint>

// Import Address type
using Address = uintptr_t;

namespace v8 {

// Forward declarations
class Isolate;
class Context;
// Value is defined in v8-value.h, don't forward declare
class String;
class Object;
// Primitive is defined in v8-primitive.h, don't forward declare
class Boolean;
class Number;
class Integer;
class Uint32;
class Int32;
class BigInt;
class Symbol;
class Name;
template <typename T> class PersistentBase;

// Complete Local template definition
template <typename T>
class Local {
 public:
  Local() : ptr_(nullptr) {}
  
  explicit Local(T* ptr) : ptr_(ptr) {}
  
  template <typename S>
  Local(Local<S> that) : ptr_(reinterpret_cast<T*>(that.ptr())) {}
  
  bool IsEmpty() const { return ptr_ == nullptr; }
  
  T* operator->() const { return ptr_; }
  T* operator*() const { return ptr_; }
  
  template <typename S>
  bool operator==(const Local<S>& that) const {
    return ptr_ == that.ptr();
  }
  
  template <typename S>
  bool operator!=(const Local<S>& that) const {
    return !operator==(that);
  }
  
  // As() method for casting
  template <typename S>
  Local<S> As() const {
    return Local<S>(reinterpret_cast<S*>(ptr_));
  }
  
  // Static factory methods
  static Local<T> New(Isolate* isolate, T* ptr) {
    return Local<T>(ptr);
  }
  
  static Local<T> New(Isolate* isolate, Address addr) {
    return Local<T>(reinterpret_cast<T*>(addr));
  }
  
  static Local<T> FromSlot(Address* slot) {
    return Local<T>(reinterpret_cast<T*>(*slot));
  }
  
  static Local<T> FromRepr(Address repr) {
    return Local<T>(reinterpret_cast<T*>(repr));
  }
  
  // Add value method for compatibility with PersistentBase
  template <typename S>
  S* value() const {
    return reinterpret_cast<S*>(ptr_);
  }
  
  // Add UnsafeAs method
  template <typename S>
  Local<S> UnsafeAs() const {
    return Local<S>(reinterpret_cast<S*>(ptr_));
  }
  
  // Add ptr() method for compatibility
  T* ptr() const { return ptr_; }
  
  // Add New overload for PersistentBase
  template <typename S>
  static Local<T> New(Isolate* isolate, const PersistentBase<S>& persistent) {
    // PersistentBase should have a val_ member that is T*
    return Local<T>(persistent.template value<T>());
  }
  
 private:
  T* ptr_;
  
  template <typename S> friend class Local;
  template <typename S> friend class PersistentBase;
};

// String specific functions
namespace internal {
  // Stub for NewFromUtf8Literal - only define if v8-primitive.h not included
#ifndef INCLUDE_V8_PRIMITIVE_H_
  template <int N>
  Local<String> NewFromUtf8Literal(Isolate* isolate, const char (&literal)[N]) {
    return Local<String>(reinterpret_cast<String*>(const_cast<char*>(literal)));
  }
#endif
}

// Additional v8 types (only define if not already defined)
// These are now defined in v8-script.h

} // namespace v8

#endif // WASI_V8_MISSING_TYPES_H_
