#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_METHOD_FIXES_H_
#define V8_WASI_METHOD_FIXES_H_

#ifdef __wasi__

// Targeted method fixes for WASI without conflicting class definitions

namespace v8 {

// Helper functions to access persistent handle values
template<typename T>
T* GetPersistentValue(const PersistentBase<T>& handle) {
  return reinterpret_cast<T*>(handle.val_);
}

template<typename T>
T* GetTracedValue(const TracedReference<T>& handle) {
  return reinterpret_cast<T*>(handle.val_);
}

}  // namespace v8

// Macro to help with method calls
#define WASI_GET_PERSISTENT_VALUE(handle) v8::GetPersistentValue(handle)
#define WASI_GET_TRACED_VALUE(handle) v8::GetTracedValue(handle)

#endif // __wasi__

#endif // V8_WASI_METHOD_FIXES_H_
