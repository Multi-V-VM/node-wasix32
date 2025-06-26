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
};

}  // namespace v8

#else
// Non-WASI: include original MaybeLocal definition
// (This would be the original V8 implementation)
#endif  // __wasi__

#endif  // INCLUDE_V8_MAYBE_LOCAL_H_