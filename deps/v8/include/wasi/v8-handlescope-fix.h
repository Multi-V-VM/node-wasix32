#ifndef V8_INCLUDE_WASI_V8_HANDLESCOPE_FIX_H_
#define V8_INCLUDE_WASI_V8_HANDLESCOPE_FIX_H_

// This file ensures HandleScope is properly available in the v8 namespace
// and prevents v8::v8:: double namespace issues
#include <cstdint>
#include "../v8-local-handle.h"
#include "../v8-maybe-local.h"

namespace v8 {

// Minimal forward declarations for internal types used in signatures.
namespace internal {
#ifndef V8_INTERNAL_ADDRESS_DEFINED
#define V8_INTERNAL_ADDRESS_DEFINED
using Address = uintptr_t;
#endif
class Isolate;
}  // namespace internal

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

  // Methods used by src/api implementations; declared here for ODR.
  static int NumberOfHandles(Isolate* isolate);
  // Forward-declared internal types live in v8-internal.h; signatures match src/api/api.cc
  static ::v8::internal::Address* CreateHandle(::v8::internal::Isolate* i_isolate,
                                               ::v8::internal::Address value);
#ifdef V8_ENABLE_DIRECT_HANDLE
  static ::v8::internal::Address* CreateHandleForCurrentIsolate(
      ::v8::internal::Address value);
#endif

  void Initialize(Isolate* isolate);
  Isolate* GetIsolate() const { return reinterpret_cast<Isolate*>(i_isolate_); }

 protected:
  // Default constructor for use by derived classes that call Initialize separately
  HandleScope() : i_isolate_(nullptr), prev_next_(nullptr), prev_limit_(nullptr) {}
  // Implementation details hidden
  // Match member names used by api.cc
  ::v8::internal::Isolate* i_isolate_;
  ::v8::internal::Address* prev_next_;
  ::v8::internal::Address* prev_limit_;
#ifdef V8_ENABLE_CHECKS
  int scope_level_ = 0;
#endif
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

// Base class used internally by V8 API for escapable scopes.
class EscapableHandleScopeBase : public HandleScope {
 public:
  explicit EscapableHandleScopeBase(Isolate* isolate);

 protected:
  // Implementation provided in src/api/api.cc; declared here for linkage.
  ::v8::internal::Address* EscapeSlot(::v8::internal::Address* escape_value);
  ::v8::internal::Address* escape_slot_ = nullptr;
};

// Also ensure SealHandleScope is available
#define V8_SEAL_HANDLE_SCOPE_DEFINED
class SealHandleScope {
 public:
  explicit SealHandleScope(Isolate* isolate);
  ~SealHandleScope();

 private:
  ::v8::internal::Isolate* i_isolate_;
  ::v8::internal::Address* prev_limit_;
  int prev_sealed_level_;
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
