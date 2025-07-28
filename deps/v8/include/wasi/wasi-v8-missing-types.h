#ifndef WASI_V8_MISSING_TYPES_H_
#define WASI_V8_MISSING_TYPES_H_

#include <memory>
#include <string>

// Import Address type
using Address = uintptr_t;

namespace v8 {

// Forward declarations
class Isolate;
class Context;
class Value;
class String;
class Object;
class Primitive;
class Boolean;
class Number;
class Integer;
class Uint32;
class Int32;
class BigInt;
class Symbol;
class Name;

// Complete Local template definition
template <typename T>
class Local {
 public:
  Local() : ptr_(nullptr) {}
  
  explicit Local(T* ptr) : ptr_(ptr) {}
  
  template <typename S>
  Local(Local<S> that) : ptr_(reinterpret_cast<T*>(*that)) {}
  
  bool IsEmpty() const { return ptr_ == nullptr; }
  
  T* operator->() const { return ptr_; }
  T* operator*() const { return ptr_; }
  
  template <typename S>
  bool operator==(const Local<S>& that) const {
    return ptr_ == reinterpret_cast<T*>(*that);
  }
  
  template <typename S>
  bool operator!=(const Local<S>& that) const {
    return !operator==(that);
  }
  
  // Static factory methods
  static Local<T> New(Isolate* isolate, T* ptr) {
    return Local<T>(ptr);
  }
  
  static Local<T> FromSlot(Address* slot) {
    return Local<T>(reinterpret_cast<T*>(*slot));
  }
  
 private:
  T* ptr_;
  
  template <typename S> friend class Local;
};

// String specific functions
namespace internal {
  // Stub for NewFromUtf8Literal
  template <int N>
  Local<String> NewFromUtf8Literal(Isolate* isolate, const char (&literal)[N]) {
    return Local<String>(reinterpret_cast<String*>(const_cast<char*>(literal)));
  }
}

// Additional v8 types (only define if not already defined)
#ifndef V8_SCRIPT_H_
class Script {
 public:
  static Local<Script> Compile(Local<Context> context, Local<String> source) {
    return Local<Script>();
  }
  
  Local<Value> Run(Local<Context> context) {
    return Local<Value>();
  }
};

class UnboundScript {
 public:
  Local<Script> BindToCurrentContext() {
    return Local<Script>();
  }
};
#endif // V8_SCRIPT_H_

} // namespace v8

#endif // WASI_V8_MISSING_TYPES_H_
