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

// Ensure HandleScope is available in v8 namespace.
// All implementations are in src/api/api.cc. Since WASI uses static linking,
// both V8 internal code and external code (Node.js) link to the same api.cc
// implementations.
#ifndef V8_HANDLESCOPE_DEFINED
#define V8_HANDLESCOPE_DEFINED

class HandleScope {
 public:
  explicit HandleScope(Isolate* isolate);
  ~HandleScope();

  void* operator new(size_t size) = delete;
  void* operator new[](size_t size) = delete;
  void operator delete(void*, size_t) = delete;
  void operator delete[](void*, size_t) = delete;

  static int NumberOfHandles(Isolate* isolate);
  static ::v8::internal::Address* CreateHandle(::v8::internal::Isolate* i_isolate,
                                               ::v8::internal::Address value);
#ifdef V8_ENABLE_DIRECT_HANDLE
  static ::v8::internal::Address* CreateHandleForCurrentIsolate(
      ::v8::internal::Address value);
#endif

  void Initialize(Isolate* isolate);
  Isolate* GetIsolate() const { return reinterpret_cast<Isolate*>(i_isolate_); }

 protected:
  HandleScope() : i_isolate_(nullptr), prev_next_(nullptr), prev_limit_(nullptr) {}
  ::v8::internal::Isolate* i_isolate_;
  ::v8::internal::Address* prev_next_;
  ::v8::internal::Address* prev_limit_;
#ifdef V8_ENABLE_CHECKS
  int scope_level_ = 0;
#endif
};

class EscapableHandleScopeBase : public HandleScope {
 public:
  explicit EscapableHandleScopeBase(Isolate* isolate);

 protected:
  ::v8::internal::Address* EscapeSlot(::v8::internal::Address* escape_value);
  ::v8::internal::Address* escape_slot_ = nullptr;
};

class EscapableHandleScope : public EscapableHandleScopeBase {
 public:
  explicit EscapableHandleScope(Isolate* isolate);
  ~EscapableHandleScope() = default;

  template <class T>
  Local<T> Escape(Local<T> value) {
    return value;
  }

  template <class T>
  MaybeLocal<T> EscapeMaybe(MaybeLocal<T> value) {
    return value;
  }
};

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
