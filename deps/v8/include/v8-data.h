#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_DATA_H_
#define INCLUDE_V8_DATA_H_

#include "v8config.h"

namespace v8 {

// Forward declaration for Local template
template <typename T> class Local;

#ifdef __wasi__
// Forward declare Isolate and internal::Address for Local<> implementation
class Isolate;
namespace internal { using Address = unsigned long; }
#else
class Isolate;
namespace internal { using Address = uintptr_t; }
#endif

// ===========================================================================
// Minimal Local<T> stub implementation used by both WASI and host tool builds
// ===========================================================================
// Full implementation must be available here since many headers include
// v8-data.h and immediately try to use Local<> before the heavy headers are
// included.

template <typename T>
class Local {
 public:
  Local() : val_(nullptr) {}

  // Constructor from pointer
  explicit Local(T* that) : val_(that) {}

  template <class S>
  Local(Local<S> that) : val_(reinterpret_cast<T*>(*that)) {}

  bool IsEmpty() const { return val_ == nullptr; }
  void Clear() { val_ = nullptr; }

  T* operator->() const { return val_; }
  T* operator*() const { return val_; }

  // Contextual bool conversion
  explicit operator bool() const { return !IsEmpty(); }

  // Internal pointer access for v8-handle-fixes.h
  T** ptr() const { return const_cast<T**>(&val_); }

  // Type conversion
  template <class S>
  Local<S> As() const {
    return Local<S>(reinterpret_cast<S*>(val_));
  }

  // Unsafe type conversion (no checks)
  template <class S>
  Local<S> UnsafeAs() const {
    return Local<S>(reinterpret_cast<S*>(val_));
  }

  // Template value method for accessing as different type
  template <class S>
  S* value() const {
    return reinterpret_cast<S*>(val_);
  }

  // Comparison operators
  template <class S>
  bool operator==(const Local<S>& that) const {
    return val_ == reinterpret_cast<T*>(*that);
  }

  template <class S>
  bool operator!=(const Local<S>& that) const {
    return val_ != reinterpret_cast<T*>(*that);
  }

  // Static factory methods
  template <class S>
  static Local<T> New(Isolate* isolate, const Local<S>& that) {
    return Local<T>(reinterpret_cast<T*>(*that));
  }

  template <class S>
  static Local<T> New(Isolate* isolate, S* that) {
    return Local<T>(reinterpret_cast<T*>(that));
  }

  // Overload for Address (unsigned long)
  static Local<T> New(Isolate* isolate, internal::Address value) {
    return Local<T>(reinterpret_cast<T*>(value));
  }

  // Overload for PersistentBase-style handle types (Persistent/Global).
  // Dereferences the handle's storage slot to recover the tagged pointer that
  // WASI Local<T> stores directly. An earlier stub returned a null Local
  // unconditionally, which silently broke every persistent->local conversion
  // (including PersistentBase::Get, which delegates here).
  template <class S>
  static Local<T> New(Isolate* isolate, const S& that) {
    if (that.IsEmpty()) return Local<T>();
    return Local<T>(that.template value<T>());
  }

  static Local<T> FromSlot(internal::Address* slot) {
    if (slot == nullptr || *slot == 0) return Local<T>();
    return Local<T>(reinterpret_cast<T*>(*slot));
  }

  // Returns a pointer to the slot used internally
  internal::Address* slot() const {
    return reinterpret_cast<internal::Address*>(const_cast<T**>(&val_));
  }

  // FromRepr creates a Local from an internal representation
  static Local<T> FromRepr(internal::Address value) {
    return Local<T>(reinterpret_cast<T*>(value));
  }

  // FromAddress creates a Local from an internal Address
  static Local<T> FromAddress(internal::Address address) {
    return Local<T>(reinterpret_cast<T*>(address));
  }

  // Cast - static type cast from one Local type to another
  template <class S>
  static Local<T> Cast(Local<S> that) {
    return Local<T>(reinterpret_cast<T*>(*that));
  }

  // ToLocalChecked - returns this Local (for compatibility with MaybeLocal pattern)
  Local<T> ToLocalChecked() const {
    return *this;
  }

 private:
  T* val_;
};
// end Local<T>

/**
 * The superclass of objects that can be embedded in JavaScript values
 * or passed to most functions defined in V8.
 */
class V8_EXPORT Data {
 public:
  // Type checking methods
  bool IsValue() const;
  bool IsModule() const;
  bool IsModuleRequest() const;
  bool IsFixedArray() const;
  bool IsPrivate() const;
  bool IsObjectTemplate() const;
  bool IsFunctionTemplate() const;
  bool IsContext() const;
  bool IsCppHeapExternal() const;

 private:
  Data() = delete;
};

} // namespace v8

#ifdef __wasi__
// Alias Address to global namespace for code that uses ::Address
using Address = v8::internal::Address;
#endif


#endif // INCLUDE_V8_DATA_H_
