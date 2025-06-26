#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_INTERNAL_H_
#define INCLUDE_V8_INTERNAL_H_

#ifdef __wasi__
// WASI-specific V8 internal definitions

// Fix architecture-related constants for 32-bit WASI
namespace v8 {
namespace internal {

// Use 32-bit safe values for WASI
constexpr size_t kPtrComprCageReservationSize = 0;
constexpr size_t kPtrComprCageBaseAlignment = 1;
constexpr size_t kSandboxSize = 0;
constexpr size_t kSandboxAlignment = 1;
constexpr size_t kSandboxGuardRegionSize = 0;
constexpr size_t kMaxSafeBufferSizeForSandbox = 0;

// Basic type definitions
using Address = uintptr_t;
using intptr_t = long;
constexpr int kApiSystemPointerSize = sizeof(void*);
constexpr int kApiInt64Size = 8;

// Disable pointer compression for WASI
#define V8_COMPRESS_POINTERS 0
#define V8_31BIT_SMIS_ON_64BIT_ARCH 0

class Object;
class Isolate;

} // namespace internal
} // namespace v8

#else
// Include real v8-internal.h for non-WASI
// (Original content would go here)
#endif

#endif // INCLUDE_V8_INTERNAL_H_
