#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_LOCAL_HANDLE_H_
#define INCLUDE_V8_LOCAL_HANDLE_H_

#ifdef __wasi__
// WASI stub for v8-local-handle

namespace v8 {
template<typename T> class Local {
  T* ptr_;
public:
  Local() : ptr_(nullptr) {}
  T* operator->() const { return ptr_; }
  T& operator*() const { return *ptr_; }
  bool IsEmpty() const { return ptr_ == nullptr; }
};

class Context;
class Data;

} // namespace v8

#else
// Include real v8-local-handle.h for non-WASI
#endif

#endif // INCLUDE_V8_LOCAL_HANDLE_H_
