// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if V8_TARGET_ARCH_WASM32

#include <cstdint>
#include <memory>
#include <string>

#include "src/builtins/wasm32/builtins-wasm32-abi.h"
#include "src/codegen/wasm32/register-wasm32.h"
#include "src/codegen/wasm32/wasm32-builtin-module.h"
#include "src/codegen/wasm32/wasm32-encoder.h"
#include "src/codegen/optimized-compilation-info.h"
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

constexpr int kWasmStackSlotBase = kWasmRegArg0 + 64;

thread_local std::unique_ptr<wasm32::WasmFunctionBuilder> current_body;
thread_local uint32_t current_scratch_local = 0;

void EnsureBody() {
  if (current_body != nullptr) return;
  current_body = std::make_unique<wasm32::WasmFunctionBuilder>(
      "wasm32_generated_builtin", wasm32::WasmValueType::kVoid);
  current_scratch_local = current_body->AddLocal(wasm32::WasmValueType::kI32);
}

wasm32::WasmFunctionBuilder& Body() {
  EnsureBody();
  return *current_body;
}

uint32_t ScratchLocal() {
  EnsureBody();
  return current_scratch_local;
}

bool HasBody() { return current_body != nullptr; }

std::vector<uint8_t> FinishCurrentWasmBody() {
  CHECK_NOT_NULL(current_body);
  std::vector<uint8_t> body = current_body->FinishBody();
  current_body.reset();
  current_scratch_local = 0;
  return body;
}

void ResetCurrentWasmBody() {
  current_body.reset();
  current_scratch_local = 0;
}

int StackIndexToSlot(int index) {
  if (index < 0) {
    FATAL("wasm32 negative stack slot %d would alias ABI slots", index);
  }
  int slot = kWasmStackSlotBase + index;
  if (slot < 0 || slot >= kWasmRegFileSize) {
    FATAL("wasm32 stack slot %d maps outside g_wasm_regs", index);
  }
  return slot;
}

int SlotForAllocatedOperand(InstructionOperand* operand) {
  if (operand->IsRegister()) {
    return WasmRegisterCodeToSlot(
        AllocatedOperand::cast(operand)->register_code());
  }
  if (operand->IsFPRegister()) {
    return WasmDoubleRegisterCodeToSlot(
        AllocatedOperand::cast(operand)->register_code());
  }
  if (operand->IsStackSlot() || operand->IsFloatStackSlot() ||
      operand->IsDoubleStackSlot()) {
    return StackIndexToSlot(AllocatedOperand::cast(operand)->index());
  }
  FATAL("wasm32 unsupported allocated operand kind");
}

void EmitI32Const(int32_t value) { Body().I32Const(value); }

void EmitSlotAddress(int slot) {
  if (slot < 0 || slot >= kWasmRegFileSize) {
    FATAL("wasm32 register slot %d is outside g_wasm_regs", slot);
  }
  EmitI32Const(static_cast<int32_t>(
      reinterpret_cast<uintptr_t>(&g_wasm_regs[slot])));
}

void LoadSlot(int slot) {
  EmitSlotAddress(slot);
  Body().Load32(2, 0);
}

void StoreSlot(int slot) {
  Body().LocalSet(ScratchLocal());
  EmitSlotAddress(slot);
  Body().LocalGet(ScratchLocal());
  Body().Store32(2, 0);
}

int FrameOffsetToSlot(int32_t offset) {
  return StackIndexToSlot(offset >> kSystemPointerSizeLog2);
}

class Wasm32OperandConverter final : public InstructionOperandConverter {
 public:
  Wasm32OperandConverter(CodeGenerator* gen, Instruction* instr)
      : InstructionOperandConverter(gen, instr) {}

  InstructionOperand* InputAt(size_t index) const {
    return instr_->InputAt(index);
  }

  InstructionCode opcode() const { return instr_->opcode(); }

  int OutputSlot() { return SlotForAllocatedOperand(instr_->Output()); }

  int InputInt32(size_t index) {
    Constant target = ToConstant(instr_->InputAt(index));
    if (!target.FitsInInt32()) {
      FATAL("wasm32 immediate does not fit in i32");
    }
    return target.ToInt32();
  }

  void LoadInput(size_t index) {
    InstructionOperand* operand = InputAt(index);
    if (operand->IsImmediate() || operand->IsConstant()) {
      Constant constant = ToConstant(operand);
      if (!constant.FitsInInt32()) {
        FATAL("wasm32 immediate operand does not fit in i32");
      }
      EmitI32Const(constant.ToInt32());
      return;
    }
    LoadSlot(SlotForAllocatedOperand(operand));
  }
};

uint32_t AddressScale(AddressingMode mode) {
  switch (mode) {
    case kMode_MR2I:
      return 2;
    case kMode_MR4I:
      return 4;
    case kMode_MR8I:
      return 8;
    case kMode_MR1I:
    case kMode_MRI:
    case kMode_Root:
      return 1;
    default:
      FATAL("wasm32 unsupported addressing mode %d", static_cast<int>(mode));
  }
}

void EmitAddress(Wasm32OperandConverter* i, Instruction* instr,
                 size_t first_input) {
  AddressingMode mode = AddressingModeField::decode(instr->opcode());
  switch (mode) {
    case kMode_Root:
      LoadSlot(kWasmRegRoot);
      EmitI32Const(i->InputInt32(first_input));
      Body().Opcode(wasm32::WasmOpcode::kI32Add);
      return;
    case kMode_MRI:
      i->LoadInput(first_input);
      EmitI32Const(i->InputInt32(first_input + 1));
      Body().Opcode(wasm32::WasmOpcode::kI32Add);
      return;
    case kMode_MR1I:
    case kMode_MR2I:
    case kMode_MR4I:
    case kMode_MR8I:
      i->LoadInput(first_input);
      i->LoadInput(first_input + 1);
      if (AddressScale(mode) != 1) {
        EmitI32Const(static_cast<int32_t>(AddressScale(mode)));
        Body().Opcode(wasm32::WasmOpcode::kI32Mul);
      }
      Body().Opcode(wasm32::WasmOpcode::kI32Add);
      EmitI32Const(i->InputInt32(first_input + 2));
      Body().Opcode(wasm32::WasmOpcode::kI32Add);
      return;
    default:
      FATAL("wasm32 unsupported addressing mode %d", static_cast<int>(mode));
  }
}

void EmitBinary(Wasm32OperandConverter* i, wasm32::WasmOpcode opcode) {
  i->LoadInput(0);
  i->LoadInput(1);
  Body().Opcode(opcode);
  StoreSlot(i->OutputSlot());
}

void EmitCompare(Wasm32OperandConverter* i, wasm32::WasmOpcode opcode) {
  FlagsMode mode = FlagsModeField::decode(i->opcode());
  if (mode != kFlags_none) {
    FATAL("wasm32 compare flags continuation mode %d is not implemented yet",
          static_cast<int>(mode));
  }
  EmitBinary(i, opcode);
}

void EmitOperandValue(CodeGenerator* gen, InstructionOperand* operand) {
  InstructionOperandConverter g(gen, nullptr);
  if (operand->IsImmediate() || operand->IsConstant()) {
    Constant constant = g.ToConstant(operand);
    if (!constant.FitsInInt32()) {
      FATAL("wasm32 move constant does not fit in i32");
    }
    EmitI32Const(constant.ToInt32());
    return;
  }
  if (operand->IsRegister() || operand->IsStackSlot()) {
    LoadSlot(SlotForAllocatedOperand(operand));
    return;
  }
  FATAL("wasm32 unsupported move source");
}

void StoreOperandValue(InstructionOperand* operand) {
  if (operand->IsRegister() || operand->IsStackSlot()) {
    StoreSlot(SlotForAllocatedOperand(operand));
    return;
  }
  FATAL("wasm32 unsupported move destination");
}

void EmitI32Load(Wasm32OperandConverter* i, Instruction* instr) {
  EmitAddress(i, instr, 0);
  Body().Load32(2, 0);
  StoreSlot(i->OutputSlot());
}

void EmitI32Store(Wasm32OperandConverter* i, Instruction* instr) {
  EmitAddress(i, instr, 1);
  i->LoadInput(0);
  Body().Store32(2, 0);
}

}  // namespace

CodeGenerator::CodeGenResult CodeGenerator::AssembleArchInstruction(
    Instruction* instr) {
  Wasm32OperandConverter i(this, instr);
  switch (instr->arch_opcode()) {
    case kArchNop:
    case kWasm32Nop:
      return kSuccess;
    case kWasm32I32Const:
      EmitI32Const(i.InputInt32(0));
      StoreSlot(i.OutputSlot());
      return kSuccess;
    case kWasm32LoadRoot:
      LoadSlot(kWasmRegRoot);
      StoreSlot(i.OutputSlot());
      return kSuccess;
    case kWasm32LoadSlot:
      LoadSlot(FrameOffsetToSlot(i.InputInt32(0)));
      StoreSlot(i.OutputSlot());
      return kSuccess;
    case kWasm32StoreSlot:
      i.LoadInput(0);
      StoreSlot(FrameOffsetToSlot(i.InputInt32(1)));
      return kSuccess;
    case kWasm32LoadMem8S:
      FATAL("wasm32 i32.load8_s emission is not implemented yet");
    case kWasm32LoadMem8U:
      FATAL("wasm32 i32.load8_u emission is not implemented yet");
    case kWasm32LoadMem16S:
      FATAL("wasm32 i32.load16_s emission is not implemented yet");
    case kWasm32LoadMem16U:
      FATAL("wasm32 i32.load16_u emission is not implemented yet");
    case kWasm32LoadMem32:
    case kLoadI32:
      EmitI32Load(&i, instr);
      return kSuccess;
    case kLoadF64:
      FATAL("wasm32 f64 load emission is not implemented yet");
    case kWasm32StoreMem8:
      FATAL("wasm32 i32.store8 emission is not implemented yet");
    case kWasm32StoreMem16:
      FATAL("wasm32 i32.store16 emission is not implemented yet");
    case kWasm32StoreMem32:
    case kStoreI32:
      EmitI32Store(&i, instr);
      return kSuccess;
    case kStoreF64:
      FATAL("wasm32 f64 store emission is not implemented yet");
    case kWasm32Add:
      EmitBinary(&i, wasm32::WasmOpcode::kI32Add);
      return kSuccess;
    case kWasm32Sub:
      EmitBinary(&i, wasm32::WasmOpcode::kI32Sub);
      return kSuccess;
    case kWasm32Mul:
      EmitBinary(&i, wasm32::WasmOpcode::kI32Mul);
      return kSuccess;
    case kInt32DivS:
      EmitBinary(&i, wasm32::WasmOpcode::kI32DivS);
      return kSuccess;
    case kInt32DivU:
      EmitBinary(&i, wasm32::WasmOpcode::kI32DivU);
      return kSuccess;
    case kInt32ModS:
      EmitBinary(&i, wasm32::WasmOpcode::kI32RemS);
      return kSuccess;
    case kInt32ModU:
      EmitBinary(&i, wasm32::WasmOpcode::kI32RemU);
      return kSuccess;
    case kWasm32And:
      EmitBinary(&i, wasm32::WasmOpcode::kI32And);
      return kSuccess;
    case kWasm32Or:
      EmitBinary(&i, wasm32::WasmOpcode::kI32Or);
      return kSuccess;
    case kWasm32Xor:
      EmitBinary(&i, wasm32::WasmOpcode::kI32Xor);
      return kSuccess;
    case kWasm32Shl:
      EmitBinary(&i, wasm32::WasmOpcode::kI32Shl);
      return kSuccess;
    case kWasm32ShrU:
      EmitBinary(&i, wasm32::WasmOpcode::kI32ShrU);
      return kSuccess;
    case kWasm32ShrS:
      EmitBinary(&i, wasm32::WasmOpcode::kI32ShrS);
      return kSuccess;
    case kWasm32Eq:
      EmitCompare(&i, wasm32::WasmOpcode::kI32Eq);
      return kSuccess;
    case kWasm32Ne:
      EmitCompare(&i, wasm32::WasmOpcode::kI32Ne);
      return kSuccess;
    case kWasm32LtS:
      EmitCompare(&i, wasm32::WasmOpcode::kI32LtS);
      return kSuccess;
    case kWasm32LtU:
      EmitCompare(&i, wasm32::WasmOpcode::kI32LtU);
      return kSuccess;
    case kWasm32LeS:
      EmitCompare(&i, wasm32::WasmOpcode::kI32LeS);
      return kSuccess;
    case kWasm32LeU:
      EmitCompare(&i, wasm32::WasmOpcode::kI32LeU);
      return kSuccess;
    case kWasm32GtS:
      EmitCompare(&i, wasm32::WasmOpcode::kI32GtS);
      return kSuccess;
    case kWasm32GtU:
      EmitCompare(&i, wasm32::WasmOpcode::kI32GtU);
      return kSuccess;
    case kWasm32GeS:
      EmitCompare(&i, wasm32::WasmOpcode::kI32GeS);
      return kSuccess;
    case kWasm32GeU:
      EmitCompare(&i, wasm32::WasmOpcode::kI32GeU);
      return kSuccess;
    case kS128Zero:
      FATAL("wasm32 s128.zero emission is not implemented yet");
    case kWasm32CallBuiltin:
      FATAL("wasm32 builtin call emission is not implemented yet");
    case kWasm32CallRuntime:
      FATAL("wasm32 runtime call emission is not implemented yet");
    case kWasm32Return:
      Body().Return();
      return kSuccess;
    case kArchRet:
      Body().Return();
      return kSuccess;
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

void CodeGenerator::FinishCode() {
  if (!HasBody()) return;
  if (Builtins::IsBuiltinId(info()->builtin())) {
    Builtin builtin = info()->builtin();
    wasm32::GeneratedBuiltinModule::Get().AddBuiltin(
        builtin, Builtins::name(builtin), FinishCurrentWasmBody());
    return;
  }
  ResetCurrentWasmBody();
}

void CodeGenerator::PrepareForDeoptimizationExits(
    ZoneDeque<DeoptimizationExit*>* exits) {
  USE(exits);
}

void CodeGenerator::AssembleMove(InstructionOperand* source,
                                 InstructionOperand* destination) {
  if (source->Equals(*destination)) return;
  if (source->IsFPRegister() || source->IsFloatStackSlot() ||
      source->IsDoubleStackSlot() || destination->IsFPRegister() ||
      destination->IsFloatStackSlot() || destination->IsDoubleStackSlot()) {
    FATAL("wasm32 floating-point move lowering is not implemented yet");
  }
  EmitOperandValue(this, source);
  StoreOperandValue(destination);
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
