// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <limits>

#if V8_TARGET_ARCH_WASM32

#include "src/codegen/wasm32/macro-assembler-wasm32.h"

#include "src/builtins/builtins.h"
#include "src/codegen/assembler-inl.h"
#include "src/codegen/callable.h"
#include "src/codegen/code-factory.h"
#include "src/codegen/external-reference-table.h"
#include "src/codegen/macro-assembler-inl.h"
#include "src/codegen/register-configuration.h"
#include "src/codegen/wasm32/assembler-wasm32-inl.h"
#include "src/debug/debug.h"
#include "src/execution/frames-inl.h"
#include "src/heap/mutable-page.h"
#include "src/init/bootstrapper.h"
#include "src/logging/counters.h"
#include "src/objects/heap-number.h"
#include "src/runtime/runtime.h"
#include "src/snapshot/snapshot.h"

namespace v8 {
namespace internal {

// Jump operations
void MacroAssembler::Jump(Register target) {
  assembler_.jr(target);
}

void MacroAssembler::Jump(Address target, RelocInfo::Mode rmode) {
  Jump(static_cast<intptr_t>(target), rmode);
}

void MacroAssembler::Jump(Handle<Code> code, RelocInfo::Mode rmode) {
  DCHECK(RelocInfo::IsCodeTarget(rmode));
  Builtin builtin = Builtin::kNoBuiltinId;
  if (isolate()->builtins()->IsBuiltinHandle(code, &builtin)) {
    CallBuiltin(builtin);
  } else {
    Jump(code.address(), rmode);
  }
}

void MacroAssembler::Jump(const ExternalReference& reference) {
  li(kScratchRegister, Operand(reference));
  Jump(kScratchRegister);
}

void MacroAssembler::Jump(intptr_t target, RelocInfo::Mode rmode) {
  RecordRelocInfo(rmode, target);
  li(kScratchRegister, Operand(target, rmode));
  Jump(kScratchRegister);
}

// Call operations
void MacroAssembler::Call(Register target) {
  assembler_.jalr(link(), target);
}

void MacroAssembler::Call(Address target, RelocInfo::Mode rmode) {
  Call(static_cast<intptr_t>(target), rmode);
}

void MacroAssembler::Call(Handle<Code> code, RelocInfo::Mode rmode) {
  DCHECK(RelocInfo::IsCodeTarget(rmode));
  Builtin builtin = Builtin::kNoBuiltinId;
  if (isolate()->builtins()->IsBuiltinHandle(code, &builtin)) {
    CallBuiltin(builtin);
  } else {
    Call(code.address(), rmode);
  }
}

void MacroAssembler::Call(const ExternalReference& reference) {
  li(kScratchRegister, Operand(reference));
  Call(kScratchRegister);
}

void MacroAssembler::Call(intptr_t target, RelocInfo::Mode rmode) {
  RecordRelocInfo(rmode, target);
  li(kScratchRegister, Operand(target, rmode));
  Call(kScratchRegister);
}

void MacroAssembler::Ret() {
  assembler_.jr(link());
}

void MacroAssembler::Ret(int bytes_to_pop) {
  if (bytes_to_pop > 0) {
    assembler_.addi(sp(), sp(), bytes_to_pop);
  }
  Ret();
}

// Push/Pop operations
void MacroAssembler::Push(Register src) {
  assembler_.push(src);
}

void MacroAssembler::Push(const Operand& src) {
  if (src.IsImmediate()) {
    li(kScratchRegister, src);
    Push(kScratchRegister);
  } else {
    lw(kScratchRegister, src);
    Push(kScratchRegister);
  }
}

void MacroAssembler::Push(Immediate value) {
  li(kScratchRegister, value);
  Push(kScratchRegister);
}

void MacroAssembler::Push(Handle<HeapObject> object) {
  li(kScratchRegister, object);
  Push(kScratchRegister);
}

void MacroAssembler::Push(Smi smi) {
  li(kScratchRegister, smi);
  Push(kScratchRegister);
}

void MacroAssembler::Pop(Register dst) {
  assembler_.pop(dst);
}

void MacroAssembler::Pop(const Operand& dst) {
  Pop(kScratchRegister);
  sw(kScratchRegister, dst);
}

void MacroAssembler::PushMultiple(RegList registers) {
  assembler_.pushm(registers);
}

void MacroAssembler::PopMultiple(RegList registers) {
  assembler_.popm(registers);
}

void MacroAssembler::Drop(int count) {
  if (count > 0) {
    assembler_.addi(sp(), sp(), count * kPointerSize);
  }
}

void MacroAssembler::DropArguments(int count, ArgumentsCountType type) {
  switch (type) {
    case kCountIsInteger:
      Drop(count);
      break;
    case kCountIsBytes:
      if (count > 0) {
        assembler_.addi(sp(), sp(), count);
      }
      break;
    case kCountIncludesReceiver:
      Drop(count - 1);
      break;
  }
}

// Frame operations
void MacroAssembler::EnterFrame(StackFrame::Type type) {
  Push(fp());
  mov(fp(), sp());
  Push(Immediate(StackFrame::TypeToMarker(type)));
  Push(kContextRegister);
  Push(kJSFunctionRegister);
}

void MacroAssembler::LeaveFrame(StackFrame::Type type) {
  mov(sp(), fp());
  Pop(fp());
}

void MacroAssembler::EnterExitFrame(bool save_doubles, int stack_space) {
  // Set up the exit frame
  EnterFrame(StackFrame::EXIT);
  
  // Reserve space for C function arguments
  if (stack_space > 0) {
    assembler_.subi(sp(), sp(), stack_space * kPointerSize);
  }
  
  // Save callee-saved registers if needed
  if (save_doubles) {
    // Save floating point registers
    // WASM32 stub - would save FP registers here
  }
}

void MacroAssembler::LeaveExitFrame(bool restore_doubles) {
  if (restore_doubles) {
    // Restore floating point registers
    // WASM32 stub - would restore FP registers here
  }
  
  LeaveFrame(StackFrame::EXIT);
}

// Load/Store operations
void MacroAssembler::LoadRoot(Register destination, RootIndex index) {
  lw(destination, MemOperand(kRootRegister, RootRegisterOffsetForRootIndex(index)));
}

void MacroAssembler::PushRoot(RootIndex index) {
  LoadRoot(kScratchRegister, index);
  Push(kScratchRegister);
}

// Smi operations
void MacroAssembler::SmiTag(Register dst, Register src) {
  static_assert(kSmiTag == 0);
  if (dst != src) {
    mov(dst, src);
  }
  assembler_.sll(dst, dst, Operand(kSmiTagSize));
}

void MacroAssembler::SmiUntag(Register dst, Register src) {
  static_assert(kSmiTag == 0);
  if (dst != src) {
    mov(dst, src);
  }
  assembler_.sra(dst, dst, Operand(kSmiTagSize));
}

// Comparisons
void MacroAssembler::CompareTagged(Register lhs, Register rhs) {
  assembler_.sub(zero_reg, lhs, rhs);
}

void MacroAssembler::CompareTagged(Register lhs, const Operand& rhs) {
  if (rhs.IsImmediate()) {
    li(kScratchRegister, rhs);
    CompareTagged(lhs, kScratchRegister);
  } else {
    lw(kScratchRegister, rhs);
    CompareTagged(lhs, kScratchRegister);
  }
}

void MacroAssembler::CompareRoot(Register obj, RootIndex index) {
  LoadRoot(kScratchRegister, index);
  CompareTagged(obj, kScratchRegister);
}

// Conditional branching
void MacroAssembler::Branch(Label* label, Condition cond,
                           Register rs, const Operand& rt) {
  if (rt.IsImmediate()) {
    li(kScratchRegister, rt);
    assembler_.beq(rs, kScratchRegister, label);
  } else if (rt.IsRegister()) {
    switch (cond) {
      case eq:
        assembler_.beq(rs, rt.reg(), label);
        break;
      case ne:
        assembler_.bne(rs, rt.reg(), label);
        break;
      case lt:
        assembler_.blt(rs, rt.reg(), label);
        break;
      case ge:
        assembler_.bge(rs, rt.reg(), label);
        break;
      case lo:
        assembler_.bltu(rs, rt.reg(), label);
        break;
      case hs:
        assembler_.bgeu(rs, rt.reg(), label);
        break;
      case al:
        assembler_.j(label);
        break;
      default:
        UNREACHABLE();
    }
  }
}

// Type checks
void MacroAssembler::JumpIfSmi(Register value, Label* smi_label) {
  STATIC_ASSERT(kSmiTag == 0);
  assembler_.andi(kScratchRegister, value, kSmiTagMask);
  Branch(smi_label, eq, kScratchRegister, Operand(zero_reg));
}

void MacroAssembler::JumpIfNotSmi(Register value, Label* not_smi_label) {
  STATIC_ASSERT(kSmiTag == 0);
  assembler_.andi(kScratchRegister, value, kSmiTagMask);
  Branch(not_smi_label, ne, kScratchRegister, Operand(zero_reg));
}

// Runtime calls
void MacroAssembler::CallRuntime(const Runtime::Function* f, int num_arguments) {
  // All arguments must be on the stack
  li(r0(), ExternalReference::Create(f));
  li(r1(), num_arguments);
  CallBuiltin(Builtin::kCEntry_Return1_DontSaveFPRegs_ArgvOnStack_NoBuiltinExit);
}

void MacroAssembler::TailCallRuntime(Runtime::FunctionId fid) {
  const Runtime::Function* function = Runtime::FunctionForId(fid);
  DCHECK_EQ(1, function->result_size());
  li(r1(), function->nargs);
  li(r0(), ExternalReference::Create(fid));
  TailCallBuiltin(Builtin::kCEntry_Return1_DontSaveFPRegs_ArgvOnStack_NoBuiltinExit);
}

void MacroAssembler::CallExternalReference(const ExternalReference& ref,
                                          int num_arguments) {
  li(r0(), ref);
  li(r1(), num_arguments);
  CallBuiltin(Builtin::kCEntry_Return1_DontSaveFPRegs_ArgvOnStack_NoBuiltinExit);
}

// C function calls
void MacroAssembler::CallCFunction(ExternalReference ref, int num_arguments) {
  li(kScratchRegister, ref);
  CallCFunction(kScratchRegister, num_arguments);
}

void MacroAssembler::CallCFunction(Register function, int num_arguments) {
  DCHECK_LE(num_arguments, 4);  // WASM32 has 4 argument registers
  
  // Call the C function
  assembler_.jalr(link(), function);
}

// Debugging
void MacroAssembler::Abort(AbortReason reason) {
  li(r0(), static_cast<int>(reason));
  Push(r0());
  CallRuntime(Runtime::kAbort);
  // Unreachable
  assembler_.brk(0);
}

void MacroAssembler::Check(Condition cond, AbortReason reason) {
  Label ok;
  Branch(&ok, NegateCondition(cond));
  Abort(reason);
  bind(&ok);
}

// Prologue/Epilogue helpers
void MacroAssembler::Prologue() {
  Push(fp());
  mov(fp(), sp());
  Push(kContextRegister);
  Push(kJSFunctionRegister);
}

void MacroAssembler::EnterStubFrame() {
  EnterFrame(StackFrame::STUB);
}

void MacroAssembler::LeaveStubFrame() {
  LeaveFrame(StackFrame::STUB);
}

// Stack limit check
void MacroAssembler::StackOverflowCheck(Register num_args, Register scratch,
                                        Label* stack_overflow) {
  LoadStackLimit(scratch, StackLimitKind::kRealStackLimit);
  // Calculate required stack size
  assembler_.sll(kScratchRegister, num_args, Operand(kPointerSizeLog2));
  assembler_.sub(kScratchRegister, sp(), kScratchRegister);
  // Check if we have enough stack space
  Branch(stack_overflow, lo, kScratchRegister, Operand(scratch));
}

// Operand helpers
Operand MacroAssembler::StackLimitAsOperand(StackLimitKind kind) {
  DCHECK(root_array_available());
  intptr_t offset = kind == StackLimitKind::kRealStackLimit
                        ? IsolateData::real_jslimit_offset()
                        : IsolateData::jslimit_offset();
  return MemOperand(kRootRegister, offset);
}

Operand MacroAssembler::ExternalReferenceAsOperand(ExternalReference reference,
                                                  Register scratch) {
  li(scratch, reference);
  return MemOperand(scratch, 0);
}

// Memory barriers
void MacroAssembler::MemoryBarrier() {
  assembler_.fence();
}

// Move operations
void MacroAssembler::Move(Register dst, const Immediate& imm) {
  li(dst, imm);
}

void MacroAssembler::Move(Register dst, Smi smi) {
  li(dst, smi);
}

void MacroAssembler::Move(Register dst, Handle<HeapObject> object) {
  li(dst, object);
}

void MacroAssembler::Move(Register dst, const ExternalReference& reference) {
  li(dst, reference);
}

void MacroAssembler::Move(Register dst, Register src, Condition cond) {
  if (cond == al) {
    mov(dst, src);
  } else {
    // Conditional move not directly supported in WASM32
    Label done;
    Branch(&done, NegateCondition(cond));
    mov(dst, src);
    bind(&done);
  }
}

void MacroAssembler::Move(DoubleRegister dst, DoubleRegister src) {
  if (dst != src) {
    assembler_.fmov_d(dst, src);
  }
}

// Additional required implementations...
void MacroAssembler::LoadFromConstantsTable(Register dst, int index) {
  DCHECK(root_array_available());
  LoadRoot(dst, RootIndex::kBuiltinsConstantsTable);
  lw(dst, FieldMemOperand(dst, FixedArray::OffsetOfElementAt(index)));
}

void MacroAssembler::LoadRootRegisterOffset(Register dst, intptr_t offset) {
  lw(dst, MemOperand(kRootRegister, offset));
}

void MacroAssembler::LoadRootRelative(Register dst, int32_t offset) {
  lw(dst, MemOperand(kRootRegister, offset));
}

void MacroAssembler::LoadAddress(Register dst, const ExternalReference& ext) {
  li(dst, ext);
}

void MacroAssembler::LoadExternalPointerField(Register dst, const Operand& src,
                                             ExternalPointerTag tag,
                                             Register scratch) {
  lw(dst, src);
  // WASM32 doesn't use pointer compression/sandboxing
}

void MacroAssembler::CallBuiltin(Builtin builtin) {
  CommentForOffHeapTrampoline("call", builtin);
  li(kScratchRegister, BuiltinEntry(builtin));
  Call(kScratchRegister);
}

void MacroAssembler::TailCallBuiltin(Builtin builtin) {
  CommentForOffHeapTrampoline("tail call", builtin);
  li(kScratchRegister, BuiltinEntry(builtin));
  Jump(kScratchRegister);
}

void MacroAssembler::LoadMap(Register dst, Register object) {
  lw(dst, FieldMemOperand(object, HeapObject::kMapOffset));
}

void MacroAssembler::LoadCompressedMap(Register dst, Register object) {
  LoadMap(dst, object);  // No compression in WASM32
}

void MacroAssembler::AssertSmi(Register object) {
  if (!v8_flags.debug_code) return;
  Label ok;
  JumpIfSmi(object, &ok);
  Abort(AbortReason::kOperandIsNotASmi);
  bind(&ok);
}

void MacroAssembler::AssertNotSmi(Register object) {
  if (!v8_flags.debug_code) return;
  Label ok;
  JumpIfNotSmi(object, &ok);
  Abort(AbortReason::kOperandIsASmi);
  bind(&ok);
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32