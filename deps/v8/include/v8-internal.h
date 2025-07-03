#ifndef INCLUDE_V8_INTERNAL_H_
#define INCLUDE_V8_INTERNAL_H_

#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif

#ifdef __wasi__
// WASI-specific V8 internal definitions

// Include nuclear-fix.h if not already included
#ifndef V8_WASI_NUCLEAR_FIX_CLEAN_H_
#include "wasi/nuclear-fix.h"
#endif

#include <cstdint>
#include <atomic>
#include <cstring>
#include <climits>

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

// Import constants from nuclear-fix.h
#ifdef kApiSystemPointerSize
constexpr int kApiSystemPointerSizeConst = kApiSystemPointerSize;
#else
constexpr int kApiSystemPointerSizeConst = sizeof(void*);
#endif
constexpr int kApiInt32Size = 4;
constexpr int kApiInt64Size = 8;

// Pointer and tagging constants
#ifdef kSystemPointerSize
constexpr int kSystemPointerSizeConst = kSystemPointerSize;
#else
constexpr int kSystemPointerSizeConst = sizeof(void*);
#endif
constexpr int kSystemPointerSizeLog2 = 2;  // Assuming 32-bit pointers

#ifdef kTaggedSize
constexpr int kTaggedSizeConst = kTaggedSize;
#else
constexpr int kTaggedSizeConst = 4;
#endif
constexpr int kTaggedSizeLog2 = 2;

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

// Disable pointer compression for WASI
#define V8_COMPRESS_POINTERS 0
#define V8_31BIT_SMIS_ON_64BIT_ARCH 0

// SmiValuesAre functions
constexpr bool SmiValuesAre31Bits() { return true; }
constexpr bool SmiValuesAre32Bits() { return false; }

// Additional constants
#ifdef kApiTaggedSize
constexpr int kApiTaggedSizeConst = kApiTaggedSize;
#else
constexpr int kApiTaggedSizeConst = 4; // Same as kTaggedSize
#endif
#ifndef kIsSmiValueInUpper32Bits
#define kIsSmiValueInUpper32Bits false
#endif
#ifndef kIsSmiValueInLower32Bits
#define kIsSmiValueInLower32Bits true
#endif

// Exception handling constants
constexpr int kDontThrow = 0;
constexpr int kThrowOnError = 1;

class Object;
class Isolate;

// Use Internals from nuclear-fix.h if available, otherwise define it
#ifndef V8_INTERNALS_CLASS_DEFINED
#define V8_INTERNALS_CLASS_DEFINED
class Internals {
 public:
  static constexpr int kDontThrow = 0;
  static constexpr int kThrowOnError = 1;
  static constexpr size_t kExternalAllocationSoftLimit_ = 64 * 1024 * 1024;
  
  // Add missing methods
  static constexpr int IntToSmi(int value) {
    return (value << 1) | 1;
  }
  
  static constexpr int SmiValue(int smi) {
    return smi >> 1;
  }
};
#endif

} // namespace internal

namespace base {
// base types are already defined in nuclear-fix.h
} // namespace base

namespace internal {
// bits namespace is already available via nuclear-fix.h
} // namespace internal

} // namespace v8

#else
// Include real v8-internal.h for non-WASI
// (Original content would go here)
#endif

#endif // INCLUDE_V8_INTERNAL_H_
