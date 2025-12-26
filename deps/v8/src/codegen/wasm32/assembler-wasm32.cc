// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/codegen/wasm32/assembler-wasm32.h"

#include "src/base/cpu.h"
#include "src/codegen/assembler-inl.h"
#include "src/codegen/wasm32/assembler-wasm32-inl.h"

namespace v8 {
namespace internal {

// CPU feature detection
static const unsigned wasm32_features = 
    (1u << WASM32_BASELINE) |  // Always available
    (1u << WASM32_SIMD) |      // Assume SIMD support
    (1u << WASM32_ATOMICS);    // Assume atomics support

// Assembler implementation
Assembler::Assembler()
    : AssemblerBase(AssemblerOptions{}, nullptr) {
  buffer_start_ = nullptr;
  buffer_end_ = nullptr;
  pc_ = nullptr;
}

Assembler::Assembler(MaybeAssemblerZone zone, const AssemblerOptions& options,
                     std::unique_ptr<AssemblerBuffer> buffer)
    : AssemblerBase(options, std::move(buffer)) {
  // Zone is ignored for WASM32 but accepted for API compatibility
  (void)zone;
  buffer_start_ = reinterpret_cast<uint8_t*>(buffer_->start());
  buffer_end_ = buffer_start_ + buffer_->size();
  pc_ = buffer_start_;
}

Assembler::Assembler(const AssemblerOptions& options,
                     std::unique_ptr<AssemblerBuffer> buffer)
    : AssemblerBase(options, std::move(buffer)) {
  buffer_start_ = reinterpret_cast<uint8_t*>(buffer_->start());
  buffer_end_ = buffer_start_ + buffer_->size();
  pc_ = buffer_start_;
}

Assembler::~Assembler() = default;

void Assembler::GetCode(LocalIsolate* isolate, CodeDesc* desc,
                             SafepointTableBuilder* safepoint_table_builder,
                             int handler_table_offset) {
  // Finalize any pending branches
  for (const auto& branch : pending_branches_) {
    if (branch.label->is_bound()) {
      int offset = branch.label->pos() - (branch.pc - buffer_start_);
      if (branch.is_jump) {
        PatchBranchOffset(branch.pc, offset);
      } else {
        PatchBranchOffset(branch.pc, offset);
      }
    }
  }

  // Emit constant pool if necessary (WASM32 doesn't use one)
  desc->constant_pool_offset = 0;
  desc->constant_pool_size = 0;

  // Setup code descriptor
  desc->buffer = buffer_->start();
  desc->buffer_size = buffer_->size();
  desc->instr_size = pc_offset();
  desc->reloc_size = 
      static_cast<int>((reinterpret_cast<uint8_t*>(reloc_info_writer.pos()) -
                       reloc_info_writer.buffer()->start()));
  desc->origin = this;
  desc->unwinding_info_size = 0;
  desc->unwinding_info = nullptr;
}

void Assembler::bind(Label* L) {
  DCHECK(!L->is_bound());
  L->bind_to(pc_offset());
}

void Assembler::nop() {
  Emit(EncodeRType(kNop, 0, 0, 0));
}

void Assembler::CheckBufferSpace() {
  if (pc_ + kGap < buffer_end_) return;
  GrowBuffer();
}

void Assembler::GrowBuffer() {
  DCHECK(buffer_overflow());
  
  // Calculate new buffer size
  int old_size = buffer_->size();
  int new_size = old_size + (old_size >> 1);  // Grow by 50%
  
  // Create new buffer
  std::unique_ptr<AssemblerBuffer> new_buffer = buffer_->Grow(new_size);
  
  // Update pointers
  uint8_t* new_start = reinterpret_cast<uint8_t*>(new_buffer->start());
  int pc_offset = static_cast<int>(pc_ - buffer_start_);
  buffer_start_ = new_start;
  buffer_end_ = new_start + new_buffer->size();
  pc_ = new_start + pc_offset;
  
  buffer_ = std::move(new_buffer);
}

void Assembler::Emit(uint32_t instr) {
  CheckSpace(kWasm32InstrSize);
  *reinterpret_cast<uint32_t*>(pc_) = instr;
  pc_ += kWasm32InstrSize;
}

void Assembler::EmitBranch(Label* L, Condition cond, Register rs1, Register rs2) {
  if (L->is_bound()) {
    int32_t offset = L->pos() - pc_offset();
    DCHECK(is_int12(offset));
    Emit(EncodeBType(static_cast<Opcode>(kBranchEq + cond), rs1.code(), rs2.code(), offset));
  } else {
    // Record for later patching
    pending_branches_.push_back({pc_, L, false});
    Emit(EncodeBType(static_cast<Opcode>(kBranchEq + cond), rs1.code(), rs2.code(), 0));
  }
}

void Assembler::EmitJump(Label* L) {
  if (L->is_bound()) {
    int32_t offset = L->pos() - pc_offset();
    DCHECK(is_int20(offset));
    Emit(EncodeIType(kJump, 0, 0, offset));
  } else {
    // Record for later patching
    pending_branches_.push_back({pc_, L, true});
    Emit(EncodeIType(kJump, 0, 0, 0));
  }
}

// Load/Store implementations
void Assembler::lw(Register rd, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStore(kLoad32, rd, base, offset));
}

void Assembler::sw(Register rs, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStore(kStore32, rs, base, offset));
}

void Assembler::lh(Register rd, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStore(kLoad16, rd, base, offset));
}

void Assembler::sh(Register rs, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStore(kStore16, rs, base, offset));
}

void Assembler::lb(Register rd, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStore(kLoad8, rd, base, offset));
}

void Assembler::sb(Register rs, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStore(kStore8, rs, base, offset));
}

// Arithmetic operations
void Assembler::add(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kAdd, rd.code(), rs1.code(), rs2.code()));
}

void Assembler::sub(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kSub, rd.code(), rs1.code(), rs2.code()));
}

void Assembler::mul(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kMul, rd.code(), rs1.code(), rs2.code()));
}

void Assembler::div(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kDiv, rd.code(), rs1.code(), rs2.code()));
}

void Assembler::rem(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kRem, rd.code(), rs1.code(), rs2.code()));
}

void Assembler::addi(Register rd, Register rs1, int32_t imm) {
  DCHECK(is_int12(imm));
  Emit(EncodeIType(kAddi, rd.code(), rs1.code(), imm));
}

// Logical operations
void Assembler::and_(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kAnd, rd.code(), rs1.code(), rs2.code()));
}

void Assembler::or_(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kOr, rd.code(), rs1.code(), rs2.code()));
}

void Assembler::xor_(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kXor, rd.code(), rs1.code(), rs2.code()));
}

// Shift operations
void Assembler::sll(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kShl, rd.code(), rs1.code(), rs2.code()));
}

void Assembler::srl(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kShr, rd.code(), rs1.code(), rs2.code()));
}

void Assembler::sra(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kSar, rd.code(), rs1.code(), rs2.code()));
}

// Compare operations
void Assembler::slt(Register rd, Register rs1, Register rs2) {
  Emit(EncodeRType(kCmpLt, rd.code(), rs1.code(), rs2.code()));
}

void Assembler::sltu(Register rd, Register rs1, Register rs2) {
  // Use a different encoding for unsigned comparison
  Emit(EncodeRType(static_cast<Opcode>(kCmpLt + 8), rd.code(), rs1.code(), rs2.code()));
}

// Jump operations
void Assembler::jal(Register rd, Label* L) {
  if (L->is_bound()) {
    int32_t offset = L->pos() - pc_offset();
    DCHECK(is_int20(offset));
    Emit(EncodeIType(kCall, rd.code(), 0, offset));
  } else {
    pending_branches_.push_back({pc_, L, true});
    Emit(EncodeIType(kCall, rd.code(), 0, 0));
  }
}

void Assembler::jr(Register rs) {
  Emit(EncodeIType(kJumpIndirect, 0, rs.code(), 0));
}

void Assembler::jalr(Register rd, Register rs, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeIType(kCallIndirect, rd.code(), rs.code(), offset));
}

// Move operations
void Assembler::mov(Register rd, Register rs) {
  if (rd != rs) {
    Emit(EncodeRType(kMov, rd.code(), rs.code(), 0));
  }
}

void Assembler::li(Register rd, int32_t imm) {
  if (is_int12(imm)) {
    Emit(EncodeIType(kMovi, rd.code(), 0, imm));
  } else {
    // For larger immediates, we need multiple instructions
    // Load lower 12 bits
    Emit(EncodeIType(kMovi, rd.code(), 0, imm & 0xFFF));
    // Load upper bits
    int32_t upper = imm >> 12;
    if (upper != 0) {
      Emit(EncodeIType(kAddi, rd.code(), rd.code(), upper << 12));
    }
  }
}

// Stack operations
void Assembler::push(Register rs) {
  addi(sp(), sp(), -kPointerSize);
  sw(rs, sp(), 0);
}

void Assembler::pop(Register rd) {
  lw(rd, sp(), 0);
  addi(sp(), sp(), kPointerSize);
}

void Assembler::pushm(RegList regs) {
  int count = regs.Count();
  addi(sp(), sp(), -count * kPointerSize);
  
  int offset = 0;
  for (Register reg : regs) {
    sw(reg, sp(), offset);
    offset += kPointerSize;
  }
}

void Assembler::popm(RegList regs) {
  int offset = 0;
  for (Register reg : regs) {
    lw(reg, sp(), offset);
    offset += kPointerSize;
  }
  
  int count = regs.Count();
  addi(sp(), sp(), count * kPointerSize);
}

// Floating point operations
void Assembler::flw(FloatRegister fd, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStoreFloat(kLoadF32, fd, base, offset));
}

void Assembler::fsw(FloatRegister fs, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStoreFloat(kStoreF32, fs, base, offset));
}

void Assembler::fld(DoubleRegister fd, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStoreDouble(kLoadF64, fd, base, offset));
}

void Assembler::fsd(DoubleRegister fs, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStoreDouble(kStoreF64, fs, base, offset));
}

void Assembler::fadd_s(FloatRegister fd, FloatRegister fs1, FloatRegister fs2) {
  Emit(EncodeRType(kAddF32, fd.code(), fs1.code(), fs2.code()));
}

void Assembler::fsub_s(FloatRegister fd, FloatRegister fs1, FloatRegister fs2) {
  Emit(EncodeRType(kSubF32, fd.code(), fs1.code(), fs2.code()));
}

void Assembler::fmul_s(FloatRegister fd, FloatRegister fs1, FloatRegister fs2) {
  Emit(EncodeRType(kMulF32, fd.code(), fs1.code(), fs2.code()));
}

void Assembler::fdiv_s(FloatRegister fd, FloatRegister fs1, FloatRegister fs2) {
  Emit(EncodeRType(kDivF32, fd.code(), fs1.code(), fs2.code()));
}

void Assembler::fadd_d(DoubleRegister fd, DoubleRegister fs1, DoubleRegister fs2) {
  Emit(EncodeRType(kAddF64, fd.code(), fs1.code(), fs2.code()));
}

void Assembler::fsub_d(DoubleRegister fd, DoubleRegister fs1, DoubleRegister fs2) {
  Emit(EncodeRType(kSubF64, fd.code(), fs1.code(), fs2.code()));
}

void Assembler::fmul_d(DoubleRegister fd, DoubleRegister fs1, DoubleRegister fs2) {
  Emit(EncodeRType(kMulF64, fd.code(), fs1.code(), fs2.code()));
}

void Assembler::fdiv_d(DoubleRegister fd, DoubleRegister fs1, DoubleRegister fs2) {
  Emit(EncodeRType(kDivF64, fd.code(), fs1.code(), fs2.code()));
}

void Assembler::fmov_s(FloatRegister fd, FloatRegister fs) {
  if (fd != fs) {
    Emit(EncodeRType(kMovF32, fd.code(), fs.code(), 0));
  }
}

void Assembler::fmov_d(DoubleRegister fd, DoubleRegister fs) {
  if (fd != fs) {
    Emit(EncodeRType(kMovF64, fd.code(), fs.code(), 0));
  }
}

// SIMD operations
void Assembler::vld(Simd128Register vd, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStoreSimd(kLoadSimd128, vd, base, offset));
}

void Assembler::vst(Simd128Register vs, Register base, int32_t offset) {
  DCHECK(is_int12(offset));
  Emit(EncodeLoadStoreSimd(kStoreSimd128, vs, base, offset));
}

// Conversion operations
void Assembler::fcvt_w_s(Register rd, FloatRegister fs) {
  Emit(EncodeRType(kF32ToI32, rd.code(), fs.code(), 0));
}

void Assembler::fcvt_s_w(FloatRegister fd, Register rs) {
  Emit(EncodeRType(kI32ToF32, fd.code(), rs.code(), 0));
}

void Assembler::fcvt_w_d(Register rd, DoubleRegister fs) {
  Emit(EncodeRType(kF64ToI32, rd.code(), fs.code(), 0));
}

void Assembler::fcvt_d_w(DoubleRegister fd, Register rs) {
  Emit(EncodeRType(kI32ToF64, fd.code(), rs.code(), 0));
}

void Assembler::fcvt_d_s(DoubleRegister fd, FloatRegister fs) {
  Emit(EncodeRType(kF32ToF64, fd.code(), fs.code(), 0));
}

void Assembler::fcvt_s_d(FloatRegister fd, DoubleRegister fs) {
  Emit(EncodeRType(kF64ToF32, fd.code(), fs.code(), 0));
}

// System operations
void Assembler::brk(int code) {
  Emit(EncodeIType(kBreakpoint, 0, 0, code & 0xFFF));
}

void Assembler::fence() {
  Emit(EncodeRType(kFence, 0, 0, 0));
}

// Helper functions
uint32_t Assembler::EncodeLoadStore(Opcode op, Register rd_rs, 
                                         Register base, int32_t offset) {
  return EncodeIType(op, rd_rs.code(), base.code(), offset);
}

uint32_t Assembler::EncodeLoadStoreFloat(Opcode op, FloatRegister fd_fs,
                                              Register base, int32_t offset) {
  return EncodeIType(op, fd_fs.code(), base.code(), offset);
}

uint32_t Assembler::EncodeLoadStoreDouble(Opcode op, DoubleRegister fd_fs,
                                               Register base, int32_t offset) {
  return EncodeIType(op, fd_fs.code(), base.code(), offset);
}

uint32_t Assembler::EncodeLoadStoreSimd(Opcode op, Simd128Register vd_vs,
                                             Register base, int32_t offset) {
  return EncodeIType(op, vd_vs.code(), base.code(), offset);
}

void Assembler::PatchBranchOffset(uint8_t* pc, int32_t offset) {
  uint32_t* instr_ptr = reinterpret_cast<uint32_t*>(pc);
  uint32_t instr = *instr_ptr;
  
  // Clear old offset
  instr &= ~Instruction::kImmMask;
  
  // Set new offset
  instr |= ((offset & 0xFFF) << Instruction::kImmShift);
  
  *instr_ptr = instr;
}

void Assembler::RecordComment(const char* comment) {
  EnsureSpace ensure_space(this);
  RecordRelocInfo(RelocInfo::COMMENT, reinterpret_cast<intptr_t>(comment));
}

void Assembler::RecordDeoptReason(DeoptimizeReason reason, uint32_t node_id,
                                       SourcePosition position, int id) {
  EnsureSpace ensure_space(this);
  RecordRelocInfo(RelocInfo::DEOPT_SCRIPT_OFFSET, position.ScriptOffset());
  RecordRelocInfo(RelocInfo::DEOPT_INLINING_ID, position.InliningId());
  RecordRelocInfo(RelocInfo::DEOPT_REASON, static_cast<int>(reason));
  RecordRelocInfo(RelocInfo::DEOPT_ID, id);
}

void Assembler::RecordRelocInfo(RelocInfo::Mode rmode, intptr_t data) {
  if (!ShouldRecordRelocInfo(rmode)) return;
  
  RelocInfo rinfo(reinterpret_cast<Address>(pc_), rmode, data, Code());
  reloc_info_writer.Write(&rinfo);
}

int32_t Assembler::branch_offset(Label* L) {
  DCHECK(L->is_bound());
  return L->pos() - pc_offset();
}

int32_t Assembler::jump_offset(Label* L) {
  DCHECK(L->is_bound());
  return L->pos() - pc_offset();
}

}  // namespace internal
}  // namespace v8