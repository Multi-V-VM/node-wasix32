// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// WASM32 stub implementation for Liftoff assembler.
// WASM32 is a JIT-less target - all operations bail out to TurboFan or
// the interpreter.

#ifndef V8_WASM_BASELINE_WASM32_LIFTOFF_ASSEMBLER_WASM32_INL_H_
#define V8_WASM_BASELINE_WASM32_LIFTOFF_ASSEMBLER_WASM32_INL_H_

#include "src/wasm/baseline/liftoff-assembler.h"
#include "src/codegen/wasm32/assembler-wasm32-inl.h"

namespace v8::internal::wasm {

namespace liftoff {

#if defined(V8_TARGET_BIG_ENDIAN)
constexpr int32_t kLowWordOffset = 4;
constexpr int32_t kHighWordOffset = 0;
#else
constexpr int32_t kLowWordOffset = 0;
constexpr int32_t kHighWordOffset = 4;
#endif

inline MemOperand GetStackSlot(int offset) {
  return MemOperand(kFramePointerRegister, -offset);
}

inline MemOperand GetInstanceDataOperand() {
  return GetStackSlot(WasmLiftoffFrameConstants::kInstanceDataOffset);
}

}  // namespace liftoff

// WASM32 does not support Liftoff compilation. All operations bail out.
#define BAILOUT(reason) bailout(kUnsupportedArchitecture, reason)
#define BAILOUT_RETURN(reason, ret) \
  do {                              \
    BAILOUT(reason);                \
    return ret;                     \
  } while (false)

int LiftoffAssembler::PrepareStackFrame() {
  BAILOUT("PrepareStackFrame");
  return 0;
}

void LiftoffAssembler::CallFrameSetupStub(int declared_function_index) {
  BAILOUT("CallFrameSetupStub");
}

void LiftoffAssembler::PrepareTailCall(int num_callee_stack_params,
                                        int stack_param_delta) {
  BAILOUT("PrepareTailCall");
}

void LiftoffAssembler::AlignFrameSize() {}

void LiftoffAssembler::PatchPrepareStackFrame(
    int offset, SafepointTableBuilder* safepoint_table_builder,
    bool feedback_vector_slot, size_t stack_param_slots) {
  BAILOUT("PatchPrepareStackFrame");
}

void LiftoffAssembler::FinishCode() {}

void LiftoffAssembler::AbortCompilation() {}

// static
constexpr int LiftoffAssembler::StaticStackFrameSize() { return 0; }

int LiftoffAssembler::SlotSizeForType(ValueKind kind) {
  switch (kind) {
    case kS128:
      return 16;
    case kF64:
    case kI64:
      return 8;
    default:
      return 4;
  }
}

bool LiftoffAssembler::NeedsAlignment(ValueKind kind) {
  return kind == kS128;
}

void LiftoffAssembler::CheckTierUp(int declared_func_index, int budget_used,
                                    Label* ool_label,
                                    const FreezeCacheState& frozen) {
  BAILOUT("CheckTierUp");
}

Register LiftoffAssembler::LoadOldFramePointer() {
  BAILOUT("LoadOldFramePointer");
  return no_reg;
}

void LiftoffAssembler::CheckStackShrink() {
  BAILOUT("CheckStackShrink");
}

void LiftoffAssembler::LoadConstant(LiftoffRegister reg, WasmValue value) {
  BAILOUT("LoadConstant");
}

void LiftoffAssembler::LoadInstanceDataFromFrame(Register dst) {
  BAILOUT("LoadInstanceDataFromFrame");
}

void LiftoffAssembler::LoadTrustedPointer(Register dst, Register base,
                                           int offset,
                                           IndirectPointerTag tag) {
  BAILOUT("LoadTrustedPointer");
}

void LiftoffAssembler::LoadFromInstance(Register dst, Register instance,
                                          int offset, int size) {
  BAILOUT("LoadFromInstance");
}

void LiftoffAssembler::LoadTaggedPointerFromInstance(Register dst,
                                                      Register instance,
                                                      int offset) {
  BAILOUT("LoadTaggedPointerFromInstance");
}

void LiftoffAssembler::SpillInstanceData(Register instance) {
  BAILOUT("SpillInstanceData");
}

void LiftoffAssembler::ResetOSRTarget() {}

void LiftoffAssembler::LoadTaggedPointer(Register dst, Register src_addr,
                                          Register offset_reg,
                                          int32_t offset_imm,
                                          uint32_t* protected_load_pc,
                                          bool needs_shift) {
  BAILOUT("LoadTaggedPointer");
}

void LiftoffAssembler::LoadProtectedPointer(Register dst, Register src_addr,
                                             int32_t offset_imm) {
  BAILOUT("LoadProtectedPointer");
}

void LiftoffAssembler::LoadFullPointer(Register dst, Register src_addr,
                                        int32_t offset_imm) {
  BAILOUT("LoadFullPointer");
}

void LiftoffAssembler::StoreTaggedPointer(Register dst_addr,
                                           Register offset_reg,
                                           int32_t offset_imm,
                                           Register src,
                                           LiftoffRegList pinned,
                                           uint32_t* protected_store_pc,
                                           SkipWriteBarrier skip_write_barrier) {
  BAILOUT("StoreTaggedPointer");
}

void LiftoffAssembler::Load(LiftoffRegister dst, Register src_addr,
                             Register offset_reg, uintptr_t offset_imm,
                             LoadType type, uint32_t* protected_load_pc,
                             bool is_load_mem, bool i64_offset,
                             bool needs_shift) {
  BAILOUT("Load");
}

void LiftoffAssembler::Store(Register dst_addr, Register offset_reg,
                              uintptr_t offset_imm, LiftoffRegister src,
                              StoreType type, LiftoffRegList pinned,
                              uint32_t* protected_store_pc, bool is_store_mem,
                              bool i64_offset) {
  BAILOUT("Store");
}

void LiftoffAssembler::AtomicLoad(LiftoffRegister dst, Register src_addr,
                                   Register offset_reg, uintptr_t offset_imm,
                                   LoadType type, LiftoffRegList pinned,
                                   bool i64_offset) {
  BAILOUT("AtomicLoad");
}

void LiftoffAssembler::AtomicStore(Register dst_addr, Register offset_reg,
                                    uintptr_t offset_imm, LiftoffRegister src,
                                    StoreType type, LiftoffRegList pinned,
                                    bool i64_offset) {
  BAILOUT("AtomicStore");
}

void LiftoffAssembler::AtomicAdd(Register dst_addr, Register offset_reg,
                                  uintptr_t offset_imm, LiftoffRegister value,
                                  LiftoffRegister result, StoreType type,
                                  bool i64_offset) {
  BAILOUT("AtomicAdd");
}

void LiftoffAssembler::AtomicSub(Register dst_addr, Register offset_reg,
                                  uintptr_t offset_imm, LiftoffRegister value,
                                  LiftoffRegister result, StoreType type,
                                  bool i64_offset) {
  BAILOUT("AtomicSub");
}

void LiftoffAssembler::AtomicAnd(Register dst_addr, Register offset_reg,
                                  uintptr_t offset_imm, LiftoffRegister value,
                                  LiftoffRegister result, StoreType type,
                                  bool i64_offset) {
  BAILOUT("AtomicAnd");
}

void LiftoffAssembler::AtomicOr(Register dst_addr, Register offset_reg,
                                 uintptr_t offset_imm, LiftoffRegister value,
                                 LiftoffRegister result, StoreType type,
                                 bool i64_offset) {
  BAILOUT("AtomicOr");
}

void LiftoffAssembler::AtomicXor(Register dst_addr, Register offset_reg,
                                  uintptr_t offset_imm, LiftoffRegister value,
                                  LiftoffRegister result, StoreType type,
                                  bool i64_offset) {
  BAILOUT("AtomicXor");
}

void LiftoffAssembler::AtomicExchange(Register dst_addr, Register offset_reg,
                                       uintptr_t offset_imm,
                                       LiftoffRegister value,
                                       LiftoffRegister result, StoreType type,
                                       bool i64_offset) {
  BAILOUT("AtomicExchange");
}

void LiftoffAssembler::AtomicCompareExchange(
    Register dst_addr, Register offset_reg, uintptr_t offset_imm,
    LiftoffRegister expected, LiftoffRegister new_value, LiftoffRegister result,
    StoreType type, bool i64_offset) {
  BAILOUT("AtomicCompareExchange");
}

void LiftoffAssembler::AtomicFence() { BAILOUT("AtomicFence"); }

void LiftoffAssembler::LoadCallerFrameSlot(LiftoffRegister dst,
                                            uint32_t caller_slot_idx,
                                            ValueKind kind) {
  BAILOUT("LoadCallerFrameSlot");
}

void LiftoffAssembler::StoreCallerFrameSlot(LiftoffRegister src,
                                             uint32_t caller_slot_idx,
                                             ValueKind kind,
                                             Register frame_pointer) {
  BAILOUT("StoreCallerFrameSlot");
}

void LiftoffAssembler::LoadReturnStackSlot(LiftoffRegister dst, int offset,
                                            ValueKind kind) {
  BAILOUT("LoadReturnStackSlot");
}

void LiftoffAssembler::MoveStackValue(uint32_t dst_offset, uint32_t src_offset,
                                       ValueKind kind) {
  BAILOUT("MoveStackValue");
}

void LiftoffAssembler::Move(Register dst, Register src, ValueKind kind) {
  BAILOUT("Move Register");
}

void LiftoffAssembler::Move(DoubleRegister dst, DoubleRegister src,
                             ValueKind kind) {
  BAILOUT("Move DoubleRegister");
}

void LiftoffAssembler::Spill(int offset, LiftoffRegister reg, ValueKind kind) {
  BAILOUT("Spill");
}

void LiftoffAssembler::Spill(int offset, WasmValue value) {
  BAILOUT("Spill WasmValue");
}

void LiftoffAssembler::Fill(LiftoffRegister reg, int offset, ValueKind kind) {
  BAILOUT("Fill");
}

void LiftoffAssembler::FillI64Half(Register reg, int offset, RegPairHalf half) {
  BAILOUT("FillI64Half");
}

void LiftoffAssembler::FillStackSlotsWithZero(int start, int size) {
  BAILOUT("FillStackSlotsWithZero");
}

void LiftoffAssembler::LoadSpillAddress(Register dst, int offset,
                                         ValueKind kind) {
  BAILOUT("LoadSpillAddress");
}

void LiftoffAssembler::emit_trace_instruction(uint32_t markid) {
  BAILOUT("emit_trace_instruction");
}

void LiftoffAssembler::emit_i32_add(Register dst, Register lhs, Register rhs) {
  BAILOUT("emit_i32_add");
}

void LiftoffAssembler::emit_i32_addi(Register dst, Register lhs, int32_t imm) {
  BAILOUT("emit_i32_addi");
}

void LiftoffAssembler::emit_i32_sub(Register dst, Register lhs, Register rhs) {
  BAILOUT("emit_i32_sub");
}

void LiftoffAssembler::emit_i32_subi(Register dst, Register lhs, int32_t imm) {
  BAILOUT("emit_i32_subi");
}

void LiftoffAssembler::emit_i32_mul(Register dst, Register lhs, Register rhs) {
  BAILOUT("emit_i32_mul");
}

void LiftoffAssembler::emit_i32_muli(Register dst, Register lhs, int32_t imm) {
  BAILOUT("emit_i32_muli");
}

void LiftoffAssembler::emit_i32_divs(Register dst, Register lhs, Register rhs,
                                      Label* trap_div_by_zero,
                                      Label* trap_div_unrepresentable) {
  BAILOUT("emit_i32_divs");
}

void LiftoffAssembler::emit_i32_divu(Register dst, Register lhs, Register rhs,
                                      Label* trap_div_by_zero) {
  BAILOUT("emit_i32_divu");
}

void LiftoffAssembler::emit_i32_rems(Register dst, Register lhs, Register rhs,
                                      Label* trap_rem_by_zero) {
  BAILOUT("emit_i32_rems");
}

void LiftoffAssembler::emit_i32_remu(Register dst, Register lhs, Register rhs,
                                      Label* trap_rem_by_zero) {
  BAILOUT("emit_i32_remu");
}

void LiftoffAssembler::emit_i32_and(Register dst, Register lhs, Register rhs) {
  BAILOUT("emit_i32_and");
}

void LiftoffAssembler::emit_i32_andi(Register dst, Register lhs, int32_t imm) {
  BAILOUT("emit_i32_andi");
}

void LiftoffAssembler::emit_i32_or(Register dst, Register lhs, Register rhs) {
  BAILOUT("emit_i32_or");
}

void LiftoffAssembler::emit_i32_ori(Register dst, Register lhs, int32_t imm) {
  BAILOUT("emit_i32_ori");
}

void LiftoffAssembler::emit_i32_xor(Register dst, Register lhs, Register rhs) {
  BAILOUT("emit_i32_xor");
}

void LiftoffAssembler::emit_i32_xori(Register dst, Register lhs, int32_t imm) {
  BAILOUT("emit_i32_xori");
}

void LiftoffAssembler::emit_i32_shl(Register dst, Register src,
                                     Register amount) {
  BAILOUT("emit_i32_shl");
}

void LiftoffAssembler::emit_i32_shli(Register dst, Register src,
                                      int32_t amount) {
  BAILOUT("emit_i32_shli");
}

void LiftoffAssembler::emit_i32_sar(Register dst, Register src,
                                     Register amount) {
  BAILOUT("emit_i32_sar");
}

void LiftoffAssembler::emit_i32_sari(Register dst, Register src,
                                      int32_t amount) {
  BAILOUT("emit_i32_sari");
}

void LiftoffAssembler::emit_i32_shr(Register dst, Register src,
                                     Register amount) {
  BAILOUT("emit_i32_shr");
}

void LiftoffAssembler::emit_i32_shri(Register dst, Register src,
                                      int32_t amount) {
  BAILOUT("emit_i32_shri");
}

void LiftoffAssembler::emit_i32_clz(Register dst, Register src) {
  BAILOUT("emit_i32_clz");
}

void LiftoffAssembler::emit_i32_ctz(Register dst, Register src) {
  BAILOUT("emit_i32_ctz");
}

bool LiftoffAssembler::emit_i32_popcnt(Register dst, Register src) {
  BAILOUT_RETURN("emit_i32_popcnt", false);
}

void LiftoffAssembler::emit_i64_add(LiftoffRegister dst, LiftoffRegister lhs,
                                     LiftoffRegister rhs) {
  BAILOUT("emit_i64_add");
}

void LiftoffAssembler::emit_i64_addi(LiftoffRegister dst, LiftoffRegister lhs,
                                      int64_t imm) {
  BAILOUT("emit_i64_addi");
}

void LiftoffAssembler::emit_i64_sub(LiftoffRegister dst, LiftoffRegister lhs,
                                     LiftoffRegister rhs) {
  BAILOUT("emit_i64_sub");
}

void LiftoffAssembler::emit_i64_mul(LiftoffRegister dst, LiftoffRegister lhs,
                                     LiftoffRegister rhs) {
  BAILOUT("emit_i64_mul");
}

void LiftoffAssembler::emit_i64_muli(LiftoffRegister dst, LiftoffRegister lhs,
                                      int32_t imm) {
  BAILOUT("emit_i64_muli");
}

bool LiftoffAssembler::emit_i64_divs(LiftoffRegister dst, LiftoffRegister lhs,
                                      LiftoffRegister rhs,
                                      Label* trap_div_by_zero,
                                      Label* trap_div_unrepresentable) {
  BAILOUT_RETURN("emit_i64_divs", false);
}

bool LiftoffAssembler::emit_i64_divu(LiftoffRegister dst, LiftoffRegister lhs,
                                      LiftoffRegister rhs,
                                      Label* trap_div_by_zero) {
  BAILOUT_RETURN("emit_i64_divu", false);
}

bool LiftoffAssembler::emit_i64_rems(LiftoffRegister dst, LiftoffRegister lhs,
                                      LiftoffRegister rhs,
                                      Label* trap_rem_by_zero) {
  BAILOUT_RETURN("emit_i64_rems", false);
}

bool LiftoffAssembler::emit_i64_remu(LiftoffRegister dst, LiftoffRegister lhs,
                                      LiftoffRegister rhs,
                                      Label* trap_rem_by_zero) {
  BAILOUT_RETURN("emit_i64_remu", false);
}

void LiftoffAssembler::emit_i64_shl(LiftoffRegister dst, LiftoffRegister src,
                                     Register amount) {
  BAILOUT("emit_i64_shl");
}

void LiftoffAssembler::emit_i64_shli(LiftoffRegister dst, LiftoffRegister src,
                                      int32_t amount) {
  BAILOUT("emit_i64_shli");
}

void LiftoffAssembler::emit_i64_sar(LiftoffRegister dst, LiftoffRegister src,
                                     Register amount) {
  BAILOUT("emit_i64_sar");
}

void LiftoffAssembler::emit_i64_sari(LiftoffRegister dst, LiftoffRegister src,
                                      int32_t amount) {
  BAILOUT("emit_i64_sari");
}

void LiftoffAssembler::emit_i64_shr(LiftoffRegister dst, LiftoffRegister src,
                                     Register amount) {
  BAILOUT("emit_i64_shr");
}

void LiftoffAssembler::emit_i64_shri(LiftoffRegister dst, LiftoffRegister src,
                                      int32_t amount) {
  BAILOUT("emit_i64_shri");
}

void LiftoffAssembler::emit_i64_clz(LiftoffRegister dst, LiftoffRegister src) {
  BAILOUT("emit_i64_clz");
}

void LiftoffAssembler::emit_i64_ctz(LiftoffRegister dst, LiftoffRegister src) {
  BAILOUT("emit_i64_ctz");
}

bool LiftoffAssembler::emit_i64_popcnt(LiftoffRegister dst,
                                        LiftoffRegister src) {
  BAILOUT_RETURN("emit_i64_popcnt", false);
}

// emit_u32_to_uintptr is defined in liftoff-assembler-inl.h for 32-bit
// architectures.

bool LiftoffAssembler::emit_type_conversion(WasmOpcode opcode,
                                             LiftoffRegister dst,
                                             LiftoffRegister src, Label* trap) {
  BAILOUT_RETURN("emit_type_conversion", false);
}

void LiftoffAssembler::emit_i32_signextend_i8(Register dst, Register src) {
  BAILOUT("emit_i32_signextend_i8");
}

void LiftoffAssembler::emit_i32_signextend_i16(Register dst, Register src) {
  BAILOUT("emit_i32_signextend_i16");
}

void LiftoffAssembler::emit_i64_signextend_i8(LiftoffRegister dst,
                                               LiftoffRegister src) {
  BAILOUT("emit_i64_signextend_i8");
}

void LiftoffAssembler::emit_i64_signextend_i16(LiftoffRegister dst,
                                                LiftoffRegister src) {
  BAILOUT("emit_i64_signextend_i16");
}

void LiftoffAssembler::emit_i64_signextend_i32(LiftoffRegister dst,
                                                LiftoffRegister src) {
  BAILOUT("emit_i64_signextend_i32");
}

void LiftoffAssembler::emit_jump(Label* label) { BAILOUT("emit_jump"); }

void LiftoffAssembler::emit_jump(Register target) { BAILOUT("emit_jump reg"); }

void LiftoffAssembler::emit_cond_jump(Condition cond, Label* label,
                                       ValueKind kind, Register lhs,
                                       Register rhs,
                                       const FreezeCacheState& frozen) {
  BAILOUT("emit_cond_jump");
}

void LiftoffAssembler::emit_i32_cond_jumpi(Condition cond, Label* label,
                                            Register lhs, int32_t imm,
                                            const FreezeCacheState& frozen) {
  BAILOUT("emit_i32_cond_jumpi");
}

void LiftoffAssembler::emit_i32_eqz(Register dst, Register src) {
  BAILOUT("emit_i32_eqz");
}

void LiftoffAssembler::emit_i32_set_cond(Condition cond, Register dst,
                                          Register lhs, Register rhs) {
  BAILOUT("emit_i32_set_cond");
}

void LiftoffAssembler::emit_i64_eqz(Register dst, LiftoffRegister src) {
  BAILOUT("emit_i64_eqz");
}

void LiftoffAssembler::emit_i64_set_cond(Condition cond, Register dst,
                                          LiftoffRegister lhs,
                                          LiftoffRegister rhs) {
  BAILOUT("emit_i64_set_cond");
}

void LiftoffAssembler::emit_f32_set_cond(Condition cond, Register dst,
                                          DoubleRegister lhs,
                                          DoubleRegister rhs) {
  BAILOUT("emit_f32_set_cond");
}

void LiftoffAssembler::emit_f64_set_cond(Condition cond, Register dst,
                                          DoubleRegister lhs,
                                          DoubleRegister rhs) {
  BAILOUT("emit_f64_set_cond");
}

bool LiftoffAssembler::emit_select(LiftoffRegister dst, Register condition,
                                    LiftoffRegister true_value,
                                    LiftoffRegister false_value,
                                    ValueKind kind) {
  BAILOUT_RETURN("emit_select", false);
}

void LiftoffAssembler::emit_smi_check(Register obj, Label* target,
                                       SmiCheckMode mode,
                                       const FreezeCacheState& frozen) {
  BAILOUT("emit_smi_check");
}

void LiftoffAssembler::LoadTransform(LiftoffRegister dst, Register src_addr,
                                      Register offset_reg,
                                      uintptr_t offset_imm, LoadType type,
                                      LoadTransformationKind transform,
                                      uint32_t* protected_load_pc,
                                      bool i64_offset) {
  BAILOUT("LoadTransform");
}

void LiftoffAssembler::LoadLane(LiftoffRegister dst, LiftoffRegister src,
                                 Register addr, Register offset_reg,
                                 uintptr_t offset_imm, LoadType type,
                                 uint8_t lane, uint32_t* protected_load_pc,
                                 bool i64_offset) {
  BAILOUT("LoadLane");
}

void LiftoffAssembler::StoreLane(Register dst, Register offset,
                                  uintptr_t offset_imm, LiftoffRegister src,
                                  StoreType type, uint8_t lane,
                                  uint32_t* protected_store_pc,
                                  bool i64_offset) {
  BAILOUT("StoreLane");
}

void LiftoffAssembler::emit_i8x16_shuffle(LiftoffRegister dst,
                                           LiftoffRegister lhs,
                                           LiftoffRegister rhs,
                                           const uint8_t shuffle[16],
                                           bool is_swizzle) {
  BAILOUT("emit_i8x16_shuffle");
}

void LiftoffAssembler::emit_i8x16_swizzle(LiftoffRegister dst,
                                           LiftoffRegister lhs,
                                           LiftoffRegister rhs) {
  BAILOUT("emit_i8x16_swizzle");
}

void LiftoffAssembler::emit_i8x16_relaxed_swizzle(LiftoffRegister dst,
                                                   LiftoffRegister lhs,
                                                   LiftoffRegister rhs) {
  BAILOUT("emit_i8x16_relaxed_swizzle");
}

void LiftoffAssembler::emit_i32x4_relaxed_trunc_f32x4_s(LiftoffRegister dst,
                                                         LiftoffRegister src) {
  BAILOUT("emit_i32x4_relaxed_trunc_f32x4_s");
}

void LiftoffAssembler::emit_i32x4_relaxed_trunc_f32x4_u(LiftoffRegister dst,
                                                         LiftoffRegister src) {
  BAILOUT("emit_i32x4_relaxed_trunc_f32x4_u");
}

void LiftoffAssembler::emit_i32x4_relaxed_trunc_f64x2_s_zero(
    LiftoffRegister dst, LiftoffRegister src) {
  BAILOUT("emit_i32x4_relaxed_trunc_f64x2_s_zero");
}

void LiftoffAssembler::emit_i32x4_relaxed_trunc_f64x2_u_zero(
    LiftoffRegister dst, LiftoffRegister src) {
  BAILOUT("emit_i32x4_relaxed_trunc_f64x2_u_zero");
}

void LiftoffAssembler::emit_s128_relaxed_laneselect(LiftoffRegister dst,
                                                     LiftoffRegister src1,
                                                     LiftoffRegister src2,
                                                     LiftoffRegister mask,
                                                     int lane_width) {
  BAILOUT("emit_s128_relaxed_laneselect");
}

void LiftoffAssembler::emit_f32_neg(DoubleRegister dst, DoubleRegister src) {
  BAILOUT("emit_f32_neg");
}

void LiftoffAssembler::emit_f64_neg(DoubleRegister dst, DoubleRegister src) {
  BAILOUT("emit_f64_neg");
}

void LiftoffAssembler::emit_f32_min(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f32_min");
}

void LiftoffAssembler::emit_f32_max(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f32_max");
}

void LiftoffAssembler::emit_f32_copysign(DoubleRegister dst, DoubleRegister lhs,
                                          DoubleRegister rhs) {
  BAILOUT("emit_f32_copysign");
}

void LiftoffAssembler::emit_f64_min(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f64_min");
}

void LiftoffAssembler::emit_f64_max(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f64_max");
}

void LiftoffAssembler::emit_f64_copysign(DoubleRegister dst, DoubleRegister lhs,
                                          DoubleRegister rhs) {
  BAILOUT("emit_f64_copysign");
}

void LiftoffAssembler::emit_f32_abs(DoubleRegister dst, DoubleRegister src) {
  BAILOUT("emit_f32_abs");
}

void LiftoffAssembler::emit_f64_abs(DoubleRegister dst, DoubleRegister src) {
  BAILOUT("emit_f64_abs");
}

bool LiftoffAssembler::emit_f32_ceil(DoubleRegister dst, DoubleRegister src) {
  BAILOUT_RETURN("emit_f32_ceil", false);
}

bool LiftoffAssembler::emit_f32_floor(DoubleRegister dst, DoubleRegister src) {
  BAILOUT_RETURN("emit_f32_floor", false);
}

bool LiftoffAssembler::emit_f32_trunc(DoubleRegister dst, DoubleRegister src) {
  BAILOUT_RETURN("emit_f32_trunc", false);
}

bool LiftoffAssembler::emit_f32_nearest_int(DoubleRegister dst,
                                             DoubleRegister src) {
  BAILOUT_RETURN("emit_f32_nearest_int", false);
}

bool LiftoffAssembler::emit_f64_ceil(DoubleRegister dst, DoubleRegister src) {
  BAILOUT_RETURN("emit_f64_ceil", false);
}

bool LiftoffAssembler::emit_f64_floor(DoubleRegister dst, DoubleRegister src) {
  BAILOUT_RETURN("emit_f64_floor", false);
}

bool LiftoffAssembler::emit_f64_trunc(DoubleRegister dst, DoubleRegister src) {
  BAILOUT_RETURN("emit_f64_trunc", false);
}

bool LiftoffAssembler::emit_f64_nearest_int(DoubleRegister dst,
                                             DoubleRegister src) {
  BAILOUT_RETURN("emit_f64_nearest_int", false);
}

void LiftoffAssembler::emit_f32_sqrt(DoubleRegister dst, DoubleRegister src) {
  BAILOUT("emit_f32_sqrt");
}

void LiftoffAssembler::emit_f64_sqrt(DoubleRegister dst, DoubleRegister src) {
  BAILOUT("emit_f64_sqrt");
}

void LiftoffAssembler::emit_f32_add(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f32_add");
}

void LiftoffAssembler::emit_f32_sub(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f32_sub");
}

void LiftoffAssembler::emit_f32_mul(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f32_mul");
}

void LiftoffAssembler::emit_f32_div(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f32_div");
}

void LiftoffAssembler::emit_f64_add(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f64_add");
}

void LiftoffAssembler::emit_f64_sub(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f64_sub");
}

void LiftoffAssembler::emit_f64_mul(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f64_mul");
}

void LiftoffAssembler::emit_f64_div(DoubleRegister dst, DoubleRegister lhs,
                                     DoubleRegister rhs) {
  BAILOUT("emit_f64_div");
}

void LiftoffAssembler::IncrementSmi(LiftoffRegister dst, int offset) {
  BAILOUT("IncrementSmi");
}

void LiftoffAssembler::DropStackSlotsAndRet(uint32_t num_stack_slots) {
  BAILOUT("DropStackSlotsAndRet");
}

void LiftoffAssembler::CallCWithStackBuffer(
    const std::initializer_list<VarState> args, const LiftoffRegister* rets,
    ValueKind return_kind, ValueKind out_argument_kind, int stack_bytes,
    ExternalReference ext_ref) {
  BAILOUT("CallCWithStackBuffer");
}

void LiftoffAssembler::CallC(const std::initializer_list<VarState> args,
                              ExternalReference ext_ref) {
  BAILOUT("CallC");
}

void LiftoffAssembler::CallNativeWasmCode(Address addr) {
  BAILOUT("CallNativeWasmCode");
}

void LiftoffAssembler::TailCallNativeWasmCode(Address addr) {
  BAILOUT("TailCallNativeWasmCode");
}

void LiftoffAssembler::CallIndirect(const ValueKindSig* sig,
                                     compiler::CallDescriptor* call_descriptor,
                                     Register target) {
  BAILOUT("CallIndirect");
}

void LiftoffAssembler::TailCallIndirect(
    compiler::CallDescriptor* call_descriptor, Register target) {
  BAILOUT("TailCallIndirect");
}

void LiftoffAssembler::CallBuiltin(Builtin builtin) {
  BAILOUT("CallBuiltin");
}

void LiftoffAssembler::AllocateStackSlot(Register addr, uint32_t size) {
  BAILOUT("AllocateStackSlot");
}

void LiftoffAssembler::DeallocateStackSlot(uint32_t size) {
  BAILOUT("DeallocateStackSlot");
}

void LiftoffAssembler::MaybeOSR() {}

void LiftoffAssembler::emit_store_nonzero_if_nan(Register dst,
                                                  DoubleRegister src,
                                                  ValueKind kind) {
  BAILOUT("emit_store_nonzero_if_nan");
}

void LiftoffAssembler::emit_s128_store_nonzero_if_nan(Register dst,
                                                       LiftoffRegister src,
                                                       Register tmp_gp,
                                                       LiftoffRegister tmp_s128,
                                                       ValueKind lane_kind) {
  BAILOUT("emit_s128_store_nonzero_if_nan");
}

void LiftoffAssembler::emit_store_nonzero(Register dst) {
  BAILOUT("emit_store_nonzero");
}

// Stub SIMD operations
#define SIMD_BINOP_STUB(name)                                        \
  void LiftoffAssembler::emit_##name(LiftoffRegister dst,            \
                                     LiftoffRegister lhs,            \
                                     LiftoffRegister rhs) {          \
    BAILOUT("emit_" #name);                                          \
  }

#define SIMD_UNOP_STUB(name)                                         \
  void LiftoffAssembler::emit_##name(LiftoffRegister dst,            \
                                     LiftoffRegister src) {          \
    BAILOUT("emit_" #name);                                          \
  }

#define SIMD_BOOL_UNOP_STUB(name)                                    \
  bool LiftoffAssembler::emit_##name(LiftoffRegister dst,            \
                                     LiftoffRegister src) {          \
    BAILOUT_RETURN("emit_" #name, false);                            \
  }

SIMD_UNOP_STUB(i8x16_popcnt)
SIMD_UNOP_STUB(i8x16_splat)
SIMD_UNOP_STUB(i16x8_splat)
SIMD_UNOP_STUB(i32x4_splat)
SIMD_UNOP_STUB(i64x2_splat)
SIMD_BOOL_UNOP_STUB(f16x8_splat)
SIMD_UNOP_STUB(f32x4_splat)
SIMD_UNOP_STUB(f64x2_splat)

SIMD_BINOP_STUB(i8x16_eq)
SIMD_BINOP_STUB(i8x16_ne)
SIMD_BINOP_STUB(i8x16_gt_s)
SIMD_BINOP_STUB(i8x16_gt_u)
SIMD_BINOP_STUB(i8x16_ge_s)
SIMD_BINOP_STUB(i8x16_ge_u)
SIMD_BINOP_STUB(i16x8_eq)
SIMD_BINOP_STUB(i16x8_ne)
SIMD_BINOP_STUB(i16x8_gt_s)
SIMD_BINOP_STUB(i16x8_gt_u)
SIMD_BINOP_STUB(i16x8_ge_s)
SIMD_BINOP_STUB(i16x8_ge_u)
SIMD_BINOP_STUB(i32x4_eq)
SIMD_BINOP_STUB(i32x4_ne)
SIMD_BINOP_STUB(i32x4_gt_s)
SIMD_BINOP_STUB(i32x4_gt_u)
SIMD_BINOP_STUB(i32x4_ge_s)
SIMD_BINOP_STUB(i32x4_ge_u)
SIMD_BINOP_STUB(i64x2_eq)
SIMD_BINOP_STUB(i64x2_ne)
SIMD_BINOP_STUB(i64x2_gt_s)
SIMD_BINOP_STUB(i64x2_ge_s)
SIMD_BINOP_STUB(f32x4_eq)
SIMD_BINOP_STUB(f32x4_ne)
SIMD_BINOP_STUB(f32x4_lt)
SIMD_BINOP_STUB(f32x4_le)
SIMD_BINOP_STUB(f64x2_eq)
SIMD_BINOP_STUB(f64x2_ne)
SIMD_BINOP_STUB(f64x2_lt)
SIMD_BINOP_STUB(f64x2_le)
SIMD_BINOP_STUB(s128_and)
SIMD_BINOP_STUB(s128_or)
SIMD_BINOP_STUB(s128_xor)
SIMD_BINOP_STUB(s128_and_not)
SIMD_UNOP_STUB(s128_not)
SIMD_BINOP_STUB(i8x16_add)
SIMD_BINOP_STUB(i8x16_add_sat_s)
SIMD_BINOP_STUB(i8x16_add_sat_u)
SIMD_BINOP_STUB(i8x16_sub)
SIMD_BINOP_STUB(i8x16_sub_sat_s)
SIMD_BINOP_STUB(i8x16_sub_sat_u)
SIMD_BINOP_STUB(i8x16_min_s)
SIMD_BINOP_STUB(i8x16_min_u)
SIMD_BINOP_STUB(i8x16_max_s)
SIMD_BINOP_STUB(i8x16_max_u)
SIMD_UNOP_STUB(i8x16_neg)
SIMD_BINOP_STUB(i16x8_add)
SIMD_BINOP_STUB(i16x8_add_sat_s)
SIMD_BINOP_STUB(i16x8_add_sat_u)
SIMD_BINOP_STUB(i16x8_sub)
SIMD_BINOP_STUB(i16x8_sub_sat_s)
SIMD_BINOP_STUB(i16x8_sub_sat_u)
SIMD_BINOP_STUB(i16x8_mul)
SIMD_BINOP_STUB(i16x8_min_s)
SIMD_BINOP_STUB(i16x8_min_u)
SIMD_BINOP_STUB(i16x8_max_s)
SIMD_BINOP_STUB(i16x8_max_u)
SIMD_UNOP_STUB(i16x8_neg)
SIMD_BINOP_STUB(i32x4_add)
SIMD_BINOP_STUB(i32x4_sub)
SIMD_BINOP_STUB(i32x4_mul)
SIMD_BINOP_STUB(i32x4_min_s)
SIMD_BINOP_STUB(i32x4_min_u)
SIMD_BINOP_STUB(i32x4_max_s)
SIMD_BINOP_STUB(i32x4_max_u)
SIMD_UNOP_STUB(i32x4_neg)
SIMD_BINOP_STUB(i64x2_add)
SIMD_BINOP_STUB(i64x2_sub)
SIMD_BINOP_STUB(i64x2_mul)
SIMD_UNOP_STUB(i64x2_neg)
SIMD_BINOP_STUB(f32x4_add)
SIMD_BINOP_STUB(f32x4_sub)
SIMD_BINOP_STUB(f32x4_mul)
SIMD_BINOP_STUB(f32x4_div)
SIMD_BINOP_STUB(f32x4_min)
SIMD_BINOP_STUB(f32x4_max)
SIMD_BINOP_STUB(f32x4_pmin)
SIMD_BINOP_STUB(f32x4_pmax)
SIMD_UNOP_STUB(f32x4_abs)
SIMD_UNOP_STUB(f32x4_neg)
SIMD_UNOP_STUB(f32x4_sqrt)
SIMD_BOOL_UNOP_STUB(f32x4_ceil)
SIMD_BOOL_UNOP_STUB(f32x4_floor)
SIMD_BOOL_UNOP_STUB(f32x4_trunc)
SIMD_BOOL_UNOP_STUB(f32x4_nearest_int)
SIMD_BINOP_STUB(f64x2_add)
SIMD_BINOP_STUB(f64x2_sub)
SIMD_BINOP_STUB(f64x2_mul)
SIMD_BINOP_STUB(f64x2_div)
SIMD_BINOP_STUB(f64x2_min)
SIMD_BINOP_STUB(f64x2_max)
SIMD_BINOP_STUB(f64x2_pmin)
SIMD_BINOP_STUB(f64x2_pmax)
SIMD_UNOP_STUB(f64x2_abs)
SIMD_UNOP_STUB(f64x2_neg)
SIMD_UNOP_STUB(f64x2_sqrt)
SIMD_BOOL_UNOP_STUB(f64x2_ceil)
SIMD_BOOL_UNOP_STUB(f64x2_floor)
SIMD_BOOL_UNOP_STUB(f64x2_trunc)
SIMD_BOOL_UNOP_STUB(f64x2_nearest_int)
SIMD_UNOP_STUB(i8x16_abs)
SIMD_UNOP_STUB(i16x8_abs)
SIMD_UNOP_STUB(i32x4_abs)
SIMD_UNOP_STUB(i64x2_abs)
SIMD_UNOP_STUB(i16x8_extadd_pairwise_i8x16_s)
SIMD_UNOP_STUB(i16x8_extadd_pairwise_i8x16_u)
SIMD_UNOP_STUB(i32x4_extadd_pairwise_i16x8_s)
SIMD_UNOP_STUB(i32x4_extadd_pairwise_i16x8_u)
SIMD_BINOP_STUB(i16x8_extmul_low_i8x16_s)
SIMD_BINOP_STUB(i16x8_extmul_low_i8x16_u)
SIMD_BINOP_STUB(i16x8_extmul_high_i8x16_s)
SIMD_BINOP_STUB(i16x8_extmul_high_i8x16_u)
SIMD_BINOP_STUB(i32x4_extmul_low_i16x8_s)
SIMD_BINOP_STUB(i32x4_extmul_low_i16x8_u)
SIMD_BINOP_STUB(i32x4_extmul_high_i16x8_s)
SIMD_BINOP_STUB(i32x4_extmul_high_i16x8_u)
SIMD_BINOP_STUB(i64x2_extmul_low_i32x4_s)
SIMD_BINOP_STUB(i64x2_extmul_low_i32x4_u)
SIMD_BINOP_STUB(i64x2_extmul_high_i32x4_s)
SIMD_BINOP_STUB(i64x2_extmul_high_i32x4_u)
SIMD_BINOP_STUB(i16x8_q15mulr_sat_s)
SIMD_BINOP_STUB(i16x8_relaxed_q15mulr_s)
SIMD_BINOP_STUB(i16x8_dot_i8x16_i7x16_s)
SIMD_UNOP_STUB(i32x4_sconvert_f32x4)
SIMD_UNOP_STUB(i32x4_uconvert_f32x4)
SIMD_UNOP_STUB(f32x4_sconvert_i32x4)
SIMD_UNOP_STUB(f32x4_uconvert_i32x4)
SIMD_UNOP_STUB(i32x4_trunc_sat_f64x2_s_zero)
SIMD_UNOP_STUB(i32x4_trunc_sat_f64x2_u_zero)
SIMD_UNOP_STUB(f64x2_convert_low_i32x4_s)
SIMD_UNOP_STUB(f64x2_convert_low_i32x4_u)
SIMD_UNOP_STUB(f64x2_promote_low_f32x4)
SIMD_UNOP_STUB(f32x4_demote_f64x2_zero)
SIMD_BINOP_STUB(i8x16_sconvert_i16x8)
SIMD_BINOP_STUB(i8x16_uconvert_i16x8)
SIMD_BINOP_STUB(i16x8_sconvert_i32x4)
SIMD_BINOP_STUB(i16x8_uconvert_i32x4)
SIMD_UNOP_STUB(i16x8_sconvert_i8x16_low)
SIMD_UNOP_STUB(i16x8_sconvert_i8x16_high)
SIMD_UNOP_STUB(i16x8_uconvert_i8x16_low)
SIMD_UNOP_STUB(i16x8_uconvert_i8x16_high)
SIMD_UNOP_STUB(i32x4_sconvert_i16x8_low)
SIMD_UNOP_STUB(i32x4_sconvert_i16x8_high)
SIMD_UNOP_STUB(i32x4_uconvert_i16x8_low)
SIMD_UNOP_STUB(i32x4_uconvert_i16x8_high)
SIMD_UNOP_STUB(i64x2_sconvert_i32x4_low)
SIMD_UNOP_STUB(i64x2_sconvert_i32x4_high)
SIMD_UNOP_STUB(i64x2_uconvert_i32x4_low)
SIMD_UNOP_STUB(i64x2_uconvert_i32x4_high)
SIMD_BINOP_STUB(i8x16_rounding_average_u)
SIMD_BINOP_STUB(i16x8_rounding_average_u)
SIMD_BINOP_STUB(i8x16_shl)
SIMD_BINOP_STUB(i8x16_shr_s)
SIMD_BINOP_STUB(i8x16_shr_u)
SIMD_BINOP_STUB(i16x8_shl)
SIMD_BINOP_STUB(i16x8_shr_s)
SIMD_BINOP_STUB(i16x8_shr_u)
SIMD_BINOP_STUB(i32x4_shl)
SIMD_BINOP_STUB(i32x4_shr_s)
SIMD_BINOP_STUB(i32x4_shr_u)
SIMD_BINOP_STUB(i64x2_shl)
SIMD_BINOP_STUB(i64x2_shr_s)
SIMD_BINOP_STUB(i64x2_shr_u)

#undef SIMD_BINOP_STUB
#undef SIMD_UNOP_STUB
#undef SIMD_BOOL_UNOP_STUB

void LiftoffAssembler::emit_i8x16_shli(LiftoffRegister dst, LiftoffRegister lhs,
                                        int32_t rhs) {
  BAILOUT("emit_i8x16_shli");
}

void LiftoffAssembler::emit_i8x16_shri_s(LiftoffRegister dst,
                                          LiftoffRegister lhs, int32_t rhs) {
  BAILOUT("emit_i8x16_shri_s");
}

void LiftoffAssembler::emit_i8x16_shri_u(LiftoffRegister dst,
                                          LiftoffRegister lhs, int32_t rhs) {
  BAILOUT("emit_i8x16_shri_u");
}

void LiftoffAssembler::emit_i16x8_shli(LiftoffRegister dst, LiftoffRegister lhs,
                                        int32_t rhs) {
  BAILOUT("emit_i16x8_shli");
}

void LiftoffAssembler::emit_i16x8_shri_s(LiftoffRegister dst,
                                          LiftoffRegister lhs, int32_t rhs) {
  BAILOUT("emit_i16x8_shri_s");
}

void LiftoffAssembler::emit_i16x8_shri_u(LiftoffRegister dst,
                                          LiftoffRegister lhs, int32_t rhs) {
  BAILOUT("emit_i16x8_shri_u");
}

void LiftoffAssembler::emit_i32x4_shli(LiftoffRegister dst, LiftoffRegister lhs,
                                        int32_t rhs) {
  BAILOUT("emit_i32x4_shli");
}

void LiftoffAssembler::emit_i32x4_shri_s(LiftoffRegister dst,
                                          LiftoffRegister lhs, int32_t rhs) {
  BAILOUT("emit_i32x4_shri_s");
}

void LiftoffAssembler::emit_i32x4_shri_u(LiftoffRegister dst,
                                          LiftoffRegister lhs, int32_t rhs) {
  BAILOUT("emit_i32x4_shri_u");
}

void LiftoffAssembler::emit_i64x2_shli(LiftoffRegister dst, LiftoffRegister lhs,
                                        int32_t rhs) {
  BAILOUT("emit_i64x2_shli");
}

void LiftoffAssembler::emit_i64x2_shri_s(LiftoffRegister dst,
                                          LiftoffRegister lhs, int32_t rhs) {
  BAILOUT("emit_i64x2_shri_s");
}

void LiftoffAssembler::emit_i64x2_shri_u(LiftoffRegister dst,
                                          LiftoffRegister lhs, int32_t rhs) {
  BAILOUT("emit_i64x2_shri_u");
}

void LiftoffAssembler::emit_i32x4_dot_i8x16_i7x16_add_s(LiftoffRegister dst,
                                                         LiftoffRegister lhs,
                                                         LiftoffRegister rhs,
                                                         LiftoffRegister acc) {
  BAILOUT("emit_i32x4_dot_i8x16_i7x16_add_s");
}

void LiftoffAssembler::emit_i32x4_dot_i16x8_s(LiftoffRegister dst,
                                               LiftoffRegister lhs,
                                               LiftoffRegister rhs) {
  BAILOUT("emit_i32x4_dot_i16x8_s");
}

void LiftoffAssembler::emit_s128_const(LiftoffRegister dst,
                                        const uint8_t imms[16]) {
  BAILOUT("emit_s128_const");
}

void LiftoffAssembler::emit_s128_select(LiftoffRegister dst,
                                         LiftoffRegister src1,
                                         LiftoffRegister src2,
                                         LiftoffRegister mask) {
  BAILOUT("emit_s128_select");
}

void LiftoffAssembler::emit_i8x16_extract_lane_s(LiftoffRegister dst,
                                                  LiftoffRegister lhs,
                                                  uint8_t imm_lane_idx) {
  BAILOUT("emit_i8x16_extract_lane_s");
}

void LiftoffAssembler::emit_i8x16_extract_lane_u(LiftoffRegister dst,
                                                  LiftoffRegister lhs,
                                                  uint8_t imm_lane_idx) {
  BAILOUT("emit_i8x16_extract_lane_u");
}

void LiftoffAssembler::emit_i16x8_extract_lane_s(LiftoffRegister dst,
                                                  LiftoffRegister lhs,
                                                  uint8_t imm_lane_idx) {
  BAILOUT("emit_i16x8_extract_lane_s");
}

void LiftoffAssembler::emit_i16x8_extract_lane_u(LiftoffRegister dst,
                                                  LiftoffRegister lhs,
                                                  uint8_t imm_lane_idx) {
  BAILOUT("emit_i16x8_extract_lane_u");
}

void LiftoffAssembler::emit_i32x4_extract_lane(LiftoffRegister dst,
                                                LiftoffRegister lhs,
                                                uint8_t imm_lane_idx) {
  BAILOUT("emit_i32x4_extract_lane");
}

void LiftoffAssembler::emit_i64x2_extract_lane(LiftoffRegister dst,
                                                LiftoffRegister lhs,
                                                uint8_t imm_lane_idx) {
  BAILOUT("emit_i64x2_extract_lane");
}

void LiftoffAssembler::emit_f32x4_extract_lane(LiftoffRegister dst,
                                                LiftoffRegister lhs,
                                                uint8_t imm_lane_idx) {
  BAILOUT("emit_f32x4_extract_lane");
}

void LiftoffAssembler::emit_f64x2_extract_lane(LiftoffRegister dst,
                                                LiftoffRegister lhs,
                                                uint8_t imm_lane_idx) {
  BAILOUT("emit_f64x2_extract_lane");
}

void LiftoffAssembler::emit_i8x16_replace_lane(LiftoffRegister dst,
                                                LiftoffRegister src1,
                                                LiftoffRegister src2,
                                                uint8_t imm_lane_idx) {
  BAILOUT("emit_i8x16_replace_lane");
}

void LiftoffAssembler::emit_i16x8_replace_lane(LiftoffRegister dst,
                                                LiftoffRegister src1,
                                                LiftoffRegister src2,
                                                uint8_t imm_lane_idx) {
  BAILOUT("emit_i16x8_replace_lane");
}

void LiftoffAssembler::emit_i32x4_replace_lane(LiftoffRegister dst,
                                                LiftoffRegister src1,
                                                LiftoffRegister src2,
                                                uint8_t imm_lane_idx) {
  BAILOUT("emit_i32x4_replace_lane");
}

void LiftoffAssembler::emit_i64x2_replace_lane(LiftoffRegister dst,
                                                LiftoffRegister src1,
                                                LiftoffRegister src2,
                                                uint8_t imm_lane_idx) {
  BAILOUT("emit_i64x2_replace_lane");
}

void LiftoffAssembler::emit_f32x4_replace_lane(LiftoffRegister dst,
                                                LiftoffRegister src1,
                                                LiftoffRegister src2,
                                                uint8_t imm_lane_idx) {
  BAILOUT("emit_f32x4_replace_lane");
}

void LiftoffAssembler::emit_f64x2_replace_lane(LiftoffRegister dst,
                                                LiftoffRegister src1,
                                                LiftoffRegister src2,
                                                uint8_t imm_lane_idx) {
  BAILOUT("emit_f64x2_replace_lane");
}

void LiftoffAssembler::emit_f32x4_qfma(LiftoffRegister dst, LiftoffRegister a,
                                        LiftoffRegister b, LiftoffRegister c) {
  BAILOUT("emit_f32x4_qfma");
}

void LiftoffAssembler::emit_f32x4_qfms(LiftoffRegister dst, LiftoffRegister a,
                                        LiftoffRegister b, LiftoffRegister c) {
  BAILOUT("emit_f32x4_qfms");
}

void LiftoffAssembler::emit_f64x2_qfma(LiftoffRegister dst, LiftoffRegister a,
                                        LiftoffRegister b, LiftoffRegister c) {
  BAILOUT("emit_f64x2_qfma");
}

void LiftoffAssembler::emit_f64x2_qfms(LiftoffRegister dst, LiftoffRegister a,
                                        LiftoffRegister b, LiftoffRegister c) {
  BAILOUT("emit_f64x2_qfms");
}

void LiftoffAssembler::set_trap_on_oob_mem64(Register index, uint64_t max_index,
                                              Label* trap_label) {
  BAILOUT("set_trap_on_oob_mem64");
}

bool LiftoffAssembler::emit_f16x8_extract_lane(LiftoffRegister dst,
                                                LiftoffRegister lhs,
                                                uint8_t imm_lane_idx) {
  BAILOUT_RETURN("emit_f16x8_extract_lane", false);
}

bool LiftoffAssembler::emit_f16x8_replace_lane(LiftoffRegister dst,
                                                LiftoffRegister src1,
                                                LiftoffRegister src2,
                                                uint8_t imm_lane_idx) {
  BAILOUT_RETURN("emit_f16x8_replace_lane", false);
}

bool LiftoffAssembler::emit_f16x8_abs(LiftoffRegister dst,
                                       LiftoffRegister src) {
  BAILOUT_RETURN("emit_f16x8_abs", false);
}

bool LiftoffAssembler::emit_f16x8_neg(LiftoffRegister dst,
                                       LiftoffRegister src) {
  BAILOUT_RETURN("emit_f16x8_neg", false);
}

bool LiftoffAssembler::emit_f16x8_sqrt(LiftoffRegister dst,
                                        LiftoffRegister src) {
  BAILOUT_RETURN("emit_f16x8_sqrt", false);
}

bool LiftoffAssembler::emit_f16x8_ceil(LiftoffRegister dst,
                                        LiftoffRegister src) {
  BAILOUT_RETURN("emit_f16x8_ceil", false);
}

bool LiftoffAssembler::emit_f16x8_floor(LiftoffRegister dst,
                                         LiftoffRegister src) {
  BAILOUT_RETURN("emit_f16x8_floor", false);
}

bool LiftoffAssembler::emit_f16x8_trunc(LiftoffRegister dst,
                                         LiftoffRegister src) {
  BAILOUT_RETURN("emit_f16x8_trunc", false);
}

bool LiftoffAssembler::emit_f16x8_nearest_int(LiftoffRegister dst,
                                               LiftoffRegister src) {
  BAILOUT_RETURN("emit_f16x8_nearest_int", false);
}

bool LiftoffAssembler::emit_f16x8_add(LiftoffRegister dst, LiftoffRegister lhs,
                                       LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_add", false);
}

bool LiftoffAssembler::emit_f16x8_sub(LiftoffRegister dst, LiftoffRegister lhs,
                                       LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_sub", false);
}

bool LiftoffAssembler::emit_f16x8_mul(LiftoffRegister dst, LiftoffRegister lhs,
                                       LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_mul", false);
}

bool LiftoffAssembler::emit_f16x8_div(LiftoffRegister dst, LiftoffRegister lhs,
                                       LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_div", false);
}

bool LiftoffAssembler::emit_f16x8_min(LiftoffRegister dst, LiftoffRegister lhs,
                                       LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_min", false);
}

bool LiftoffAssembler::emit_f16x8_max(LiftoffRegister dst, LiftoffRegister lhs,
                                       LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_max", false);
}

bool LiftoffAssembler::emit_f16x8_pmin(LiftoffRegister dst, LiftoffRegister lhs,
                                        LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_pmin", false);
}

bool LiftoffAssembler::emit_f16x8_pmax(LiftoffRegister dst, LiftoffRegister lhs,
                                        LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_pmax", false);
}

bool LiftoffAssembler::emit_i16x8_sconvert_f16x8(LiftoffRegister dst,
                                                  LiftoffRegister src) {
  BAILOUT_RETURN("emit_i16x8_sconvert_f16x8", false);
}

bool LiftoffAssembler::emit_i16x8_uconvert_f16x8(LiftoffRegister dst,
                                                  LiftoffRegister src) {
  BAILOUT_RETURN("emit_i16x8_uconvert_f16x8", false);
}

bool LiftoffAssembler::emit_f16x8_sconvert_i16x8(LiftoffRegister dst,
                                                  LiftoffRegister src) {
  BAILOUT_RETURN("emit_f16x8_sconvert_i16x8", false);
}

bool LiftoffAssembler::emit_f16x8_uconvert_i16x8(LiftoffRegister dst,
                                                  LiftoffRegister src) {
  BAILOUT_RETURN("emit_f16x8_uconvert_i16x8", false);
}

bool LiftoffAssembler::emit_f16x8_demote_f32x4_zero(LiftoffRegister dst,
                                                     LiftoffRegister src) {
  BAILOUT_RETURN("emit_f16x8_demote_f32x4_zero", false);
}

bool LiftoffAssembler::emit_f16x8_demote_f64x2_zero(LiftoffRegister dst,
                                                     LiftoffRegister src) {
  BAILOUT_RETURN("emit_f16x8_demote_f64x2_zero", false);
}

bool LiftoffAssembler::emit_f32x4_promote_low_f16x8(LiftoffRegister dst,
                                                     LiftoffRegister src) {
  BAILOUT_RETURN("emit_f32x4_promote_low_f16x8", false);
}

bool LiftoffAssembler::emit_f16x8_eq(LiftoffRegister dst, LiftoffRegister lhs,
                                      LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_eq", false);
}

bool LiftoffAssembler::emit_f16x8_ne(LiftoffRegister dst, LiftoffRegister lhs,
                                      LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_ne", false);
}

bool LiftoffAssembler::emit_f16x8_lt(LiftoffRegister dst, LiftoffRegister lhs,
                                      LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_lt", false);
}

bool LiftoffAssembler::emit_f16x8_le(LiftoffRegister dst, LiftoffRegister lhs,
                                      LiftoffRegister rhs) {
  BAILOUT_RETURN("emit_f16x8_le", false);
}

bool LiftoffAssembler::emit_f16x8_qfma(LiftoffRegister dst, LiftoffRegister a,
                                        LiftoffRegister b, LiftoffRegister c) {
  BAILOUT_RETURN("emit_f16x8_qfma", false);
}

bool LiftoffAssembler::emit_f16x8_qfms(LiftoffRegister dst, LiftoffRegister a,
                                        LiftoffRegister b, LiftoffRegister c) {
  BAILOUT_RETURN("emit_f16x8_qfms", false);
}

bool LiftoffAssembler::supports_f16_mem_access() { return false; }

void LiftoffAssembler::StackCheck(Label* ool_code) { BAILOUT("StackCheck"); }

void LiftoffAssembler::AssertUnreachable(AbortReason reason) {
  BAILOUT("AssertUnreachable");
}

void LiftoffAssembler::PushRegisters(LiftoffRegList regs) {
  BAILOUT("PushRegisters");
}

void LiftoffAssembler::PopRegisters(LiftoffRegList regs) {
  BAILOUT("PopRegisters");
}

void LiftoffAssembler::RecordSpillsInSafepoint(
    SafepointTableBuilder::Safepoint& safepoint, LiftoffRegList all_spills,
    LiftoffRegList ref_spills, int spill_offset) {
  BAILOUT("RecordSpillsInSafepoint");
}

void LiftoffAssembler::emit_i8x16_bitmask(LiftoffRegister dst,
                                           LiftoffRegister src) {
  BAILOUT("emit_i8x16_bitmask");
}

void LiftoffAssembler::emit_i16x8_bitmask(LiftoffRegister dst,
                                           LiftoffRegister src) {
  BAILOUT("emit_i16x8_bitmask");
}

void LiftoffAssembler::emit_i32x4_bitmask(LiftoffRegister dst,
                                           LiftoffRegister src) {
  BAILOUT("emit_i32x4_bitmask");
}

void LiftoffAssembler::emit_i64x2_bitmask(LiftoffRegister dst,
                                           LiftoffRegister src) {
  BAILOUT("emit_i64x2_bitmask");
}

void LiftoffAssembler::emit_v128_anytrue(LiftoffRegister dst,
                                          LiftoffRegister src) {
  BAILOUT("emit_v128_anytrue");
}

void LiftoffAssembler::emit_i8x16_alltrue(LiftoffRegister dst,
                                           LiftoffRegister src) {
  BAILOUT("emit_i8x16_alltrue");
}

void LiftoffAssembler::emit_i16x8_alltrue(LiftoffRegister dst,
                                           LiftoffRegister src) {
  BAILOUT("emit_i16x8_alltrue");
}

void LiftoffAssembler::emit_i32x4_alltrue(LiftoffRegister dst,
                                           LiftoffRegister src) {
  BAILOUT("emit_i32x4_alltrue");
}

void LiftoffAssembler::emit_i64x2_alltrue(LiftoffRegister dst,
                                           LiftoffRegister src) {
  BAILOUT("emit_i64x2_alltrue");
}

#undef BAILOUT
#undef BAILOUT_RETURN

}  // namespace v8::internal::wasm

#endif  // V8_WASM_BASELINE_WASM32_LIFTOFF_ASSEMBLER_WASM32_INL_H_
