// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// WASM32 builtins stub - WASM32 is a virtual architecture that doesn't
// generate native code, so builtins are minimal stubs.

#if V8_TARGET_ARCH_WASM32

#include "src/builtins/builtins.h"
#include "src/builtins/wasm32/builtins-wasm32-abi.h"
#include "src/codegen/macro-assembler.h"
#include "src/execution/frame-constants.h"
#include "src/objects/smi.h"

namespace v8 {
namespace internal {

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

// Hand-written JSEntry. Signature mirrors execution.cc's JSEntryFunction.
// Milestone scope: prove dispatch succeeds. Calling into the target's code is
// the next plan; here we return Smi::zero() so the trap, if any, moves to the
// caller's handling of the result rather than the JSEntry call_indirect.
extern "C" Address WasmJSEntry(Address root, Address new_target, Address target,
                               Address receiver, intptr_t argc,
                               Address** argv) {
  USE(new_target);
  USE(target);
  USE(receiver);
  USE(argc);
  USE(argv);
  g_wasm_regs[kWasmRegRoot] = root;
  return Smi::zero().ptr();
}

// Registers all hand-written wasm builtins. Called once during builtin setup.
void RegisterAllWasmBuiltins() {
  RegisterWasmBuiltin(Builtin::kIllegal,
                      reinterpret_cast<void*>(&WasmProbeBuiltin));
  RegisterWasmBuiltin(Builtin::kJSEntry, reinterpret_cast<void*>(&WasmJSEntry));
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
