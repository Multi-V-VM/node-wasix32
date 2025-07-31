#ifdef __wasi__
// WASI compatibility - must be included before any other headers
#include <stdint.h>
#include <stddef.h>
#include <vector>

namespace v8 {
namespace internal {

// Missing sandbox constants
constexpr size_t kExternalPointerTableReservationSize = 1024 * 1024;
constexpr size_t kCodePointerTableReservationSize = 1024 * 1024;
constexpr uint32_t kMaxExternalPointers = 65536;
constexpr uint32_t kMaxCodePointers = 65536;
constexpr uint32_t kMaxCapacity = 65536;
constexpr size_t kCodePointerTableEntrySize = 8;
constexpr int kExternalPointerTagShift = 48;

using ExternalPointerHandle = uint32_t;
using CodePointerHandle = uint32_t;
constexpr ExternalPointerHandle kNullExternalPointerHandle = 0;
constexpr CodePointerHandle kNullCodePointerHandle = 0;

} // namespace internal
} // namespace v8

namespace v8 {
// LocalVector template
template<typename T> class Local;
template<typename T>
class LocalVector : public std::vector<Local<T>> {
 public:
  explicit LocalVector(Isolate* isolate) {}
};
} // namespace v8
#endif // __wasi__

// Copyright 2023 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/compiler/turboshaft/wasm-turboshaft-compiler.h"

#include "src/codegen/optimized-compilation-info.h"
#include "src/compiler/backend/instruction-selector.h"
#include "src/compiler/common-operator.h"
#include "src/compiler/compiler-source-position-table.h"
#include "src/compiler/machine-operator.h"
#include "src/compiler/node-origin-table.h"
#include "src/compiler/pipeline.h"
#include "src/compiler/turbofan-graph-visualizer.h"
// TODO(14108): Remove.
#include "src/compiler/wasm-compiler.h"
#include "src/wasm/wasm-engine.h"

namespace v8::internal::compiler::turboshaft {

wasm::WasmCompilationResult ExecuteTurboshaftWasmCompilation(
    wasm::CompilationEnv* env, compiler::WasmCompilationData& data,
    wasm::WasmDetectedFeatures* detected, Counters* counters) {
  wasm::WasmCompilationResult result =
      Pipeline::GenerateWasmCode(env, data, detected, counters);
  DCHECK(result.succeeded());
  DCHECK_EQ(wasm::ExecutionTier::kTurbofan, result.result_tier);
  DCHECK_NULL(result.assumptions);
  result.assumptions = std::move(data.assumptions);
  DCHECK_IMPLIES(result.assumptions, !result.assumptions->empty());
  return result;
}

}  // namespace v8::internal::compiler::turboshaft
