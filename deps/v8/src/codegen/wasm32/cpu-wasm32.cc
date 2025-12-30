// Copyright 2024 the V8 project authors. All rights reserved.
// Minimal WASM32 CPU features stub - provides symbols for linking but
// actual CPU probing is not needed since WASM32 is a virtual ISA.

#if V8_TARGET_ARCH_WASM32

#include "src/codegen/cpu-features.h"

namespace v8 {
namespace internal {

void CpuFeatures::FlushICache(void* start, size_t size) {
  // No instruction cache to flush on WASM32 virtual ISA
}

void CpuFeatures::ProbeImpl(bool cross_compile) {
  // WASM32 is a virtual ISA - no CPU probing needed
  icache_line_size_ = 64;
  dcache_line_size_ = 64;
}

void CpuFeatures::PrintTarget() {
  // Stub
}

void CpuFeatures::PrintFeatures() {
  // Stub
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
