#ifndef V8_INCLUDE_WASI_V8_HANDLESCOPE_FIX_H_
#define V8_INCLUDE_WASI_V8_HANDLESCOPE_FIX_H_

// This file ensures HandleScope is properly available in the v8 namespace
// and prevents v8::v8:: double namespace issues
#include <cstdint>
#include "../v8-local-handle.h"
#include "../v8-maybe-local.h"

namespace v8 {

// Forward declare if not already defined
class Isolate;

// Ensure HandleScope is available in v8 namespace
#ifndef V8_HANDLESCOPE_DEFINED
#define V8_HANDLESCOPE_DEFINED

class HandleScope {
 public:
  explicit HandleScope(Isolate* isolate);
  ~HandleScope();

  // Prevent heap allocation
  void* operator new(size_t size) = delete;
  void* operator new[](size_t size) = delete;
  void operator delete(void*, size_t) = delete;
  void operator delete[](void*, size_t) = delete;

 private:
  // Implementation details hidden
  void* internal_isolate_;
  void* prev_next_;
  void* prev_limit_;
};

class EscapableHandleScope : public HandleScope {
 public:
  explicit EscapableHandleScope(Isolate* isolate);
  ~EscapableHandleScope();
  
  template <class T>
  Local<T> Escape(Local<T> value);
  
  template <class T>
  T* Escape(T* value);
  
  // WASI: Add EscapeMaybe method
  template <class T>
  MaybeLocal<T> EscapeMaybe(MaybeLocal<T> value) {
    if (value.IsEmpty()) {
      return MaybeLocal<T>();
    }
    Local<T> local;
    if (value.ToLocal(&local)) {
      return MaybeLocal<T>(Escape(local));
    }
    return MaybeLocal<T>();
  }

 private:
  void* escape_slot_;
};

// Also ensure SealHandleScope is available
#define V8_SEAL_HANDLE_SCOPE_DEFINED
class SealHandleScope {
 public:
  explicit SealHandleScope(Isolate* isolate);
  ~SealHandleScope();

 private:
  void* internal_isolate_;
  void* prev_limit_;
};

#endif // V8_HANDLESCOPE_DEFINED

} // namespace v8

// Ensure types are available without v8:: prefix when needed
#ifdef V8_ENABLE_CHECKS
using v8::HandleScope;
using v8::EscapableHandleScope;
using v8::SealHandleScope;
#endif

#endif // V8_INCLUDE_WASI_V8_HANDLESCOPE_FIX_H_