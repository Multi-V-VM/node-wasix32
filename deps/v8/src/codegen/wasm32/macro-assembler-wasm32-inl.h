// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_MACRO_ASSEMBLER_WASM32_INL_H_
#define V8_CODEGEN_WASM32_MACRO_ASSEMBLER_WASM32_INL_H_

#include "src/codegen/macro-assembler.h"
#include "src/codegen/wasm32/macro-assembler-wasm32.h"
#include "src/execution/isolate-data.h"

namespace v8 {
namespace internal {

// Inline implementations for MacroAssembler

inline MemOperand FieldMemOperand(Register object, int offset) {
  return MemOperand(object, offset);
}

inline Operand::Operand(Register reg) : reg_(reg), type_(Type::kRegister) {}

inline Operand::Operand(int32_t immediate) 
    : immediate_(immediate), type_(Type::kImmediate) {}

inline Operand::Operand(const ExternalReference& ref)
    : immediate_(ref.address()), type_(Type::kImmediate) {}

inline Operand::Operand(Smi smi) 
    : immediate_(smi.ptr()), type_(Type::kImmediate) {}

inline Operand::Operand(Handle<HeapObject> handle)
    : immediate_(handle.address()), type_(Type::kImmediate) {}

inline bool Operand::IsRegister() const { return type_ == Type::kRegister; }
inline bool Operand::IsImmediate() const { return type_ == Type::kImmediate; }

inline Register Operand::reg() const {
  DCHECK(IsRegister());
  return reg_;
}

inline int32_t Operand::immediate() const {
  DCHECK(IsImmediate());
  return immediate_;
}

// MemOperand implementation
inline MemOperand::MemOperand(Register base, int32_t offset)
    : base_(base), offset_(offset) {}

inline MemOperand::MemOperand(Register base, Register index, int32_t offset)
    : base_(base), index_(index), offset_(offset) {}

// MacroAssembler inline functions
inline void MacroAssembler::li(Register dst, const Operand& imm) {
  if (imm.IsImmediate()) {
    assembler_.li(dst, imm.immediate());
  } else {
    assembler_.mov(dst, imm.reg());
  }
}

inline void MacroAssembler::li(Register dst, int32_t imm) {
  assembler_.li(dst, imm);
}

inline void MacroAssembler::li(Register dst, const ExternalReference& ref) {
  li(dst, static_cast<int32_t>(ref.address()));
}

inline void MacroAssembler::li(Register dst, Handle<HeapObject> handle) {
  li(dst, static_cast<int32_t>(handle.address()));
}

inline void MacroAssembler::li(Register dst, Smi smi) {
  li(dst, static_cast<int32_t>(smi.ptr()));
}

inline void MacroAssembler::mov(Register dst, Register src) {
  assembler_.mov(dst, src);
}

inline void MacroAssembler::lw(Register dst, const MemOperand& src) {
  assembler_.lw(dst, src.base(), src.offset());
}

inline void MacroAssembler::sw(Register src, const MemOperand& dst) {
  assembler_.sw(src, dst.base(), dst.offset());
}

inline void MacroAssembler::bind(Label* label) {
  assembler_.bind(label);
}

inline void MacroAssembler::PushArguments(Register start, Register count,
                                         Register scratch) {
  PushArguments(start, count);
}

inline void MacroAssembler::PushArguments(Register start, Register count) {
  Label loop, done;
  assembler_.beq(count, zero_reg, &done);
  bind(&loop);
  lw(kScratchRegister, MemOperand(start, 0));
  Push(kScratchRegister);
  assembler_.addi(start, start, kPointerSize);
  assembler_.subi(count, count, 1);
  assembler_.bne(count, zero_reg, &loop);
  bind(&done);
}

inline void MacroAssembler::PopArguments(Register count) {
  assembler_.sll(count, count, Operand(kPointerSizeLog2));
  assembler_.add(sp(), sp(), count);
}

inline void MacroAssembler::ComputeCodeStartAddress(Register dst) {
  assembler_.LoadPC(dst);
  // Adjust for current position in code
  assembler_.subi(dst, dst, pc_offset());
}

inline void MacroAssembler::LoadPC(Register dst) {
  Label current;
  assembler_.mov(dst, Operand(&current));
  bind(&current);
}

inline void MacroAssembler::CallCFunctionHelper(Register function,
                                               int num_reg_arguments) {
  // Set up C calling convention
  // WASM32 uses registers r0-r3 for arguments
  assembler_.jalr(link(), function);
}

inline void MacroAssembler::MovToFloatParameter(DoubleRegister src) {
  // Move double to f0 for C calling convention
  if (src != d0()) {
    assembler_.fmov_d(d0(), src);
  }
}

inline void MacroAssembler::MovToFloatParameters(DoubleRegister src1,
                                                DoubleRegister src2) {
  // Move doubles to f0 and f1 for C calling convention
  if (src2 != d1()) {
    assembler_.fmov_d(d1(), src2);
  }
  if (src1 != d0()) {
    assembler_.fmov_d(d0(), src1);
  }
}

inline void MacroAssembler::MovToFloatResult(DoubleRegister dst) {
  // Move result from f0 to dst
  if (dst != d0()) {
    assembler_.fmov_d(dst, d0());
  }
}

inline void MacroAssembler::PrepareCallCFunction(int num_reg_arguments,
                                                Register scratch) {
  // Align stack if necessary
  assembler_.andi(scratch, sp(), kStackAlignment - 1);
  Label aligned;
  assembler_.beq(scratch, zero_reg, &aligned);
  assembler_.subi(sp(), sp(), kStackAlignment);
  bind(&aligned);
}

inline void MacroAssembler::LoadNativeContextSlot(Register dst, int index) {
  LoadRoot(dst, RootIndex::kNativeContextTable);
  lw(dst, MemOperand(dst, index * kPointerSize));
}

inline void MacroAssembler::LoadGlobalProxy(Register dst) {
  LoadNativeContextSlot(dst, Context::GLOBAL_PROXY_INDEX);
}

inline void MacroAssembler::LoadFeedbackVector(Register dst, Register closure,
                                              Label* fbv_null) {
  lw(dst, FieldMemOperand(closure, JSFunction::kFeedbackCellOffset));
  lw(dst, FieldMemOperand(dst, FeedbackCell::kValueOffset));
  CompareRoot(dst, RootIndex::kUndefinedValue);
  Branch(fbv_null, eq);
}

inline void MacroAssembler::RecordWriteField(Register object, int offset,
                                            Register value,
                                            Register slot_address,
                                            LinkRegisterStatus lr_status,
                                            SaveFPRegsMode save_fp,
                                            SmiCheck smi_check) {
  // WASM32 simplified write barrier
  // In a real implementation, this would:
  // 1. Check if write barrier is needed
  // 2. Call the write barrier runtime if necessary
  
  if (smi_check == SmiCheck::kInline) {
    JumpIfSmi(value, nullptr);
  }
  
  // Store the value
  sw(value, FieldMemOperand(object, offset));
}

inline void MacroAssembler::StubPrologue(StackFrame::Type type) {
  EnterFrame(type);
}

inline void MacroAssembler::StubEpilogue() {
  LeaveFrame(StackFrame::STUB);
}

inline void MacroAssembler::PushStandardFrame(Register function) {
  Push(fp());
  Push(kContextRegister);
  Push(function);
  Push(kJavaScriptCallArgCountRegister);
  assembler_.addi(fp(), sp(), StandardFrameConstants::kFixedFrameSizeFromFp);
}

inline void MacroAssembler::RestoreFrameStateForTailCall() {
  lw(fp(), MemOperand(fp(), StandardFrameConstants::kCallerFPOffset));
}

inline void MacroAssembler::LoadCodeInstructionStart(Register dst,
                                                   Register code_object,
                                                   CodeEntrypointTag tag) {
  lw(dst, FieldMemOperand(code_object, Code::kInstructionStartOffset));
}

inline void MacroAssembler::CallCodeObject(Register code_object) {
  LoadCodeInstructionStart(kScratchRegister, code_object);
  Call(kScratchRegister);
}

inline void MacroAssembler::JumpCodeObject(Register code_object,
                                          JumpMode jump_mode) {
  LoadCodeInstructionStart(kScratchRegister, code_object);
  Jump(kScratchRegister);
}

inline void MacroAssembler::LoadBuiltinByIndex(Register builtin_index) {
  LoadRoot(kScratchRegister, RootIndex::kBuiltinsTable);
  assembler_.sll(builtin_index, builtin_index, Operand(kPointerSizeLog2));
  assembler_.add(kScratchRegister, kScratchRegister, builtin_index);
  lw(builtin_index, MemOperand(kScratchRegister, 0));
}

inline void MacroAssembler::CallBuiltinByIndex(Register builtin_index) {
  LoadBuiltinByIndex(builtin_index);
  CallCodeObject(builtin_index);
}

inline void MacroAssembler::LoadSimd128(Simd128Register dst,
                                       const MemOperand& src) {
  assembler_.vld(dst, src.base(), src.offset());
}

inline void MacroAssembler::StoreSimd128(Simd128Register src,
                                        const MemOperand& dst) {
  assembler_.vst(src, dst.base(), dst.offset());
}

inline Condition NegateCondition(Condition cond) {
  switch (cond) {
    case eq:
      return ne;
    case ne:
      return eq;
    case lt:
      return ge;
    case ge:
      return lt;
    case lo:
      return hs;
    case hs:
      return lo;
    case al:
      return nv;
    case nv:
      return al;
    default:
      return nv;
  }
}

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_MACRO_ASSEMBLER_WASM32_INL_H_