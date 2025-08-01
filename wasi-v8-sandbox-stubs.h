#ifndef WASI_V8_SANDBOX_STUBS_H_
#define WASI_V8_SANDBOX_STUBS_H_

// This file provides V8 sandbox-related stubs for WASI builds
// Include this before V8 headers that use sandbox features

#ifdef __wasi__

#include <stdint.h>
#include <stddef.h>

namespace v8 {
namespace internal {

// Missing sandbox constants
#ifndef V8_EXTERNAL_POINTER_TABLE_SIZE
#define V8_EXTERNAL_POINTER_TABLE_SIZE
constexpr size_t kExternalPointerTableReservationSize = 1024 * 1024;  // 1MB
#endif

#ifndef V8_MAX_EXTERNAL_POINTERS
#define V8_MAX_EXTERNAL_POINTERS
constexpr size_t kMaxExternalPointers = 65536;
#endif

#ifndef V8_MAX_CAPACITY
#define V8_MAX_CAPACITY
constexpr size_t kMaxCapacity = 65536;
#endif

constexpr size_t kCodePointerTableReservationSize = 1024 * 1024;      // 1MB

#ifndef V8_MAX_CODE_POINTERS
#define V8_MAX_CODE_POINTERS
constexpr size_t kMaxCodePointers = 65536;
#endif

#ifndef V8_CODE_POINTER_TABLE_ENTRY_SIZE
#define V8_CODE_POINTER_TABLE_ENTRY_SIZE
constexpr size_t kCodePointerTableEntrySize = 8;
#endif

#ifndef V8_EXTERNAL_POINTER_TAG_SHIFT
#define V8_EXTERNAL_POINTER_TAG_SHIFT
constexpr int kExternalPointerTagShift = 48;
#endif

// External pointer handle type
using ExternalPointerHandle = uint32_t;
using CodePointerHandle = uint32_t;
using CppHeapPointerHandle = uint32_t;

#ifndef V8_NULL_EXTERNAL_POINTER_HANDLE
#define V8_NULL_EXTERNAL_POINTER_HANDLE
constexpr ExternalPointerHandle kNullExternalPointerHandle = 0;
#endif

constexpr CodePointerHandle kNullCodePointerHandle = 0;
constexpr CppHeapPointerHandle kNullCppHeapPointerHandle = 0;

} // namespace internal
} // namespace v8

#endif // __wasi__

#endif // WASI_V8_SANDBOX_STUBS_H_