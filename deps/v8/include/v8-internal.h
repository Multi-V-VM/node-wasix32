#ifndef INCLUDE_V8_INTERNAL_H_
#define INCLUDE_V8_INTERNAL_H_

#ifdef __wasi__
// Include necessary WASI fixes but avoid redefinitions
#include "wasi/nuclear-fix.h"
#include "wasi/v8-wasi-compat.h"
#include "../../../../wasi-v8-internals-minimal.h"
// Correct path to local fixes header in this repository
#include "../../../../wasi-v8-bits-fixes.h"
#include "wasi/wasi-v8-missing-types.h"
#include "../../../../wasi-v8-custom-arguments.h"
// #include "../../../../wasi-v8-minimal-missing.h" // Now included from util.h
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

#endif // INCLUDE_V8_INTERNAL_H_
