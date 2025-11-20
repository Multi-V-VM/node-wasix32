#ifndef WASI_WASM32_ARCH_FIXES_H_
#define WASI_WASM32_ARCH_FIXES_H_

// ============================================================================
// WASM32 Architecture-Specific Definitions
// ============================================================================

#ifdef V8_TARGET_ARCH_WASM32

namespace v8 {
namespace base {
  // Address type definition for WASM32
  using Address = uintptr_t;
}

namespace internal {
  // Register definition for WASM32
  class Register {
  public:
    explicit Register(int code) : code_(code) {}
    static Register from_code(int code) { return Register(code); }
    int code() const { return code_; }
  private:
    int code_;
  };

  enum class Builtin : int;
  using Address = base::Address;

// ============================================================================
// Memory Operand Definitions
// ============================================================================

class MemOperand {
public:
  MemOperand() : base_(0), offset_(0) {}
  explicit MemOperand(Register base, int32_t offset = 0) : base_(base.code()), offset_(offset) {}

  Register base() const { return Register::from_code(base_); }
  int32_t offset() const { return offset_; }

private:
  int base_;
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

  Operand() : tag_(IMMEDIATE), imm_(0), mem_code_(0), mem_offset_(0) {}
  explicit Operand(int32_t imm) : tag_(IMMEDIATE), imm_(imm), mem_code_(0), mem_offset_(0) {}
  explicit Operand(Register reg) : tag_(REGISTER), imm_(reg.code()), mem_code_(0), mem_offset_(0) {}
  explicit Operand(const MemOperand& mem) : tag_(MEMORY), imm_(0), mem_code_(mem.base().code()), mem_offset_(mem.offset()) {}

  Tag tag() const { return tag_; }
  int32_t immediate() const { return imm_; }
  Register reg() const { return Register::from_code(imm_); }
  MemOperand memory() const { return MemOperand(Register::from_code(mem_code_), mem_offset_); }

private:
  Tag tag_;
  int32_t imm_;
  int mem_code_;
  int32_t mem_offset_;
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

// Frame pointer register (static for WASM32)
extern Register fp;

// Stack pointer (usually defined elsewhere, but adding for completeness)
// constexpr Register sp = Register::from_code(13);

} // namespace internal
} // namespace v8

#endif  // V8_TARGET_ARCH_WASM32

#endif  // WASI_WASM32_ARCH_FIXES_H_
