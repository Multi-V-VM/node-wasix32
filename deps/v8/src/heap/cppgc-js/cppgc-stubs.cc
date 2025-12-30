// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Stub implementations for missing symbols when building for WASM32

#if V8_TARGET_ARCH_WASM32

#include <sstream>
#include "src/heap/heap.h"
#include "src/api/api.h"

namespace v8 {
namespace internal {

// Stub for HandleScopeImplementer::Iterate
void HandleScopeImplementer::Iterate(RootVisitor* v) {
  // No-op stub for WASM32
}

// Stub for InvokeFinalizationRegistryCleanupFromTask
void InvokeFinalizationRegistryCleanupFromTask(
    DirectHandle<NativeContext> native_context,
    DirectHandle<JSFinalizationRegistry> finalization_registry) {
  // No-op stub for WASM32
}

// Stub for Heap::DumpJSONHeapStatistics
void Heap::DumpJSONHeapStatistics(std::stringstream& stream) {
  stream << "{}";  // Empty JSON object
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
