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
constexpr size_t kExternalPointerTableReservationSize = 1024 * 1024;  // 1MB
constexpr size_t kCodePointerTableReservationSize = 1024 * 1024;      // 1MB
constexpr uint32_t kMaxExternalPointers = 65536;
constexpr uint32_t kMaxCodePointers = 65536;
constexpr uint32_t kMaxCapacity = 65536;
constexpr size_t kCodePointerTableEntrySize = 8;
constexpr int kExternalPointerTagShift = 48;

// External pointer handle type
using ExternalPointerHandle = uint32_t;
using CodePointerHandle = uint32_t;

constexpr ExternalPointerHandle kNullExternalPointerHandle = 0;
constexpr CodePointerHandle kNullCodePointerHandle = 0;

} // namespace internal
} // namespace v8

#endif // __wasi__

#endif // WASI_V8_SANDBOX_STUBS_H_