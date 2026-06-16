// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#if V8_TARGET_ARCH_WASM32

#include "src/builtins/wasm32/builtins-wasm32-abi.h"

#include <array>

namespace v8 {
namespace internal {

extern "C" Address g_wasm_regs[kWasmRegFileSize] = {0};
extern "C" Address g_wasm_interpreter_frame[kWasmInterpreterFrameSlots] = {0};
extern "C" Address g_wasm_current_frame_pointer = 0;
extern "C" int g_wasm_trace_memory = 0;

namespace {
std::array<void*, Builtins::kBuiltinCount>& Table() {
  static std::array<void*, Builtins::kBuiltinCount> table = {};
  return table;
}

void EnsureRegistered() {
  static bool registered = false;
  if (registered) return;
  registered = true;
  RegisterAllWasmBuiltins();
}
}  // namespace

void RegisterWasmBuiltin(Builtin builtin, void* fnptr) {
  int id = Builtins::ToInt(builtin);
  if (id < 0 || id >= Builtins::kBuiltinCount) return;
  Table()[id] = fnptr;
}

extern "C" void RegisterWasmBuiltinById(int builtin_id, void* fnptr) {
  if (builtin_id < 0 || builtin_id >= Builtins::kBuiltinCount) return;
  RegisterWasmBuiltin(Builtins::FromInt(builtin_id), fnptr);
}

void* WasmBuiltinFuncref(Builtin builtin) {
  EnsureRegistered();
  return Table()[Builtins::ToInt(builtin)];
}

Address WasmBuiltinEntryOr(Builtin builtin, Address fallback_entry) {
  void* fnptr = WasmBuiltinFuncref(builtin);
  if (fnptr == nullptr) return fallback_entry;
  return reinterpret_cast<Address>(fnptr);
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
