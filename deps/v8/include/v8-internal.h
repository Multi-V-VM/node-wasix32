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

// Include WASI compatibility definitions
#include "wasi/v8-wasi-compat.h"

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
constexpr size_t kMaxSafeBufferSizeForSandbox = 2147483647;  // INT32_MAX

// Basic type definitions
#ifndef V8_INTERNAL_ADDRESS_DEFINED
using Address = uintptr_t;
#endif
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
#ifndef V8_SMI_VALUE_FUNCTIONS_DEFINED
constexpr bool SmiValuesAre31Bits() { return true; }
constexpr bool SmiValuesAre32Bits() { return false; }
#endif

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
  static constexpr size_t kExternalAllocationSoftLimit = 64 * 1024 * 1024;
  static constexpr int kApiTaggedSize = kApiTaggedSizeConst;
  
  // Add missing methods
  static inline int IntToSmi(int value) {
    return (value << 1) | 1;
  }
  
  static inline int SmiValue(int smi) {
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
// Temporary fix: Define missing constants for build

#ifdef __cplusplus

#include <stdint.h>
#include <stddef.h>

// Define base constants
constexpr int kSystemPointerSize = sizeof(void*);
constexpr int kInt32Size = sizeof(int32_t);
constexpr int kInt64Size = sizeof(int64_t);

// Define kTaggedSize based on pointer compression
#ifdef V8_COMPRESS_POINTERS
constexpr int kTaggedSize = kInt32Size;
#else
constexpr int kTaggedSize = kSystemPointerSize;
#endif

namespace v8 {
namespace internal {

// Define Address type for non-WASI builds
using Address = uintptr_t;

class Internals {
 public:
  // Basic constants
  static constexpr int kApiTaggedSize = kTaggedSize;
  static constexpr size_t kExternalAllocationSoftLimit = 64 * 1024 * 1024;
  static constexpr int kDontThrow = 0;
  static constexpr int kThrowOnError = 1;
  
  // Root indices
  static constexpr int kNodeStateIsWeakValue = 1;
  static constexpr int kNodeClassIdOffset = 4;
  static constexpr int kEmptyStringRootIndex = 0;
  static constexpr int kUndefinedValueRootIndex = 1;
  static constexpr int kNullValueRootIndex = 2;
  static constexpr int kTrueValueRootIndex = 3;
  static constexpr int kFalseValueRootIndex = 4;
  
  // String constants
  static constexpr int kStringResourceOffset = 8;
  static constexpr int kStringRepresentationAndEncodingMask = 0x0f;
  static constexpr int kStringEncodingMask = 0x08;
  static constexpr int kExternalOneByteRepresentationTag = 0x04;
  static constexpr int kExternalTwoByteRepresentationTag = 0x02;
  static constexpr int kExternalStringResourceTag = 2;
  
  // Object layout constants
  static constexpr int kJSAPIObjectWithEmbedderSlotsHeaderSize = 16;
  static constexpr int kEmbedderDataSlotSize = 8;
  static constexpr int kEmbedderDataSlotExternalPointerOffset = 0;
  static constexpr int kJSObjectHeaderSize = 12;
  static constexpr uintptr_t kEmbedderDataSlotPayloadTag = 1;
  static constexpr int kNativeContextEmbedderDataOffset = 32;
  static constexpr int kEmbedderDataArrayHeaderSize = 8;
  static constexpr int kInferShouldThrowMode = -1;
  
  // SMI conversion functions
  static inline int IntToSmi(int value) {
    return (value << 1) | 1;
  }
  
  static inline int SmiValue(int smi) {
    return smi >> 1;
  }
  
  static constexpr Address IntegralToSmi(int value) {
    return (static_cast<Address>(value) << 1) | 1;
  }
  
  // Helper functions
  static constexpr bool IsValidSmi(intptr_t value) {
    // For 32-bit systems, SMI values are 31 bits
    return value >= (-static_cast<intptr_t>(1) << 30) &&
           value <= (static_cast<intptr_t>(1) << 30) - 1;
  }
  
  static Address* TryIntegralToSmi(intptr_t value) {
    if (IsValidSmi(value)) {
      static thread_local Address smi = IntegralToSmi(value);
      return &smi;
    }
    return nullptr;
  }
  
  // Stub functions for compatibility
  static int GetNodeState(void* slot) { return 0; }
  static void CheckInitialized(void* isolate) {}
  
  template<typename T>
  static void CheckInitialized(T* isolate) {}
  
  static Address* GetRootSlot(void* isolate, int index) {
    static Address dummy = 0;
    return &dummy;
  }
  
  static bool IsExternalTwoByteString(int instance_type) { return false; }
  static int GetInstanceType(Address obj) { return 0; }
  static v8::Isolate* GetIsolateForSandbox(Address obj) { return nullptr; }
  static bool CanHaveInternalField(int instance_type) { return true; }
  
  static Address ReadTaggedPointerField(Address obj, int offset) {
    return *reinterpret_cast<Address*>(obj + offset);
  }
  
  static Address GetRoot(v8::Isolate* isolate, int index) {
    // Return dummy addresses for root values
    switch (index) {
      case kUndefinedValueRootIndex: return 0x1000;
      case kNullValueRootIndex: return 0x1004;
      case kTrueValueRootIndex: return 0x1008;
      case kFalseValueRootIndex: return 0x100C;
      case kEmptyStringRootIndex: return 0x1010;
      default: return 0;
    }
  }
  
  static bool HasHeapObjectTag(Address value) {
    return (value & 1) == 1;
  }
  
  template<typename T>
  static T ReadRawField(Address obj, int offset) {
    return *reinterpret_cast<T*>(obj + offset);
  }
  
  static Address DecompressTaggedField(Address obj, uint32_t value) {
    return static_cast<Address>(value);
  }
  
  static void* IsolateFromNeverReadOnlySpaceObject(Address obj) {
    return nullptr;
  }

  template <int tag>
  static Address ReadExternalPointerField(void* isolate, Address obj,
                                          int offset) {
    return 0;
  }
};

}  // namespace internal
}  // namespace v8

#endif // __cplusplus

#endif

#endif // INCLUDE_V8_INTERNAL_H_
