#ifndef INCLUDE_V8_MAYBE_LOCAL_H_
#define INCLUDE_V8_MAYBE_LOCAL_H_

#ifdef __wasi__
#include "v8-local-handle.h"
#include "wasi/concepts-fix.h"

namespace v8 {

template <class T>
class MaybeLocal {
 public:
  MaybeLocal() = default;
  template <class S>
  MaybeLocal(Local<S> that) {}

  bool IsEmpty() const { return true; }
  Local<T> ToLocalChecked() const { return Local<T>(); }
  bool ToLocal(Local<T>* out) const { return false; }
  
  // Allow conversion to compatible types
  template <class S>
  bool ToLocal(Local<S>* out) const { 
    *out = Local<S>();
    return false; 
  }
  
  Local<T> FromMaybe(Local<T> default_value) const {
    return default_value;
  }
  
  template<typename S>
  MaybeLocal<S> As() const {
    return MaybeLocal<S>();
  }
};

}  // namespace v8

#else
// Non-WASI: include original implementation
#include "v8-local-handle.h"  // NOLINT(build/include_directory)
#include "v8-maybe.h"  // NOLINT(build/include_directory)

namespace v8 {

template <class T>
class MaybeLocal {
 public:
  MaybeLocal() = default;
  template <class S>
  MaybeLocal(Local<S> that) : val_(that) {}

  bool IsEmpty() const { return val_.IsEmpty(); }
  Local<T> ToLocalChecked() const { return val_; }
  bool ToLocal(Local<T>* out) const { 
    if (!IsEmpty()) {
      *out = val_;
      return true;
    }
    return false;
  }
  
  Local<T> FromMaybe(Local<T> default_value) const {
    return IsEmpty() ? default_value : val_;
  }
  
  template<typename S>
  MaybeLocal<S> As() const {
    return MaybeLocal<S>(val_.template As<S>());
  }

 private:
  Local<T> val_;
};

}  // namespace v8
#endif  // __wasi__

#endif  // INCLUDE_V8_MAYBE_LOCAL_H_