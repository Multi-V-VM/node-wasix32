// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if V8_TARGET_ARCH_WASM32

#include "src/compiler/backend/instruction-codes.h"
#include "src/compiler/backend/instruction-scheduler.h"
#include "src/compiler/backend/instruction.h"

namespace v8 {
namespace internal {
namespace compiler {

bool InstructionScheduler::SchedulerSupported() { return false; }

int InstructionScheduler::GetTargetInstructionFlags(
    const Instruction* instr) const {
  switch (instr->arch_opcode()) {
    case kWasm32LoadMem8S:
    case kWasm32LoadMem8U:
    case kWasm32LoadMem16S:
    case kWasm32LoadMem16U:
    case kWasm32LoadMem32:
    case kWasm32LoadSlot:
    case kLoadI32:
    case kLoadF64:
      return kIsLoadOperation;
    case kWasm32StoreSlot:
    case kWasm32StoreMem8:
    case kWasm32StoreMem16:
    case kWasm32StoreMem32:
    case kStoreI32:
    case kStoreF64:
    case kWasm32CallBuiltin:
    case kWasm32CallRuntime:
    case kWasm32Return:
      return kHasSideEffect;
    default:
      return kNoOpcodeFlags;
  }
}

int InstructionScheduler::GetInstructionLatency(const Instruction* instr) {
  return 1;
}

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
