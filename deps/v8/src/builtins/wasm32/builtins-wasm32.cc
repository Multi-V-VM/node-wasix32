// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// WASM32 builtins stub - WASM32 is a virtual architecture that doesn't
// generate native code, so builtins are minimal stubs.

#if V8_TARGET_ARCH_WASM32

#include "src/base/logging.h"
#include "src/builtins/builtins.h"
#include "src/builtins/wasm32/builtins-wasm32-abi.h"
#include "src/codegen/macro-assembler.h"
#include "src/codegen/wasm32/register-wasm32.h"
#include "src/execution/execution.h"
#include "src/execution/frame-constants.h"
#include "src/execution/isolate-inl.h"
#include "src/ic/ic.h"
#include "src/interpreter/bytecode-flags-and-tokens.h"
#include "src/interpreter/bytecode-register.h"
#include "src/interpreter/bytecodes.h"
#include "src/interpreter/interpreter.h"
#include "src/objects/bytecode-array-inl.h"
#include "src/objects/code-inl.h"
#include "src/objects/feedback-cell-inl.h"
#include "src/objects/feedback-vector-inl.h"
#include "src/objects/fixed-array-inl.h"
#include "src/objects/js-array-inl.h"
#include "src/objects/js-function-inl.h"
#include "src/objects/js-objects-inl.h"
#include "src/objects/keys.h"
#include "src/objects/objects-inl.h"
#include "src/objects/scope-info-inl.h"
#include "src/objects/shared-function-info-inl.h"
#include "src/objects/smi.h"
#include "src/objects/tagged-index.h"
#include "src/roots/roots-inl.h"
#include "src/runtime/runtime.h"

namespace v8 {
namespace internal {

Address Runtime_CreateArrayLiteral(int args_length, Address* args_object,
                                   Isolate* isolate);
Address Runtime_CreateObjectLiteral(int args_length, Address* args_object,
                                    Isolate* isolate);
Address Runtime_CreateRegExpLiteral(int args_length, Address* args_object,
                                    Isolate* isolate);
Address Runtime_DefineKeyedOwnPropertyInLiteral(int args_length,
                                                Address* args_object,
                                                Isolate* isolate);
Address Runtime_SetKeyedProperty(int args_length, Address* args_object,
                                 Isolate* isolate);
Address Runtime_Add(int args_length, Address* args_object, Isolate* isolate);
extern "C" Address WasmJSEntry(Address root, Address new_target,
                               Address target, Address receiver, intptr_t argc,
                               Address** argv);

// WASM32 doesn't generate native builtins - these are stub implementations
// that satisfy the linker. Actual functionality is provided through
// the interpreter or runtime calls.

void Builtins::Generate_Adaptor(MacroAssembler* masm,
                                int formal_parameter_count, Address address) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CEntry(MacroAssembler* masm, int result_size,
                               ArgvMode argv_mode, bool builtin_exit_frame,
                               bool switch_to_central_stack) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSConstructStubGeneric(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSBuiltinsConstructStub(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ResumeGeneratorTrampoline(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ConstructedNonConstructable(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSEntry(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSConstructEntry(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSRunMicrotasksEntry(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSEntryTrampoline(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSConstructEntryTrampoline(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_RunMicrotasksTrampoline(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_DeoptimizationEntry_Eager(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_DeoptimizationEntry_Lazy(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_BaselineOutOfLinePrologue(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_BaselineOutOfLinePrologueDeopt(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterOnStackReplacement_ToBaseline(
    MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterEntryTrampoline(
    MacroAssembler* masm, InterpreterEntryTrampolineMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterPushArgsThenCallImpl(
    MacroAssembler* masm, ConvertReceiverMode receiver_mode,
    InterpreterPushArgsMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterPushArgsThenConstructImpl(
    MacroAssembler* masm, InterpreterPushArgsMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ConstructForwardAllArgsImpl(
    MacroAssembler* masm, ForwardWhichFrame which_frame) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterPushArgsThenFastConstructFunction(
    MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterEnterAtNextBytecode(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterEnterAtBytecode(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ContinueToCodeStubBuiltin(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ContinueToCodeStubBuiltinWithResult(
    MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ContinueToJavaScriptBuiltin(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ContinueToJavaScriptBuiltinWithResult(
    MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_NotifyDeoptimized(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterOnStackReplacement(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_BaselineOnStackReplacement(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_FunctionPrototypeApply(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_FunctionPrototypeCall(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ReflectApply(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ReflectConstruct(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallOrConstructVarargs(MacroAssembler* masm,
                                               Builtin target_builtin) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallOrConstructForwardVarargs(MacroAssembler* masm,
                                                      CallOrConstructMode mode,
                                                      Builtin target_builtin) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallFunction(MacroAssembler* masm,
                                     ConvertReceiverMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallBoundFunctionImpl(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_Call(MacroAssembler* masm, ConvertReceiverMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ConstructFunction(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ConstructBoundFunction(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_Construct(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

#if V8_ENABLE_WEBASSEMBLY
void Builtins::Generate_WasmLiftoffFrameSetup(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmCompileLazy(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmDebugBreak(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmToJsWrapperAsm(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmTrapHandlerLandingPad(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmSuspend(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmResume(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmReject(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmOnStackReplace(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSToWasmWrapperAsm(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmReturnPromiseOnSuspendAsm(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSToWasmStressSwitchStacksAsm(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmHandleStackOverflow(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}
#endif  // V8_ENABLE_WEBASSEMBLY

void Builtins::Generate_DoubleToI(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallApiCallbackImpl(MacroAssembler* masm,
                                            CallApiCallbackMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallApiGetter(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_DirectCEntry(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_RestartFrameTrampoline(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

// Probe builtin: a trap-free no-op used to validate the dispatch spine.
// Signature is irrelevant here — it is never actually called from generated
// code in this milestone, only its funcref/instruction_start wiring is checked.
extern "C" void WasmProbeBuiltin() { /* no-op */ }

extern "C" Address WasmTraceMemoryAccess(Address address, int kind) {
  if (!g_wasm_trace_memory) return address;
  size_t mem_bytes = static_cast<size_t>(__builtin_wasm_memory_size(0)) * 65536u;
  Address value = 0;
  bool readable = (address <= mem_bytes - sizeof(Address)) &&
                  (address % sizeof(Address) == 0);
  if (readable) value = *reinterpret_cast<Address*>(address);
  PrintF("WasmMem: kind=%d addr=0x%x mem=0x%zx readable=%d value=0x%x "
         "r3=0x%x fp=0x%x\n",
         kind, static_cast<unsigned>(address), mem_bytes, readable,
         static_cast<unsigned>(value),
         static_cast<unsigned>(g_wasm_regs[WasmRegisterCodeToSlot(
             Register::r3().code())]),
         static_cast<unsigned>(g_wasm_current_frame_pointer));
  return address;
}

namespace {

constexpr int SlotFor(Register reg) {
  return WasmRegisterCodeToSlot(reg.code());
}

void ClearEntrypointStackWindow() {
  for (int i = kWasmStackSlotBase; i < kWasmRegFileSize; ++i) {
    g_wasm_regs[i] = 0;
  }
}

int InterpreterFrameSlotForOffset(int offset) {
  DCHECK_EQ(offset % kSystemPointerSize, 0);
  int slot = kWasmInterpreterFrameFpSlot + offset / kSystemPointerSize;
  if (slot < 0 || slot >= kWasmInterpreterFrameSlots) {
    FATAL("wasm32 interpreter frame offset %d maps outside frame", offset);
  }
  return slot;
}

int GeneratedFrameSlotForOffset(int offset) {
  DCHECK_EQ(offset % kSystemPointerSize, 0);
  int index = offset / kSystemPointerSize;
  int slot = index < 0 ? kWasmFixedFrameSlotBase + (-index - 1)
                       : kWasmStackSlotBase + index;
  if (slot < 0 || slot >= kWasmRegFileSize) {
    FATAL("wasm32 generated frame offset %d maps outside g_wasm_regs",
          offset);
  }
  return slot;
}

void StoreInterpreterFrameOffset(int offset, Address value) {
  g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(offset)] = value;
}

void StoreGeneratedFrameOffset(int offset, Address value) {
  g_wasm_regs[GeneratedFrameSlotForOffset(offset)] = value;
}

Address ReadGeneratedJSArgument(int index) {
  DCHECK_GE(index, 0);
  int offset = CommonFrameConstants::kFixedFrameSizeAboveFp +
               (index + 1) * kSystemPointerSize;
  return g_wasm_regs[GeneratedFrameSlotForOffset(offset)];
}

void StoreInterpreterRegister(interpreter::Register reg, Address value) {
  StoreInterpreterFrameOffset(reg.ToOperand() * kSystemPointerSize, value);
}

void ClearInterpreterFrame() {
  for (int i = 0; i < kWasmInterpreterFrameSlots; ++i) {
    g_wasm_interpreter_frame[i] = 0;
  }
  g_wasm_current_frame_pointer = reinterpret_cast<Address>(
      &g_wasm_interpreter_frame[kWasmInterpreterFrameFpSlot]);
}

size_t WasmMemoryBytes() {
  return static_cast<size_t>(__builtin_wasm_memory_size(0)) * 65536u;
}

bool IsReadableWasmAddress(Address address, size_t byte_count) {
  size_t mem_bytes = WasmMemoryBytes();
  if (byte_count == 0 || mem_bytes < byte_count) return false;
  return address <= mem_bytes - byte_count;
}

bool IsReadableTaggedHeapObject(Address value, size_t min_bytes) {
  if (!HAS_STRONG_HEAP_OBJECT_TAG(value) || value < kHeapObjectTag) {
    return false;
  }
  Address object_address = value - kHeapObjectTag;
  if ((object_address % sizeof(Address)) != 0) return false;
  return IsReadableWasmAddress(object_address, min_bytes);
}

bool IsPlausibleTaggedValue(Address value) {
  if (HAS_SMI_TAG(value)) return true;
  return IsReadableTaggedHeapObject(value, HeapObject::kMapOffset +
                                               sizeof(Address));
}

bool TryReadHeapObjectMap(Address value, Address* out_map) {
  if (!IsReadableTaggedHeapObject(value, HeapObject::kMapOffset +
                                             sizeof(Address))) {
    return false;
  }
  Address object_address = value - kHeapObjectTag;
  Address map_value =
      *reinterpret_cast<Address*>(object_address + HeapObject::kMapOffset);
  if (!IsReadableTaggedHeapObject(map_value, 64)) return false;
  *out_map = map_value;
  return true;
}

bool HasReadableHeapObjectMap(Address value) {
  Address map_value = kNullAddress;
  return TryReadHeapObjectMap(value, &map_value);
}

bool IsSafeTaggedHandleValue(Address value) {
  if (!IsPlausibleTaggedValue(value)) return false;
  return HAS_SMI_TAG(value) || HasReadableHeapObjectMap(value);
}

void DumpRuntimeArg(const char* label, int index, Address value) {
  PrintF(" %s[%d]=0x%x", label, index, static_cast<unsigned>(value));
  if (!IsPlausibleTaggedValue(value)) {
    PrintF("(invalid-tagged)");
    return;
  }
  Tagged<Object> object(value);
  if (IsSmi(object)) {
    PrintF("(smi=%d)", Smi::ToInt(object));
  } else if (IsHeapObject(object)) {
    Address map_value = kNullAddress;
    if (!TryReadHeapObjectMap(value, &map_value)) {
      PrintF("(invalid-map)");
      return;
    }
    Tagged<Map> map = Cast<Map>(Tagged<Object>(map_value));
    PrintF("(type=%d)", static_cast<int>(map->instance_type()));
  }
}

void DumpNameForTrace(Tagged<Object> name_object) {
  if (IsString(name_object)) {
    PrintF("<string len=%d ptr=0x%x>", Cast<String>(name_object)->length(),
           static_cast<unsigned>(name_object.ptr()));
  } else if (IsSymbol(name_object)) {
    PrintF("<symbol ptr=0x%x>", static_cast<unsigned>(name_object.ptr()));
  } else {
    PrintF("<name ptr=0x%x>", static_cast<unsigned>(name_object.ptr()));
  }
}

Address RuntimeVisibleArg(Address* argv, int argc, int index) {
  DCHECK_GE(index, 0);
  DCHECK_LT(index, argc);
  return argv[argc - 1 - index];
}

bool IsFeedbackVectorAddress(Address value) {
  return IsReadableTaggedHeapObject(value, HeapObject::kMapOffset +
                                               sizeof(Address)) &&
         HasReadableHeapObjectMap(value) &&
         IsFeedbackVector(Tagged<Object>(value));
}

bool IsNameAddress(Address value) {
  return IsReadableTaggedHeapObject(value, HeapObject::kMapOffset +
                                               sizeof(Address)) &&
         HasReadableHeapObjectMap(value) && IsName(Tagged<Object>(value));
}

bool IsJSAnyForWasmPropertyLookup(Isolate* isolate, Address value) {
  if (!IsPlausibleTaggedValue(value)) return false;
  Tagged<Object> object(value);
  ReadOnlyRoots roots(isolate);
  if (IsSmi(object)) return true;
  if (!IsHeapObject(object) || !HasReadableHeapObjectMap(value)) return false;
  return IsJSReceiver(object) || IsString(object) || IsSymbol(object) ||
         IsBigInt(object) || IsHeapNumber(object) ||
         IsUndefined(object, roots) || IsNull(object, roots) ||
         IsTrue(object, roots) || IsFalse(object, roots);
}

Address CurrentInterpreterContext() {
  return g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
      interpreter::Register::current_context().ToOperand() * kSystemPointerSize)];
}

uint32_t ReadBytecodeUnsignedOperand(Tagged<BytecodeArray> bytecode,
                                     int bytecode_index,
                                     interpreter::Bytecode bytecode_enum,
                                     int operand_index,
                                     interpreter::OperandScale operand_scale) {
  int offset = interpreter::Bytecodes::GetOperandOffset(bytecode_enum,
                                                        operand_index,
                                                        operand_scale);
  interpreter::OperandSize size =
      interpreter::Bytecodes::GetOperandSize(bytecode_enum, operand_index,
                                             operand_scale);
  int index = bytecode_index + offset;
  switch (size) {
    case interpreter::OperandSize::kByte:
      return bytecode->get(index);
    case interpreter::OperandSize::kShort:
      return bytecode->get(index) | (bytecode->get(index + 1) << 8);
    case interpreter::OperandSize::kQuad:
      return bytecode->get(index) | (bytecode->get(index + 1) << 8) |
             (bytecode->get(index + 2) << 16) |
             (bytecode->get(index + 3) << 24);
    case interpreter::OperandSize::kNone:
      return 0;
  }
}

int32_t ReadBytecodeSignedOperand(Tagged<BytecodeArray> bytecode,
                                  int bytecode_index,
                                  interpreter::Bytecode bytecode_enum,
                                  int operand_index,
                                  interpreter::OperandScale operand_scale) {
  int offset = interpreter::Bytecodes::GetOperandOffset(bytecode_enum,
                                                        operand_index,
                                                        operand_scale);
  interpreter::OperandSize size =
      interpreter::Bytecodes::GetOperandSize(bytecode_enum, operand_index,
                                             operand_scale);
  int index = bytecode_index + offset;
  switch (size) {
    case interpreter::OperandSize::kByte:
      return static_cast<int8_t>(bytecode->get(index));
    case interpreter::OperandSize::kShort:
      return static_cast<int16_t>(bytecode->get(index) |
                                  (bytecode->get(index + 1) << 8));
    case interpreter::OperandSize::kQuad:
      return static_cast<int32_t>(bytecode->get(index) |
                                  (bytecode->get(index + 1) << 8) |
                                  (bytecode->get(index + 2) << 16) |
                                  (bytecode->get(index + 3) << 24));
    case interpreter::OperandSize::kNone:
      return 0;
  }
}

Address ReadInterpreterRegister(interpreter::Register reg) {
  return g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
      reg.ToOperand() * kSystemPointerSize)];
}

void DumpInterpreterRegisterValue(const char* label, int index,
                                  interpreter::Register reg) {
  PrintF(" %s[%d]{operand=%d,index=%d}", label, index, reg.ToOperand(),
         reg.index());
  DumpRuntimeArg("value", 0, ReadInterpreterRegister(reg));
}

bool TryRunCreateFunctionContextBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateFunctionContext &&
      bytecode_enum != interpreter::Bytecode::kCreateEvalContext) {
    return false;
  }

  uint32_t scope_info_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (scope_info_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad scope_info index=%u length=%d\n",
           scope_info_index, constant_pool->length());
    return false;
  }
  Tagged<Object> scope_info_object = constant_pool->get(scope_info_index);
  if (!IsScopeInfo(scope_info_object)) {
    PrintF("WasmInterpreterEntryTrampoline: constant is not ScopeInfo index=%u ",
           scope_info_index);
    DumpRuntimeArg("value", 0, scope_info_object.ptr());
    PrintF("\n");
    return false;
  }

  Address context_address = CurrentInterpreterContext();
  if (!IsContext(Tagged<Object>(context_address))) {
    PrintF("WasmInterpreterEntryTrampoline: current context is invalid ");
    DumpRuntimeArg("context", 0, context_address);
    PrintF("\n");
    return false;
  }

  HandleScope scope(isolate);
  DirectHandle<Context> outer =
      direct_handle(Cast<Context>(Tagged<Object>(context_address)), isolate);
  DirectHandle<ScopeInfo> scope_info =
      direct_handle(Cast<ScopeInfo>(scope_info_object), isolate);
  DirectHandle<Context> result =
      isolate->factory()->NewFunctionContext(outer, scope_info);
  *out_result = (*result).ptr();
  return true;
}

bool TryRunCreateClosureBytecode(Isolate* isolate,
                                 Tagged<BytecodeArray> bytecode,
                                 int bytecode_index,
                                 interpreter::Bytecode bytecode_enum,
                                 interpreter::OperandScale operand_scale,
                                 Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateClosure) return false;

  uint32_t shared_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  uint32_t feedback_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
  uint32_t flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (shared_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad shared index=%u length=%d\n",
           shared_index, constant_pool->length());
    return false;
  }
  Tagged<Object> shared_object = constant_pool->get(shared_index);
  if (!IsSharedFunctionInfo(shared_object)) {
    PrintF("WasmInterpreterEntryTrampoline: constant is not SFI index=%u ",
           shared_index);
    DumpRuntimeArg("value", 0, shared_object.ptr());
    PrintF("\n");
    return false;
  }

  Address function_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          StandardFrameConstants::kFunctionOffset)];
  if (!IsJSFunction(Tagged<Object>(function_address))) {
    PrintF("WasmInterpreterEntryTrampoline: function slot is invalid ");
    DumpRuntimeArg("function", 0, function_address);
    PrintF("\n");
    return false;
  }
  Address context_address = CurrentInterpreterContext();
  if (!IsContext(Tagged<Object>(context_address))) {
    PrintF("WasmInterpreterEntryTrampoline: current context is invalid ");
    DumpRuntimeArg("context", 0, context_address);
    PrintF("\n");
    return false;
  }

  HandleScope scope(isolate);
  DirectHandle<JSFunction> closure =
      direct_handle(Cast<JSFunction>(Tagged<Object>(function_address)), isolate);
  JSFunction::EnsureClosureFeedbackCellArray(isolate, closure);
  Tagged<ClosureFeedbackCellArray> feedback_cell_array =
      closure->has_feedback_vector()
          ? closure->feedback_vector()->closure_feedback_cell_array()
          : closure->closure_feedback_cell_array();
  if (feedback_index >=
      static_cast<uint32_t>(feedback_cell_array->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad feedback index=%u length=%d\n",
           feedback_index, feedback_cell_array->length());
    return false;
  }

  DirectHandle<SharedFunctionInfo> shared =
      direct_handle(Cast<SharedFunctionInfo>(shared_object), isolate);
  DirectHandle<FeedbackCell> feedback_cell =
      direct_handle(feedback_cell_array->get(feedback_index), isolate);
  DirectHandle<Context> context =
      direct_handle(Cast<Context>(Tagged<Object>(context_address)), isolate);
  AllocationType allocation =
      interpreter::CreateClosureFlags::PretenuredBit::decode(flags)
          ? AllocationType::kOld
          : AllocationType::kYoung;
  Handle<JSFunction> result =
      Factory::JSFunctionBuilder{isolate, shared, context}
          .set_feedback_cell(feedback_cell)
          .set_allocation_type(allocation)
          .Build();
  *out_result = (*result).ptr();
  return true;
}

bool TryRunLdaGlobalBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                             int bytecode_index,
                             interpreter::Bytecode bytecode_enum,
                             interpreter::OperandScale operand_scale,
                             Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kLdaGlobal &&
      bytecode_enum != interpreter::Bytecode::kLdaGlobalInsideTypeof) {
    return false;
  }

  uint32_t name_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (name_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad global name index=%u "
           "length=%d\n",
           name_index, constant_pool->length());
    return false;
  }
  Tagged<Object> name_object = constant_pool->get(name_index);
  if (!IsName(name_object)) {
    PrintF("WasmInterpreterEntryTrampoline: global constant is not Name "
           "index=%u ",
           name_index);
    DumpRuntimeArg("value", 0, name_object.ptr());
    PrintF("\n");
    return false;
  }

  Address feedback_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          InterpreterFrameConstants::kFeedbackVectorFromFp)];
  HandleScope scope(isolate);
  Handle<FeedbackVector> vector;
  if (IsFeedbackVectorAddress(feedback_address)) {
    vector =
        handle(Cast<FeedbackVector>(Tagged<Object>(feedback_address)), isolate);
  }

  FeedbackSlot vector_slot = FeedbackVector::ToSlot(slot_index);
  FeedbackSlotKind kind =
      bytecode_enum == interpreter::Bytecode::kLdaGlobalInsideTypeof
          ? FeedbackSlotKind::kLoadGlobalInsideTypeof
          : FeedbackSlotKind::kLoadGlobalNotInsideTypeof;
  DirectHandle<JSGlobalObject> global = isolate->global_object();
  Handle<Name> name = handle(Cast<Name>(name_object), isolate);
  LoadGlobalIC ic(isolate, vector, vector_slot, kind);
  if (!vector.is_null()) ic.UpdateState(global, name);

  DirectHandle<Object> result;
  if (!ic.Load(name).ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryRunGetNamedPropertyBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kGetNamedProperty) return false;

  int32_t receiver_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  uint32_t name_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (name_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad property name index=%u "
           "length=%d\n",
           name_index, constant_pool->length());
    return false;
  }
  Tagged<Object> name_object = constant_pool->get(name_index);
  if (!IsName(name_object)) {
    PrintF("WasmInterpreterEntryTrampoline: property constant is not Name "
           "index=%u ",
           name_index);
    DumpRuntimeArg("value", 0, name_object.ptr());
    PrintF("\n");
    return false;
  }

  Address receiver_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(receiver_operand));
  ReadOnlyRoots roots(isolate);
  if (!IsPlausibleTaggedValue(receiver_address)) {
    PrintF("WasmInterpreterEntryTrampoline: named load receiver invalid ");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF(" ");
    DumpRuntimeArg("name", 0, name_object.ptr());
    PrintF("\n");
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  Tagged<Object> receiver_object(receiver_address);
  if (IsTheHole(receiver_object, roots) ||
      IsUninitialized(receiver_object, roots) ||
      !IsJSAnyForWasmPropertyLookup(isolate, receiver_address)) {
    PrintF("WasmInterpreterEntryTrampoline: named load receiver is not JSAny ");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF(" ");
    DumpRuntimeArg("name", 0, name_object.ptr());
    PrintF("\n");
    *out_result = roots.undefined_value().ptr();
    return true;
  }

  HandleScope scope(isolate);
  Handle<JSAny> receiver = handle(Cast<JSAny>(receiver_object), isolate);
  Handle<Name> name = handle(Cast<Name>(name_object), isolate);

  DirectHandle<Object> result;
  if (IsJSFunction(receiver_object) &&
      Name::Equals(isolate, name, isolate->factory()->prototype_string())) {
    Tagged<JSFunction> function = Cast<JSFunction>(receiver_object);
    if (function->has_prototype_slot() && function->has_prototype()) {
      *out_result = function->prototype().ptr();
      PrintF("WasmInterpreterEntryTrampoline: named load JSFunction "
             "prototype fallback ");
      DumpRuntimeArg("receiver", 0, receiver_address);
      PrintF(" ");
      DumpRuntimeArg("result", 0, *out_result);
      PrintF("\n");
      return true;
    }
  }
  if (!Runtime::GetObjectProperty(isolate, receiver, name).ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  PrintF("WasmInterpreterEntryTrampoline: named load detail name=");
  DumpNameForTrace(name_object);
  PrintF(" receiver_operand=%d ", receiver_operand);
  DumpRuntimeArg("receiver", 0, receiver_address);
  PrintF(" ");
  DumpRuntimeArg("result", 0, *out_result);
  PrintF("\n");
  return true;
}

bool TryRunGetKeyedPropertyBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kGetKeyedProperty) return false;

  int32_t receiver_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);

  Address receiver_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(receiver_operand));
  Address key_address = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  ReadOnlyRoots roots(isolate);

  HandleScope scope(isolate);
  if (!IsPlausibleTaggedValue(receiver_address)) {
    PrintF("WasmInterpreterEntryTrampoline: keyed load receiver invalid ");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF(" ");
    DumpRuntimeArg("key", 0, key_address);
    PrintF("\n");
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  Tagged<Object> receiver_object(receiver_address);
  if (IsTheHole(receiver_object, roots) ||
      IsUninitialized(receiver_object, roots) ||
      !IsJSAnyForWasmPropertyLookup(isolate, receiver_address)) {
    PrintF("WasmInterpreterEntryTrampoline: keyed load receiver is not JSAny ");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF(" ");
    DumpRuntimeArg("key", 0, key_address);
    PrintF("\n");
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  if (!IsSafeTaggedHandleValue(key_address)) {
    key_address = roots.undefined_value().ptr();
  }
  Tagged<Object> key_object(key_address);
  if (IsTheHole(key_object, roots) || IsUninitialized(key_object, roots)) {
    key_object = roots.undefined_value();
  }

  Handle<JSAny> receiver =
      handle(Cast<JSAny>(receiver_object), isolate);
  Handle<Object> key = handle(key_object, isolate);

  DirectHandle<Object> result;
  if (!Runtime::GetObjectProperty(isolate, receiver, key).ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryRunToNameBytecode(Isolate* isolate,
                          interpreter::Bytecode bytecode_enum,
                          Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kToName) return false;

  Address input_address = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  ReadOnlyRoots roots(isolate);
  if (!IsSafeTaggedHandleValue(input_address)) {
    input_address = roots.undefined_value().ptr();
  }
  Tagged<Object> input_object(input_address);
  if (IsTheHole(input_object, roots) || IsUninitialized(input_object, roots)) {
    input_object = roots.undefined_value();
  }

  HandleScope scope(isolate);
  DirectHandle<Object> input = direct_handle(input_object, isolate);
  DirectHandle<Name> result;
  if (!Object::ToName(isolate, input).ToHandle(&result)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

Address SafeTaggedOrUndefined(Isolate* isolate, Address value);

bool TryRunTestInBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                          int bytecode_index,
                          interpreter::Bytecode bytecode_enum,
                          interpreter::OperandScale operand_scale,
                          Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kTestIn) return false;

  int32_t key_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  Address key_address = SafeTaggedOrUndefined(
      isolate, ReadInterpreterRegister(
                   interpreter::Register::FromOperand(key_operand)));
  Address object_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);

  HandleScope scope(isolate);
  DirectHandle<Object> object =
      direct_handle(Tagged<Object>(object_address), isolate);
  DirectHandle<Object> key = direct_handle(Tagged<Object>(key_address),
                                           isolate);

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  DirectHandle<Object> result;
  MaybeDirectHandle<Object> maybe_result =
      Runtime::HasProperty(isolate, object, key);
  if (switched_context) isolate->set_context(saved_context);
  if (!maybe_result.ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

Address SafeTaggedOrUndefined(Isolate* isolate, Address value) {
  ReadOnlyRoots roots(isolate);
  if (!IsSafeTaggedHandleValue(value)) return roots.undefined_value().ptr();
  Tagged<Object> object(value);
  if (IsTheHole(object, roots) || IsUninitialized(object, roots)) {
    return roots.undefined_value().ptr();
  }
  return value;
}

interpreter::Register RegisterFromListOperand(int32_t first_operand,
                                              int index) {
  return interpreter::Register::FromOperand(first_operand - index);
}

struct WasmSlotSnapshot {
  Address raw;
  int handle_index;
};

void SaveWasmSlot(Isolate* isolate, Address value,
                  DirectHandleVector<Object>* handles,
                  WasmSlotSnapshot* snapshot) {
  snapshot->raw = value;
  snapshot->handle_index = -1;
  if (!IsSafeTaggedHandleValue(value)) return;
  if (HAS_SMI_TAG(value)) return;

  snapshot->handle_index = static_cast<int>(handles->size());
  handles->push_back(direct_handle(Tagged<Object>(value), isolate));
}

Address RestoreWasmSlot(const DirectHandleVector<Object>& handles,
                        const WasmSlotSnapshot& snapshot) {
  if (snapshot.handle_index < 0) return snapshot.raw;
  return (*handles[static_cast<size_t>(snapshot.handle_index)]).ptr();
}

class WasmInterpreterStateSnapshot {
 public:
  explicit WasmInterpreterStateSnapshot(Isolate* isolate)
      : handles_(isolate), frame_pointer_(g_wasm_current_frame_pointer) {
    handles_.reserve(256);
    for (int i = 0; i < kWasmRegFileSize; ++i) {
      SaveWasmSlot(isolate, g_wasm_regs[i], &handles_, &regs_[i]);
    }
    for (int i = 0; i < kWasmInterpreterFrameSlots; ++i) {
      SaveWasmSlot(isolate, g_wasm_interpreter_frame[i], &handles_, &frame_[i]);
    }
  }

  void Restore() const {
    for (int i = 0; i < kWasmRegFileSize; ++i) {
      g_wasm_regs[i] = RestoreWasmSlot(handles_, regs_[i]);
    }
    for (int i = 0; i < kWasmInterpreterFrameSlots; ++i) {
      g_wasm_interpreter_frame[i] = RestoreWasmSlot(handles_, frame_[i]);
    }
    g_wasm_current_frame_pointer = frame_pointer_;
  }

 private:
  DirectHandleVector<Object> handles_;
  WasmSlotSnapshot regs_[kWasmRegFileSize];
  WasmSlotSnapshot frame_[kWasmInterpreterFrameSlots];
  Address frame_pointer_;
};

void AddCallArgument(Isolate* isolate, DirectHandleVector<Object>* args,
                     Address value) {
  value = SafeTaggedOrUndefined(isolate, value);
  args->push_back(direct_handle(Tagged<Object>(value), isolate));
}

bool TryCallJSFunctionDirect(Isolate* isolate, DirectHandle<Object> callable,
                             DirectHandle<Object> receiver,
                             DirectHandleVector<Object>* args,
                             Address* out_result) {
  if (!IsJSFunction(*callable)) return false;
  if (args->size() > 64) return false;

  Address arg_values[64];
  Address* argv[64];
  for (size_t i = 0; i < args->size(); ++i) {
    arg_values[i] = (*(*args)[i]).ptr();
    argv[i] = &arg_values[i];
  }

  ReadOnlyRoots roots(isolate);
  SaveContext save(isolate);
  *out_result = WasmJSEntry(isolate->isolate_data()->isolate_root(),
                            roots.undefined_value().ptr(), (*callable).ptr(),
                            (*receiver).ptr(),
                            JSParameterCount(static_cast<int>(args->size())),
                            argv);
  return true;
}

bool IsJSFunctionBuiltin(Isolate* isolate, DirectHandle<Object> callable,
                         Builtin builtin) {
  if (!IsJSFunction(*callable)) return false;
  Tagged<Code> code = Cast<JSFunction>(*callable)->code(isolate);
  return code->is_builtin() && code->builtin_id() == builtin;
}

bool TryRunStringPrototypeToUpperCaseBuiltin(Isolate* isolate,
                                             DirectHandle<Object> callable,
                                             DirectHandle<Object> receiver,
                                             Address* out_result) {
#ifdef V8_INTL_SUPPORT
  bool is_to_upper = IsJSFunctionBuiltin(
      isolate, callable, Builtin::kStringPrototypeToUpperCaseIntl);
#else
  bool is_to_upper = IsJSFunctionBuiltin(
      isolate, callable, Builtin::kStringPrototypeToUpperCase);
#endif
  if (!is_to_upper) {
    return false;
  }
  if (!IsString(*receiver)) return false;

  Tagged<String> input = Cast<String>(*receiver);
  if (input->length() != 1) return false;
  uint16_t code = input->Get(0);
  if (code > 0x7f) return false;
  if (code >= 'a' && code <= 'z') code -= 'a' - 'A';

  DirectHandle<String> result =
      isolate->factory()->LookupSingleCharacterStringFromCode(code);
  *out_result = (*result).ptr();
  PrintF("WasmInterpreterEntryTrampoline: fallback "
         "StringPrototypeToUpperCase result=0x%x\n",
         static_cast<unsigned>(*out_result));
  return true;
}

bool TryReadRelativeSmiIndex(DirectHandle<Object> value, uint32_t length,
                             uint32_t* out_index) {
  if (!IsSmi(*value)) return false;
  int index = Smi::ToInt(*value);
  if (index < 0) {
    int64_t relative = static_cast<int64_t>(length) + index;
    *out_index = relative > 0 ? static_cast<uint32_t>(relative) : 0;
  } else {
    uint32_t relative = static_cast<uint32_t>(index);
    *out_index = relative < length ? relative : length;
  }
  return true;
}

bool TryRunStringPrototypeSliceBuiltin(Isolate* isolate,
                                       DirectHandle<Object> callable,
                                       DirectHandle<Object> receiver,
                                       DirectHandleVector<Object>* args,
                                       Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable,
                           Builtin::kStringPrototypeSlice)) {
    return false;
  }
  if (!IsString(*receiver)) return false;

  DirectHandle<String> input = Cast<String>(receiver);
  uint32_t length = static_cast<uint32_t>(input->length());
  ReadOnlyRoots roots(isolate);

  uint32_t start = 0;
  if (!args->empty() && !IsUndefined(*(*args)[0], roots) &&
      !TryReadRelativeSmiIndex((*args)[0], length, &start)) {
    return false;
  }

  uint32_t end = length;
  if (args->size() > 1 && !IsUndefined(*(*args)[1], roots) &&
      !TryReadRelativeSmiIndex((*args)[1], length, &end)) {
    return false;
  }

  DirectHandle<String> result =
      end <= start ? isolate->factory()->empty_string()
                   : isolate->factory()->NewProperSubString(input, start, end);
  *out_result = (*result).ptr();
  PrintF("WasmInterpreterEntryTrampoline: fallback StringPrototypeSlice "
         "start=%u end=%u result=0x%x\n",
         start, end, static_cast<unsigned>(*out_result));
  return true;
}

bool TryRunArrayForEachBuiltin(Isolate* isolate, DirectHandle<Object> callable,
                               DirectHandle<Object> receiver,
                               DirectHandleVector<Object>* args,
                               Address* out_result) {
  if (!IsJSFunction(*callable)) return false;
  Tagged<Code> code = Cast<JSFunction>(*callable)->code(isolate);
  if (!code->is_builtin() || code->builtin_id() != Builtin::kArrayForEach) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (args->empty() || !IsCallable(*(*args)[0])) {
    PrintF("WasmInterpreterEntryTrampoline: ArrayForEach callback invalid ");
    if (!args->empty()) DumpRuntimeArg("callback", 0, (*(*args)[0]).ptr());
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> object;
  if (!Object::ToObject(isolate, receiver, "Array.prototype.forEach")
           .ToHandle(&object)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> length_object;
  if (!Object::GetLengthFromArrayLike(isolate, object).ToHandle(&length_object)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  double raw_length = Object::NumberValue(*length_object);
  uint32_t length = raw_length > static_cast<double>(0xffffffffu)
                        ? 0xffffffffu
                        : static_cast<uint32_t>(raw_length);

  DirectHandleVector<Object> protected_values(isolate);
  protected_values.reserve(3);
  protected_values.push_back(object);
  protected_values.push_back((*args)[0]);
  protected_values.push_back(args->size() > 1
                                 ? (*args)[1]
                                 : direct_handle(roots.undefined_value(),
                                                 isolate));

  for (uint32_t index = 0; index < length; ++index) {
    DirectHandle<JSReceiver> current_object = Cast<JSReceiver>(protected_values[0]);
    Maybe<bool> maybe_has_element =
        JSReceiver::HasElement(isolate, current_object, index);
    if (maybe_has_element.IsNothing()) {
      *out_result = roots.exception().ptr();
      return true;
    }
    if (!maybe_has_element.FromJust()) continue;

    DirectHandle<Object> element;
    if (!JSReceiver::GetElement(isolate, current_object, index)
             .ToHandle(&element)) {
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandleVector<Object> callback_args(isolate);
    callback_args.reserve(3);
    callback_args.push_back(element);
    callback_args.push_back(isolate->factory()->NewNumberFromUint(index));
    callback_args.push_back(current_object);

    WasmInterpreterStateSnapshot state(isolate);
    DirectHandle<Object> callback = protected_values[1];
    DirectHandle<Object> this_arg = protected_values[2];
    Address callback_result = roots.exception().ptr();
    bool direct_call = TryCallJSFunctionDirect(isolate, callback, this_arg,
                                               &callback_args,
                                               &callback_result);
    MaybeHandle<Object> maybe_result;
    if (!direct_call) {
      maybe_result = Execution::Call(
          isolate, callback, this_arg,
          ZoneVector<const DirectHandle<Object>>(callback_args.data(),
                                                 callback_args.size()));
    }
    state.Restore();
    DirectHandle<Object> ignored;
    if (!direct_call && !maybe_result.ToHandle(&ignored)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    if (direct_call && IsException(Tagged<Object>(callback_result), isolate)) {
      *out_result = callback_result;
      return true;
    }
  }

  *out_result = roots.undefined_value().ptr();
  return true;
}

bool TryRunReflectOwnKeysBuiltin(Isolate* isolate,
                                 DirectHandle<Object> callable,
                                 DirectHandleVector<Object>* args,
                                 Address* out_result) {
  if (!IsJSFunction(*callable)) return false;

  Tagged<JSFunction> function = Cast<JSFunction>(*callable);
  Tagged<Code> code = function->code(isolate);
  if (!code->is_builtin() || code->builtin_id() != Builtin::kReflectOwnKeys) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (args->size() != 1 || !IsJSReceiver(*(*args)[0])) {
    PrintF("WasmInterpreterEntryTrampoline: ReflectOwnKeys target invalid");
    if (!args->empty()) {
      DumpRuntimeArg("target", 0, (*(*args)[0]).ptr());
    }
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> target =
      direct_handle(Cast<JSReceiver>(*(*args)[0]), isolate);
  DirectHandle<FixedArray> keys;
  if (!KeyAccumulator::GetKeys(isolate, target, KeyCollectionMode::kOwnOnly,
                               ALL_PROPERTIES,
                               GetKeysConversion::kConvertToString)
           .ToHandle(&keys)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSArray> result =
      isolate->factory()->NewJSArrayWithElements(keys);
  *out_result = (*result).ptr();
  PrintF("WasmInterpreterEntryTrampoline: fallback ReflectOwnKeys result=0x%x\n",
         static_cast<unsigned>(*out_result));
  return true;
}

bool TryRunArrayIteratorPrototypeNextBuiltin(Isolate* isolate,
                                             DirectHandle<Object> callable,
                                             DirectHandle<Object> receiver,
                                             Address* out_result) {
  if (!IsJSFunction(*callable)) return false;

  Tagged<JSFunction> function = Cast<JSFunction>(*callable);
  Tagged<Code> code = function->code(isolate);
  if (!code->is_builtin() ||
      code->builtin_id() != Builtin::kArrayIteratorPrototypeNext) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (!IsJSArrayIterator(*receiver)) {
    PrintF("WasmInterpreterEntryTrampoline: ArrayIterator.next receiver "
           "invalid ");
    DumpRuntimeArg("receiver", 0, (*receiver).ptr());
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<JSArrayIterator> iterator = Cast<JSArrayIterator>(*receiver);
  Tagged<JSReceiver> iterated_object = iterator->iterated_object();
  Tagged<Number> next_index = iterator->next_index();

  uint32_t index = 0;
  if (!Object::ToArrayLength(next_index, &index)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  uint32_t length = 0;
  if (IsJSArray(iterated_object)) {
    if (!Object::ToArrayLength(Cast<JSArray>(iterated_object)->length(),
                               &length)) {
      *out_result = roots.exception().ptr();
      return true;
    }
  } else {
    DirectHandle<JSReceiver> object = direct_handle(iterated_object, isolate);
    DirectHandle<Object> length_key = isolate->factory()->length_string();
    DirectHandle<Object> length_object;
    if (!Runtime::GetObjectProperty(isolate, object, length_key)
             .ToHandle(&length_object) ||
        !Object::ToArrayLength(*length_object, &length)) {
      *out_result = roots.exception().ptr();
      return true;
    }
  }

  DirectHandle<Object> value = isolate->factory()->undefined_value();
  bool done = index >= length;
  if (done) {
    iterator->set_next_index(*isolate->factory()->NewNumber(kMaxUInt32));
  } else {
    iterator->set_next_index(*isolate->factory()->NewNumberFromUint(index + 1));
    switch (iterator->kind()) {
      case IterationKind::kKeys:
        value = isolate->factory()->NewNumberFromUint(index);
        break;
      case IterationKind::kValues:
        if (!JSReceiver::GetElement(isolate, direct_handle(iterated_object,
                                                           isolate),
                                    index)
                 .ToHandle(&value)) {
          *out_result = roots.exception().ptr();
          return true;
        }
        break;
      case IterationKind::kEntries: {
        DirectHandle<Object> element;
        if (!JSReceiver::GetElement(isolate, direct_handle(iterated_object,
                                                           isolate),
                                    index)
                 .ToHandle(&element)) {
          *out_result = roots.exception().ptr();
          return true;
        }
        DirectHandle<FixedArray> entry = isolate->factory()->NewFixedArray(2);
        entry->set(0, *isolate->factory()->NewNumberFromUint(index));
        entry->set(1, *element);
        value = isolate->factory()->NewJSArrayWithElements(entry);
        break;
      }
    }
  }

  DirectHandle<JSIteratorResult> result =
      isolate->factory()->NewJSIteratorResult(value, done);
  *out_result = (*result).ptr();
  PrintF("WasmInterpreterEntryTrampoline: fallback ArrayIterator.next "
         "done=%d result=0x%x\n",
         done, static_cast<unsigned>(*out_result));
  return true;
}

bool TryRunCallBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                        int bytecode_index,
                        interpreter::Bytecode bytecode_enum,
                        interpreter::OperandScale operand_scale,
                        Address* out_result) {
  bool is_supported_call = false;
  bool receiver_is_implicit_undefined = false;
  bool uses_register_list = false;
  int fixed_arg_count = 0;

  switch (bytecode_enum) {
    case interpreter::Bytecode::kCallAnyReceiver:
    case interpreter::Bytecode::kCallProperty:
      is_supported_call = true;
      uses_register_list = true;
      break;
    case interpreter::Bytecode::kCallProperty0:
      is_supported_call = true;
      fixed_arg_count = 0;
      break;
    case interpreter::Bytecode::kCallProperty1:
      is_supported_call = true;
      fixed_arg_count = 1;
      break;
    case interpreter::Bytecode::kCallProperty2:
      is_supported_call = true;
      fixed_arg_count = 2;
      break;
    case interpreter::Bytecode::kCallUndefinedReceiver:
      is_supported_call = true;
      receiver_is_implicit_undefined = true;
      uses_register_list = true;
      break;
    case interpreter::Bytecode::kCallUndefinedReceiver0:
      is_supported_call = true;
      receiver_is_implicit_undefined = true;
      fixed_arg_count = 0;
      break;
    case interpreter::Bytecode::kCallUndefinedReceiver1:
      is_supported_call = true;
      receiver_is_implicit_undefined = true;
      fixed_arg_count = 1;
      break;
    case interpreter::Bytecode::kCallUndefinedReceiver2:
      is_supported_call = true;
      receiver_is_implicit_undefined = true;
      fixed_arg_count = 2;
      break;
    default:
      break;
  }
  if (!is_supported_call) return false;

  ReadOnlyRoots roots(isolate);
  int32_t callable_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  Address callable_address = SafeTaggedOrUndefined(
      isolate, ReadInterpreterRegister(
                   interpreter::Register::FromOperand(callable_operand)));
  interpreter::Register callable_reg =
      interpreter::Register::FromOperand(callable_operand);
  PrintF("WasmInterpreterEntryTrampoline: call detail bytecode=%s "
         "callable_operand=%d callable_index=%d ",
         interpreter::Bytecodes::ToString(bytecode_enum), callable_operand,
         callable_reg.index());
  DumpRuntimeArg("callable", 0, callable_address);
  PrintF("\n");
  if (!IsCallable(Tagged<Object>(callable_address))) {
    PrintF("WasmInterpreterEntryTrampoline: call target is not callable "
           "bytecode=%s ",
           interpreter::Bytecodes::ToString(bytecode_enum));
    DumpRuntimeArg("target", 0, callable_address);
    PrintF("\n");
    *out_result = roots.undefined_value().ptr();
    return true;
  }

  HandleScope scope(isolate);
  DirectHandleVector<Object> args(isolate);
  Address receiver_address = roots.undefined_value().ptr();
  if (uses_register_list) {
    int32_t first_arg_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
    uint32_t reg_count =
        ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                    operand_scale);
    PrintF("WasmInterpreterEntryTrampoline: call reglist first_operand=%d "
           "reg_count=%u implicit_receiver=%d",
           first_arg_operand, reg_count, receiver_is_implicit_undefined);
    if (!receiver_is_implicit_undefined) {
      if (reg_count == 0) {
        PrintF("WasmInterpreterEntryTrampoline: call missing receiver "
               "bytecode=%s\n",
               interpreter::Bytecodes::ToString(bytecode_enum));
        *out_result = roots.undefined_value().ptr();
        return true;
      }
      receiver_address = SafeTaggedOrUndefined(
          isolate, ReadInterpreterRegister(
                       RegisterFromListOperand(first_arg_operand, 0)));
      DumpInterpreterRegisterValue("receiver", 0,
                                   RegisterFromListOperand(first_arg_operand, 0));
      args.reserve(reg_count - 1);
      for (uint32_t i = 1; i < reg_count; ++i) {
        DumpInterpreterRegisterValue(
            "arg", static_cast<int>(i - 1),
            RegisterFromListOperand(first_arg_operand, i));
        AddCallArgument(isolate, &args,
                        ReadInterpreterRegister(
                            RegisterFromListOperand(first_arg_operand, i)));
      }
    } else {
      args.reserve(reg_count);
      for (uint32_t i = 0; i < reg_count; ++i) {
        DumpInterpreterRegisterValue(
            "arg", static_cast<int>(i),
            RegisterFromListOperand(first_arg_operand, i));
        AddCallArgument(isolate, &args,
                        ReadInterpreterRegister(
                            RegisterFromListOperand(first_arg_operand, i)));
      }
    }
    PrintF("\n");
  } else {
    if (!receiver_is_implicit_undefined) {
      int32_t receiver_operand =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                    operand_scale);
      receiver_address = SafeTaggedOrUndefined(
          isolate, ReadInterpreterRegister(
                       interpreter::Register::FromOperand(receiver_operand)));
      DumpInterpreterRegisterValue(
          "receiver", 0, interpreter::Register::FromOperand(receiver_operand));
    }
    args.reserve(fixed_arg_count);
    for (int i = 0; i < fixed_arg_count; ++i) {
      int operand_index = receiver_is_implicit_undefined ? 1 + i : 2 + i;
      int32_t arg_operand =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum,
                                    operand_index, operand_scale);
      DumpInterpreterRegisterValue(
          "arg", i, interpreter::Register::FromOperand(arg_operand));
      AddCallArgument(
          isolate, &args,
          ReadInterpreterRegister(interpreter::Register::FromOperand(
              arg_operand)));
    }
    PrintF("\n");
  }

  DirectHandle<Object> callable =
      direct_handle(Tagged<Object>(callable_address), isolate);
  DirectHandle<Object> receiver =
      direct_handle(Tagged<Object>(receiver_address), isolate);
  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  if (TryRunArrayForEachBuiltin(isolate, callable, receiver, &args,
                                out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunReflectOwnKeysBuiltin(isolate, callable, &args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunArrayIteratorPrototypeNextBuiltin(isolate, callable, receiver,
                                              out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunStringPrototypeToUpperCaseBuiltin(isolate, callable, receiver,
                                              out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunStringPrototypeSliceBuiltin(isolate, callable, receiver, &args,
                                        out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }

  WasmInterpreterStateSnapshot state(isolate);
  Address result_address = roots.exception().ptr();
  if (!TryCallJSFunctionDirect(isolate, callable, receiver, &args,
                               &result_address)) {
    DirectHandle<Object> result;
    MaybeHandle<Object> maybe_result = Execution::Call(
        isolate, callable, receiver,
        ZoneVector<const DirectHandle<Object>>(args.data(), args.size()));
    if (maybe_result.ToHandle(&result)) {
      result_address = (*result).ptr();
    }
  }
  if (switched_context) isolate->set_context(saved_context);
  state.Restore();

  *out_result = result_address;
  return true;
}

bool TryRunAddBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                       int bytecode_index,
                       interpreter::Bytecode bytecode_enum,
                       interpreter::OperandScale operand_scale,
                       Address* out_result) {
  Address lhs_address = kNullAddress;
  Address rhs_address = kNullAddress;
  if (bytecode_enum == interpreter::Bytecode::kAdd ||
      bytecode_enum ==
          interpreter::Bytecode::kAdd_LhsIsStringConstant_Internalize) {
    int32_t lhs_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    lhs_address = ReadInterpreterRegister(
        interpreter::Register::FromOperand(lhs_operand));
    rhs_address = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  } else if (bytecode_enum == interpreter::Bytecode::kAddSmi) {
    int32_t rhs =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    lhs_address = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
    rhs_address = Smi::FromInt(rhs).ptr();
  } else {
    return false;
  }

  lhs_address = SafeTaggedOrUndefined(isolate, lhs_address);
  rhs_address = SafeTaggedOrUndefined(isolate, rhs_address);

  Address args[2] = {rhs_address, lhs_address};
  *out_result = Runtime_Add(2, &args[1], isolate);
  return true;
}

bool TryReadJumpOffset(Tagged<BytecodeArray> bytecode, int bytecode_index,
                       interpreter::Bytecode bytecode_enum,
                       interpreter::OperandScale operand_scale,
                       int operand_index, intptr_t* out_offset) {
  if (interpreter::Bytecodes::IsJumpConstant(bytecode_enum)) {
    uint32_t constant_index =
        ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum,
                                    operand_index, operand_scale);
    Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
    if (constant_index >= static_cast<uint32_t>(constant_pool->length())) {
      PrintF("WasmInterpreterEntryTrampoline: bad jump constant index=%u "
             "length=%d\n",
             constant_index, constant_pool->length());
      return false;
    }
    Tagged<Object> offset_object = constant_pool->get(constant_index);
    if (!IsSmi(offset_object)) {
      PrintF("WasmInterpreterEntryTrampoline: jump constant is not Smi ");
      DumpRuntimeArg("value", 0, offset_object.ptr());
      PrintF("\n");
      return false;
    }
    *out_offset = Smi::ToInt(offset_object);
    return true;
  }

  *out_offset = static_cast<intptr_t>(ReadBytecodeUnsignedOperand(
      bytecode, bytecode_index, bytecode_enum, operand_index, operand_scale));
  return true;
}

bool TryRunJumpBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                        int bytecode_index,
                        interpreter::Bytecode bytecode_enum,
                        interpreter::OperandScale operand_scale,
                        Address current_offset, Address* out_next_offset) {
  if (!interpreter::Bytecodes::IsJump(bytecode_enum)) return false;

  intptr_t relative_jump = 0;
  if (!TryReadJumpOffset(bytecode, bytecode_index, bytecode_enum, operand_scale,
                         0, &relative_jump)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  Address accumulator_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  Tagged<Object> accumulator(accumulator_address);
  bool should_jump = false;

  switch (bytecode_enum) {
    case interpreter::Bytecode::kJump:
    case interpreter::Bytecode::kJumpConstant:
      should_jump = true;
      break;
    case interpreter::Bytecode::kJumpLoop:
      should_jump = true;
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] =
          roots.undefined_value().ptr();
      break;
    case interpreter::Bytecode::kJumpIfTrue:
    case interpreter::Bytecode::kJumpIfTrueConstant:
      should_jump = IsTrue(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfFalse:
    case interpreter::Bytecode::kJumpIfFalseConstant:
      should_jump = IsFalse(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfToBooleanTrue:
    case interpreter::Bytecode::kJumpIfToBooleanTrueConstant:
      should_jump = Object::BooleanValue(accumulator, isolate);
      break;
    case interpreter::Bytecode::kJumpIfToBooleanFalse:
    case interpreter::Bytecode::kJumpIfToBooleanFalseConstant:
      should_jump = !Object::BooleanValue(accumulator, isolate);
      break;
    case interpreter::Bytecode::kJumpIfNull:
    case interpreter::Bytecode::kJumpIfNullConstant:
      should_jump = IsNull(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfNotNull:
    case interpreter::Bytecode::kJumpIfNotNullConstant:
      should_jump = !IsNull(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfUndefined:
    case interpreter::Bytecode::kJumpIfUndefinedConstant:
      should_jump = IsUndefined(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfNotUndefined:
    case interpreter::Bytecode::kJumpIfNotUndefinedConstant:
      should_jump = !IsUndefined(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfUndefinedOrNull:
    case interpreter::Bytecode::kJumpIfUndefinedOrNullConstant:
      should_jump = IsUndefined(accumulator, roots) ||
                    IsNull(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfJSReceiver:
    case interpreter::Bytecode::kJumpIfJSReceiverConstant:
      should_jump = IsJSReceiver(accumulator);
      break;
    case interpreter::Bytecode::kJumpIfForInDone:
    case interpreter::Bytecode::kJumpIfForInDoneConstant: {
      int32_t index_operand =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                    operand_scale);
      int32_t cache_length_operand =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                    operand_scale);
      Address index_address = ReadInterpreterRegister(
          interpreter::Register::FromOperand(index_operand));
      Address cache_length_address = ReadInterpreterRegister(
          interpreter::Register::FromOperand(cache_length_operand));
      should_jump = index_address == cache_length_address;
      break;
    }
    default:
      return false;
  }

  Address next_offset = current_offset +
                        interpreter::Bytecodes::Size(bytecode_enum,
                                                     operand_scale);
  if (should_jump) {
    next_offset = bytecode_enum == interpreter::Bytecode::kJumpLoop
                      ? current_offset - static_cast<Address>(relative_jump)
                      : current_offset + static_cast<Address>(relative_jump);
  }

  Address bytecode_start = BytecodeArray::kHeaderSize - kHeapObjectTag;
  Address bytecode_limit =
      bytecode_start + static_cast<Address>(bytecode->length());
  if (next_offset < bytecode_start || next_offset >= bytecode_limit) {
    PrintF("WasmInterpreterEntryTrampoline: jump target OOB bytecode=%s "
           "current=0x%x relative=%d target=0x%x range=[0x%x,0x%x)\n",
           interpreter::Bytecodes::ToString(bytecode_enum),
           static_cast<unsigned>(current_offset),
           static_cast<int>(relative_jump), static_cast<unsigned>(next_offset),
           static_cast<unsigned>(bytecode_start),
           static_cast<unsigned>(bytecode_limit));
    return false;
  }

  *out_next_offset = next_offset;
  return true;
}

bool TryRunSetNamedPropertyBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kSetNamedProperty &&
      bytecode_enum != interpreter::Bytecode::kDefineNamedOwnProperty) {
    return false;
  }

  int32_t object_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  uint32_t name_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (name_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad store name index=%u "
           "length=%d\n",
           name_index, constant_pool->length());
    return false;
  }
  Tagged<Object> name_object = constant_pool->get(name_index);
  if (!IsName(name_object)) {
    PrintF("WasmInterpreterEntryTrampoline: store constant is not Name "
           "index=%u ",
           name_index);
    DumpRuntimeArg("value", 0, name_object.ptr());
    PrintF("\n");
    return false;
  }

  Address object_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(object_operand));
  Address value_address =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  ReadOnlyRoots roots(isolate);
  if (!IsJSAnyForWasmPropertyLookup(isolate, object_address)) {
    PrintF("WasmInterpreterEntryTrampoline: named store receiver is not JSAny ");
    DumpRuntimeArg("receiver", 0, object_address);
    PrintF(" ");
    DumpRuntimeArg("name", 0, name_object.ptr());
    PrintF("\n");
    *out_result = IsSafeTaggedHandleValue(value_address)
                      ? value_address
                      : roots.undefined_value().ptr();
    return true;
  }
  if (!IsSafeTaggedHandleValue(value_address)) {
    value_address = roots.undefined_value().ptr();
  }
  Address feedback_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          InterpreterFrameConstants::kFeedbackVectorFromFp)];

  HandleScope scope(isolate);
  FeedbackSlot vector_slot = FeedbackVector::ToSlot(slot_index);
  FeedbackSlotKind kind =
      bytecode_enum == interpreter::Bytecode::kDefineNamedOwnProperty
          ? FeedbackSlotKind::kDefineNamedOwn
          : FeedbackSlotKind::kSetNamedStrict;
  Handle<FeedbackVector> vector;
  if (IsFeedbackVectorAddress(feedback_address)) {
    vector =
        handle(Cast<FeedbackVector>(Tagged<Object>(feedback_address)), isolate);
    kind = vector->GetKind(vector_slot);
  }

  Handle<JSAny> object =
      handle(Cast<JSAny>(Tagged<Object>(object_address)), isolate);
  Handle<Name> name = handle(Cast<Name>(name_object), isolate);
  DirectHandle<Object> value =
      direct_handle(Tagged<Object>(value_address), isolate);
  StoreIC ic(isolate, vector, vector_slot, kind);
  ic.UpdateState(object, name);

  DirectHandle<Object> result;
  if (!ic.Store(object, name, value).ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryRunSetKeyedPropertyBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kSetKeyedProperty) return false;

  int32_t object_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  int32_t key_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                operand_scale);

  Address object_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(object_operand));
  Address key_address =
      ReadInterpreterRegister(interpreter::Register::FromOperand(key_operand));
  Address value_address =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  ReadOnlyRoots roots(isolate);
  if (!IsJSAnyForWasmPropertyLookup(isolate, object_address)) {
    PrintF("WasmInterpreterEntryTrampoline: keyed store receiver is not JSAny ");
    DumpRuntimeArg("receiver", 0, object_address);
    PrintF(" ");
    DumpRuntimeArg("key", 0, key_address);
    PrintF(" ");
    DumpRuntimeArg("value", 0, value_address);
    PrintF("\n");
    *out_result = IsSafeTaggedHandleValue(value_address)
                      ? value_address
                      : roots.undefined_value().ptr();
    return true;
  }

  object_address = SafeTaggedOrUndefined(isolate, object_address);
  key_address = SafeTaggedOrUndefined(isolate, key_address);
  value_address = SafeTaggedOrUndefined(isolate, value_address);

  Address args[3] = {value_address, key_address, object_address};
  *out_result = Runtime_SetKeyedProperty(3, &args[2], isolate);
  return true;
}

bool TryRunCreateArrayLiteralBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateArrayLiteral) return false;

  uint32_t elements_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  uint32_t literal_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
  uint32_t flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (elements_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad array literal index=%u "
           "length=%d\n",
           elements_index, constant_pool->length());
    return false;
  }
  Tagged<Object> elements_object = constant_pool->get(elements_index);
  if (!IsArrayBoilerplateDescription(elements_object)) {
    PrintF("WasmInterpreterEntryTrampoline: array literal constant invalid "
           "index=%u ",
           elements_index);
    DumpRuntimeArg("value", 0, elements_object.ptr());
    PrintF("\n");
    return false;
  }

  Address feedback_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          InterpreterFrameConstants::kFeedbackVectorFromFp)];
  if (!IsFeedbackVectorAddress(feedback_address)) {
    feedback_address = ReadOnlyRoots(isolate).undefined_value().ptr();
  }

  Address args[4] = {
      Smi::FromInt(static_cast<int>(flags)).ptr(),
      elements_object.ptr(),
      TaggedIndex::FromIntptr(static_cast<intptr_t>(literal_index)).ptr(),
      feedback_address,
  };
  *out_result = Runtime_CreateArrayLiteral(4, &args[3], isolate);
  return true;
}

bool TryRunCreateRegExpLiteralBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateRegExpLiteral) {
    return false;
  }

  uint32_t pattern_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  uint32_t literal_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
  uint32_t flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (pattern_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad regexp literal index=%u "
           "length=%d\n",
           pattern_index, constant_pool->length());
    return false;
  }
  Tagged<Object> pattern_object = constant_pool->get(pattern_index);
  if (!IsString(pattern_object)) {
    PrintF("WasmInterpreterEntryTrampoline: regexp literal pattern invalid "
           "index=%u ",
           pattern_index);
    DumpRuntimeArg("value", 0, pattern_object.ptr());
    PrintF("\n");
    return false;
  }

  Address feedback_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          InterpreterFrameConstants::kFeedbackVectorFromFp)];
  if (!IsFeedbackVectorAddress(feedback_address)) {
    feedback_address = ReadOnlyRoots(isolate).undefined_value().ptr();
  }

  Address args[4] = {
      Smi::FromInt(static_cast<int>(flags)).ptr(),
      pattern_object.ptr(),
      TaggedIndex::FromIntptr(static_cast<intptr_t>(literal_index)).ptr(),
      feedback_address,
  };
  *out_result = Runtime_CreateRegExpLiteral(4, &args[3], isolate);
  return true;
}

bool TryRunStaInArrayLiteralBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kStaInArrayLiteral) return false;

  int32_t array_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  int32_t index_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                operand_scale);
  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Address array_address =
      ReadInterpreterRegister(interpreter::Register::FromOperand(array_operand));
  Address index_address =
      ReadInterpreterRegister(interpreter::Register::FromOperand(index_operand));
  Address value_address =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];

  if (!IsPlausibleTaggedValue(array_address) ||
      !HasReadableHeapObjectMap(array_address) ||
      !IsJSArray(Tagged<Object>(array_address))) {
    PrintF("WasmInterpreterEntryTrampoline: array literal store receiver "
           "invalid ");
    DumpRuntimeArg("array", 0, array_address);
    PrintF("\n");
    return false;
  }
  if (!IsPlausibleTaggedValue(index_address) ||
      (!HAS_SMI_TAG(index_address) && !HasReadableHeapObjectMap(index_address)) ||
      !IsNumber(Tagged<Object>(index_address))) {
    PrintF("WasmInterpreterEntryTrampoline: array literal store index invalid ");
    DumpRuntimeArg("index", 0, index_address);
    PrintF("\n");
    return false;
  }
  if (!IsSafeTaggedHandleValue(value_address)) {
    value_address = ReadOnlyRoots(isolate).undefined_value().ptr();
  }

  Address feedback_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          InterpreterFrameConstants::kFeedbackVectorFromFp)];
  HandleScope scope(isolate);
  Handle<FeedbackVector> vector;
  if (IsFeedbackVectorAddress(feedback_address)) {
    vector =
        handle(Cast<FeedbackVector>(Tagged<Object>(feedback_address)), isolate);
  }

  FeedbackSlot vector_slot = FeedbackVector::ToSlot(slot_index);
  DirectHandle<JSArray> array =
      direct_handle(Cast<JSArray>(Tagged<Object>(array_address)), isolate);
  Handle<Object> index = handle(Tagged<Object>(index_address), isolate);
  DirectHandle<Object> value =
      direct_handle(Tagged<Object>(value_address), isolate);
  StoreInArrayLiteralIC ic(isolate, vector, vector_slot);

  DirectHandle<Object> result;
  if (!ic.Store(array, index, value).ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryRunCreateObjectLiteralBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateObjectLiteral) {
    return false;
  }

  uint32_t description_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  uint32_t literal_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
  uint32_t flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (description_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad object literal index=%u "
           "length=%d\n",
           description_index, constant_pool->length());
    return false;
  }
  Tagged<Object> description_object = constant_pool->get(description_index);
  if (!IsObjectBoilerplateDescription(description_object)) {
    PrintF("WasmInterpreterEntryTrampoline: object literal constant invalid "
           "index=%u ",
           description_index);
    DumpRuntimeArg("value", 0, description_object.ptr());
    PrintF("\n");
    return false;
  }

  Address feedback_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          InterpreterFrameConstants::kFeedbackVectorFromFp)];
  if (!IsFeedbackVectorAddress(feedback_address)) {
    feedback_address = ReadOnlyRoots(isolate).undefined_value().ptr();
  }

  Address args[4] = {
      Smi::FromInt(static_cast<int>(flags)).ptr(),
      description_object.ptr(),
      TaggedIndex::FromIntptr(static_cast<intptr_t>(literal_index)).ptr(),
      feedback_address,
  };
  *out_result = Runtime_CreateObjectLiteral(4, &args[3], isolate);
  return true;
}

bool TryRunDefineKeyedOwnPropertyInLiteralBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kDefineKeyedOwnPropertyInLiteral) {
    return false;
  }

  int32_t object_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  int32_t name_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                operand_scale);
  uint32_t flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);
  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 3,
                                  operand_scale);

  Address object_address =
      ReadInterpreterRegister(interpreter::Register::FromOperand(object_operand));
  Address name_address =
      ReadInterpreterRegister(interpreter::Register::FromOperand(name_operand));
  Address value_address =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  ReadOnlyRoots roots(isolate);

  if (!IsPlausibleTaggedValue(object_address) ||
      !HasReadableHeapObjectMap(object_address) ||
      !IsJSReceiver(Tagged<Object>(object_address))) {
    PrintF("WasmInterpreterEntryTrampoline: keyed literal receiver invalid ");
    DumpRuntimeArg("object", 0, object_address);
    PrintF(" ");
    DumpRuntimeArg("name", 0, name_address);
    PrintF("\n");
    *out_result = IsSafeTaggedHandleValue(value_address)
                      ? value_address
                      : roots.undefined_value().ptr();
    return true;
  }

  if (!IsSafeTaggedHandleValue(name_address)) {
    name_address = roots.undefined_value().ptr();
  }
  if (!IsSafeTaggedHandleValue(value_address)) {
    value_address = roots.undefined_value().ptr();
  }

  Tagged<Object> name_object(name_address);
  if (IsTheHole(name_object, roots) || IsUninitialized(name_object, roots)) {
    name_address = roots.undefined_value().ptr();
  }

  // The wasm32 interpreter frame can carry placeholder feedback state while
  // bootstrapping. Avoid the optional FeedbackNexus update and let the runtime
  // perform the actual literal define.
  Address feedback_address = roots.undefined_value().ptr();

  Address args[6] = {
      TaggedIndex::FromIntptr(static_cast<intptr_t>(slot_index)).ptr(),
      feedback_address,
      Smi::FromInt(static_cast<int>(flags)).ptr(),
      value_address,
      name_address,
      object_address,
  };
  *out_result = Runtime_DefineKeyedOwnPropertyInLiteral(6, &args[5], isolate);
  return true;
}

bool TryRunGetIteratorBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                               int bytecode_index,
                               interpreter::Bytecode bytecode_enum,
                               interpreter::OperandScale operand_scale,
                               Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kGetIterator) return false;

  int32_t receiver_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  Address receiver_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(receiver_operand));
  ReadOnlyRoots roots(isolate);
  if (!IsJSAnyForWasmPropertyLookup(isolate, receiver_address)) {
    PrintF("WasmInterpreterEntryTrampoline: iterator receiver invalid ");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<Object> receiver_object(receiver_address);
  HandleScope scope(isolate);
  Handle<JSAny> receiver = handle(Cast<JSAny>(receiver_object), isolate);
  if (IsJSArray(receiver_object)) {
    DirectHandle<Map> iterator_map(
        isolate->native_context()->initial_array_iterator_map(), isolate);
    DirectHandle<JSArrayIterator> iterator =
        Cast<JSArrayIterator>(isolate->factory()->NewJSObjectFromMap(
            iterator_map, AllocationType::kYoung));
    iterator->set_iterated_object(Cast<JSReceiver>(receiver_object));
    iterator->set_next_index(Smi::zero(), SKIP_WRITE_BARRIER);
    iterator->set_kind(IterationKind::kValues);
    *out_result = (*iterator).ptr();
    return true;
  }

  DirectHandle<Name> iterator_symbol = isolate->factory()->iterator_symbol();
  DirectHandle<Object> method;
  if (!Runtime::GetObjectProperty(isolate, receiver, iterator_symbol)
           .ToHandle(&method)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  if (!IsCallable(*method)) {
    PrintF("WasmInterpreterEntryTrampoline: iterator method not callable ");
    DumpRuntimeArg("method", 0, (*method).ptr());
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> iterator;
  if (!Execution::Call(isolate, method, receiver, {}).ToHandle(&iterator)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  if (!IsJSReceiver(*iterator)) {
    PrintF("WasmInterpreterEntryTrampoline: iterator result is not receiver ");
    DumpRuntimeArg("iterator", 0, (*iterator).ptr());
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result = (*iterator).ptr();
  return true;
}

Address NamedLoadKeyFromRuntimeArgs(Address* argv, int argc) {
  Address key = RuntimeVisibleArg(argv, argc, 1);
  if (!IsNameAddress(key) && argc > 2) {
    Address alternate = RuntimeVisibleArg(argv, argc, 2);
    if (IsNameAddress(alternate)) key = alternate;
  }
  return key;
}

Address KeyedLoadKeyFromRuntimeArgs(Address* argv, int argc) {
  Address key = RuntimeVisibleArg(argv, argc, 1);
  if (argc > 2 && HAS_SMI_TAG(key) && IsTaggedIndex(Tagged<Object>(key))) {
    Address alternate = RuntimeVisibleArg(argv, argc, 2);
    if (IsNameAddress(alternate)) key = alternate;
  }
  return key;
}

bool TryFallbackICMiss(Isolate* isolate, Runtime::FunctionId function_id,
                       Address* argv, int argc, Address* out_result) {
  if (argc != 4) return false;
  if (function_id != Runtime::kLoadIC_Miss &&
      function_id != Runtime::kKeyedLoadIC_Miss) {
    return false;
  }

  Address maybe_vector = RuntimeVisibleArg(argv, argc, 3);
  if (IsFeedbackVectorAddress(maybe_vector)) return false;

  HandleScope scope(isolate);
  Address receiver_address = RuntimeVisibleArg(argv, argc, 0);
  ReadOnlyRoots roots(isolate);
  if (!IsJSAnyForWasmPropertyLookup(isolate, receiver_address)) {
    PrintF("WasmRuntimeCallFromGenerated: IC fallback receiver invalid");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF("\n");
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  Tagged<Object> receiver_object(receiver_address);
  DirectHandle<JSAny> receiver =
      direct_handle(Cast<JSAny>(receiver_object), isolate);

  Address key_address;
  if (function_id == Runtime::kLoadIC_Miss) {
    key_address = NamedLoadKeyFromRuntimeArgs(argv, argc);
    if (!IsNameAddress(key_address)) {
      PrintF("WasmRuntimeCallFromGenerated: LoadIC fallback missing Name key");
      DumpRuntimeArg("key", 0, key_address);
      PrintF("\n");
      *out_result = roots.exception().ptr();
      return true;
    }
  } else {
    key_address = KeyedLoadKeyFromRuntimeArgs(argv, argc);
    if (!IsSafeTaggedHandleValue(key_address)) {
      key_address = roots.undefined_value().ptr();
    }
  }

  DirectHandle<Object> key = direct_handle(Tagged<Object>(key_address), isolate);
  MaybeDirectHandle<Object> maybe_result =
      Runtime::GetObjectProperty(isolate, receiver, key);

  DirectHandle<Object> result;
  if (!maybe_result.ToHandle(&result)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryFallbackGeneratedRuntime(Isolate* isolate,
                                 Runtime::FunctionId function_id,
                                 Address* argv, int argc,
                                 Address* out_result) {
  if (function_id != Runtime::kJSReceiverGetPrototypeOf) return false;
  if (argc != 1) return false;

  ReadOnlyRoots roots(isolate);
  Address receiver_address = RuntimeVisibleArg(argv, argc, 0);
  if (!IsSafeTaggedHandleValue(receiver_address) ||
      !IsJSReceiver(Tagged<Object>(receiver_address))) {
    Address generated_arg0 = ReadGeneratedJSArgument(0);
    if (IsSafeTaggedHandleValue(generated_arg0) &&
        IsJSReceiver(Tagged<Object>(generated_arg0))) {
      PrintF("WasmRuntimeCallFromGenerated: JSReceiverGetPrototypeOf "
             "recovered receiver from JS arg0");
      DumpRuntimeArg("receiver", 0, generated_arg0);
      PrintF("\n");
      receiver_address = generated_arg0;
    }
  }
  if (!IsSafeTaggedHandleValue(receiver_address) ||
      !IsJSReceiver(Tagged<Object>(receiver_address))) {
    PrintF("WasmRuntimeCallFromGenerated: JSReceiverGetPrototypeOf "
           "receiver invalid");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  HandleScope scope(isolate);
  DirectHandle<JSReceiver> receiver =
      direct_handle(Cast<JSReceiver>(Tagged<Object>(receiver_address)),
                    isolate);
  DirectHandle<JSPrototype> prototype;
  if (!JSReceiver::GetPrototype(isolate, receiver).ToHandle(&prototype)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result = (*prototype).ptr();
  return true;
}

Address WasmBytecodeHandlerEntry(Isolate* isolate, uint8_t opcode,
                                 interpreter::OperandScale operand_scale,
                                 Builtin* out_builtin) {
  interpreter::Bytecode bytecode = interpreter::Bytecodes::FromByte(opcode);
  Tagged<Code> handler =
      isolate->interpreter()->GetBytecodeHandler(bytecode, operand_scale);
  Builtin builtin = handler->builtin_id();
  if (out_builtin != nullptr) *out_builtin = builtin;
  void* fn = WasmBuiltinFuncref(builtin);
  return fn == nullptr ? kNullAddress : reinterpret_cast<Address>(fn);
}

bool TryFallbackJSEntryBuiltin(Isolate* isolate, Builtin builtin,
                               Tagged<JSFunction> function, int actual_argc,
                               Address** argv, Address* out_result) {
  if (builtin != Builtin::kReflectOwnKeys) return false;

  ReadOnlyRoots roots(isolate);
  PrintF("WasmJSEntry: ReflectOwnKeys args actual_argc=%d", actual_argc);
  for (int i = 0; i < actual_argc && i < 4; ++i) {
    DumpRuntimeArg("argv", i, *argv[i]);
  }
  PrintF("\n");
  Address target_address = actual_argc > 0
                               ? SafeTaggedOrUndefined(isolate, *argv[0])
                               : roots.undefined_value().ptr();
  if (!IsSafeTaggedHandleValue(target_address) ||
      !IsJSReceiver(Tagged<Object>(target_address))) {
    PrintF("WasmJSEntry: ReflectOwnKeys target invalid ");
    DumpRuntimeArg("target", 0, target_address);
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<Context> saved_context = isolate->context();
  isolate->set_context(function->context());

  HandleScope scope(isolate);
  DirectHandle<JSReceiver> target =
      direct_handle(Cast<JSReceiver>(Tagged<Object>(target_address)), isolate);
  DirectHandle<FixedArray> keys;
  if (!KeyAccumulator::GetKeys(isolate, target, KeyCollectionMode::kOwnOnly,
                               ALL_PROPERTIES,
                               GetKeysConversion::kConvertToString)
           .ToHandle(&keys)) {
    isolate->set_context(saved_context);
    *out_result = roots.exception().ptr();
    return true;
  }
  DirectHandle<JSArray> result =
      isolate->factory()->NewJSArrayWithElements(keys);
  *out_result = (*result).ptr();
  isolate->set_context(saved_context);
  PrintF("WasmJSEntry: fallback ReflectOwnKeys result=0x%x\n",
         static_cast<unsigned>(*out_result));
  return true;
}

}  // namespace

extern "C" Address WasmRuntimeCallFromGenerated(Address runtime_entry,
                                                int32_t argc) {
  constexpr int kMaxWasmRuntimeArgs = 64;
  Address root = g_wasm_regs[kWasmRegRoot];
  if (root == kNullAddress) root = g_wasm_regs[SlotFor(kRootRegister)];
  Isolate* isolate = Isolate::FromRootAddress(root);
  if (argc < 0 || argc > kMaxWasmRuntimeArgs) {
    PrintF("WasmRuntimeCallFromGenerated: bad entry=0x%x argc=%d\n",
           static_cast<unsigned>(runtime_entry), argc);
    return Smi::zero().ptr();
  }

  const Runtime::Function* function = nullptr;
  int32_t encoded_runtime_entry = static_cast<int32_t>(runtime_entry);
  if (encoded_runtime_entry < 0) {
    int runtime_id = -encoded_runtime_entry - 1;
    if (runtime_id >= 0 && runtime_id < Runtime::kNumFunctions) {
      function =
          Runtime::FunctionForId(static_cast<Runtime::FunctionId>(runtime_id));
    }
  } else {
    function = Runtime::FunctionForEntry(runtime_entry);
  }
  if (function == nullptr) {
    PrintF("WasmRuntimeCallFromGenerated: unknown entry=0x%x argc=%d\n",
           static_cast<unsigned>(runtime_entry), argc);
    return Smi::zero().ptr();
  }
  if (function->result_size != 1 ||
      (function->nargs >= 0 && function->nargs != argc)) {
    PrintF("WasmRuntimeCallFromGenerated: unsupported %s id=%d argc=%d "
           "expected=%d results=%d\n",
           function->name, static_cast<int>(function->function_id), argc,
           function->nargs,
           function->result_size);
    return Smi::zero().ptr();
  }

  Address argv[kMaxWasmRuntimeArgs == 0 ? 1 : kMaxWasmRuntimeArgs];
  for (int i = 0; i < argc; ++i) {
    int slot = kWasmOutgoingArgSlotBase + i;
    argv[argc - 1 - i] = g_wasm_regs[slot];
  }

  if (function->function_id == Runtime::kLoadIC_Miss ||
      function->function_id == Runtime::kKeyedLoadIC_Miss ||
      function->function_id == Runtime::kJSReceiverGetPrototypeOf) {
    PrintF("WasmRuntimeCallFromGenerated: entering %s id=%d argc=%d "
           "entry=0x%x context=0x%x\n",
           function->name, static_cast<int>(function->function_id), argc,
           static_cast<unsigned>(runtime_entry),
           static_cast<unsigned>(g_wasm_regs[SlotFor(kContextRegister)]));
    for (int i = 0; i < argc; ++i) {
      DumpRuntimeArg("arg", i, argv[argc - 1 - i]);
    }
    PrintF("\n");
    for (int i = 0; i < argc; ++i) {
      DumpRuntimeArg("slot", kWasmOutgoingArgSlotBase + i,
                     g_wasm_regs[kWasmOutgoingArgSlotBase + i]);
    }
    PrintF("\n");
    for (int i = -8; i < 16; ++i) {
      int slot = kWasmOutgoingArgSlotBase + i;
      if (slot >= 0 && slot < kWasmRegFileSize) {
        DumpRuntimeArg("win", slot, g_wasm_regs[slot]);
      }
    }
    PrintF("\n");
  }

  Tagged<Context> saved_context = isolate->context();
  Address context = g_wasm_regs[SlotFor(kContextRegister)];
  bool switched_context = false;
  if (context != kNullAddress && IsContext(Tagged<Object>(context))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context)));
    switched_context = true;
  }

  Address fallback_result = kNullAddress;
  if (TryFallbackICMiss(isolate, function->function_id, argv, argc,
                        &fallback_result)) {
    if (switched_context) isolate->set_context(saved_context);
    g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
    PrintF("WasmRuntimeCallFromGenerated: %s id=%d argc=%d fallback=0x%x\n",
           function->name, static_cast<int>(function->function_id), argc,
           static_cast<unsigned>(fallback_result));
    return fallback_result;
  }
  if (TryFallbackGeneratedRuntime(isolate, function->function_id, argv, argc,
                                  &fallback_result)) {
    if (switched_context) isolate->set_context(saved_context);
    g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
    PrintF("WasmRuntimeCallFromGenerated: %s id=%d argc=%d fallback=0x%x\n",
           function->name, static_cast<int>(function->function_id), argc,
           static_cast<unsigned>(fallback_result));
    return fallback_result;
  }

  using RuntimeEntry = Address (*)(int, Address*, Isolate*);
  Address* args_object = argc == 0 ? argv : &argv[argc - 1];
  Address result =
      reinterpret_cast<RuntimeEntry>(function->entry)(argc, args_object,
                                                      isolate);
  if (switched_context) isolate->set_context(saved_context);

  g_wasm_regs[SlotFor(kReturnRegister0)] = result;
  PrintF("WasmRuntimeCallFromGenerated: %s id=%d argc=%d result=0x%x\n",
         function->name, static_cast<int>(function->function_id), argc,
         static_cast<unsigned>(result));
  return result;
}

extern "C" void WasmInterpreterEntryTrampoline() {
  Address root = g_wasm_regs[kWasmRegRoot];
  Isolate* isolate = Isolate::FromRootAddress(root);
  Address target = g_wasm_regs[SlotFor(kJavaScriptCallTargetRegister)];
  Address new_target = g_wasm_regs[SlotFor(kJavaScriptCallNewTargetRegister)];
  int argc =
      static_cast<int>(g_wasm_regs[SlotFor(kJavaScriptCallArgCountRegister)]);

  PrintF("WasmInterpreterEntryTrampoline: target=0x%x new_target=0x%x argc=%d\n",
         static_cast<unsigned>(target), static_cast<unsigned>(new_target), argc);

  Tagged<Object> target_object(target);
  if (!IsJSFunction(target_object)) {
    PrintF("WasmInterpreterEntryTrampoline: target is not JSFunction\n");
    g_wasm_regs[SlotFor(kReturnRegister0)] = Smi::zero().ptr();
    return;
  }

  Tagged<JSFunction> function = Cast<JSFunction>(target_object);
  Tagged<SharedFunctionInfo> shared = function->shared();
  if (!shared->HasBytecodeArray()) {
    PrintF("WasmInterpreterEntryTrampoline: missing bytecode array\n");
    g_wasm_regs[SlotFor(kReturnRegister0)] = Smi::zero().ptr();
    return;
  }

  Tagged<BytecodeArray> bytecode = shared->GetBytecodeArray(isolate);
  Address bytecode_offset = BytecodeArray::kHeaderSize - kHeapObjectTag;
  Address dispatch_table = isolate->interpreter()->dispatch_table_address();
  Address undefined = ReadOnlyRoots(isolate).undefined_value().ptr();
  Address feedback_vector =
      function->has_feedback_vector() ? function->feedback_vector().ptr()
                                      : undefined;
  PrintF("WasmInterpreterEntryTrampoline: feedback has=%d value=",
         function->has_feedback_vector());
  DumpRuntimeArg("fbv", 0, feedback_vector);
  PrintF(" fbv_operand=%d fbv_offset=%d\n",
         interpreter::Register::feedback_vector().ToOperand(),
         InterpreterFrameConstants::kFeedbackVectorFromFp);

  ClearInterpreterFrame();
  StoreInterpreterFrameOffset(CommonFrameConstants::kCallerFPOffset, 0);
  StoreInterpreterFrameOffset(CommonFrameConstants::kCallerPCOffset, 0);
  StoreInterpreterFrameOffset(StandardFrameConstants::kContextOffset,
                              function->context().ptr());
  StoreInterpreterFrameOffset(StandardFrameConstants::kFunctionOffset, target);
  StoreInterpreterFrameOffset(StandardFrameConstants::kArgCOffset,
                              static_cast<Address>(argc));
  StoreInterpreterFrameOffset(InterpreterFrameConstants::kBytecodeArrayFromFp,
                              bytecode.ptr());
  StoreInterpreterFrameOffset(InterpreterFrameConstants::kBytecodeOffsetFromFp,
                              Smi::FromInt(static_cast<int>(bytecode_offset))
                                  .ptr());
  StoreInterpreterFrameOffset(InterpreterFrameConstants::kFeedbackVectorFromFp,
                              feedback_vector);
  Address current_context_from_frame =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          interpreter::Register::current_context().ToOperand() *
          kSystemPointerSize)];
  PrintF("WasmInterpreterEntryTrampoline: context=0x%x current_operand=%d "
         "frame_current=0x%x ctx_slot=0x%x fp=0x%x\n",
         static_cast<unsigned>(function->context().ptr()),
         interpreter::Register::current_context().ToOperand(),
         static_cast<unsigned>(current_context_from_frame),
         static_cast<unsigned>(g_wasm_interpreter_frame[
             InterpreterFrameSlotForOffset(StandardFrameConstants::kContextOffset)]),
         static_cast<unsigned>(g_wasm_current_frame_pointer));
  PrintF("WasmInterpreterEntryTrampoline:");
  DumpRuntimeArg("ctx", 0, function->context().ptr());
  PrintF(" mem_pages=%zu mem_bytes=0x%zx\n",
         static_cast<size_t>(__builtin_wasm_memory_size(0)),
         static_cast<size_t>(__builtin_wasm_memory_size(0)) * 65536u);

  int register_count = bytecode->register_count();
  for (int i = 0; i < register_count; ++i) {
    StoreInterpreterRegister(interpreter::Register(i), undefined);
  }

  int actual_argc = argc - kJSArgcReceiverSlots;
  if (actual_argc < 0) actual_argc = 0;
  int parameter_count = bytecode->parameter_count();
  int frame_parameter_count = parameter_count;
  if (frame_parameter_count < actual_argc + 1) {
    frame_parameter_count = actual_argc + 1;
  }
  for (int i = 0; i < frame_parameter_count; ++i) {
    Address value = undefined;
    if (i == 0) {
      value = g_wasm_regs[kWasmStackSlotBase];
    } else if (i - 1 < actual_argc) {
      value = g_wasm_regs[kWasmStackSlotBase + i];
    }
    StoreInterpreterRegister(interpreter::Register::FromParameterIndex(i), value);
  }

  interpreter::Register incoming =
      bytecode->incoming_new_target_or_generator_register();
  if (incoming.is_valid()) {
    StoreInterpreterRegister(incoming, new_target);
  }

  g_wasm_regs[SlotFor(kRootRegister)] = root;
  g_wasm_regs[SlotFor(kContextRegister)] = function->context().ptr();
  g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = undefined;
  g_wasm_regs[SlotFor(Register::r4())] = bytecode_offset;
  g_wasm_regs[SlotFor(Register::r5())] = bytecode.ptr();
  g_wasm_regs[SlotFor(Register::r6())] = dispatch_table;
  g_wasm_regs[SlotFor(kInterpreterBytecodeOffsetRegister)] = bytecode_offset;
  g_wasm_regs[SlotFor(kInterpreterBytecodeArrayRegister)] = bytecode.ptr();
  g_wasm_regs[kWasmStackSlotBase] = bytecode_offset;
  g_wasm_regs[kWasmStackSlotBase + 1] = bytecode.ptr();
  g_wasm_regs[kWasmStackSlotBase + 2] = dispatch_table;
  g_wasm_regs[kWasmStackSlotBase + 3] = g_wasm_current_frame_pointer;

  using WasmRegFileFn = void (*)();
  constexpr int kMaxInterpreterSteps = 100000;
  Address current_offset = bytecode_offset;
  interpreter::OperandScale operand_scale = interpreter::OperandScale::kSingle;

  for (int step = 0; step < kMaxInterpreterSteps; ++step) {
    int bytecode_index = static_cast<int>(current_offset - bytecode_offset);
    if (bytecode_index < 0 || bytecode_index >= bytecode->length()) {
      PrintF("WasmInterpreterEntryTrampoline: bytecode offset OOB index=%d "
             "length=%d raw_offset=0x%x\n",
             bytecode_index, bytecode->length(),
             static_cast<unsigned>(current_offset));
      g_wasm_regs[SlotFor(kReturnRegister0)] =
          ReadOnlyRoots(isolate).exception().ptr();
      return;
    }

    uint8_t opcode = bytecode->get(bytecode_index);
    interpreter::Bytecode bytecode_enum =
        interpreter::Bytecodes::FromByte(opcode);
    if (interpreter::Bytecodes::IsPrefixScalingBytecode(bytecode_enum)) {
      operand_scale =
          interpreter::Bytecodes::PrefixBytecodeToOperandScale(bytecode_enum);
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum,
                                      interpreter::OperandScale::kSingle);
      continue;
    }

    Builtin handler_builtin = Builtin::kIllegal;
    Address entry =
        WasmBytecodeHandlerEntry(isolate, opcode, operand_scale,
                                 &handler_builtin);
    bool should_log_step =
        step < 16 || bytecode_enum == interpreter::Bytecode::kCreateClosure ||
        bytecode_enum == interpreter::Bytecode::kLdaGlobal ||
        bytecode_enum == interpreter::Bytecode::kLdaGlobalInsideTypeof ||
        bytecode_enum == interpreter::Bytecode::kGetNamedProperty ||
        bytecode_enum == interpreter::Bytecode::kGetKeyedProperty ||
        bytecode_enum == interpreter::Bytecode::kToName ||
        bytecode_enum == interpreter::Bytecode::kAdd ||
        bytecode_enum ==
            interpreter::Bytecode::kAdd_LhsIsStringConstant_Internalize ||
        bytecode_enum == interpreter::Bytecode::kAddSmi ||
        bytecode_enum == interpreter::Bytecode::kSetNamedProperty ||
        bytecode_enum == interpreter::Bytecode::kDefineNamedOwnProperty ||
        bytecode_enum == interpreter::Bytecode::kCreateRegExpLiteral ||
        bytecode_enum == interpreter::Bytecode::kCreateArrayLiteral ||
        bytecode_enum == interpreter::Bytecode::kStaInArrayLiteral ||
        bytecode_enum == interpreter::Bytecode::kCreateObjectLiteral ||
        bytecode_enum ==
            interpreter::Bytecode::kDefineKeyedOwnPropertyInLiteral ||
        bytecode_enum == interpreter::Bytecode::kGetIterator ||
        interpreter::Bytecodes::IsJump(bytecode_enum) ||
        interpreter::Bytecodes::IsSwitch(bytecode_enum) ||
        bytecode_enum == interpreter::Bytecode::kCallAnyReceiver ||
        bytecode_enum == interpreter::Bytecode::kCallProperty ||
        bytecode_enum == interpreter::Bytecode::kCallProperty0 ||
        bytecode_enum == interpreter::Bytecode::kCallProperty1 ||
        bytecode_enum == interpreter::Bytecode::kCallProperty2 ||
        bytecode_enum == interpreter::Bytecode::kCallUndefinedReceiver ||
        bytecode_enum == interpreter::Bytecode::kCallUndefinedReceiver0 ||
        bytecode_enum == interpreter::Bytecode::kCallUndefinedReceiver1 ||
        bytecode_enum == interpreter::Bytecode::kCallUndefinedReceiver2 ||
        bytecode_enum == interpreter::Bytecode::kTestEqual ||
        bytecode_enum == interpreter::Bytecode::kTestEqualStrict ||
        bytecode_enum == interpreter::Bytecode::kTestLessThan ||
        bytecode_enum == interpreter::Bytecode::kTestGreaterThan ||
        bytecode_enum == interpreter::Bytecode::kTestLessThanOrEqual ||
        bytecode_enum == interpreter::Bytecode::kTestGreaterThanOrEqual ||
        bytecode_enum == interpreter::Bytecode::kTestReferenceEqual ||
        bytecode_enum == interpreter::Bytecode::kTestNull ||
        bytecode_enum == interpreter::Bytecode::kTestUndefined ||
        bytecode_enum == interpreter::Bytecode::kTestTypeOf ||
        (step >= 420 && step <= 440);
    if (should_log_step) {
      PrintF("WasmInterpreterEntryTrampoline: step=%d bytecode=0x%x(%s) "
             "offset=0x%x index=%d size=%d scale=%d handler_builtin=%d "
             "handler=0x%x frame_fp=0x%x regs=%d params=%d\n",
             step,
             static_cast<unsigned>(opcode),
             interpreter::Bytecodes::ToString(bytecode_enum),
             static_cast<unsigned>(current_offset),
             bytecode_index,
             interpreter::Bytecodes::Size(bytecode_enum, operand_scale),
             static_cast<int>(operand_scale),
             static_cast<int>(handler_builtin), static_cast<unsigned>(entry),
             static_cast<unsigned>(g_wasm_current_frame_pointer),
             register_count, parameter_count);
      int operand_count = interpreter::Bytecodes::NumberOfOperands(bytecode_enum);
      PrintF("  operands:");
      for (int operand_index = 0; operand_index < operand_count; ++operand_index) {
        PrintF(" #%d=0x%x", operand_index,
               ReadBytecodeUnsignedOperand(bytecode, bytecode_index,
                                           bytecode_enum, operand_index,
                                           operand_scale));
      }
      PrintF(" raw:");
      for (int raw_index = 0;
           raw_index < interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
           ++raw_index) {
        PrintF(" %02x", bytecode->get(bytecode_index + raw_index));
      }
      PrintF("\n");
      PrintF("  frame:");
      DumpRuntimeArg("function", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         StandardFrameConstants::kFunctionOffset)]);
      DumpRuntimeArg("context", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         StandardFrameConstants::kContextOffset)]);
      DumpRuntimeArg("current_context", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         interpreter::Register::current_context().ToOperand() *
                         kSystemPointerSize)]);
      DumpRuntimeArg("bytecode_array", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         InterpreterFrameConstants::kBytecodeArrayFromFp)]);
      DumpRuntimeArg("bytecode_offset", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         InterpreterFrameConstants::kBytecodeOffsetFromFp)]);
      DumpRuntimeArg("feedback", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         InterpreterFrameConstants::kFeedbackVectorFromFp)]);
      int logged_parameter_count = parameter_count < 5 ? parameter_count : 5;
      for (int parameter_index = 0; parameter_index < logged_parameter_count;
           ++parameter_index) {
        DumpRuntimeArg(
            "p", parameter_index,
            ReadInterpreterRegister(
                interpreter::Register::FromParameterIndex(parameter_index)));
      }
      int logged_register_count =
          (step >= 420 && step <= 440)
              ? register_count
              : (register_count < 8 ? register_count : 8);
      for (int register_index = 0; register_index < logged_register_count;
           ++register_index) {
        DumpRuntimeArg("l", register_index,
                       ReadInterpreterRegister(
                           interpreter::Register(register_index)));
      }
      PrintF(" r0=0x%x r10=0x%x r11=0x%x\n",
             static_cast<unsigned>(
                 g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]),
             static_cast<unsigned>(
                 g_wasm_regs[SlotFor(kInterpreterBytecodeOffsetRegister)]),
             static_cast<unsigned>(
                 g_wasm_regs[SlotFor(kInterpreterBytecodeArrayRegister)]));
    }
    if (entry == kNullAddress) {
      PrintF("WasmInterpreterEntryTrampoline: missing handler bytecode=0x%x "
             "scale=%d\n",
             static_cast<unsigned>(opcode), static_cast<int>(operand_scale));
      g_wasm_regs[SlotFor(kReturnRegister0)] = Smi::zero().ptr();
      return;
    }

    Address fallback_result = kNullAddress;
    Address fallback_offset = kNullAddress;
    if (TryRunJumpBytecode(isolate, bytecode, bytecode_index, bytecode_enum,
                           operand_scale, current_offset, &fallback_offset)) {
      if (should_log_step) {
        PrintF("  fallback %s next=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_offset));
      }
      current_offset = fallback_offset;
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCallBytecode(isolate, bytecode, bytecode_index, bytecode_enum,
                           operand_scale, &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateFunctionContextBytecode(isolate, bytecode, bytecode_index,
                                            bytecode_enum, operand_scale,
                                            &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback CreateFunctionContext result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateClosureBytecode(isolate, bytecode, bytecode_index,
                                    bytecode_enum, operand_scale,
                                    &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback CreateClosure result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunLdaGlobalBytecode(isolate, bytecode, bytecode_index,
                                bytecode_enum, operand_scale,
                                &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback LdaGlobal result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunGetNamedPropertyBytecode(isolate, bytecode, bytecode_index,
                                       bytecode_enum, operand_scale,
                                       &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback GetNamedProperty result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunGetKeyedPropertyBytecode(isolate, bytecode, bytecode_index,
                                       bytecode_enum, operand_scale,
                                       &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback GetKeyedProperty result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunToNameBytecode(isolate, bytecode_enum, &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback ToName result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunTestInBytecode(isolate, bytecode, bytecode_index, bytecode_enum,
                             operand_scale, &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback TestIn result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunAddBytecode(isolate, bytecode, bytecode_index, bytecode_enum,
                          operand_scale, &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunSetNamedPropertyBytecode(isolate, bytecode, bytecode_index,
                                       bytecode_enum, operand_scale,
                                       &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunSetKeyedPropertyBytecode(isolate, bytecode, bytecode_index,
                                       bytecode_enum, operand_scale,
                                       &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateRegExpLiteralBytecode(isolate, bytecode, bytecode_index,
                                          bytecode_enum, operand_scale,
                                          &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback CreateRegExpLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateArrayLiteralBytecode(isolate, bytecode, bytecode_index,
                                         bytecode_enum, operand_scale,
                                         &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback CreateArrayLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunStaInArrayLiteralBytecode(isolate, bytecode, bytecode_index,
                                        bytecode_enum, operand_scale,
                                        &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback StaInArrayLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateObjectLiteralBytecode(isolate, bytecode, bytecode_index,
                                          bytecode_enum, operand_scale,
                                          &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback CreateObjectLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunDefineKeyedOwnPropertyInLiteralBytecode(
            isolate, bytecode, bytecode_index, bytecode_enum, operand_scale,
            &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback DefineKeyedOwnPropertyInLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunGetIteratorBytecode(isolate, bytecode, bytecode_index,
                                  bytecode_enum, operand_scale,
                                  &fallback_result)) {
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = fallback_result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
      if (should_log_step) {
        PrintF("  fallback GetIterator result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }

    g_wasm_regs[SlotFor(kInterpreterBytecodeOffsetRegister)] = current_offset;
    g_wasm_regs[SlotFor(Register::r4())] = current_offset;
    g_wasm_regs[SlotFor(Register::r5())] = bytecode.ptr();
    g_wasm_regs[SlotFor(Register::r6())] = dispatch_table;
    g_wasm_regs[SlotFor(kInterpreterBytecodeArrayRegister)] = bytecode.ptr();
    StoreInterpreterFrameOffset(InterpreterFrameConstants::kBytecodeOffsetFromFp,
                                Smi::FromInt(static_cast<int>(current_offset))
                                    .ptr());

    reinterpret_cast<WasmRegFileFn>(entry)();

    if (interpreter::Bytecodes::Returns(bytecode_enum)) {
      Address result = g_wasm_regs[SlotFor(kReturnRegister0)];
      if (result == kNullAddress) {
        result = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
      }
      PrintF("WasmInterpreterEntryTrampoline: return bytecode=0x%x steps=%d "
             "result=0x%x\n",
             static_cast<unsigned>(opcode), step + 1,
             static_cast<unsigned>(result));
      g_wasm_regs[SlotFor(kReturnRegister0)] = result;
      return;
    }

    Address next_offset = current_offset +
                          interpreter::Bytecodes::Size(bytecode_enum,
                                                       operand_scale);
    if (interpreter::Bytecodes::IsJump(bytecode_enum) ||
        interpreter::Bytecodes::IsSwitch(bytecode_enum)) {
      Address handler_offset =
          g_wasm_regs[SlotFor(kInterpreterBytecodeOffsetRegister)];
      Address bytecode_start = bytecode_offset;
      Address bytecode_limit =
          bytecode_offset + static_cast<Address>(bytecode->length());
      if (handler_offset != current_offset) {
        if (handler_offset >= bytecode_start && handler_offset < bytecode_limit) {
          next_offset = handler_offset;
        } else {
          PrintF("WasmInterpreterEntryTrampoline: ignoring invalid branch "
                 "offset=0x%x bytecode=%s current=0x%x range=[0x%x,0x%x)\n",
                 static_cast<unsigned>(handler_offset),
                 interpreter::Bytecodes::ToString(bytecode_enum),
                 static_cast<unsigned>(current_offset),
                 static_cast<unsigned>(bytecode_start),
                 static_cast<unsigned>(bytecode_limit));
        }
      }
    }
    current_offset = next_offset;
    operand_scale = interpreter::OperandScale::kSingle;
  }

  PrintF("WasmInterpreterEntryTrampoline: exceeded max steps length=%d\n",
         bytecode->length());
  g_wasm_regs[SlotFor(kReturnRegister0)] =
      ReadOnlyRoots(isolate).exception().ptr();
}

// Hand-written JSEntry. Signature mirrors execution.cc's JSEntryFunction.
// This is the narrow C++ -> generated-wasm bridge: seed the emulated wasm32
// register/stack slots, then call the JSFunction's current code entry.
extern "C" Address WasmJSEntry(Address root, Address new_target, Address target,
                               Address receiver, intptr_t argc,
                               Address** argv) {
  Isolate* isolate = Isolate::FromRootAddress(root);
  g_wasm_regs[kWasmRegRoot] = root;
  PrintF("WasmJSEntry: enter root=0x%x new_target=0x%x target=0x%x "
         "receiver=0x%x argc=%d\n",
         static_cast<unsigned>(root), static_cast<unsigned>(new_target),
         static_cast<unsigned>(target), static_cast<unsigned>(receiver),
         static_cast<int>(argc));

  Tagged<Object> target_object(target);
  if (!IsJSFunction(target_object)) {
    PrintF("WasmJSEntry: target is not JSFunction\n");
    return Smi::zero().ptr();
  }

  Tagged<JSFunction> function = Cast<JSFunction>(target_object);
  Tagged<Code> code = function->code(isolate);
  Address entry = code->instruction_start();
  bool uses_interpreter_entry = false;
  int actual_argc = static_cast<int>(argc) - kJSArgcReceiverSlots;
  if (actual_argc < 0) actual_argc = 0;
  PrintF("WasmJSEntry: argv actual_argc=%d", actual_argc);
  for (int i = 0; i < actual_argc && i < 6; ++i) {
    DumpRuntimeArg("argv", i, *argv[i]);
  }
  PrintF("\n");
  if (code->is_builtin()) {
    Builtin builtin = code->builtin_id();
    uses_interpreter_entry =
        builtin == Builtin::kInterpreterEntryTrampoline ||
        builtin == Builtin::kInterpreterEntryTrampolineForProfiling;
    void* fn = WasmBuiltinFuncref(builtin);
    if (fn == nullptr) {
      Address fallback_result = kNullAddress;
      if (TryFallbackJSEntryBuiltin(isolate, builtin, function, actual_argc,
                                    argv, &fallback_result)) {
        return fallback_result;
      }
      PrintF("WasmJSEntry: unregistered builtin target=%d name=%s "
             "entry=0x%x\n",
             static_cast<int>(builtin), Builtins::name(builtin),
             static_cast<unsigned>(entry));
      return Smi::zero().ptr();
    }
    entry = reinterpret_cast<Address>(fn);
  }
  PrintF("WasmJSEntry: code builtin=%d name=%s is_builtin=%d entry=0x%x\n",
         static_cast<int>(code->builtin_id()),
         code->is_builtin() ? Builtins::name(code->builtin_id()) : "<none>",
         code->is_builtin(),
         static_cast<unsigned>(entry));
  if (entry == kNullAddress) {
    PrintF("WasmJSEntry: null code entry\n");
    return Smi::zero().ptr();
  }

  constexpr int kWasmFixedFrameSlotLimit = kWasmStackSlotBase - 1;
  int last_argument_offset =
      CommonFrameConstants::kFixedFrameSizeAboveFp +
      actual_argc * kSystemPointerSize;
  if (kWasmFixedFrameSlotBase + StandardFrameConstants::kFixedSlotCountFromFp >=
          kWasmFixedFrameSlotLimit ||
      GeneratedFrameSlotForOffset(last_argument_offset) >=
          kWasmOutgoingArgSlotBase) {
    return Smi::zero().ptr();
  }

  ClearInterpreterFrame();
  ClearEntrypointStackWindow();
  Address undefined = ReadOnlyRoots(isolate).undefined_value().ptr();
  for (int i = kWasmFixedFrameSlotBase; i < kWasmFixedFrameSlotLimit; ++i) {
    g_wasm_regs[i] = undefined;
  }
  StoreInterpreterFrameOffset(CommonFrameConstants::kCallerFPOffset, 0);
  StoreInterpreterFrameOffset(CommonFrameConstants::kCallerPCOffset, 0);
  StoreInterpreterFrameOffset(StandardFrameConstants::kContextOffset,
                              function->context().ptr());
  StoreInterpreterFrameOffset(StandardFrameConstants::kFunctionOffset, target);
  StoreInterpreterFrameOffset(StandardFrameConstants::kArgCOffset,
                              static_cast<Address>(argc));
  StoreGeneratedFrameOffset(CommonFrameConstants::kCallerFPOffset, 0);
  StoreGeneratedFrameOffset(CommonFrameConstants::kCallerPCOffset, 0);
  StoreGeneratedFrameOffset(StandardFrameConstants::kContextOffset,
                            function->context().ptr());
  StoreGeneratedFrameOffset(StandardFrameConstants::kFunctionOffset, target);
  StoreGeneratedFrameOffset(StandardFrameConstants::kArgCOffset,
                            static_cast<Address>(argc));

  StoreInterpreterFrameOffset(CommonFrameConstants::kFixedFrameSizeAboveFp,
                              receiver);
  StoreGeneratedFrameOffset(CommonFrameConstants::kFixedFrameSizeAboveFp,
                            receiver);
  for (int i = 0; i < actual_argc; ++i) {
    Address value = *argv[i];
    int offset = CommonFrameConstants::kFixedFrameSizeAboveFp +
                 (i + 1) * kSystemPointerSize;
    StoreInterpreterFrameOffset(offset, value);
    StoreGeneratedFrameOffset(offset, value);
  }

  g_wasm_regs[SlotFor(kRootRegister)] = root;
  g_wasm_regs[SlotFor(kContextRegister)] = function->context().ptr();
  g_wasm_regs[SlotFor(kJavaScriptCallTargetRegister)] = target;
  g_wasm_regs[SlotFor(kJavaScriptCallNewTargetRegister)] = new_target;
  g_wasm_regs[SlotFor(kJavaScriptCallArgCountRegister)] =
      static_cast<Address>(argc);
  g_wasm_regs[SlotFor(kJavaScriptCallCodeStartRegister)] = entry;
  g_wasm_regs[SlotFor(kJavaScriptCallDispatchHandleRegister)] = 0;
  PrintF("WasmJSEntry: frame slots");
  DumpRuntimeArg("ctx_slot", 0,
                 g_wasm_regs[GeneratedFrameSlotForOffset(
                     StandardFrameConstants::kContextOffset)]);
  DumpRuntimeArg("receiver_slot", 0,
                 g_wasm_regs[GeneratedFrameSlotForOffset(
                     CommonFrameConstants::kFixedFrameSizeAboveFp)]);
  if (actual_argc > 0) {
    DumpRuntimeArg("arg0_slot", 0,
                   g_wasm_regs[GeneratedFrameSlotForOffset(
                       CommonFrameConstants::kFixedFrameSizeAboveFp +
                       kSystemPointerSize)]);
  }
  PrintF("\n");
  if (uses_interpreter_entry) {
    g_wasm_regs[kWasmStackSlotBase] = receiver;
    for (int i = 0; i < actual_argc; ++i) {
      g_wasm_regs[kWasmStackSlotBase + 1 + i] = *argv[i];
    }
  }

  using WasmRegFileFn = void (*)();
  reinterpret_cast<WasmRegFileFn>(entry)();
  PrintF("WasmJSEntry: return=0x%x\n",
         static_cast<unsigned>(g_wasm_regs[SlotFor(kReturnRegister0)]));
  return g_wasm_regs[SlotFor(kReturnRegister0)];
}

void RegisterGeneratedWasmBuiltins() __attribute__((weak));

// Registers all hand-written wasm builtins. Called once during builtin setup.
void RegisterAllWasmBuiltins() {
  RegisterWasmBuiltin(Builtin::kIllegal,
                      reinterpret_cast<void*>(&WasmProbeBuiltin));
  RegisterWasmBuiltin(Builtin::kJSEntry, reinterpret_cast<void*>(&WasmJSEntry));
  RegisterWasmBuiltin(Builtin::kInterpreterEntryTrampoline,
                      reinterpret_cast<void*>(&WasmInterpreterEntryTrampoline));
  RegisterWasmBuiltin(Builtin::kInterpreterEntryTrampolineForProfiling,
                      reinterpret_cast<void*>(&WasmInterpreterEntryTrampoline));
  if (RegisterGeneratedWasmBuiltins != nullptr) {
    RegisterGeneratedWasmBuiltins();
  }
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
