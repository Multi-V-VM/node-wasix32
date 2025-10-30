#ifndef INCLUDE_V8_INTERNAL_H_
#define INCLUDE_V8_INTERNAL_H_

// Do not include std-preinclude here; this header may be parsed from within
// a `namespace v8 {}` context, and any standard headers pulled in here would
// end up defining symbols under a nested v8::std. Ensure TUs include standard
// headers at global scope instead.

#ifdef __wasi__
// Include necessary WASI fixes but avoid redefinitions, and avoid pulling in
// any heavy base headers that include the C++ standard library from here.
// This header is sometimes included while inside `namespace v8 {}` blocks.
// Keep these WASI shims minimal.
#include "v8-data.h"              // Ensure Local<T> is defined in WASI builds
#include "v8-forward.h"           // Ensure template Local<T> is declared
#include "wasi/nuclear-fix.h"
#include "wasi/v8-wasi-compat.h"
#include "../../../wasi-v8-internals-minimal.h"
// Avoid including src/base/* headers here; include them at TU global scope.
// Provide namespace bridge helpers without touching std::
#include "wasi/v8-namespace-fix.h"

// Avoid including heavy roots headers here in WASI builds; Internals is
// provided by wasi/nuclear-fix.h guarded by V8_INTERNALS_CLASS_DEFINED.
#endif

// Forward declarations needed by public API headers that befriend these types
// before the actual definitions are visible in src/ headers.
namespace v8 {
namespace internal {
template <typename T>
class CustomArguments;
void PrintFunctionCallbackInfo(void*);
void PrintPropertyCallbackInfo(void*);
}  // namespace internal
}  // namespace v8

#ifdef __wasi__
#else
// Non-WASI content would go here
#include <cstdint>
#include <cstddef>

namespace v8 {
namespace internal {

// Minimal definitions for non-WASI builds
using Address = uintptr_t;

class Internals {
 public:
  static constexpr int kApiTaggedSize = sizeof(void*);
  static constexpr int kNumIsolateDataSlots = 4;
  // Add other minimal definitions as needed
};

}  // namespace internal
}  // namespace v8
#endif

// Provide core size and feature constants expected by src/common/globals.h
// These must be in the v8::internal namespace and available for both WASI and
// non-WASI builds.
namespace v8 {
namespace internal {

#ifndef V8_INTERNAL_KINT64SIZE_DEFINED
#define V8_INTERNAL_KINT64SIZE_DEFINED
inline constexpr int kInt64Size = static_cast<int>(sizeof(int64_t));
#endif

// kSystemPointerSize is provided by WASI stubs; define a fallback for
// non-WASI builds to ensure availability before including globals.
#if !defined(__wasi__)
#ifndef V8_INTERNAL_KSYSTEMPOINTERSIZE_DEFINED
#define V8_INTERNAL_KSYSTEMPOINTERSIZE_DEFINED
inline constexpr int kSystemPointerSize = static_cast<int>(sizeof(void*));
#endif
#endif  // !defined(__wasi__)

// In our WASI configuration there is no Trusted Space for code objects. Make
// this constant available to silence static_assert checks in various files.
#ifndef V8_INTERNAL_KALLCODEOBJECTSLIVEINTRUSTEDSPACE_DEFINED
#define V8_INTERNAL_KALLCODEOBJECTSLIVEINTRUSTEDSPACE_DEFINED
inline constexpr bool kAllCodeObjectsLiveInTrustedSpace = false;
#endif

}  // namespace internal
}  // namespace v8

#ifdef V8_TARGET_ARCH_WASM32
// Adjust for WASI 32-bit pointers
#undef kFixedArrayHeaderSize
static constexpr int kFixedArrayHeaderSize = sizeof(void*) * 2;  // 8 bytes on 32-bit
#endif


#ifndef V8_WASI_VALUEHELPER_DEFINED
#define V8_WASI_VALUEHELPER_DEFINED
namespace v8 {
namespace internal {
// Lightweight helpers used by public API headers to avoid pulling V8 internals.
struct ValueHelper {
  using InternalRepresentationType = Address;
  static constexpr InternalRepresentationType kEmpty = 0;
  static constexpr InternalRepresentationType kTaggedNullAddress =
      static_cast<InternalRepresentationType>(0x1);

  template <typename T>
  static bool IsEmpty(T* that) { return that == nullptr; }

  template <typename V, bool kCheck = false>
  static ::v8::Local<V> SlotAsValue(Address* slot) {
    return ::v8::Local<V>::FromSlot(slot);
  }

  template <typename T>
  static Address ValueAsAddress(T* ptr) {
    return reinterpret_cast<Address>(ptr);
  }

  template <typename T>
  static ::v8::Local<T> ReprAsValue(InternalRepresentationType repr) {
    return ::v8::Local<T>::FromRepr(repr);
  }
};
}  // namespace internal
}  // namespace v8
#endif  // V8_WASI_VALUEHELPER_DEFINED

#endif // INCLUDE_V8_INTERNAL_H_
