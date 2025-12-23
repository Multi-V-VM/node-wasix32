#ifndef INCLUDE_V8_INTERNAL_H_
#define INCLUDE_V8_INTERNAL_H_

// Do not include std-preinclude here; this header may be parsed from within
// a `namespace v8 {}` context, and any standard headers pulled in here would
// end up defining symbols under a nested v8::std. Ensure TUs include standard
// headers at global scope instead.

// Forward declare v8::Isolate and template Local<T> for pointer signatures
namespace v8 { class Isolate; template <typename T> class Local; }

#if defined(__wasi__) || defined(V8_USING_WASI_SHIMS)
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
#endif  // defined(__wasi__) || defined(V8_USING_WASI_SHIMS)

// Forward declarations needed by public API headers that befriend these types
// before the actual definitions are visible in src/ headers.
namespace v8 {
namespace internal {
// Forward declare internal Isolate for friend declarations in headers.
class Isolate;
class Isolate;
template <typename T>
class CustomArguments;
void PrintFunctionCallbackInfo(void*);
void PrintPropertyCallbackInfo(void*);
}  // namespace internal
}  // namespace v8

#if defined(__wasi__) || defined(V8_USING_WASI_SHIMS)
#else
// Non-WASI content would go here
#include <cstdint>
#include <cstddef>
#include <optional>

namespace v8 {
namespace internal {

// Minimal definitions for non-WASI builds
using Address = uintptr_t;

class Internals {
 public:
  static constexpr int kApiTaggedSize = sizeof(void*);
  static constexpr int kNumIsolateDataSlots = 4;
  // Minimal placeholders used by public headers
  static constexpr uint8_t kNodeStateIsWeakValue = 1;
  static constexpr Address kNullAddress = 0;
  static constexpr int kNodeClassIdOffset = 0;
  static constexpr int kNativeContextEmbedderDataOffset = 0;
  static constexpr int kEmbedderDataArrayHeaderSize = 0;
  static constexpr int kEmbedderDataSlotSize = 0;
  static constexpr int kEmbedderDataSlotExternalPointerOffset = 0;
  // String layout related constants (host stub values)
  static constexpr int kStringResourceOffset = 0;
  static constexpr int kExternalStringResourceTag = 0;
  static constexpr int kStringRepresentationAndEncodingMask = 0;
  static constexpr int kStringEncodingMask = 0;
  static constexpr int kExternalOneByteRepresentationTag = 0;
  static constexpr int kExternalTwoByteRepresentationTag = 0;
  // CppHeap pointer tagging shift used by sandbox helpers
  static constexpr int kCppHeapPointerTagShift = 1;
  // Root indices (must match RootIndex enum in src/roots/roots.h)
  static constexpr int kUndefinedValueRootIndex = 4;
  static constexpr int kNullValueRootIndex = 6;
  static constexpr int kTrueValueRootIndex = 7;
  static constexpr int kFalseValueRootIndex = 8;
  static constexpr int kEmptyStringRootIndex = 9;
  template <int kTag>
  static uintptr_t ReadExternalPointerField(::v8::Isolate*, Address, int) {
    return 0;
  }
  static Address ReadTaggedPointerField(Address, int) { return 0; }
  static Address ReadRawField(Address a, int) { return a; }
  template <typename T>
  static T ReadRawField(Address a, int) { return static_cast<T>(a); }
  static Address DecompressTaggedField(Address a, uint32_t) { return a; }
  static uint8_t GetNodeState(Address*) { return 0; }
  static int GetInstanceType(Address) { return 0; }
  static ::v8::Isolate* GetIsolateForSandbox(Address) { return nullptr; }
  static bool IsExternalTwoByteString(int) { return false; }
  template <typename T>
  static constexpr bool IsValidSmi(T) { return true; }
  template <typename T>
  static Address IntegralToSmi(T v) { return static_cast<Address>(v); }
  template <typename T>
  static ::std::optional<Address> TryIntegralToSmi(T) { return ::std::nullopt; }
  static bool CanHaveInternalField(int) { return false; }
  static void CheckInitialized(::v8::Isolate*) {}
  static Address* GetRootSlot(::v8::Isolate*, int) { return nullptr; }
  static Address GetRoot(::v8::Isolate*, int) { return 0; }
  // Overload with explicit tag parameter
  static uintptr_t ReadExternalPointerField(::v8::Isolate*, Address, int, int) {
    return 0;
  }
  static constexpr int kJSAPIObjectWithEmbedderSlotsHeaderSize = 0;
  static constexpr int kJSObjectHeaderSize = 0;
  static constexpr int kEmbedderDataSlotPayloadTag = 0;
  static constexpr int kInferShouldThrowMode = 0;
  static constexpr int kDontThrow = 0;
  static constexpr int kThrowOnError = 1;
  static bool HasHeapObjectTag(Address) { return false; }
};

}  // namespace internal
}  // namespace v8
#endif  // defined(__wasi__) || defined(V8_USING_WASI_SHIMS)

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
#if !defined(__wasi__) && !defined(V8_USING_WASI_SHIMS)
#ifndef V8_INTERNAL_KSYSTEMPOINTERSIZE_DEFINED
#define V8_INTERNAL_KSYSTEMPOINTERSIZE_DEFINED
inline constexpr int kSystemPointerSize = static_cast<int>(sizeof(void*));
#endif
#ifndef V8_INTERNAL_KTAGGEDSIZE_DEFINED
#define V8_INTERNAL_KTAGGEDSIZE_DEFINED
#if defined(V8_TARGET_ARCH_WASM32)
inline constexpr int kTaggedSize = 4;
inline constexpr int kTaggedSizeLog2 = 2;
inline constexpr int kApiTaggedSize = 4;
#else
inline constexpr int kTaggedSize = static_cast<int>(sizeof(void*));
inline constexpr int kTaggedSizeLog2 = (sizeof(void*) == 8 ? 3 : 2);
inline constexpr int kApiTaggedSize = static_cast<int>(sizeof(void*));
#endif
#endif
#endif  // !defined(__wasi__) && !defined(V8_USING_WASI_SHIMS)

// Expose sandbox-related constants for API headers that look for them
// directly under v8::internal rather than via Internals. Avoid defining on
// WASI builds, where the WASI prelude provides its own definitions.
#if !defined(__wasi__)
#ifndef V8_INTERNAL_KCPPHEAP_TAG_SHIFT_DEFINED
#define V8_INTERNAL_KCPPHEAP_TAG_SHIFT_DEFINED
inline constexpr int kCppHeapPointerTagShift = 1;
#endif
#endif

// In our WASI configuration there is no Trusted Space for code objects. Make
// this constant available to silence static_assert checks in various files.
#ifndef V8_INTERNAL_KALLCODEOBJECTSLIVEINTRUSTEDSPACE_DEFINED
#define V8_INTERNAL_KALLCODEOBJECTSLIVEINTRUSTEDSPACE_DEFINED
inline constexpr bool kAllCodeObjectsLiveInTrustedSpace = false;
#endif

// Some API headers look up this helper under v8::internal.
#if !defined(__wasi__) && !defined(V8_USING_WASI_SHIMS)
inline ::v8::Isolate* IsolateFromNeverReadOnlySpaceObject(Address) {
  return nullptr;
}

// Property callback helper used by v8-function-callback.h slow paths.
inline bool ShouldThrowOnError(class Isolate*) { return false; }

// Provide minimal host stub for ArrayBuffer BackingStore base type.
struct BackingStoreBase {};
#endif  // !defined(__wasi__) && !defined(V8_USING_WASI_SHIMS)

// Debug helper used by v8-function-callback.h when V8_ENABLE_CHECKS.
// Only provide this stub for WASI builds; other builds use the definition in api.cc.
#if defined(__wasi__) || defined(V8_USING_WASI_SHIMS)
inline void VerifyHandleIsNonEmpty(bool) {}
#else
V8_EXPORT void VerifyHandleIsNonEmpty(bool is_empty);
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

#ifdef __wasi__
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
#else
  // Non-WASI: avoid referencing ::v8::Local here; callers in public API
  // should include <v8.h> which defines Local and friends.
  template <typename T>
  static Address ValueAsAddress(T* ptr) {
    return reinterpret_cast<Address>(ptr);
  }
#endif
  template <typename T>
  static void PerformCastCheck(T) {}
  template <typename A, typename B>
  static bool EqualHandles(const A&, const B&) { return false; }
};
}  // namespace internal
}  // namespace v8
#endif  // V8_WASI_VALUEHELPER_DEFINED

#ifndef __wasi__
// Provide a free function in v8::internal to match headers expecting
// internal::PerformCastCheck(...) symbols on host builds.
namespace v8 { namespace internal {
template <typename T>
inline void PerformCastCheck(T t) { ValueHelper::PerformCastCheck(t); }
} }
#endif

#endif // INCLUDE_V8_INTERNAL_H_
