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
    case kWasm32Nop:
      return kSuccess;
    case kWasm32I32Const:
      FATAL("wasm32 i32.const emission is not implemented yet");
    case kWasm32LoadRoot:
      FATAL("wasm32 root load emission is not implemented yet");
    case kWasm32LoadSlot:
      FATAL("wasm32 slot load emission is not implemented yet");
    case kWasm32StoreSlot:
      FATAL("wasm32 slot store emission is not implemented yet");
    case kWasm32LoadMem8S:
      FATAL("wasm32 i32.load8_s emission is not implemented yet");
    case kWasm32LoadMem8U:
      FATAL("wasm32 i32.load8_u emission is not implemented yet");
    case kWasm32LoadMem16S:
      FATAL("wasm32 i32.load16_s emission is not implemented yet");
    case kWasm32LoadMem16U:
      FATAL("wasm32 i32.load16_u emission is not implemented yet");
    case kWasm32LoadMem32:
      FATAL("wasm32 i32.load emission is not implemented yet");
    case kLoadI32:
      FATAL("wasm32 i32 load emission is not implemented yet");
    case kLoadF64:
      FATAL("wasm32 f64 load emission is not implemented yet");
    case kWasm32StoreMem8:
      FATAL("wasm32 i32.store8 emission is not implemented yet");
    case kWasm32StoreMem16:
      FATAL("wasm32 i32.store16 emission is not implemented yet");
    case kWasm32StoreMem32:
      FATAL("wasm32 i32.store emission is not implemented yet");
    case kStoreI32:
      FATAL("wasm32 i32 store emission is not implemented yet");
    case kStoreF64:
      FATAL("wasm32 f64 store emission is not implemented yet");
    case kWasm32Add:
      FATAL("wasm32 add emission is not implemented yet");
    case kWasm32Sub:
      FATAL("wasm32 sub emission is not implemented yet");
    case kWasm32Mul:
      FATAL("wasm32 mul emission is not implemented yet");
    case kInt32DivS:
      FATAL("wasm32 signed div emission is not implemented yet");
    case kInt32DivU:
      FATAL("wasm32 unsigned div emission is not implemented yet");
    case kInt32ModS:
      FATAL("wasm32 signed mod emission is not implemented yet");
    case kInt32ModU:
      FATAL("wasm32 unsigned mod emission is not implemented yet");
    case kWasm32And:
      FATAL("wasm32 and emission is not implemented yet");
    case kWasm32Or:
      FATAL("wasm32 or emission is not implemented yet");
    case kWasm32Xor:
      FATAL("wasm32 xor emission is not implemented yet");
    case kWasm32Shl:
      FATAL("wasm32 shl emission is not implemented yet");
    case kWasm32ShrU:
      FATAL("wasm32 shr_u emission is not implemented yet");
    case kWasm32ShrS:
      FATAL("wasm32 shr_s emission is not implemented yet");
    case kWasm32Eq:
      FATAL("wasm32 eq emission is not implemented yet");
    case kWasm32Ne:
      FATAL("wasm32 ne emission is not implemented yet");
    case kWasm32LtS:
      FATAL("wasm32 lt_s emission is not implemented yet");
    case kWasm32LtU:
      FATAL("wasm32 lt_u emission is not implemented yet");
    case kWasm32LeS:
      FATAL("wasm32 le_s emission is not implemented yet");
    case kWasm32LeU:
      FATAL("wasm32 le_u emission is not implemented yet");
    case kWasm32GtS:
      FATAL("wasm32 gt_s emission is not implemented yet");
    case kWasm32GtU:
      FATAL("wasm32 gt_u emission is not implemented yet");
    case kWasm32GeS:
      FATAL("wasm32 ge_s emission is not implemented yet");
    case kWasm32GeU:
      FATAL("wasm32 ge_u emission is not implemented yet");
    case kS128Zero:
      FATAL("wasm32 s128.zero emission is not implemented yet");
    case kWasm32CallBuiltin:
      FATAL("wasm32 builtin call emission is not implemented yet");
    case kWasm32CallRuntime:
      FATAL("wasm32 runtime call emission is not implemented yet");
    case kWasm32Return:
      FATAL("wasm32 return emission is not implemented yet");
    case kArchRet:
      FATAL("wasm32 arch return emission is not implemented yet");
    case kArchPrepareCallCFunction:
      FATAL("wasm32 C function call preparation is not implemented yet");
    case kArchStackPointerGreaterThan:
      FATAL("wasm32 stack-pointer check lowering is not implemented yet");
#if V8_ENABLE_WEBASSEMBLY
    case kArchSetStackPointer:
      FATAL("wasm32 set-stack-pointer lowering is not implemented yet");
#endif  // V8_ENABLE_WEBASSEMBLY
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

void CodeGenerator::AssembleJumpTable(ZoneVector<Label*> targets) {
  USE(targets);
  FATAL("wasm32 jump table emission requires dispatch-loop lowering");
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
  FATAL("wasm32 non-redundant move lowering is not implemented yet");
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
