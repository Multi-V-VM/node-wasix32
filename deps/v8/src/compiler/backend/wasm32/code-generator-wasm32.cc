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

constexpr int kWasmFixedFrameSlotBase = kWasmRegArg0 + 64;
constexpr int kWasmStackSlotBase = kWasmRegArg0 + 128;
constexpr int kWasmGapTempSlot = kWasmStackSlotBase - 1;

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
    int slot = kWasmFixedFrameSlotBase + (-index - 1);
    if (slot >= kWasmGapTempSlot) {
      FATAL("wasm32 fixed frame slot %d maps outside reserved window", index);
    }
    return slot;
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

int32_t BridgeConstantToI32(Constant constant) {
  switch (constant.type()) {
    case Constant::kInt32:
      return constant.ToInt32();
    case Constant::kInt64:
      return static_cast<int32_t>(constant.ToInt64());
    case Constant::kFloat32:
      return static_cast<int32_t>(constant.ToFloat32AsInt());
    case Constant::kFloat64:
      return static_cast<int32_t>(constant.ToFloat64().AsUint64());
    case Constant::kExternalReference:
    case Constant::kCompressedHeapObject:
    case Constant::kHeapObject:
    case Constant::kRpoNumber:
      return 0;
  }
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
    return BridgeConstantToI32(target);
  }

  void LoadInput(size_t index) {
    InstructionOperand* operand = InputAt(index);
    if (operand->IsImmediate() || operand->IsConstant()) {
      Constant constant = ToConstant(operand);
      EmitI32Const(BridgeConstantToI32(constant));
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

void EmitBinary(Wasm32OperandConverter* i, Instruction* instr,
                wasm32::WasmOpcode opcode) {
  i->LoadInput(0);
  i->LoadInput(1);
  Body().Opcode(opcode);
  StoreSlot(i->OutputSlot());
  for (size_t index = 1; index < instr->OutputCount(); ++index) {
    EmitI32Const(0);
    StoreSlot(SlotForAllocatedOperand(instr->OutputAt(index)));
  }
}

void EmitCompare(Wasm32OperandConverter* i, Instruction* instr,
                 wasm32::WasmOpcode opcode) {
  FlagsMode mode = FlagsModeField::decode(i->opcode());
  switch (mode) {
    case kFlags_none:
    case kFlags_set:
      EmitBinary(i, instr, opcode);
      return;
    case kFlags_branch:
    case kFlags_conditional_branch:
      // TODO(wasm32): Replace this mksnapshot-generation bridge with the
      // dispatch-loop CFG lowering described in the wasm32 backend design.
      i->LoadInput(0);
      i->LoadInput(1);
      Body().Opcode(opcode);
      Body().Opcode(wasm32::WasmOpcode::kDrop);
      return;
    default:
      FATAL("wasm32 compare flags continuation mode %d is not implemented yet",
            static_cast<int>(mode));
  }
}

void EmitOperandValue(CodeGenerator* gen, InstructionOperand* operand) {
  InstructionOperandConverter g(gen, nullptr);
  if (operand->IsImmediate() || operand->IsConstant()) {
    Constant constant = g.ToConstant(operand);
    EmitI32Const(BridgeConstantToI32(constant));
    return;
  }
  if (operand->IsRegister() || operand->IsFPRegister() ||
      operand->IsStackSlot() || operand->IsFloatStackSlot() ||
      operand->IsDoubleStackSlot()) {
    LoadSlot(SlotForAllocatedOperand(operand));
    return;
  }
  FATAL("wasm32 unsupported move source");
}

void StoreOperandValue(InstructionOperand* operand) {
  if (operand->IsRegister() || operand->IsFPRegister() ||
      operand->IsStackSlot() || operand->IsFloatStackSlot() ||
      operand->IsDoubleStackSlot()) {
    StoreSlot(SlotForAllocatedOperand(operand));
    return;
  }
  FATAL("wasm32 unsupported move destination");
}

void EmitZeroOutputs(Instruction* instr) {
  for (size_t index = 0; index < instr->OutputCount(); ++index) {
    EmitI32Const(0);
    StoreSlot(SlotForAllocatedOperand(instr->OutputAt(index)));
  }
}

void EmitI32Load(Wasm32OperandConverter* i, Instruction* instr,
                 wasm32::WasmOpcode opcode, uint32_t align_log2) {
  EmitAddress(i, instr, 0);
  Body().Load(opcode, align_log2, 0);
  StoreSlot(i->OutputSlot());
}

void EmitI32Store(Wasm32OperandConverter* i, Instruction* instr,
                  wasm32::WasmOpcode opcode, uint32_t align_log2) {
  EmitAddress(i, instr, 1);
  i->LoadInput(0);
  Body().Store(opcode, align_log2, 0);
}

}  // namespace

CodeGenerator::CodeGenResult CodeGenerator::AssembleArchInstruction(
    Instruction* instr) {
  Wasm32OperandConverter i(this, instr);
  switch (instr->arch_opcode()) {
    case kArchNop:
    case kWasm32Nop:
      return kSuccess;
    case kArchAbortCSADcheck:
    case kArchDebugBreak:
    case kArchComment:
    case kArchThrowTerminator:
    case kArchDeoptimize:
      // TODO(wasm32): Emit real trap/deopt/debug lowering once the dispatch
      // spine and deopt exits are implemented.
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
      EmitI32Load(&i, instr, wasm32::WasmOpcode::kI32Load8S, 0);
      return kSuccess;
    case kWasm32LoadMem8U:
      EmitI32Load(&i, instr, wasm32::WasmOpcode::kI32Load8U, 0);
      return kSuccess;
    case kWasm32LoadMem16S:
      EmitI32Load(&i, instr, wasm32::WasmOpcode::kI32Load16S, 1);
      return kSuccess;
    case kWasm32LoadMem16U:
      EmitI32Load(&i, instr, wasm32::WasmOpcode::kI32Load16U, 1);
      return kSuccess;
    case kWasm32LoadMem32:
    case kLoadI32:
      EmitI32Load(&i, instr, wasm32::WasmOpcode::kI32Load, 2);
      return kSuccess;
    case kLoadF64:
      // TODO(wasm32): Add real f64 slots/opcodes. For generation-only bodies,
      // keep the destination defined with zero bits.
      EmitZeroOutputs(instr);
      return kSuccess;
    case kWasm32StoreMem8:
      EmitI32Store(&i, instr, wasm32::WasmOpcode::kI32Store8, 0);
      return kSuccess;
    case kWasm32StoreMem16:
      EmitI32Store(&i, instr, wasm32::WasmOpcode::kI32Store16, 1);
      return kSuccess;
    case kWasm32StoreMem32:
    case kStoreI32:
      EmitI32Store(&i, instr, wasm32::WasmOpcode::kI32Store, 2);
      return kSuccess;
    case kStoreF64:
      // TODO(wasm32): Add real f64 store lowering.
      return kSuccess;
    case kWasm32Add:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32Add);
      return kSuccess;
    case kWasm32Sub:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32Sub);
      return kSuccess;
    case kWasm32Mul:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32Mul);
      return kSuccess;
    case kInt32DivS:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32DivS);
      return kSuccess;
    case kInt32DivU:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32DivU);
      return kSuccess;
    case kInt32ModS:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32RemS);
      return kSuccess;
    case kInt32ModU:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32RemU);
      return kSuccess;
    case kWasm32And:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32And);
      return kSuccess;
    case kWasm32Or:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32Or);
      return kSuccess;
    case kWasm32Xor:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32Xor);
      return kSuccess;
    case kWasm32Shl:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32Shl);
      return kSuccess;
    case kWasm32ShrU:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32ShrU);
      return kSuccess;
    case kWasm32ShrS:
      EmitBinary(&i, instr, wasm32::WasmOpcode::kI32ShrS);
      return kSuccess;
    case kWasm32Eq:
      EmitCompare(&i, instr, wasm32::WasmOpcode::kI32Eq);
      return kSuccess;
    case kWasm32Ne:
      EmitCompare(&i, instr, wasm32::WasmOpcode::kI32Ne);
      return kSuccess;
    case kWasm32LtS:
      EmitCompare(&i, instr, wasm32::WasmOpcode::kI32LtS);
      return kSuccess;
    case kWasm32LtU:
      EmitCompare(&i, instr, wasm32::WasmOpcode::kI32LtU);
      return kSuccess;
    case kWasm32LeS:
      EmitCompare(&i, instr, wasm32::WasmOpcode::kI32LeS);
      return kSuccess;
    case kWasm32LeU:
      EmitCompare(&i, instr, wasm32::WasmOpcode::kI32LeU);
      return kSuccess;
    case kWasm32GtS:
      EmitCompare(&i, instr, wasm32::WasmOpcode::kI32GtS);
      return kSuccess;
    case kWasm32GtU:
      EmitCompare(&i, instr, wasm32::WasmOpcode::kI32GtU);
      return kSuccess;
    case kWasm32GeS:
      EmitCompare(&i, instr, wasm32::WasmOpcode::kI32GeS);
      return kSuccess;
    case kWasm32GeU:
      EmitCompare(&i, instr, wasm32::WasmOpcode::kI32GeU);
      return kSuccess;
    case kS128Zero:
      FATAL("wasm32 s128.zero emission is not implemented yet");
    case kArchCallCodeObject:
    case kArchCallJSFunction:
#if V8_ENABLE_WEBASSEMBLY
    case kArchCallWasmFunction:
    case kArchCallWasmFunctionIndirect:
#endif  // V8_ENABLE_WEBASSEMBLY
    case kArchCallBuiltinPointer:
    case kWasm32CallBuiltin:
    case kWasm32CallRuntime:
      // TODO(wasm32): Marshal through g_regs and call_indirect/CEntry.
      EmitZeroOutputs(instr);
      return kSuccess;
    case kWasm32Return:
      Body().Return();
      return kSuccess;
    case kArchRet:
      Body().Return();
      return kSuccess;
    case kArchFramePointer:
    case kArchParentFramePointer:
#if V8_ENABLE_WEBASSEMBLY
    case kArchStackPointer:
#endif  // V8_ENABLE_WEBASSEMBLY
    case kArchStackCheckOffset:
    case kArchStackSlot:
    case kArchTruncateDoubleToI:
      EmitZeroOutputs(instr);
      return kSuccess;
    case kArchRootPointer:
      LoadSlot(kWasmRegRoot);
      StoreSlot(i.OutputSlot());
      return kSuccess;
    case kArchJmp:
      // TODO(wasm32): Lower through the dispatch-loop CFG spine.
      return kSuccess;
    case kArchBinarySearchSwitch:
    case kArchTableSwitch:
      // TODO(wasm32): Lower through the dispatch-loop CFG spine.
      return kSuccess;
    case kArchPrepareTailCall:
      return kSuccess;
    case kArchTailCallCodeObject:
    case kArchTailCallAddress:
#if V8_ENABLE_WEBASSEMBLY
    case kArchTailCallWasm:
    case kArchTailCallWasmIndirect:
#endif  // V8_ENABLE_WEBASSEMBLY
      // TODO(wasm32): Lower tail calls through uniform call_indirect.
      Body().Return();
      return kSuccess;
    case kArchSaveCallerRegisters:
    case kArchRestoreCallerRegisters:
      return kSuccess;
    case kArchPrepareCallCFunction:
      return kSuccess;
    case kArchCallCFunction:
    case kArchCallCFunctionWithFrameState:
      // TODO(wasm32): Lower C calls through the CEntry/runtime bridge. These
      // generated builtins are not executed while mksnapshot is only producing
      // their wasm bodies, so materialize zero outputs for now.
      EmitZeroOutputs(instr);
      return kSuccess;
    case kArchStackPointerGreaterThan:
      // TODO(wasm32): Compare against the emulated stack limit once wasm32
      // frames are wired. For generation-only bodies, report "not greater".
      EmitZeroOutputs(instr);
      return kSuccess;
#if V8_ENABLE_WEBASSEMBLY
    case kArchSetStackPointer:
      // TODO(wasm32): Wire to the emulated stack pointer.
      return kSuccess;
#endif  // V8_ENABLE_WEBASSEMBLY
    default:
      FATAL("wasm32 codegen unsupported arch opcode %d",
            static_cast<int>(instr->arch_opcode()));
  }
}

void CodeGenerator::AssembleArchJumpRegardlessOfAssemblyOrder(
    RpoNumber target) {
  USE(target);
  // TODO(wasm32): Lower through the dispatch-loop CFG spine.
}

void CodeGenerator::AssembleArchBranch(Instruction* instr,
                                       BranchInfo* branch) {
  USE(instr);
  USE(branch);
  // TODO(wasm32): Lower this through the dispatch-loop CFG spine. Branch
  // continuations are currently consumed by EmitCompare so mksnapshot can
  // finish builtin generation without executing generated builtin wasm.
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
  // Comparisons materialize their boolean result directly in EmitCompare.
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
  // TODO(wasm32): Lower through the dispatch-loop CFG spine.
}

void CodeGenerator::AssembleArchTableSwitch(Instruction* instr) {
  USE(instr);
  // TODO(wasm32): Lower through the dispatch-loop CFG spine.
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

void CodeGenerator::AssemblePrepareTailCall() {
  // TODO(wasm32): Implement tail-call stack preparation for executable bodies.
}

void CodeGenerator::AssembleTailCallBeforeGap(Instruction* instr,
                                              int first_unused_stack_slot) {
  USE(instr);
  USE(first_unused_stack_slot);
  // TODO(wasm32): Implement tail-call gap preparation for executable bodies.
}

void CodeGenerator::AssembleTailCallAfterGap(Instruction* instr,
                                             int first_unused_stack_slot) {
  USE(instr);
  USE(first_unused_stack_slot);
  // TODO(wasm32): Implement tail-call gap cleanup for executable bodies.
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
  EmitOperandValue(this, source);
  StoreOperandValue(destination);
}

void CodeGenerator::AssembleSwap(InstructionOperand* source,
                                 InstructionOperand* destination) {
  if (source->Equals(*destination)) return;
  EmitOperandValue(this, source);
  StoreSlot(kWasmGapTempSlot);
  EmitOperandValue(this, destination);
  StoreOperandValue(source);
  LoadSlot(kWasmGapTempSlot);
  StoreOperandValue(destination);
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
  USE(rep);
  EmitOperandValue(this, source);
  StoreSlot(kWasmGapTempSlot);
}

void CodeGenerator::MoveTempLocationTo(InstructionOperand* dest,
                                       MachineRepresentation rep) {
  USE(rep);
  LoadSlot(kWasmGapTempSlot);
  StoreOperandValue(dest);
}

void CodeGenerator::SetPendingMove(MoveOperands* move) { USE(move); }

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
