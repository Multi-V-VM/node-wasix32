// Copyright 2024 the V8 project authors. All rights reserved.
// Simplified WASI compatibility header - only provides essential internal stubs
#ifndef V8_INCLUDE_WASI_V8_WASI_COMPAT_SIMPLIFIED_H_
#define V8_INCLUDE_WASI_V8_WASI_COMPAT_SIMPLIFIED_H_

#ifdef __wasi__

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <type_traits>

#ifdef DEBUG
#include "wasi/nuclear-fix.h"
#endif

#ifndef V8_WASI_CONSTANTS_DEFINED
#define V8_WASI_CONSTANTS_DEFINED

namespace v8 {
namespace internal {

// Constants for WASI build
constexpr int kApiSystemPointerSize = sizeof(void*);
constexpr int kApiDoubleSize = sizeof(double);
constexpr int kApiInt32Size = sizeof(int32_t);
constexpr int kApiInt64Size = sizeof(int64_t);
constexpr int kApiSizetSize = sizeof(size_t);

// Pointer compression and sandboxing constants  
constexpr bool kBuiltinPointerCompressionSupported = false;
constexpr bool kAllowFullCompression = false;
constexpr bool kPointersAre32Bits = false;

#ifdef V8_COMPRESS_POINTERS
constexpr size_t kPtrComprCageReservationSize = size_t{1} << 32;
constexpr size_t kPtrComprCageBaseAlignment = size_t{1} << 32;
#else
constexpr size_t kPtrComprCageReservationSize = 0;
constexpr size_t kPtrComprCageBaseAlignment = 1;
#endif

// Sandboxing is not supported on WASI
constexpr bool kSandboxIsEnabled = false;
constexpr size_t kSandboxReservationSize = 0;
constexpr size_t kSandboxAlignment = 1;
constexpr size_t kSandboxSizeLog2 = 0;
constexpr size_t kSandboxMinimumReservationSize = 0;

// External pointer table constants
constexpr uint32_t kExternalPointerTableReservationSize = 1024;
constexpr uint32_t kExternalPointerTableEntrySize = 8;
constexpr int kExternalPointerTableEntrySizeLog2 = 3;

// Memory constants
constexpr int kStackAlignment = 16;  
constexpr size_t kMinVirtualMemory = 1ULL * 1024 * 1024 * 1024;  // 1GB

// Common tag values
constexpr int kHeapObjectTag = 1;
constexpr int kWeakHeapObjectTag = 3;
constexpr int kSmiTag = 0;
constexpr int kSmiTagSize = 1;
constexpr intptr_t kSmiTagMask = (1 << kSmiTagSize) - 1;

// Object alignment
constexpr int kObjectAlignment = 1 << kObjectAlignmentBits;
constexpr int kObjectAlignmentBits = 3;
constexpr intptr_t kObjectAlignmentMask = kObjectAlignment - 1;

// Pointer alignment
constexpr int kPointerAlignment = sizeof(void*);
constexpr int kPointerAlignmentBits = 3;
constexpr intptr_t kPointerAlignmentMask = kPointerAlignment - 1;

// Double alignment
constexpr int kDoubleAlignment = sizeof(double);
constexpr int kDoubleAlignmentBits = 3;
constexpr intptr_t kDoubleAlignmentMask = kDoubleAlignment - 1;

// Code alignment
constexpr int kCodeAlignment = 32;
constexpr int kCodeAlignmentBits = 5;
constexpr intptr_t kCodeAlignmentMask = kCodeAlignment - 1;

// System pointer size
constexpr int kSystemPointerSize = sizeof(void*);
constexpr int kSystemPointerSizeLog2 = 3;

// Handle scope constants
constexpr int kHandleBlockSize = 256;

// Misc constants
constexpr int kRandomStateSize = 2 * kInt64Size;

// Array buffer allocator mode
enum class ArrayBufferAllocatorMode { kNormal, kReservation };

// Embedder ID
using EmbedderIdType = uint16_t;
constexpr EmbedderIdType kDefaultEmbedderId = 0;

}  // namespace internal
}  // namespace v8

#endif  // V8_WASI_CONSTANTS_DEFINED

#endif  // __wasi__

#endif  // V8_INCLUDE_WASI_V8_WASI_COMPAT_SIMPLIFIED_H_