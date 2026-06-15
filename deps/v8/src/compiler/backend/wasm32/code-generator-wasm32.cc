// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if V8_TARGET_ARCH_WASM32

#include "src/builtins/wasm32/builtins-wasm32-abi.h"
#include "src/codegen/wasm32/register-wasm32.h"
#include "src/codegen/wasm32/wasm32-encoder.h"
#include "src/compiler/backend/code-generator-impl.h"
#include "src/compiler/backend/code-generator.h"
#include "src/compiler/backend/gap-resolver.h"
#include "src/compiler/backend/instruction-codes.h"
#include "src/compiler/backend/instruction.h"
#include "src/objects/smi.h"

namespace v8 {
namespace internal {
namespace compiler {

namespace {

class Wasm32OperandConverter final : public InstructionOperandConverter {
 public:
  Wasm32OperandConverter(CodeGenerator* gen, Instruction* instr)
      : InstructionOperandConverter(gen, instr) {}

  int InputRegisterCode(size_t index) const {
    return ToRegister(instr_->InputAt(index)).code();
  }

  Builtin InputBuiltin(size_t index) const {
    Constant target = ToConstant(instr_->InputAt(index));
    return static_cast<Builtin>(target.ToInt32());
  }

  int OutputRegisterCode() const {
    return ToRegister(instr_->Output()).code();
  }
};

}  // namespace

CodeGenerator::CodeGenResult CodeGenerator::AssembleArchInstruction(
    Instruction* instr) {
  switch (instr->arch_opcode()) {
    case kArchNop:
    case kArchPrepareCallCFunction:
    case kArchStackPointerGreaterThan:
#if V8_ENABLE_WEBASSEMBLY
    case kArchSetStackPointer:
#endif  // V8_ENABLE_WEBASSEMBLY
    case kWasm32Nop:
    case kWasm32I32Const:
    case kWasm32LoadRoot:
    case kWasm32LoadSlot:
    case kWasm32StoreSlot:
    case kWasm32LoadMem8S:
    case kWasm32LoadMem8U:
    case kWasm32LoadMem16S:
    case kWasm32LoadMem16U:
    case kWasm32LoadMem32:
    case kLoadI32:
    case kLoadF64:
    case kWasm32StoreMem8:
    case kWasm32StoreMem16:
    case kWasm32StoreMem32:
    case kStoreI32:
    case kStoreF64:
    case kWasm32Add:
    case kWasm32Sub:
    case kWasm32Mul:
    case kInt32DivS:
    case kInt32DivU:
    case kInt32ModS:
    case kInt32ModU:
    case kWasm32And:
    case kWasm32Or:
    case kWasm32Xor:
    case kWasm32Shl:
    case kWasm32ShrU:
    case kWasm32ShrS:
    case kWasm32Eq:
    case kWasm32Ne:
    case kWasm32LtS:
    case kWasm32LtU:
    case kWasm32LeS:
    case kWasm32LeU:
    case kWasm32GtS:
    case kWasm32GtU:
    case kWasm32GeS:
    case kWasm32GeU:
    case kS128Zero:
    case kWasm32CallBuiltin:
    case kWasm32CallRuntime:
    case kWasm32Return:
      return kSuccess;
    case kArchRet:
      AssembleReturn(instr->InputAt(0));
      return kSuccess;
    default:
      FATAL("wasm32 codegen unsupported arch opcode %d",
            static_cast<int>(instr->arch_opcode()));
  }
}

void CodeGenerator::AssembleArchJumpRegardlessOfAssemblyOrder(
    RpoNumber target) {
  USE(target);
  FATAL("wasm32 jump emission requires dispatch-loop lowering");
}

void CodeGenerator::AssembleArchBranch(Instruction* instr,
                                       BranchInfo* branch) {
  USE(instr);
  USE(branch);
  FATAL("wasm32 branch emission requires dispatch-loop lowering");
}

void CodeGenerator::AssembleArchConditionalBranch(Instruction* instr,
                                                 BranchInfo* branch) {
  AssembleArchBranch(instr, branch);
}

void CodeGenerator::AssembleArchDeoptBranch(Instruction* instr,
                                            BranchInfo* branch) {
  USE(instr);
  USE(branch);
  FATAL("wasm32 deopt branch lowering is not implemented yet");
}

void CodeGenerator::AssembleArchBoolean(Instruction* instr,
                                        FlagsCondition condition) {
  USE(instr);
  USE(condition);
  FATAL("wasm32 boolean emission must be selected as an explicit compare");
}

void CodeGenerator::AssembleArchConditionalBoolean(Instruction* instr) {
  USE(instr);
  FATAL("wasm32 conditional boolean lowering is not implemented yet");
}

void CodeGenerator::AssembleArchSelect(Instruction* instr,
                                       FlagsCondition condition) {
  USE(instr);
  USE(condition);
  FATAL("wasm32 select lowering is not implemented yet");
}

#if V8_ENABLE_WEBASSEMBLY
void CodeGenerator::AssembleArchTrap(Instruction* instr,
                                     FlagsCondition condition) {
  USE(instr);
  USE(condition);
  FATAL("wasm32 trap lowering is not implemented yet");
}
#endif  // V8_ENABLE_WEBASSEMBLY

void CodeGenerator::AssembleArchBinarySearchSwitch(Instruction* instr) {
  USE(instr);
  FATAL("wasm32 binary-search switch requires dispatch-loop lowering");
}

void CodeGenerator::AssembleArchTableSwitch(Instruction* instr) {
  USE(instr);
  FATAL("wasm32 table switch requires dispatch-loop lowering");
}

void CodeGenerator::AssembleCodeStartRegisterCheck() {}

#ifdef V8_ENABLE_LEAPTIERING
void CodeGenerator::AssembleDispatchHandleRegisterCheck() {}
#endif  // V8_ENABLE_LEAPTIERING

void CodeGenerator::BailoutIfDeoptimized() {
  FATAL("wasm32 lazy deopt bailout is not implemented yet");
}

void CodeGenerator::AssemblePlaceHolderForLazyDeopt(Instruction* instr) {
  USE(instr);
}

void CodeGenerator::AssemblePrepareTailCall() {
  FATAL("wasm32 tail-call stack preparation is not implemented yet");
}

void CodeGenerator::AssembleTailCallBeforeGap(Instruction* instr,
                                              int first_unused_stack_slot) {
  USE(instr);
  USE(first_unused_stack_slot);
  FATAL("wasm32 tail-call gap preparation is not implemented yet");
}

void CodeGenerator::AssembleTailCallAfterGap(Instruction* instr,
                                             int first_unused_stack_slot) {
  USE(instr);
  USE(first_unused_stack_slot);
  FATAL("wasm32 tail-call gap cleanup is not implemented yet");
}

void CodeGenerator::FinishFrame(Frame* frame) { USE(frame); }

void CodeGenerator::AssembleConstructFrame() {}

void CodeGenerator::AssembleReturn(InstructionOperand* pop) { USE(pop); }

void CodeGenerator::AssembleDeconstructFrame() {}

void CodeGenerator::FinishCode() {}

void CodeGenerator::PrepareForDeoptimizationExits(
    ZoneDeque<DeoptimizationExit*>* exits) {
  USE(exits);
}

void CodeGenerator::AssembleMove(InstructionOperand* source,
                                 InstructionOperand* destination) {
  if (source->Equals(*destination)) return;
  USE(source);
  USE(destination);
}

void CodeGenerator::AssembleSwap(InstructionOperand* source,
                                 InstructionOperand* destination) {
  USE(source);
  USE(destination);
  FATAL("wasm32 swap lowering requires a temporary slot");
}

AllocatedOperand CodeGenerator::Push(InstructionOperand* source) {
  MachineRepresentation rep = LocationOperand::cast(source)->representation();
  USE(source);
  FATAL("wasm32 gap push lowering is not implemented yet");
  return AllocatedOperand(LocationOperand::STACK_SLOT, rep, 0);
}

void CodeGenerator::Pop(InstructionOperand* dest, MachineRepresentation rep) {
  USE(dest);
  USE(rep);
  FATAL("wasm32 gap pop lowering is not implemented yet");
}

void CodeGenerator::PopTempStackSlots() {}

void CodeGenerator::MoveToTempLocation(InstructionOperand* source,
                                       MachineRepresentation rep) {
  USE(source);
  USE(rep);
  FATAL("wasm32 move-cycle temporary lowering is not implemented yet");
}

void CodeGenerator::MoveTempLocationTo(InstructionOperand* dest,
                                       MachineRepresentation rep) {
  USE(dest);
  USE(rep);
  FATAL("wasm32 move-cycle temporary restore is not implemented yet");
}

void CodeGenerator::SetPendingMove(MoveOperands* move) { USE(move); }

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
