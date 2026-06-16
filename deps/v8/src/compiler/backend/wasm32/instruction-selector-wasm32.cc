// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <cstdint>

#include "src/base/logging.h"
#include "src/codegen/machine-type.h"
#include "src/common/globals.h"
#include "src/compiler/backend/instruction-codes.h"
#include "src/compiler/backend/instruction-selector-adapter.h"
#include "src/compiler/backend/instruction-selector-impl.h"
#include "src/compiler/backend/instruction-selector.h"
#include "src/compiler/backend/instruction.h"
#include "src/compiler/linkage.h"
#include "src/compiler/machine-operator.h"
#include "src/compiler/turboshaft/operations.h"
#include "src/compiler/turboshaft/opmasks.h"
#include "src/compiler/turboshaft/representations.h"

#if V8_TARGET_ARCH_WASM32

namespace v8 {
namespace internal {
namespace compiler {

using namespace turboshaft;  // NOLINT(build/namespaces)

namespace {

class Wasm32OperandGeneratorT final : public OperandGeneratorT {
 public:
  explicit Wasm32OperandGeneratorT(InstructionSelectorT* selector)
      : OperandGeneratorT(selector) {}

  InstructionOperand UseRegisterOrImmediateZero(OpIndex node) {
    if (MatchZero(node)) return UseImmediate(node);
    return UseRegister(node);
  }

  InstructionOperand UseOperand(OpIndex node) {
    int64_t unused;
    if (selector()->MatchSignedIntegralConstant(node, &unused)) {
      return UseImmediate(node);
    }
    return UseRegister(node);
  }
};

ArchOpcode SelectLoadOpcode(LoadRepresentation load_rep) {
  switch (load_rep.representation()) {
    case MachineRepresentation::kBit:
    case MachineRepresentation::kWord8:
      return load_rep.IsUnsigned() ? kWasm32LoadMem8U : kWasm32LoadMem8S;
    case MachineRepresentation::kWord16:
      return load_rep.IsUnsigned() ? kWasm32LoadMem16U : kWasm32LoadMem16S;
    case MachineRepresentation::kTaggedSigned:
    case MachineRepresentation::kTaggedPointer:
    case MachineRepresentation::kTagged:
    case MachineRepresentation::kWord32:
    case MachineRepresentation::kCompressedPointer:
    case MachineRepresentation::kCompressed:
      return kLoadI32;
    case MachineRepresentation::kFloat64:
      return kLoadF64;
    case MachineRepresentation::kFloat32:
    case MachineRepresentation::kSandboxedPointer:
    case MachineRepresentation::kMapWord:
    case MachineRepresentation::kWord64:
    case MachineRepresentation::kNone:
    case MachineRepresentation::kSimd128:
    case MachineRepresentation::kSimd256:
    case MachineRepresentation::kProtectedPointer:
    case MachineRepresentation::kIndirectPointer:
    case MachineRepresentation::kFloat16:
    case MachineRepresentation::kFloat16RawBits:
      return kLoadI32;
  }
}

ArchOpcode SelectStoreOpcode(StoreRepresentation store_rep) {
  switch (store_rep.representation()) {
    case MachineRepresentation::kBit:
    case MachineRepresentation::kWord8:
      return kWasm32StoreMem8;
    case MachineRepresentation::kWord16:
      return kWasm32StoreMem16;
    case MachineRepresentation::kTaggedSigned:
    case MachineRepresentation::kTaggedPointer:
    case MachineRepresentation::kTagged:
    case MachineRepresentation::kWord32:
    case MachineRepresentation::kCompressedPointer:
    case MachineRepresentation::kCompressed:
      return kStoreI32;
    case MachineRepresentation::kFloat64:
      return kStoreF64;
    case MachineRepresentation::kFloat32:
    case MachineRepresentation::kSandboxedPointer:
    case MachineRepresentation::kMapWord:
    case MachineRepresentation::kWord64:
    case MachineRepresentation::kNone:
    case MachineRepresentation::kSimd128:
    case MachineRepresentation::kSimd256:
    case MachineRepresentation::kProtectedPointer:
    case MachineRepresentation::kIndirectPointer:
    case MachineRepresentation::kFloat16:
    case MachineRepresentation::kFloat16RawBits:
      return kStoreI32;
  }
}

AddressingMode SelectMemoryAddressingMode(uint8_t element_size_log2) {
  switch (element_size_log2) {
    case 0:
      return kMode_MR1I;
    case 1:
      return kMode_MR2I;
    case 2:
      return kMode_MR4I;
    case 3:
      return kMode_MR8I;
    default:
      return kMode_MR1I;
  }
}

void EmitLoad(InstructionSelectorT* selector, OpIndex node,
              InstructionCode opcode) {
  Wasm32OperandGeneratorT g(selector);
  auto load = selector->load_view(node);
  OpIndex base = load.base();
  OpIndex index = load.index();
  int32_t displacement = load.displacement();
  InstructionOperand output = g.DefineAsRegister(node);

  if (selector->is_load_root_register(base) && !index.valid()) {
    opcode |= AddressingModeField::encode(kMode_Root);
    InstructionOperand inputs[] = {g.TempImmediate(displacement)};
    selector->Emit(opcode, 1, &output, arraysize(inputs), inputs);
    return;
  }

  if (index.valid()) {
    InstructionOperand inputs[] = {g.UseRegister(base), g.UseRegister(index),
                                   g.TempImmediate(displacement)};
    selector->Emit(opcode | AddressingModeField::encode(
                                SelectMemoryAddressingMode(
                                    load.element_size_log2())),
                   1, &output, arraysize(inputs), inputs);
  } else {
    InstructionOperand inputs[] = {g.UseRegister(base),
                                   g.TempImmediate(displacement)};
    selector->Emit(opcode | AddressingModeField::encode(kMode_MRI), 1, &output,
                   arraysize(inputs), inputs);
  }
}

void EmitStore(InstructionSelectorT* selector, OpIndex node,
               InstructionCode opcode) {
  Wasm32OperandGeneratorT g(selector);
  auto store = selector->store_view(node);
  OpIndex base = store.base();
  OptionalOpIndex index = store.index();
  OpIndex value = store.value();
  int32_t displacement = store.displacement();

  if (selector->is_load_root_register(base) && !index.has_value()) {
    InstructionOperand inputs[] = {g.UseRegisterOrImmediateZero(value),
                                   g.TempImmediate(displacement)};
    selector->Emit(opcode | AddressingModeField::encode(kMode_Root), 0,
                   nullptr, arraysize(inputs), inputs);
    return;
  }

  if (index.has_value()) {
    InstructionOperand inputs[] = {g.UseRegisterOrImmediateZero(value),
                                   g.UseRegister(base),
                                   g.UseRegister(selector->value(index)),
                                   g.TempImmediate(displacement)};
    selector->Emit(opcode | AddressingModeField::encode(
                                SelectMemoryAddressingMode(
                                    store.element_size_log2())),
                   0, nullptr, arraysize(inputs), inputs);
  } else {
    InstructionOperand inputs[] = {g.UseRegisterOrImmediateZero(value),
                                   g.UseRegister(base),
                                   g.TempImmediate(displacement)};
    selector->Emit(opcode | AddressingModeField::encode(kMode_MRI), 0, nullptr,
                   arraysize(inputs), inputs);
  }
}

void VisitRRR(InstructionSelectorT* selector, InstructionCode opcode,
              OpIndex node) {
  Wasm32OperandGeneratorT g(selector);
  selector->Emit(opcode, g.DefineAsRegister(node),
                 g.UseRegister(selector->input_at(node, 0)),
                 g.UseOperand(selector->input_at(node, 1)));
}

void VisitOverflowBinop(InstructionSelectorT* selector, InstructionCode opcode,
                        OpIndex node) {
  Wasm32OperandGeneratorT g(selector);
  InstructionOperand inputs[] = {g.UseRegister(selector->input_at(node, 0)),
                                 g.UseOperand(selector->input_at(node, 1))};
  InstructionOperand output = g.DefineAsRegister(node);
  OptionalOpIndex overflow = selector->FindProjection(node, 1);
  FlagsContinuationT cont =
      overflow.valid() ? FlagsContinuationT::ForSet(kOverflow, overflow.value())
                       : FlagsContinuationT();
  selector->EmitWithContinuation(opcode, 1, &output, arraysize(inputs), inputs,
                                 &cont);
}

void VisitZeroResult(InstructionSelectorT* selector, OpIndex node) {
  Wasm32OperandGeneratorT g(selector);
  selector->Emit(kWasm32I32Const, g.DefineAsRegister(node), g.TempImmediate(0));
}

void VisitSignExtend(InstructionSelectorT* selector, OpIndex node, int shift) {
  Wasm32OperandGeneratorT g(selector);
  int temp_vreg = g.AllocateVirtualRegister();
  selector->Emit(kWasm32Shl, g.DefineAsRegistertForVreg(temp_vreg),
                 g.UseRegister(selector->input_at(node, 0)),
                 g.TempImmediate(shift));
  selector->Emit(kWasm32ShrS, g.DefineAsRegister(node),
                 g.UseRegisterForVreg(temp_vreg), g.TempImmediate(shift));
}

void VisitInt32PairBinop(InstructionSelectorT* selector,
                         InstructionCode opcode, OpIndex node) {
  Wasm32OperandGeneratorT g(selector);
  const Word32PairBinopOp& op = selector->Cast<Word32PairBinopOp>(node);
  selector->Emit(opcode, g.DefineAsRegister(node), g.UseRegister(op.left_low()),
                 g.UseOperand(op.right_low()));

  OptionalOpIndex projection1 = selector->FindProjection(node, 1);
  if (!projection1.valid()) return;

  if (opcode == kWasm32Mul) {
    // Generation bridge: wasm32 does not yet have a real high-word multiply
    // lowering. Define the high projection so builtins can be serialized.
    selector->Emit(kWasm32I32Const, g.DefineAsRegister(projection1.value()),
                   g.TempImmediate(0));
    return;
  }

  selector->Emit(opcode, g.DefineAsRegister(projection1.value()),
                 g.UseRegister(op.left_high()), g.UseOperand(op.right_high()));
}

void VisitPairZeroResult(InstructionSelectorT* selector, OpIndex node) {
  VisitZeroResult(selector, node);
  OptionalOpIndex projection1 = selector->FindProjection(node, 1);
  if (projection1.valid()) VisitZeroResult(selector, projection1.value());
}

void VisitCompare(InstructionSelectorT* selector, InstructionCode opcode,
                  OpIndex node) {
  const Operation& op = selector->Get(node);
  DCHECK(op.Is<ComparisonOp>());
  Wasm32OperandGeneratorT g(selector);
  selector->Emit(opcode, g.DefineAsRegister(node), g.UseRegister(op.input(0)),
                 g.UseOperand(op.input(1)));
}

}  // namespace

void InstructionSelectorT::VisitLoad(OpIndex node) {
  EmitLoad(this, node, SelectLoadOpcode(this->load_view(node).loaded_rep()));
}

void InstructionSelectorT::VisitProtectedLoad(OpIndex node) { VisitLoad(node); }

void InstructionSelectorT::VisitStore(OpIndex node) {
  auto store = this->store_view(node);
  // TODO(wasm32): Lower write barriers through the generated builtin/runtime
  // call path. During mksnapshot generation the emitted builtin bodies are not
  // executed, so select the underlying memory store to keep generation moving.
  EmitStore(this, node, SelectStoreOpcode(store.stored_rep()));
}

void InstructionSelectorT::VisitProtectedStore(OpIndex node) {
  VisitStore(node);
}

void InstructionSelectorT::VisitUnalignedLoad(OpIndex node) {
  VisitLoad(node);
}

void InstructionSelectorT::VisitUnalignedStore(OpIndex node) {
  VisitStore(node);
}

void InstructionSelectorT::VisitWord32And(OpIndex node) {
  VisitRRR(this, kWasm32And, node);
}

void InstructionSelectorT::VisitWord32Or(OpIndex node) {
  VisitRRR(this, kWasm32Or, node);
}

void InstructionSelectorT::VisitWord32Xor(OpIndex node) {
  VisitRRR(this, kWasm32Xor, node);
}

void InstructionSelectorT::VisitWord32Shl(OpIndex node) {
  VisitRRR(this, kWasm32Shl, node);
}

void InstructionSelectorT::VisitWord32Shr(OpIndex node) {
  VisitRRR(this, kWasm32ShrU, node);
}

void InstructionSelectorT::VisitWord32Sar(OpIndex node) {
  VisitRRR(this, kWasm32ShrS, node);
}

void InstructionSelectorT::VisitWord32Equal(OpIndex node) {
  VisitCompare(this, kWasm32Eq, node);
}

void InstructionSelectorT::VisitInt32Add(OpIndex node) {
  VisitRRR(this, kWasm32Add, node);
}

void InstructionSelectorT::VisitInt32Sub(OpIndex node) {
  VisitRRR(this, kWasm32Sub, node);
}

void InstructionSelectorT::VisitInt32Mul(OpIndex node) {
  VisitRRR(this, kWasm32Mul, node);
}

void InstructionSelectorT::VisitInt32AddWithOverflow(OpIndex node) {
  VisitOverflowBinop(this, kWasm32Add, node);
}

void InstructionSelectorT::VisitInt32SubWithOverflow(OpIndex node) {
  VisitOverflowBinop(this, kWasm32Sub, node);
}

void InstructionSelectorT::VisitInt32MulWithOverflow(OpIndex node) {
  VisitOverflowBinop(this, kWasm32Mul, node);
}

void InstructionSelectorT::VisitInt32MulHigh(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitUint32MulHigh(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitInt32PairAdd(OpIndex node) {
  VisitInt32PairBinop(this, kWasm32Add, node);
}

void InstructionSelectorT::VisitInt32PairSub(OpIndex node) {
  VisitInt32PairBinop(this, kWasm32Sub, node);
}

void InstructionSelectorT::VisitInt32PairMul(OpIndex node) {
  VisitInt32PairBinop(this, kWasm32Mul, node);
}

void InstructionSelectorT::VisitWord32PairShl(OpIndex node) {
  VisitPairZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32PairShr(OpIndex node) {
  VisitPairZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32PairSar(OpIndex node) {
  VisitPairZeroResult(this, node);
}

void InstructionSelectorT::VisitInt32Div(OpIndex node) {
  VisitRRR(this, kInt32DivS, node);
}

void InstructionSelectorT::VisitUint32Div(OpIndex node) {
  VisitRRR(this, kInt32DivU, node);
}

void InstructionSelectorT::VisitInt32Mod(OpIndex node) {
  VisitRRR(this, kInt32ModS, node);
}

void InstructionSelectorT::VisitUint32Mod(OpIndex node) {
  VisitRRR(this, kInt32ModU, node);
}

void InstructionSelectorT::VisitInt32LessThan(OpIndex node) {
  VisitCompare(this, kWasm32LtS, node);
}

void InstructionSelectorT::VisitInt32LessThanOrEqual(OpIndex node) {
  VisitCompare(this, kWasm32LeS, node);
}

void InstructionSelectorT::VisitUint32LessThan(OpIndex node) {
  VisitCompare(this, kWasm32LtU, node);
}

void InstructionSelectorT::VisitUint32LessThanOrEqual(OpIndex node) {
  VisitCompare(this, kWasm32LeU, node);
}

void InstructionSelectorT::VisitSignExtendWord8ToInt32(OpIndex node) {
  VisitSignExtend(this, node, 24);
}

void InstructionSelectorT::VisitSignExtendWord16ToInt32(OpIndex node) {
  VisitSignExtend(this, node, 16);
}

void InstructionSelectorT::VisitWord32Clz(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32Ctz(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32Popcnt(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32ReverseBytes(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32Rol(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32Ror(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitStackPointerGreaterThan(
    OpIndex node, FlagsContinuation* cont) {
  const StackPointerGreaterThanOp& op = Cast<StackPointerGreaterThanOp>(node);
  InstructionCode opcode =
      kArchStackPointerGreaterThan |
      StackCheckField::encode(static_cast<StackCheckKind>(op.kind));

  Wasm32OperandGeneratorT g(this);
  InstructionOperand temps[] = {g.TempRegister()};
  size_t temp_count = op.kind == StackCheckKind::kJSFunctionEntry ? 1 : 0;
  OperandGenerator::RegisterMode register_mode =
      op.kind == StackCheckKind::kJSFunctionEntry
          ? OperandGenerator::kUniqueRegister
          : OperandGenerator::kRegister;
  InstructionOperand input =
      g.UseRegisterWithMode(op.stack_limit(), register_mode);
  EmitWithContinuation(opcode, 0, nullptr, 1, &input, temp_count, temps, cont);
}

void InstructionSelectorT::VisitWordCompareZero(OpIndex user, OpIndex value,
                                                FlagsContinuation* cont) {
  Wasm32OperandGeneratorT g(this);
  USE(user);
  EmitWithContinuation(kWasm32Ne, g.UseRegister(value), g.TempImmediate(0),
                       cont);
}

void InstructionSelectorT::VisitStackSlot(OpIndex node) {
  const StackSlotOp& stack_slot = Cast<StackSlotOp>(node);
  int slot = frame_->AllocateSpillSlot(stack_slot.size, stack_slot.alignment,
                                       stack_slot.is_tagged);
  Wasm32OperandGeneratorT g(this);
  Emit(kArchStackSlot, g.DefineAsRegister(node),
       sequence()->AddImmediate(Constant(slot)), 0, nullptr);
}

void InstructionSelectorT::VisitMemoryBarrier(OpIndex node) {
  Wasm32OperandGeneratorT g(this);
  USE(node);
  Emit(kArchNop, g.NoOutput());
}

void InstructionSelectorT::VisitAbortCSADcheck(OpIndex node) {
  Wasm32OperandGeneratorT g(this);
  const AbortCSADcheckOp& check = Cast<AbortCSADcheckOp>(node);
  DCHECK_EQ(check.input_count, 1);
  Emit(kArchAbortCSADcheck, g.NoOutput(), g.UseRegister(check.message()));
}

void InstructionSelectorT::VisitWord32AtomicLoad(OpIndex node) {
  VisitLoad(node);
}

void InstructionSelectorT::VisitWord32AtomicStore(OpIndex node) {
  VisitStore(node);
}

void InstructionSelectorT::VisitWord32AtomicAdd(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicSub(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicAnd(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicOr(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicXor(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicExchange(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicCompareExchange(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord64AtomicLoad(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord64AtomicStore(OpIndex node) {
  Wasm32OperandGeneratorT g(this);
  USE(node);
  Emit(kArchNop, g.NoOutput());
}

void InstructionSelectorT::VisitWord64AtomicAdd(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord64AtomicSub(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord64AtomicAnd(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord64AtomicOr(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord64AtomicXor(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord64AtomicExchange(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord64AtomicCompareExchange(OpIndex node) {
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicPairLoad(OpIndex node) {
  VisitPairZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicPairStore(OpIndex node) {
  Wasm32OperandGeneratorT g(this);
  USE(node);
  Emit(kArchNop, g.NoOutput());
}

void InstructionSelectorT::VisitWord32AtomicPairAdd(OpIndex node) {
  VisitPairZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicPairSub(OpIndex node) {
  VisitPairZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicPairAnd(OpIndex node) {
  VisitPairZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicPairOr(OpIndex node) {
  VisitPairZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicPairXor(OpIndex node) {
  VisitPairZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicPairExchange(OpIndex node) {
  VisitPairZeroResult(this, node);
}

void InstructionSelectorT::VisitWord32AtomicPairCompareExchange(OpIndex node) {
  VisitPairZeroResult(this, node);
}

#if V8_ENABLE_WEBASSEMBLY
void InstructionSelectorT::VisitSetStackPointer(OpIndex node) {
  Wasm32OperandGeneratorT g(this);
  const SetStackPointerOp& op = Cast<SetStackPointerOp>(node);
  DCHECK_EQ(op.input_count, 1);
  InstructionOperand input = g.UseAny(op.value());
  Emit(kArchSetStackPointer, 0, nullptr, 1, &input);
}
#endif  // V8_ENABLE_WEBASSEMBLY

void InstructionSelectorT::EmitPrepareArguments(
    ZoneVector<PushParameter>* arguments, const CallDescriptor* call_descriptor,
    OpIndex node) {
  Wasm32OperandGeneratorT g(this);
  USE(node);

  bool is_c_function_call = call_descriptor->IsCFunctionCall();
  if (call_descriptor->IsCFunctionCall()) {
    Emit(kArchPrepareCallCFunction | MiscField::encode(static_cast<int>(
                                         call_descriptor->ParameterCount())),
         0, nullptr, 0, nullptr);
  }

  for (size_t n = 0; n < arguments->size(); ++n) {
    PushParameter input = (*arguments)[n];
    if (!input.node.valid()) continue;
    int slot = static_cast<int>(n);
    InstructionCode opcode =
        is_c_function_call ? kWasm32StoreOutgoingSlot : kWasm32StoreSlot;
    Emit(opcode, g.NoOutput(), g.UseRegister(input.node),
         g.TempImmediate(slot << kSystemPointerSizeLog2));
  }
}

void InstructionSelectorT::EmitPrepareResults(
    ZoneVector<PushParameter>* results, const CallDescriptor* call_descriptor,
    OpIndex node) {
  Wasm32OperandGeneratorT g(this);
  USE(node);
  DCHECK(!call_descriptor->IsCFunctionCall());

  for (PushParameter output : *results) {
    if (!output.location.IsCallerFrameSlot()) continue;
    if (!output.node.valid()) continue;
    if (output.location.GetType() == MachineType::Float32()) {
      MarkAsFloat32(output.node);
    } else if (output.location.GetType() == MachineType::Float64()) {
      MarkAsFloat64(output.node);
    } else if (output.location.GetType() == MachineType::Simd128()) {
      MarkAsSimd128(output.node);
    }
    int offset = call_descriptor->GetOffsetToReturns();
    int reverse_slot = -output.location.GetLocation() - offset;
    Emit(kWasm32LoadSlot, g.DefineAsRegister(output.node),
         g.TempImmediate(reverse_slot << kSystemPointerSizeLog2));
  }
}

void InstructionSelectorT::VisitSwitch(OpIndex node, const SwitchInfo& sw) {
  // TODO(wasm32): Lower through the dispatch-loop CFG spine.
  Wasm32OperandGeneratorT g(this);
  EmitBinarySearchSwitch(sw, g.UseRegister(Cast<SwitchOp>(node).input()));
}

#define VISIT_UNSUPPORTED_WASM32_OP(op) \
  void InstructionSelectorT::Visit##op(OpIndex) { UNIMPLEMENTED(); }

#define VISIT_ZERO_WASM32_OP(op) \
  void InstructionSelectorT::Visit##op(OpIndex node) { VisitZeroResult(this, node); }

VISIT_ZERO_WASM32_OP(TruncateFloat64ToFloat32)
VISIT_ZERO_WASM32_OP(ChangeFloat64ToInt32)
VISIT_ZERO_WASM32_OP(TruncateFloat64ToWord32)
VISIT_ZERO_WASM32_OP(TruncateFloat64ToFloat16RawBits)
VISIT_ZERO_WASM32_OP(ChangeFloat16RawBitsToFloat64)
VISIT_ZERO_WASM32_OP(RoundInt32ToFloat32)
VISIT_ZERO_WASM32_OP(ChangeInt32ToFloat64)
VISIT_ZERO_WASM32_OP(RoundUint32ToFloat32)
VISIT_ZERO_WASM32_OP(ChangeUint32ToFloat64)
VISIT_ZERO_WASM32_OP(Float64ExtractHighWord32)
VISIT_ZERO_WASM32_OP(Float64ExtractLowWord32)
VISIT_ZERO_WASM32_OP(BitcastFloat32ToInt32)
VISIT_ZERO_WASM32_OP(BitcastInt32ToFloat32)
VISIT_ZERO_WASM32_OP(BitcastWord32PairToFloat64)
VISIT_ZERO_WASM32_OP(ChangeFloat32ToFloat64)
VISIT_ZERO_WASM32_OP(ChangeFloat64ToUint32)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Abs)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Add)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Ceil)
VISIT_UNSUPPORTED_WASM32_OP(F16x8DemoteF32x4Zero)
VISIT_UNSUPPORTED_WASM32_OP(F16x8DemoteF64x2Zero)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Div)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Eq)
VISIT_UNSUPPORTED_WASM32_OP(F16x8ExtractLane)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Floor)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Le)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Lt)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Max)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Min)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Mul)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Ne)
VISIT_UNSUPPORTED_WASM32_OP(F16x8NearestInt)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Neg)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Pmax)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Pmin)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Qfma)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Qfms)
VISIT_UNSUPPORTED_WASM32_OP(F16x8ReplaceLane)
VISIT_UNSUPPORTED_WASM32_OP(F16x8SConvertI16x8)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Splat)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Sqrt)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Sub)
VISIT_UNSUPPORTED_WASM32_OP(F16x8Trunc)
VISIT_UNSUPPORTED_WASM32_OP(F16x8UConvertI16x8)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Abs)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Add)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Ceil)
VISIT_UNSUPPORTED_WASM32_OP(F32x4DemoteF64x2Zero)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Div)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Eq)
VISIT_UNSUPPORTED_WASM32_OP(F32x4ExtractLane)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Floor)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Le)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Lt)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Max)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Min)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Mul)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Ne)
VISIT_UNSUPPORTED_WASM32_OP(F32x4NearestInt)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Neg)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Pmax)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Pmin)
VISIT_UNSUPPORTED_WASM32_OP(F32x4PromoteLowF16x8)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Qfma)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Qfms)
VISIT_UNSUPPORTED_WASM32_OP(F32x4RelaxedMax)
VISIT_UNSUPPORTED_WASM32_OP(F32x4RelaxedMin)
VISIT_UNSUPPORTED_WASM32_OP(F32x4ReplaceLane)
VISIT_UNSUPPORTED_WASM32_OP(F32x4SConvertI32x4)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Splat)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Sqrt)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Sub)
VISIT_UNSUPPORTED_WASM32_OP(F32x4Trunc)
VISIT_UNSUPPORTED_WASM32_OP(F32x4UConvertI32x4)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Abs)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Add)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Ceil)
VISIT_UNSUPPORTED_WASM32_OP(F64x2ConvertLowI32x4S)
VISIT_UNSUPPORTED_WASM32_OP(F64x2ConvertLowI32x4U)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Div)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Eq)
VISIT_UNSUPPORTED_WASM32_OP(F64x2ExtractLane)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Floor)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Le)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Lt)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Max)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Min)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Mul)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Ne)
VISIT_UNSUPPORTED_WASM32_OP(F64x2NearestInt)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Neg)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Pmax)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Pmin)
VISIT_UNSUPPORTED_WASM32_OP(F64x2PromoteLowF32x4)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Qfma)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Qfms)
VISIT_UNSUPPORTED_WASM32_OP(F64x2RelaxedMax)
VISIT_UNSUPPORTED_WASM32_OP(F64x2RelaxedMin)
VISIT_UNSUPPORTED_WASM32_OP(F64x2ReplaceLane)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Splat)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Sqrt)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Sub)
VISIT_UNSUPPORTED_WASM32_OP(F64x2Trunc)
VISIT_ZERO_WASM32_OP(Float32Abs)
VISIT_ZERO_WASM32_OP(Float32Add)
VISIT_ZERO_WASM32_OP(Float32Div)
VISIT_ZERO_WASM32_OP(Float32Equal)
VISIT_ZERO_WASM32_OP(Float32LessThan)
VISIT_ZERO_WASM32_OP(Float32LessThanOrEqual)
VISIT_ZERO_WASM32_OP(Float32Max)
VISIT_ZERO_WASM32_OP(Float32Min)
VISIT_ZERO_WASM32_OP(Float32Mul)
VISIT_ZERO_WASM32_OP(Float32Neg)
VISIT_ZERO_WASM32_OP(Float32RoundDown)
VISIT_ZERO_WASM32_OP(Float32RoundTiesEven)
VISIT_ZERO_WASM32_OP(Float32RoundTruncate)
VISIT_ZERO_WASM32_OP(Float32RoundUp)
VISIT_ZERO_WASM32_OP(Float32Sqrt)
VISIT_ZERO_WASM32_OP(Float32Sub)
VISIT_ZERO_WASM32_OP(Float64Abs)
VISIT_ZERO_WASM32_OP(Float64Add)
VISIT_ZERO_WASM32_OP(Float64Div)
VISIT_ZERO_WASM32_OP(Float64Equal)
VISIT_ZERO_WASM32_OP(Float64LessThan)
VISIT_ZERO_WASM32_OP(Float64LessThanOrEqual)
VISIT_ZERO_WASM32_OP(Float64Max)
VISIT_ZERO_WASM32_OP(Float64Min)
VISIT_ZERO_WASM32_OP(Float64Mod)
VISIT_ZERO_WASM32_OP(Float64Mul)
VISIT_ZERO_WASM32_OP(Float64Neg)
VISIT_ZERO_WASM32_OP(Float64RoundDown)
VISIT_ZERO_WASM32_OP(Float64RoundTiesEven)
VISIT_ZERO_WASM32_OP(Float64RoundTruncate)
VISIT_ZERO_WASM32_OP(Float64RoundUp)
VISIT_ZERO_WASM32_OP(Float64SilenceNaN)
VISIT_ZERO_WASM32_OP(Float64Sqrt)
VISIT_ZERO_WASM32_OP(Float64Sub)
VISIT_UNSUPPORTED_WASM32_OP(I16x8Abs)
VISIT_UNSUPPORTED_WASM32_OP(I16x8Add)
VISIT_UNSUPPORTED_WASM32_OP(I16x8AddSatS)
VISIT_UNSUPPORTED_WASM32_OP(I16x8AddSatU)
VISIT_UNSUPPORTED_WASM32_OP(I16x8AllTrue)
VISIT_UNSUPPORTED_WASM32_OP(I16x8BitMask)
VISIT_UNSUPPORTED_WASM32_OP(I16x8DotI8x16I7x16S)
VISIT_UNSUPPORTED_WASM32_OP(I16x8Eq)
VISIT_UNSUPPORTED_WASM32_OP(I16x8ExtAddPairwiseI8x16S)
VISIT_UNSUPPORTED_WASM32_OP(I16x8ExtAddPairwiseI8x16U)
VISIT_UNSUPPORTED_WASM32_OP(I16x8ExtMulHighI8x16S)
VISIT_UNSUPPORTED_WASM32_OP(I16x8ExtMulHighI8x16U)
VISIT_UNSUPPORTED_WASM32_OP(I16x8ExtMulLowI8x16S)
VISIT_UNSUPPORTED_WASM32_OP(I16x8ExtMulLowI8x16U)
VISIT_UNSUPPORTED_WASM32_OP(I16x8ExtractLaneS)
VISIT_UNSUPPORTED_WASM32_OP(I16x8ExtractLaneU)
VISIT_UNSUPPORTED_WASM32_OP(I16x8GeS)
VISIT_UNSUPPORTED_WASM32_OP(I16x8GeU)
VISIT_UNSUPPORTED_WASM32_OP(I16x8GtS)
VISIT_UNSUPPORTED_WASM32_OP(I16x8GtU)
VISIT_UNSUPPORTED_WASM32_OP(I16x8MaxS)
VISIT_UNSUPPORTED_WASM32_OP(I16x8MaxU)
VISIT_UNSUPPORTED_WASM32_OP(I16x8MinS)
VISIT_UNSUPPORTED_WASM32_OP(I16x8MinU)
VISIT_UNSUPPORTED_WASM32_OP(I16x8Mul)
VISIT_UNSUPPORTED_WASM32_OP(I16x8Ne)
VISIT_UNSUPPORTED_WASM32_OP(I16x8Neg)
VISIT_UNSUPPORTED_WASM32_OP(I16x8Q15MulRSatS)
VISIT_UNSUPPORTED_WASM32_OP(I16x8RelaxedLaneSelect)
VISIT_UNSUPPORTED_WASM32_OP(I16x8RelaxedQ15MulRS)
VISIT_UNSUPPORTED_WASM32_OP(I16x8ReplaceLane)
VISIT_UNSUPPORTED_WASM32_OP(I16x8RoundingAverageU)
VISIT_UNSUPPORTED_WASM32_OP(I16x8SConvertF16x8)
VISIT_UNSUPPORTED_WASM32_OP(I16x8SConvertI32x4)
VISIT_UNSUPPORTED_WASM32_OP(I16x8SConvertI8x16High)
VISIT_UNSUPPORTED_WASM32_OP(I16x8SConvertI8x16Low)
VISIT_UNSUPPORTED_WASM32_OP(I16x8Shl)
VISIT_UNSUPPORTED_WASM32_OP(I16x8ShrS)
VISIT_UNSUPPORTED_WASM32_OP(I16x8ShrU)
VISIT_UNSUPPORTED_WASM32_OP(I16x8Splat)
VISIT_UNSUPPORTED_WASM32_OP(I16x8Sub)
VISIT_UNSUPPORTED_WASM32_OP(I16x8SubSatS)
VISIT_UNSUPPORTED_WASM32_OP(I16x8SubSatU)
VISIT_UNSUPPORTED_WASM32_OP(I16x8UConvertF16x8)
VISIT_UNSUPPORTED_WASM32_OP(I16x8UConvertI32x4)
VISIT_UNSUPPORTED_WASM32_OP(I16x8UConvertI8x16High)
VISIT_UNSUPPORTED_WASM32_OP(I16x8UConvertI8x16Low)
VISIT_UNSUPPORTED_WASM32_OP(I32x4Abs)
VISIT_UNSUPPORTED_WASM32_OP(I32x4Add)
VISIT_UNSUPPORTED_WASM32_OP(I32x4AllTrue)
VISIT_UNSUPPORTED_WASM32_OP(I32x4BitMask)
VISIT_UNSUPPORTED_WASM32_OP(I32x4DotI16x8S)
VISIT_UNSUPPORTED_WASM32_OP(I32x4DotI8x16I7x16AddS)
VISIT_UNSUPPORTED_WASM32_OP(I32x4Eq)
VISIT_UNSUPPORTED_WASM32_OP(I32x4ExtAddPairwiseI16x8S)
VISIT_UNSUPPORTED_WASM32_OP(I32x4ExtAddPairwiseI16x8U)
VISIT_UNSUPPORTED_WASM32_OP(I32x4ExtMulHighI16x8S)
VISIT_UNSUPPORTED_WASM32_OP(I32x4ExtMulHighI16x8U)
VISIT_UNSUPPORTED_WASM32_OP(I32x4ExtMulLowI16x8S)
VISIT_UNSUPPORTED_WASM32_OP(I32x4ExtMulLowI16x8U)
VISIT_UNSUPPORTED_WASM32_OP(I32x4ExtractLane)
VISIT_UNSUPPORTED_WASM32_OP(I32x4GeS)
VISIT_UNSUPPORTED_WASM32_OP(I32x4GeU)
VISIT_UNSUPPORTED_WASM32_OP(I32x4GtS)
VISIT_UNSUPPORTED_WASM32_OP(I32x4GtU)
VISIT_UNSUPPORTED_WASM32_OP(I32x4MaxS)
VISIT_UNSUPPORTED_WASM32_OP(I32x4MaxU)
VISIT_UNSUPPORTED_WASM32_OP(I32x4MinS)
VISIT_UNSUPPORTED_WASM32_OP(I32x4MinU)
VISIT_UNSUPPORTED_WASM32_OP(I32x4Mul)
VISIT_UNSUPPORTED_WASM32_OP(I32x4Ne)
VISIT_UNSUPPORTED_WASM32_OP(I32x4Neg)
VISIT_UNSUPPORTED_WASM32_OP(I32x4RelaxedLaneSelect)
VISIT_UNSUPPORTED_WASM32_OP(I32x4RelaxedTruncF32x4S)
VISIT_UNSUPPORTED_WASM32_OP(I32x4RelaxedTruncF32x4U)
VISIT_UNSUPPORTED_WASM32_OP(I32x4RelaxedTruncF64x2SZero)
VISIT_UNSUPPORTED_WASM32_OP(I32x4RelaxedTruncF64x2UZero)
VISIT_UNSUPPORTED_WASM32_OP(I32x4ReplaceLane)
VISIT_UNSUPPORTED_WASM32_OP(I32x4SConvertF32x4)
VISIT_UNSUPPORTED_WASM32_OP(I32x4SConvertI16x8High)
VISIT_UNSUPPORTED_WASM32_OP(I32x4SConvertI16x8Low)
VISIT_UNSUPPORTED_WASM32_OP(I32x4Shl)
VISIT_UNSUPPORTED_WASM32_OP(I32x4ShrS)
VISIT_UNSUPPORTED_WASM32_OP(I32x4ShrU)
VISIT_UNSUPPORTED_WASM32_OP(I32x4Splat)
VISIT_UNSUPPORTED_WASM32_OP(I32x4Sub)
VISIT_UNSUPPORTED_WASM32_OP(I32x4TruncSatF64x2SZero)
VISIT_UNSUPPORTED_WASM32_OP(I32x4TruncSatF64x2UZero)
VISIT_UNSUPPORTED_WASM32_OP(I32x4UConvertF32x4)
VISIT_UNSUPPORTED_WASM32_OP(I32x4UConvertI16x8High)
VISIT_UNSUPPORTED_WASM32_OP(I32x4UConvertI16x8Low)
VISIT_UNSUPPORTED_WASM32_OP(I64x2Abs)
VISIT_UNSUPPORTED_WASM32_OP(I64x2Add)
VISIT_UNSUPPORTED_WASM32_OP(I64x2AllTrue)
VISIT_UNSUPPORTED_WASM32_OP(I64x2BitMask)
VISIT_UNSUPPORTED_WASM32_OP(I64x2Eq)
VISIT_UNSUPPORTED_WASM32_OP(I64x2ExtMulHighI32x4S)
VISIT_UNSUPPORTED_WASM32_OP(I64x2ExtMulHighI32x4U)
VISIT_UNSUPPORTED_WASM32_OP(I64x2ExtMulLowI32x4S)
VISIT_UNSUPPORTED_WASM32_OP(I64x2ExtMulLowI32x4U)
VISIT_UNSUPPORTED_WASM32_OP(I64x2GeS)
VISIT_UNSUPPORTED_WASM32_OP(I64x2GtS)
VISIT_UNSUPPORTED_WASM32_OP(I64x2Mul)
VISIT_UNSUPPORTED_WASM32_OP(I64x2Ne)
VISIT_UNSUPPORTED_WASM32_OP(I64x2Neg)
VISIT_UNSUPPORTED_WASM32_OP(I64x2RelaxedLaneSelect)
VISIT_UNSUPPORTED_WASM32_OP(I64x2SConvertI32x4High)
VISIT_UNSUPPORTED_WASM32_OP(I64x2SConvertI32x4Low)
VISIT_UNSUPPORTED_WASM32_OP(I64x2Shl)
VISIT_UNSUPPORTED_WASM32_OP(I64x2ShrS)
VISIT_UNSUPPORTED_WASM32_OP(I64x2ShrU)
VISIT_UNSUPPORTED_WASM32_OP(I64x2Sub)
VISIT_UNSUPPORTED_WASM32_OP(I64x2UConvertI32x4High)
VISIT_UNSUPPORTED_WASM32_OP(I64x2UConvertI32x4Low)
VISIT_UNSUPPORTED_WASM32_OP(I8x16Abs)
VISIT_UNSUPPORTED_WASM32_OP(I8x16Add)
VISIT_UNSUPPORTED_WASM32_OP(I8x16AddSatS)
VISIT_UNSUPPORTED_WASM32_OP(I8x16AddSatU)
VISIT_UNSUPPORTED_WASM32_OP(I8x16AllTrue)
VISIT_UNSUPPORTED_WASM32_OP(I8x16BitMask)
VISIT_UNSUPPORTED_WASM32_OP(I8x16Eq)
VISIT_UNSUPPORTED_WASM32_OP(I8x16ExtractLaneS)
VISIT_UNSUPPORTED_WASM32_OP(I8x16ExtractLaneU)
VISIT_UNSUPPORTED_WASM32_OP(I8x16GeS)
VISIT_UNSUPPORTED_WASM32_OP(I8x16GeU)
VISIT_UNSUPPORTED_WASM32_OP(I8x16GtS)
VISIT_UNSUPPORTED_WASM32_OP(I8x16GtU)
VISIT_UNSUPPORTED_WASM32_OP(I8x16MaxS)
VISIT_UNSUPPORTED_WASM32_OP(I8x16MaxU)
VISIT_UNSUPPORTED_WASM32_OP(I8x16MinS)
VISIT_UNSUPPORTED_WASM32_OP(I8x16MinU)
VISIT_UNSUPPORTED_WASM32_OP(I8x16Ne)
VISIT_UNSUPPORTED_WASM32_OP(I8x16Neg)
VISIT_UNSUPPORTED_WASM32_OP(I8x16Popcnt)
VISIT_UNSUPPORTED_WASM32_OP(I8x16RelaxedLaneSelect)
VISIT_UNSUPPORTED_WASM32_OP(I8x16ReplaceLane)
VISIT_UNSUPPORTED_WASM32_OP(I8x16RoundingAverageU)
VISIT_UNSUPPORTED_WASM32_OP(I8x16SConvertI16x8)
VISIT_UNSUPPORTED_WASM32_OP(I8x16Shl)
VISIT_UNSUPPORTED_WASM32_OP(I8x16ShrS)
VISIT_UNSUPPORTED_WASM32_OP(I8x16ShrU)
VISIT_UNSUPPORTED_WASM32_OP(I8x16Shuffle)
VISIT_UNSUPPORTED_WASM32_OP(I8x16Splat)
VISIT_UNSUPPORTED_WASM32_OP(I8x16Sub)
VISIT_UNSUPPORTED_WASM32_OP(I8x16SubSatS)
VISIT_UNSUPPORTED_WASM32_OP(I8x16SubSatU)
VISIT_UNSUPPORTED_WASM32_OP(I8x16Swizzle)
VISIT_UNSUPPORTED_WASM32_OP(I8x16UConvertI16x8)
VISIT_UNSUPPORTED_WASM32_OP(Int32AbsWithOverflow)
VISIT_UNSUPPORTED_WASM32_OP(Int64AbsWithOverflow)
VISIT_UNSUPPORTED_WASM32_OP(LoadLane)
VISIT_UNSUPPORTED_WASM32_OP(LoadTransform)
VISIT_ZERO_WASM32_OP(RoundFloat64ToInt32)
VISIT_UNSUPPORTED_WASM32_OP(S128And)
VISIT_UNSUPPORTED_WASM32_OP(S128AndNot)
VISIT_UNSUPPORTED_WASM32_OP(S128Const)
VISIT_UNSUPPORTED_WASM32_OP(S128Not)
VISIT_UNSUPPORTED_WASM32_OP(S128Or)
VISIT_UNSUPPORTED_WASM32_OP(S128Select)
VISIT_UNSUPPORTED_WASM32_OP(S128Xor)
VISIT_UNSUPPORTED_WASM32_OP(S128Zero)
VISIT_UNSUPPORTED_WASM32_OP(Simd128ReverseBytes)
VISIT_UNSUPPORTED_WASM32_OP(StoreLane)
VISIT_ZERO_WASM32_OP(TruncateFloat32ToInt32)
VISIT_ZERO_WASM32_OP(TruncateFloat32ToUint32)
VISIT_ZERO_WASM32_OP(TruncateFloat64ToUint32)
VISIT_UNSUPPORTED_WASM32_OP(V128AnyTrue)
VISIT_UNSUPPORTED_WASM32_OP(Word64ReverseBytes)

void InstructionSelectorT::VisitFloat64Ieee754Binop(OpIndex node,
                                                    InstructionCode opcode) {
  USE(opcode);
  VisitZeroResult(this, node);
}

void InstructionSelectorT::VisitFloat64Ieee754Unop(OpIndex node,
                                                   InstructionCode opcode) {
  USE(opcode);
  VisitZeroResult(this, node);
}

#undef VISIT_UNSUPPORTED_WASM32_OP

void InstructionSelectorT::AddOutputToSelectContinuation(OperandGenerator*,
                                                         int,
                                                         OpIndex) {
  UNREACHABLE();
}

void InstructionSelectorT::EmitMoveParamToFPR(OpIndex, int) {}

void InstructionSelectorT::EmitMoveFPRToParam(InstructionOperand*,
                                              LinkageLocation) {}

bool InstructionSelectorT::IsTailCallAddressImmediate() { return false; }

// static
MachineOperatorBuilder::Flags
InstructionSelector::SupportedMachineOperatorFlags() {
  return MachineOperatorBuilder::kWord32ShiftIsSafe;
}

// static
MachineOperatorBuilder::AlignmentRequirements
InstructionSelector::AlignmentRequirements() {
  return MachineOperatorBuilder::AlignmentRequirements::
      FullUnalignedAccessSupport();
}

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
