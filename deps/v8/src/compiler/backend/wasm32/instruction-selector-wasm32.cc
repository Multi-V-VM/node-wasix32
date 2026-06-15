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
      UNIMPLEMENTED();
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
      UNIMPLEMENTED();
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

  if (selector->is_load_root_register(base)) {
    opcode |= AddressingModeField::encode(kMode_Root);
    InstructionOperand inputs[] = {g.TempImmediate(displacement)};
    selector->Emit(opcode, 1, &output, arraysize(inputs), inputs);
    return;
  }

  if (index.valid()) {
    InstructionOperand inputs[] = {g.UseRegister(base), g.UseRegister(index),
                                   g.TempImmediate(displacement)};
    selector->Emit(opcode | AddressingModeField::encode(kMode_MR1I), 1,
                   &output, arraysize(inputs), inputs);
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

  if (selector->is_load_root_register(base)) {
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
    selector->Emit(opcode | AddressingModeField::encode(kMode_MR1I), 0,
                   nullptr, arraysize(inputs), inputs);
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
  if (store.stored_rep().write_barrier_kind() != kNoWriteBarrier) {
    UNIMPLEMENTED();
  }
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

  if (call_descriptor->IsCFunctionCall()) {
    Emit(kArchPrepareCallCFunction | MiscField::encode(static_cast<int>(
                                         call_descriptor->ParameterCount())),
         0, nullptr, 0, nullptr);
  }

  for (size_t n = 0; n < arguments->size(); ++n) {
    PushParameter input = (*arguments)[n];
    if (!input.node.valid()) continue;
    int slot = input.location.IsCallerFrameSlot()
                   ? input.location.GetLocation()
                   : static_cast<int>(n);
    Emit(kWasm32StoreSlot, g.NoOutput(), g.UseRegister(input.node),
         g.TempImmediate(slot << kSystemPointerSizeLog2));
  }
}

void InstructionSelectorT::EmitPrepareResults(
    ZoneVector<PushParameter>* results, const CallDescriptor* call_descriptor,
    OpIndex node) {
  Wasm32OperandGeneratorT g(this);
  USE(call_descriptor);
  USE(node);

  for (PushParameter output : *results) {
    if (!output.location.IsCallerFrameSlot()) continue;
    if (!output.node.valid()) continue;
    Emit(kWasm32LoadSlot, g.DefineAsRegister(output.node),
         g.TempImmediate(output.location.GetLocation()
                         << kSystemPointerSizeLog2));
  }
}

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
