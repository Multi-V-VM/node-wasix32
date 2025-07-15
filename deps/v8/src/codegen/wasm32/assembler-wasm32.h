// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_ASSEMBLER_WASM32_H_
#define V8_CODEGEN_WASM32_ASSEMBLER_WASM32_H_

#include <memory>

#include "src/base/small-vector.h"
#include "src/codegen/assembler.h"
#include "src/codegen/constant-pool.h"
#include "src/codegen/wasm32/constants-wasm32.h"
#include "src/codegen/wasm32/register-wasm32.h"

namespace v8 {
namespace internal {

class SafepointTableBuilder;

// Forward declarations
class AssemblerWasm32;

// CPU features for WASM32
enum Wasm32Feature {
  WASM32_BASELINE = 0,    // Basic WASM32 support
  WASM32_SIMD = 1,        // SIMD128 support
  WASM32_ATOMICS = 2,     // Atomic operations
  WASM32_EXCEPTION = 3,   // Exception handling
  WASM32_FEATURE_COUNT = 4
};

class V8_EXPORT_PRIVATE AssemblerWasm32 : public AssemblerBase {
 public:
  // Creation, destruction
  explicit AssemblerWasm32(const AssemblerOptions& options,
                          std::unique_ptr<AssemblerBuffer> buffer = {});
  ~AssemblerWasm32() override;

  // GetCode emits any pending (non-emitted) code and fills the descriptor desc.
  static constexpr int kNoHandlerTable = 0;
  static constexpr SafepointTableBuilder* kNoSafepointTable = nullptr;
  void GetCode(LocalIsolate* isolate, CodeDesc* desc,
               SafepointTableBuilder* safepoint_table_builder = kNoSafepointTable,
               int handler_table_offset = kNoHandlerTable);

  // Unused on this architecture.
  void MaybeEmitOutOfLineConstantPool() {}

  // Label operations
  void bind(Label* L);
  void nop();

  // Check if we have sufficient space in the buffer
  void CheckBufferSpace();
  void GrowBuffer();

  // Get current position in code buffer
  int pc_offset() const { return static_cast<int>(pc_ - buffer_start_); }
  uint8_t* pc() const { return pc_; }

  // Instruction emission
  void Emit(uint32_t instr);
  void EmitBranch(Label* L, Condition cond, Register rs1, Register rs2);
  void EmitJump(Label* L);

  // === Architecture-specific instructions ===
  
  // Load/Store operations
  void lw(Register rd, Register base, int32_t offset);   // Load word
  void lh(Register rd, Register base, int32_t offset);   // Load halfword
  void lb(Register rd, Register base, int32_t offset);   // Load byte
  void sw(Register rs, Register base, int32_t offset);   // Store word
  void sh(Register rs, Register base, int32_t offset);   // Store halfword
  void sb(Register rs, Register base, int32_t offset);   // Store byte

  // Load/Store float operations
  void flw(FloatRegister fd, Register base, int32_t offset);   // Load float
  void fsw(FloatRegister fs, Register base, int32_t offset);   // Store float
  void fld(DoubleRegister fd, Register base, int32_t offset);  // Load double
  void fsd(DoubleRegister fs, Register base, int32_t offset);  // Store double

  // SIMD Load/Store
  void vld(Simd128Register vd, Register base, int32_t offset);  // Load SIMD128
  void vst(Simd128Register vs, Register base, int32_t offset);  // Store SIMD128

  // Arithmetic operations
  void add(Register rd, Register rs1, Register rs2);
  void sub(Register rd, Register rs1, Register rs2);
  void mul(Register rd, Register rs1, Register rs2);
  void div(Register rd, Register rs1, Register rs2);
  void rem(Register rd, Register rs1, Register rs2);

  // Immediate arithmetic
  void addi(Register rd, Register rs1, int32_t imm);
  void subi(Register rd, Register rs1, int32_t imm) {
    addi(rd, rs1, -imm);
  }

  // Logical operations
  void and_(Register rd, Register rs1, Register rs2);
  void or_(Register rd, Register rs1, Register rs2);
  void xor_(Register rd, Register rs1, Register rs2);
  void not_(Register rd, Register rs) {
    xor_(rd, rs, Register::from_code(-1));
  }

  // Shift operations
  void sll(Register rd, Register rs1, Register rs2);  // Shift left logical
  void srl(Register rd, Register rs1, Register rs2);  // Shift right logical
  void sra(Register rd, Register rs1, Register rs2);  // Shift right arithmetic

  // Compare operations
  void slt(Register rd, Register rs1, Register rs2);   // Set less than
  void sltu(Register rd, Register rs1, Register rs2);  // Set less than unsigned

  // Branch operations
  void beq(Register rs1, Register rs2, Label* L) {
    EmitBranch(L, eq, rs1, rs2);
  }
  void bne(Register rs1, Register rs2, Label* L) {
    EmitBranch(L, ne, rs1, rs2);
  }
  void blt(Register rs1, Register rs2, Label* L) {
    EmitBranch(L, lt, rs1, rs2);
  }
  void bge(Register rs1, Register rs2, Label* L) {
    EmitBranch(L, ge, rs1, rs2);
  }
  void bltu(Register rs1, Register rs2, Label* L) {
    EmitBranch(L, lo, rs1, rs2);
  }
  void bgeu(Register rs1, Register rs2, Label* L) {
    EmitBranch(L, hs, rs1, rs2);
  }

  // Jump operations
  void j(Label* L) { EmitJump(L); }
  void jal(Register rd, Label* L);  // Jump and link
  void jr(Register rs);              // Jump register
  void jalr(Register rd, Register rs, int32_t offset = 0); // Jump and link register

  // Move operations
  void mov(Register rd, Register rs);
  void li(Register rd, int32_t imm);  // Load immediate

  // Stack operations
  void push(Register rs);
  void pop(Register rd);
  void pushm(RegList regs);  // Push multiple
  void popm(RegList regs);   // Pop multiple

  // Floating point operations
  void fadd_s(FloatRegister fd, FloatRegister fs1, FloatRegister fs2);
  void fsub_s(FloatRegister fd, FloatRegister fs1, FloatRegister fs2);
  void fmul_s(FloatRegister fd, FloatRegister fs1, FloatRegister fs2);
  void fdiv_s(FloatRegister fd, FloatRegister fs1, FloatRegister fs2);

  void fadd_d(DoubleRegister fd, DoubleRegister fs1, DoubleRegister fs2);
  void fsub_d(DoubleRegister fd, DoubleRegister fs1, DoubleRegister fs2);
  void fmul_d(DoubleRegister fd, DoubleRegister fs1, DoubleRegister fs2);
  void fdiv_d(DoubleRegister fd, DoubleRegister fs1, DoubleRegister fs2);

  // Floating point move
  void fmov_s(FloatRegister fd, FloatRegister fs);
  void fmov_d(DoubleRegister fd, DoubleRegister fs);

  // Conversion operations
  void fcvt_w_s(Register rd, FloatRegister fs);    // Float to int32
  void fcvt_s_w(FloatRegister fd, Register rs);    // Int32 to float
  void fcvt_w_d(Register rd, DoubleRegister fs);   // Double to int32
  void fcvt_d_w(DoubleRegister fd, Register rs);   // Int32 to double
  void fcvt_d_s(DoubleRegister fd, FloatRegister fs); // Float to double
  void fcvt_s_d(FloatRegister fd, DoubleRegister fs); // Double to float

  // System operations
  void brk(int code);  // Breakpoint
  void fence();        // Memory fence

  // Constant pool
  bool use_real_constant_pool() const { return false; }
  Handle<HeapObject> compressed_embedded_object_handle_for(
      Address maybe_address) {
    UNREACHABLE();
  }

  // Code patching
  static void PatchBranchOffset(uint8_t* pc, int32_t offset);

  // Debugging
  void RecordComment(const char* comment);
  void RecordDeoptReason(DeoptimizeReason reason, uint32_t node_id,
                        SourcePosition position, int id);

  // Constants for code generation
  static constexpr int kMaxDistToBranchImm = (1 << 12) - 1;  // 12-bit signed offset
  static constexpr int kMaxDistToJumpImm = (1 << 20) - 1;    // 20-bit signed offset

  // Stack pointer operations (for MacroAssembler)
  static constexpr Register kStackPointer = sp();

  // Required by Assembler base
  void DataAlign(int m) {
    DCHECK(m >= 2 && base::bits::IsPowerOfTwo(m));
    while ((pc_offset() & (m - 1)) != 0) {
      nop();
    }
  }

  // Required alignment for different purposes
  static constexpr int kInstrAlignment = 4;
  static constexpr int kFunctionAlignment = 16;

 protected:
  // Relocation information
  void RecordRelocInfo(RelocInfo::Mode rmode, intptr_t data = 0);

  // Label operations
  void print(const Label* L);
  int32_t branch_offset(Label* L);
  int32_t jump_offset(Label* L);

  // Buffer management
  void CheckSpace(int space_needed) {
    if (pc_ + space_needed > buffer_end_) {
      GrowBuffer();
    }
  }

 private:
  // Instruction encoding helpers
  uint32_t EncodeLoadStore(Opcode op, Register rd_rs, Register base, int32_t offset);
  uint32_t EncodeLoadStoreFloat(Opcode op, FloatRegister fd_fs, Register base, int32_t offset);
  uint32_t EncodeLoadStoreDouble(Opcode op, DoubleRegister fd_fs, Register base, int32_t offset);
  uint32_t EncodeLoadStoreSimd(Opcode op, Simd128Register vd_vs, Register base, int32_t offset);

  // Code buffer management
  uint8_t* buffer_start_;
  uint8_t* buffer_end_;
  uint8_t* pc_;

  // Constant pool (unused for WASM32, but required by base)
  int constant_pool_offset_ = 0;

  // Pending branch/jump fixups
  struct BranchInfo {
    uint8_t* pc;
    Label* label;
    bool is_jump;  // true for jumps, false for branches
  };
  base::SmallVector<BranchInfo, 32> pending_branches_;

  // No-op constants for unused features
  static constexpr int kNoConstantPool = 0;
};

// Helper class for EnsureSpace
class V8_EXPORT_PRIVATE EnsureSpace {
 public:
  explicit EnsureSpace(AssemblerWasm32* assembler) : assembler_(assembler) {
    assembler_->CheckBufferSpace();
  }

 private:
  AssemblerWasm32* const assembler_;
};

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_ASSEMBLER_WASM32_H_