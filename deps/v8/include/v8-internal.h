#ifndef INCLUDE_V8_INTERNAL_H_
#define INCLUDE_V8_INTERNAL_H_

#ifdef __wasi__
// Include necessary WASI fixes but avoid redefinitions
#include "wasi/nuclear-fix.h"
#include "wasi/v8-wasi-compat.h"
#include "../../../../wasi-v8-internals-minimal.h"
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
  // Add other minimal definitions as needed
};

}  // namespace internal
}  // namespace v8
#endif

#endif // INCLUDE_V8_INTERNAL_H_
