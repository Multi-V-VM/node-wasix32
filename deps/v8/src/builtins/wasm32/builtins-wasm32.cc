// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if V8_TARGET_ARCH_WASM32

#include "src/api/api-arguments.h"
#include "src/builtins/builtins-inl.h"
#include "src/codegen/interface-descriptors-inl.h"
#include "src/codegen/macro-assembler.h"
#include "src/objects/objects-inl.h"

namespace v8 {
namespace internal {

#define __ ACCESS_MASM(masm)

// Load the built-in Array function from the current context.
static void GenerateLoadArrayFunction(MacroAssembler* masm, Register result) {
  // Load the InternalArray function from the current native context.
  __ LoadNativeContextSlot(result, Context::ARRAY_FUNCTION_INDEX);
}

// WASM32 implementation of runtime call helpers
void Builtins::Generate_Adaptor(MacroAssembler* masm, Address address) {
  // WASM32 stub implementation
  __ Move(kContextRegister, Smi::zero());
  __ Push(kJSFunctionRegister);
  __ PushArguments(r0(), r1(), r2());
  __ CallRuntime(ExternalReference::Create(address), 3);
  __ Pop(kJSFunctionRegister);
  __ Ret();
}

void Builtins::Generate_CEntry(MacroAssembler* masm, int result_size,
                               ArgvMode argv_mode, bool builtin_exit_frame,
                               bool switch_to_central_stack) {
  // WASM32 stub implementation for C entry
  // This is a simplified version - a real implementation would need to:
  // 1. Set up the exit frame
  // 2. Call the C function
  // 3. Handle the result
  // 4. Tear down the exit frame
  
  __ EnterExitFrame(builtin_exit_frame, 0);
  
  // Call C function (simplified)
  __ mov(r0(), r1());  // Move argument
  __ CallCFunction(r2(), 1);
  
  // Handle result based on result_size
  if (result_size == 2) {
    // Two return values
    __ mov(r1(), r0());
  }
  
  __ LeaveExitFrame();
  __ Ret();
}

void Builtins::Generate_JSEntry(MacroAssembler* masm) {
  // WASM32 stub implementation for JavaScript entry
  __ EnterFrame(StackFrame::ENTRY);
  
  // Set up the context from the function
  __ lw(kContextRegister, 
        FieldMemOperand(kJSFunctionRegister, JSFunction::kContextOffset));
  
  // Push the function and receiver
  __ Push(kJSFunctionRegister);
  
  // Call the function
  __ Call(BUILTIN_CODE(masm->isolate(), JSEntryTrampoline), 
          RelocInfo::CODE_TARGET);
  
  __ LeaveFrame(StackFrame::ENTRY);
  __ Ret();
}

void Builtins::Generate_JSConstructEntry(MacroAssembler* masm) {
  // WASM32 stub implementation for JavaScript constructor entry
  Generate_JSEntry(masm);
}

void Builtins::Generate_JSRunMicrotasksEntry(MacroAssembler* masm) {
  // WASM32 stub implementation for running microtasks
  Generate_JSEntry(masm);
}

// static
void Builtins::Generate_DeoptimizationEntry_Eager(MacroAssembler* masm) {
  // WASM32 stub implementation for eager deoptimization
  __ Call(BUILTIN_CODE(masm->isolate(), DeoptimizationEntry_Lazy),
          RelocInfo::CODE_TARGET);
}

// static
void Builtins::Generate_DeoptimizationEntry_Lazy(MacroAssembler* masm) {
  // WASM32 stub implementation for lazy deoptimization
  // In a real implementation, this would:
  // 1. Save all registers
  // 2. Call the deoptimizer
  // 3. Restore the frame
  
  __ mov(r0(), Operand(Deoptimizer::kLazy));
  __ CallRuntime(Runtime::kDeoptimize);
  
  // Unreachable
  __ brk(0);
}

void Builtins::Generate_InterpreterEntryTrampoline(MacroAssembler* masm) {
  // WASM32 stub implementation for interpreter entry
  // This is highly simplified - a real implementation would set up
  // the interpreter frame and dispatch to the first bytecode
  
  __ EnterFrame(StackFrame::INTERPRETED);
  
  // Get the bytecode array
  __ lw(r3(), FieldMemOperand(kJSFunctionRegister, 
                              JSFunction::kSharedFunctionInfoOffset));
  __ lw(r3(), FieldMemOperand(r3(), SharedFunctionInfo::kFunctionDataOffset));
  
  // Set up initial bytecode offset
  __ mov(r2(), Operand(0));
  
  // Dispatch to interpreter
  __ TailCallRuntime(Runtime::kInterpreterEntry);
}

void Builtins::Generate_InterpreterPushArgsThenCall(MacroAssembler* masm) {
  // WASM32 stub implementation for pushing arguments and calling
  // r0: argument count
  // r1: address of first argument 
  // r2: the target to call
  
  // Push arguments
  Label loop;
  __ bind(&loop);
  __ subi(r0(), r0(), 1);
  __ Branch(&loop, ge, r0(), Operand(zero_reg));
  
  // Call the target
  __ Jump(r2());
}

void Builtins::Generate_InterpreterPushArgsThenConstruct(MacroAssembler* masm) {
  // WASM32 stub implementation - similar to above but for construct
  Generate_InterpreterPushArgsThenCall(masm);
}

void Builtins::Generate_Call_ReceiverIsAny(MacroAssembler* masm) {
  // WASM32 stub implementation for generic call
  __ Jump(BUILTIN_CODE(masm->isolate(), Call), RelocInfo::CODE_TARGET);
}

void Builtins::Generate_Call_ReceiverIsNullOrUndefined(MacroAssembler* masm) {
  // WASM32 stub implementation
  Generate_Call_ReceiverIsAny(masm);
}

void Builtins::Generate_Call_ReceiverIsNotNullOrUndefined(
    MacroAssembler* masm) {
  // WASM32 stub implementation
  Generate_Call_ReceiverIsAny(masm);
}

void Builtins::Generate_CallOrConstruct(MacroAssembler* masm) {
  // WASM32 stub implementation for call or construct
  // r0: argument count
  // r1: the target to call
  // r3: the new target (for [[Construct]])
  
  __ Jump(masm->isolate()->builtins()->Call(), RelocInfo::CODE_TARGET);
}

void Builtins::Generate_CallBoundFunction(MacroAssembler* masm) {
  // WASM32 stub implementation for bound function calls
  // Load [[BoundArguments]] and push them
  __ lw(r2(), FieldMemOperand(kJSFunctionRegister, 
                              JSBoundFunction::kBoundArgumentsOffset));
  
  // Load [[BoundTargetFunction]]
  __ lw(kJSFunctionRegister,
        FieldMemOperand(kJSFunctionRegister, 
                        JSBoundFunction::kBoundTargetFunctionOffset));
  
  // Call the target
  __ Jump(BUILTIN_CODE(masm->isolate(), Call), RelocInfo::CODE_TARGET);
}

void Builtins::Generate_Call(MacroAssembler* masm) {
  // WASM32 stub implementation for generic call
  // r0: argument count
  // r1: the target to call
  
  Label non_function;
  __ JumpIfSmi(kJSFunctionRegister, &non_function);
  __ CompareObjectType(kJSFunctionRegister, r4(), r5(), JS_FUNCTION_TYPE);
  __ Branch(&non_function, ne);
  
  // Call JS function
  __ Jump(BUILTIN_CODE(masm->isolate(), CallFunction), RelocInfo::CODE_TARGET);
  
  // Non-function case
  __ bind(&non_function);
  __ Jump(BUILTIN_CODE(masm->isolate(), CallProxy), RelocInfo::CODE_TARGET);
}

void Builtins::Generate_ConstructFunction(MacroAssembler* masm) {
  // WASM32 stub implementation for constructing functions
  // r0: argument count
  // r1: constructor function
  // r3: new target
  
  // Create the new object
  __ Push(r1());
  __ Push(r3());
  __ CallRuntime(Runtime::kNewObject);
  __ Pop(r3());
  __ Pop(r1());
  
  // Call the constructor
  __ Jump(BUILTIN_CODE(masm->isolate(), CallFunction), RelocInfo::CODE_TARGET);
}

void Builtins::Generate_ConstructBoundFunction(MacroAssembler* masm) {
  // WASM32 stub implementation for constructing bound functions
  // Similar to CallBoundFunction but for construction
  Generate_ConstructFunction(masm);
}

void Builtins::Generate_Construct(MacroAssembler* masm) {
  // WASM32 stub implementation for generic construct
  // r0: argument count
  // r1: constructor function
  // r3: new target
  
  Label non_constructor;
  __ JumpIfSmi(r1(), &non_constructor);
  __ lw(r4(), FieldMemOperand(r1(), HeapObject::kMapOffset));
  __ lbu(r5(), FieldMemOperand(r4(), Map::kBitFieldOffset));
  __ And(r5(), r5(), Operand(Map::Bits1::IsConstructorBit::kMask));
  __ Branch(&non_constructor, eq, r5(), Operand(zero_reg));
  
  // Dispatch based on instance type
  __ Jump(BUILTIN_CODE(masm->isolate(), ConstructFunction), 
          RelocInfo::CODE_TARGET);
  
  // Non-constructor case
  __ bind(&non_constructor);
  __ CallRuntime(Runtime::kThrowNotConstructor);
}

void Builtins::Generate_ArgumentsAdaptorTrampoline(MacroAssembler* masm) {
  // WASM32 stub implementation for arguments adaptor
  // This adapts the number of arguments passed to a function
  // r0: actual number of arguments
  // r1: function
  // r2: expected number of arguments
  
  Label enough_arguments;
  __ Branch(&enough_arguments, ge, r0(), r2());
  
  // Not enough arguments - add undefined values
  Label fill_loop;
  __ bind(&fill_loop);
  __ Push(__ UndefinedConstant());
  __ addi(r0(), r0(), 1);
  __ Branch(&fill_loop, lt, r0(), r2());
  
  __ bind(&enough_arguments);
  // Call the function
  __ Jump(masm->isolate()->builtins()->Call(), RelocInfo::CODE_TARGET);
}

void Builtins::Generate_ReflectApply(MacroAssembler* masm) {
  // WASM32 stub implementation for Reflect.apply
  __ Jump(BUILTIN_CODE(masm->isolate(), CallFunction), RelocInfo::CODE_TARGET);
}

void Builtins::Generate_ReflectConstruct(MacroAssembler* masm) {
  // WASM32 stub implementation for Reflect.construct
  __ Jump(BUILTIN_CODE(masm->isolate(), Construct), RelocInfo::CODE_TARGET);
}

void Builtins::Generate_WasmCompileLazy(MacroAssembler* masm) {
  // WASM32 stub implementation for lazy WebAssembly compilation
  // Save all registers
  __ Push(r0());
  
  // Call the lazy compile runtime function
  __ CallRuntime(Runtime::kWasmCompileLazy);
  
  // Restore registers and jump to compiled code
  __ Pop(r0());
  __ Ret();
}

void Builtins::Generate_WasmLiftoffFrameSetup(MacroAssembler* masm) {
  // WASM32 stub implementation for Liftoff frame setup
  __ EnterFrame(StackFrame::WASM);
  __ mov(r0(), r1());
  __ Ret();
}

void Builtins::Generate_WasmDebugBreak(MacroAssembler* masm) {
  // WASM32 stub implementation for WebAssembly debug break
  __ Push(r0());
  __ CallRuntime(Runtime::kWasmDebugBreak);
  __ Pop(r0());
  __ Ret();
}

void Builtins::Generate_JSToWasmLazyDeoptContinuation(MacroAssembler* masm) {
  // WASM32 stub implementation for JS to WASM lazy deopt continuation
  __ CallRuntime(Runtime::kDeoptimize);
  __ brk(0);
}

#undef __

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32