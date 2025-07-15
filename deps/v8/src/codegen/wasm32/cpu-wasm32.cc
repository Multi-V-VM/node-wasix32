// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/codegen/cpu-features.h"

namespace v8 {
namespace internal {

void CpuFeatures::FlushICache(void* start, size_t size) {
  // No instruction cache to flush on WASM32 virtual ISA
  // The WASM runtime handles any necessary cache management
}

V8_EXPORT_PRIVATE void CpuFeatures::SetAll() {
  // Enable all WASM32 features for testing/development
  supported_ |= (1u << WASM32_BASELINE);
  supported_ |= (1u << WASM32_SIMD);
  supported_ |= (1u << WASM32_ATOMICS);
  supported_ |= (1u << WASM32_EXCEPTION);
}

V8_EXPORT_PRIVATE void CpuFeatures::ProbeImpl(bool cross_compile) {
  // For WASM32, we don't need to probe actual CPU features
  // since it's a virtual ISA. We enable features based on
  // the WASM runtime capabilities.
  
  base::CPU cpu;
  
  // Always support baseline WASM32
  supported_ |= (1u << WASM32_BASELINE);
  
  // Check for SIMD support
  // In a real implementation, this would query the WASM runtime
  // For now, assume SIMD is available
  supported_ |= (1u << WASM32_SIMD);
  
  // Check for atomics support
  // Assume atomics are available in modern WASM runtimes
  supported_ |= (1u << WASM32_ATOMICS);
  
  // Exception handling is still experimental in WASM
  // Enable it conditionally based on build flags
#ifdef V8_ENABLE_WASM_EXCEPTION_HANDLING
  supported_ |= (1u << WASM32_EXCEPTION);
#endif
  
  // Set the IsSupported bits for cross-compilation
  if (cross_compile) {
    // For cross-compilation, enable all features
    SetAll();
  }
  
  // WASM32 doesn't have variable cache line sizes
  cache_line_size_ = 64;  // Standard cache line size
}

void CpuFeatures::PrintTarget() {
  // Print WASM32 target information
  printf("target: wasm32\n");
  printf("features: ");
  
  bool first = true;
  if (IsSupported(WASM32_BASELINE)) {
    printf("baseline");
    first = false;
  }
  if (IsSupported(WASM32_SIMD)) {
    if (!first) printf(" ");
    printf("simd128");
    first = false;
  }
  if (IsSupported(WASM32_ATOMICS)) {
    if (!first) printf(" ");
    printf("atomics");
    first = false;
  }
  if (IsSupported(WASM32_EXCEPTION)) {
    if (!first) printf(" ");
    printf("exception-handling");
  }
  printf("\n");
}

void CpuFeatures::PrintFeatures() {
  // Print detailed feature information
  printf("WASM32 features:\n");
  printf("  Baseline: %s\n", IsSupported(WASM32_BASELINE) ? "yes" : "no");
  printf("  SIMD128: %s\n", IsSupported(WASM32_SIMD) ? "yes" : "no");
  printf("  Atomics: %s\n", IsSupported(WASM32_ATOMICS) ? "yes" : "no");
  printf("  Exception handling: %s\n", IsSupported(WASM32_EXCEPTION) ? "yes" : "no");
}

}  // namespace internal
}  // namespace v8