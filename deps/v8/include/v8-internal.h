#ifndef INCLUDE_V8_INTERNAL_H_
#define INCLUDE_V8_INTERNAL_H_

#ifdef __wasi__
// Include necessary WASI fixes but avoid redefinitions
#include "wasi/nuclear-fix.h"
#include "wasi/v8-wasi-compat.h"
#include "../../../../wasi-v8-internals-minimal.h"
#include "../../../../node/wasi-v8-bits-fixes.h"
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
  // Add other minimal definitions as needed
};

}  // namespace internal
}  // namespace v8
#endif

#ifdef V8_TARGET_ARCH_WASM32
// Adjust for WASI 32-bit pointers
#undef kFixedArrayHeaderSize
static constexpr int kFixedArrayHeaderSize = sizeof(void*) * 2;  // 8 bytes on 32-bit
#endif

#endif // INCLUDE_V8_INTERNAL_H_
