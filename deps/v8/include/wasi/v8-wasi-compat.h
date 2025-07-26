#ifndef V8_WASI_COMPAT_H_
#define V8_WASI_COMPAT_H_

#ifdef __wasi__

#include <stdint.h>
#include <stddef.h>
#include <limits>
#include <type_traits>

#include "../v8config.h"

// Include macro fixes first
#include "macro-fixes.h"

namespace v8 {
namespace internal {

// Basic type definitions
using Address = uintptr_t;
using intptr_t = long;

// Constants from nuclear-fix.h (without duplication)
// These are already defined as macros in nuclear-fix.h
// constexpr int kApiSystemPointerSize = sizeof(void*);
constexpr int kApiInt32Size = 4;
constexpr int kApiInt64Size = 8;
// constexpr int kSystemPointerSize = sizeof(void*);
constexpr int kSystemPointerSizeLog2 = 2;  // Assuming 32-bit pointers
// constexpr int kTaggedSize = 4;
constexpr int kTaggedSizeLog2 = 2;
// constexpr int kApiTaggedSize = 4;

// Smi constants for 32-bit
constexpr int kSmiTagSize = 1;
constexpr int kSmiTagShift = 0;
constexpr int kSmiShiftSize = 0;
constexpr int kSmiValueSize = 31;
constexpr int kSmiMinValue = -(1 << 30);
constexpr int kSmiMaxValue = (1 << 30) - 1;

// Pointer types
using ExternalPointer_t = uint32_t;
using CppHeapPointer_t = uint32_t;
using IndirectPointerHandle = uint32_t;
using TrustedPointerHandle = uint32_t;

// Define kNullAddress constant
constexpr Address kNullAddress = 0;

// Smi helper functions
constexpr bool SmiValuesAre31Bits() { return kSmiValueSize == 31; }
constexpr bool SmiValuesAre32Bits() { return kSmiValueSize == 32; }

// Architecture-related constants
constexpr size_t kPtrComprCageReservationSize = 0;
constexpr size_t kPtrComprCageBaseAlignment = 1;
constexpr size_t kSandboxSize = 0;
constexpr size_t kSandboxAlignment = 1;
constexpr size_t kSandboxGuardRegionSize = 0;
constexpr size_t kMaxSafeBufferSizeForSandbox = 2147483647;  // INT32_MAX

// Exception handling constants
constexpr int kDontThrow = 0;
constexpr int kThrowOnError = 1;

// Note: Internals class is defined in nuclear-fix.h
// We don't redefine it here to avoid conflicts

}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // V8_WASI_COMPAT_H_
