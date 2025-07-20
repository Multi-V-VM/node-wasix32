#ifndef V8_INCLUDE_WASI_V8_PERSISTENT_HANDLE_FIXES_H_
#define V8_INCLUDE_WASI_V8_PERSISTENT_HANDLE_FIXES_H_

#ifdef __wasi__

// Fix for PersistentBase::Get() to work with Local<T>::New()
// This is included before v8-persistent-handle.h to override methods

namespace v8 {

template <class T>
class PersistentBase;

namespace internal {
// Helper to convert PersistentBase to pointer for New()
template<typename T>
inline T* ToPointer(const PersistentBase<T>& persistent) {
  // This is a hack but necessary for WASI compatibility
  return reinterpret_cast<T*>(const_cast<void*>(
    static_cast<const void*>(&persistent)));
}
}

}

// Redefine problematic macros
#define V8_PERSISTENT_BASE_GET_OVERRIDE 1

#endif // __wasi__

#endif // V8_INCLUDE_WASI_V8_PERSISTENT_HANDLE_FIXES_H_

// Fix for Global assignment operator
namespace v8 {
template<typename T>
class Global;
}

#define V8_GLOBAL_ASSIGNMENT_FIX 1

