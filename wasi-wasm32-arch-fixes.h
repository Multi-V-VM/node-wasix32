#ifndef WASI_WASM32_ARCH_FIXES_H_
#define WASI_WASM32_ARCH_FIXES_H_

// ============================================================================
// WASM32 Architecture-Specific Definitions
// ============================================================================

#ifdef V8_TARGET_ARCH_WASM32

namespace v8 {
namespace internal {

// ============================================================================
// Memory Operand Definitions
// ============================================================================

class MemOperand {
public:
  MemOperand() : base_(), offset_(0) {}
  explicit MemOperand(Register base, int32_t offset = 0) : base_(base), offset_(offset) {}

  Register base() const { return base_; }
  int32_t offset() const { return offset_; }

private:
  Register base_;
  int32_t offset_;
};

// ============================================================================
// Operand Definitions
// ============================================================================

class Operand {
public:
  enum Tag {
    IMMEDIATE,
    REGISTER,
    MEMORY
  };

  Operand() : tag_(IMMEDIATE), imm_(0) {}
  explicit Operand(int32_t imm) : tag_(IMMEDIATE), imm_(imm) {}
  explicit Operand(Register reg) : tag_(REGISTER), reg_(reg) {}
  explicit Operand(const MemOperand& mem) : tag_(MEMORY), mem_(mem) {}

  Tag tag() const { return tag_; }
  int32_t immediate() const { return imm_; }
  Register reg() const { return reg_; }
  MemOperand memory() const { return mem_; }

private:
  Tag tag_;
  union {
    int32_t imm_;
    Register reg_;
    MemOperand mem_;
  };
};

// ============================================================================
// Assembler Additions for WASM32
// ============================================================================

class Assembler {
public:
  // Missing dd method for data directive
  void dd(uint32_t data) {
    // Emit 4 bytes of data
  }

  // Exception handler
  void ExceptionHandler() {}

  // Trap instruction
  void Trap() {}

  // Debug break
  void DebugBreak() {}

  // Call builtin
  void CallBuiltin(Builtin builtin) {}

  // Tail call builtin
  void TailCallBuiltin(Builtin builtin) {}
};

// ============================================================================
// MacroAssembler Additions for WASM32
// ============================================================================

class MacroAssembler {
public:
  // Missing methods
  void ExceptionHandler() {}
  void Trap() {}
  void DebugBreak() {}
  void CallBuiltin(Builtin builtin) {}
  void TailCallBuiltin(Builtin builtin) {}
};

// ============================================================================
// Jump Table Assembler
// ============================================================================

namespace wasm {

class JumpTableAssembler {
public:
  // Stub for unknown architecture error
  static void EmitLazyCompileJumpSlot(Address target) {
    // For WASM32, we need a basic implementation
    // This is a placeholder
  }

  static void EmitJumpSlot(Address target) {
    // Placeholder implementation
  }
};

} // namespace wasm

// ============================================================================
// Register Definitions for WASM32
// ============================================================================

// Frame pointer
constexpr Register fp = Register::from_code(10);

// Stack pointer (usually defined elsewhere, but adding for completeness)
// constexpr Register sp = Register::from_code(13);

} // namespace internal
} // namespace v8

#endif  // V8_TARGET_ARCH_WASM32

#endif  // WASI_WASM32_ARCH_FIXES_H_
