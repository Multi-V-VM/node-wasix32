// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_MACRO_ASSEMBLER_WASM32_H_
#define V8_CODEGEN_WASM32_MACRO_ASSEMBLER_WASM32_H_

#include "src/codegen/macro-assembler-base.h"
#include "src/codegen/wasm32/assembler-wasm32.h"
#include "src/codegen/wasm32/register-wasm32.h"
#include "src/common/globals.h"
#include "src/objects/tagged.h"

namespace v8 {
namespace internal {

// Forward declarations
class SafepointTableBuilder;

// WASM32 MacroAssembler
// Provides high-level assembly operations
class V8_EXPORT_PRIVATE MacroAssembler : public MacroAssemblerBase {
 public:
  MacroAssembler(Isolate* isolate, CodeObjectRequired create_code_object,
                 const AssemblerOptions& options,
                 std::unique_ptr<AssemblerBuffer> buffer = {})
      : MacroAssemblerBase(isolate, options, std::move(buffer)),
        assembler_(isolate, options, std::move(buffer)) {
    if (create_code_object == CodeObjectRequired::kYes) {
      code_object_ = Handle<HeapObject>::New(
          ReadOnlyRoots(isolate).undefined_value(), isolate);
    }
  }

  AssemblerWasm32* GetAssembler() { return &assembler_; }
  const AssemblerWasm32* GetAssembler() const { return &assembler_; }

  // Jump, Call, and Ret operations
  void Jump(Register target);
  void Jump(Address target, RelocInfo::Mode rmode);
  void Jump(Handle<Code> code, RelocInfo::Mode rmode);
  void Jump(const ExternalReference& reference);
  
  void Call(Register target);
  void Call(Address target, RelocInfo::Mode rmode);
  void Call(Handle<Code> code, RelocInfo::Mode rmode);
  void Call(const ExternalReference& reference);
  
  void Ret();
  void Ret(int bytes_to_pop);

  // Push/Pop operations
  void Push(Register src);
  void Push(const Operand& src);
  void Push(Immediate value);
  void Push(Handle<HeapObject> object);
  void Push(Smi smi);

  void Pop(Register dst);
  void Pop(const Operand& dst);

  void PushMultiple(RegList registers);
  void PopMultiple(RegList registers);

  void Drop(int count);
  void DropArguments(int count, ArgumentsCountType type = kCountIsInteger);

  // Frame operations
  void EnterFrame(StackFrame::Type type);
  void LeaveFrame(StackFrame::Type type);
  void EnterExitFrame(bool save_doubles, int stack_space = 0);
  void LeaveExitFrame(bool restore_doubles = true);

  // Load/Store operations
  void LoadRoot(Register destination, RootIndex index) override;
  void PushRoot(RootIndex index);

  // Smi operations
  void SmiTag(Register dst, Register src);
  void SmiUntag(Register dst, Register src);

  // Comparisons
  void CompareTagged(Register lhs, Register rhs);
  void CompareTagged(Register lhs, const Operand& rhs);
  void CompareRoot(Register obj, RootIndex index);

  // Conditional branching
  void Branch(Label* label, Condition cond = al,
              Register rs = zero_reg, const Operand& rt = Operand(zero_reg));
  
  // Type checks
  void JumpIfSmi(Register value, Label* smi_label);
  void JumpIfNotSmi(Register value, Label* not_smi_label);
  
  // Runtime calls
  void CallRuntime(const Runtime::Function* f, int num_arguments);
  void CallRuntime(Runtime::FunctionId fid, int num_arguments) {
    CallRuntime(Runtime::FunctionForId(fid), num_arguments);
  }
  void TailCallRuntime(Runtime::FunctionId fid);

  void CallExternalReference(const ExternalReference& ref, int num_arguments);
  
  // C function calls
  void CallCFunction(ExternalReference ref, int num_arguments);
  void CallCFunction(Register function, int num_arguments);

  // Debugging
  void Abort(AbortReason reason);
  void Check(Condition cond, AbortReason reason);

  // Prologue/Epilogue helpers
  void Prologue();
  void EnterStubFrame();
  void LeaveStubFrame();

  // Stack limit check
  void StackOverflowCheck(Register num_args, Register scratch,
                         Label* stack_overflow);

  // Operand helpers
  Operand StackLimitAsOperand(StackLimitKind kind);
  Operand ExternalReferenceAsOperand(ExternalReference reference,
                                    Register scratch);
  Operand ExternalOperand(ExternalReference reference, Register scratch) {
    return ExternalReferenceAsOperand(reference, scratch);
  }

  // Memory barriers
  void MemoryBarrier();

  // Move operations
  void Move(Register dst, const Immediate& imm);
  void Move(Register dst, Smi smi);
  void Move(Register dst, Handle<HeapObject> object);
  void Move(Register dst, const ExternalReference& reference);
  void Move(Register dst, Register src, Condition cond = al);
  void Move(DoubleRegister dst, DoubleRegister src);

  // Load/Store operations
  void LoadFromConstantsTable(Register dst, int index);
  void LoadRootRegisterOffset(Register dst, intptr_t offset);
  void LoadRootRelative(Register dst, int32_t offset);

  void LoadAddress(Register dst, const ExternalReference& ext);
  void LoadExternalPointerField(Register dst, const Operand& src,
                               ExternalPointerTag tag, Register scratch);

  // Stack frame operations  
  void PushStandardFrame(Register function);
  void RestoreFrameStateForTailCall();

  // Arguments adaptation
  void PushArguments(Register start, Register count, Register scratch);
  void PushArguments(Register start, Register count);
  void PopArguments(Register count);

  // Write barrier support
  void RecordWriteField(Register object, int offset, Register value,
                       Register slot_address, LinkRegisterStatus lr_status,
                       SaveFPRegsMode save_fp,
                       SmiCheck smi_check = SmiCheck::kInline);

  // Inlined Smi check
  void AssertSmi(Register object);
  void AssertNotSmi(Register object);

  // Builtin calls
  void CallBuiltin(Builtin builtin);
  void TailCallBuiltin(Builtin builtin);
  void CallBuiltinByIndex(Register builtin_index);
  void LoadBuiltinByIndex(Register builtin_index);

  // Code object support
  void CallCodeObject(Register code_object);
  void JumpCodeObject(Register code_object,
                     JumpMode jump_mode = JumpMode::kJump);
  void LoadCodeInstructionStart(Register dst, Register code_object,
                               CodeEntrypointTag tag = kDefaultCodeEntrypointTag);

  // Stack manipulation for calls
  void PrepareCallCFunction(int num_reg_arguments, Register scratch);
  void MovToFloatParameter(DoubleRegister src);
  void MovToFloatParameters(DoubleRegister src1, DoubleRegister src2);
  void MovToFloatResult(DoubleRegister src);

  // Emit the constant pool
  void EmitConstantPool() { /* WASM32 doesn't use constant pools */ }

  // Misc operations
  void ComputeCodeStartAddress(Register dst);
  void LoadPC(Register dst);
  void StubPrologue(StackFrame::Type type);
  void StubEpilogue();

  // Required by the macro-assembler interface
  void LoadMap(Register dst, Register object);
  void LoadCompressedMap(Register dst, Register object);
  void LoadFeedbackVector(Register dst, Register closure, Label* fbv_null);
  void LoadGlobalProxy(Register dst);
  void LoadNativeContextSlot(Register dst, int index);

  // SIMD operations
  void LoadSimd128(Simd128Register dst, const MemOperand& src);
  void StoreSimd128(Simd128Register src, const MemOperand& dst);

  // Define ACCESS_MASM macro
  #define ACCESS_MASM(masm) masm->

  static constexpr Register kRootRegister = r13();
  static constexpr Register kScratchRegister = r12();

  // Architecture-specific constants
  static constexpr int kSmiShift = kSmiTagSize + kSmiShiftSize;
  static constexpr uint32_t kSmiShiftMask = (1 << kSmiShift) - 1;

  // Minimum size of the stack
  static constexpr int kStackMinSize = 4 * KB;

 private:
  AssemblerWasm32 assembler_;

  void Call(intptr_t target, RelocInfo::Mode rmode);
  void Jump(intptr_t target, RelocInfo::Mode rmode);

  void CallCFunctionHelper(Register function, int num_reg_arguments);

  void GenerateTailCallToReturnedCode(Runtime::FunctionId function_id);
};

// Convenience aliases
using TurboAssembler = MacroAssembler;

}  // namespace internal
}  // namespace v8

#define ACCESS_MASM(masm) masm->

#endif  // V8_CODEGEN_WASM32_MACRO_ASSEMBLER_WASM32_H_