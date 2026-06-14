// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef V8_BUILTINS_WASM32_BUILTINS_WASM32_ABI_H_
#define V8_BUILTINS_WASM32_BUILTINS_WASM32_ABI_H_

#if V8_TARGET_ARCH_WASM32

#include "src/builtins/builtins.h"
#include "src/common/globals.h"

namespace v8 {
namespace internal {

// Emulated register file in linear memory. This is the ABI between C++ and
// generated/hand-written wasm builtins. Fixed slots hold the root register,
// context, argument registers, and the return slot. Only used by builtins that
// take the uniform g_regs ABI; the entry builtins (JSEntry etc.) use direct
// typed C++ signatures and do NOT read g_regs in this milestone.
// Sized generously; tighten later against interface-descriptors-wasm32.
constexpr int kWasmRegFileSize = 64;
extern Address g_wasm_regs[kWasmRegFileSize];

// Well-known slot indices (must agree with interface-descriptors-wasm32-inl.h
// when generated builtins arrive).
enum WasmRegSlot : int {
  kWasmRegRoot = 0,      // isolate root (kRootRegister)
  kWasmRegContext = 1,   // current context (kContextRegister)
  kWasmRegReturn = 2,    // return value slot
  kWasmRegArg0 = 8,      // first argument slot; args are kWasmRegArg0 + i
};

// Builtin -> wasm function pointer registry. A registered function pointer's
// integer value is its __indirect_function_table index; instruction_start for
// the builtin is set to these bits so GeneratedCode::Call's call_indirect
// dispatches correctly.
using WasmBuiltinFn = void (*)();
void RegisterWasmBuiltin(Builtin builtin, void* fnptr);
// Returns nullptr if no wasm implementation is registered for `builtin`.
void* WasmBuiltinFuncref(Builtin builtin);
// Returns the registered wasm funcref bits for `builtin`, or `fallback_entry`
// when the builtin still uses the embedded blob entry.
Address WasmBuiltinEntryOr(Builtin builtin, Address fallback_entry);

// Registers all hand-written wasm builtins. Called once during builtin setup.
void RegisterAllWasmBuiltins();

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
#endif  // V8_BUILTINS_WASM32_BUILTINS_WASM32_ABI_H_
