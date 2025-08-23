// Copyright 2024 the V8 project authors. All rights reserved.
// WASI/WebAssembly backend for V8 code generation

#ifndef V8_CODEGEN_WASM32_ASSEMBLER_WASM32_H_
#define V8_CODEGEN_WASM32_ASSEMBLER_WASM32_H_

#include "src/codegen/assembler.h"
#include "src/codegen/label.h"
#include "src/codegen/wasm32/register-wasm32.h"

namespace v8 {
namespace internal {

class V8_EXPORT_PRIVATE AssemblerWASM32 : public AssemblerBase {
 public:
  explicit AssemblerWASM32(const AssemblerOptions& options,
                          std::unique_ptr<AssemblerBuffer> buffer = {})
      : AssemblerBase(options, std::move(buffer)) {
    InitializeWASMBackend();
  }

  // WASI doesn't support traditional assembly - emit WebAssembly bytecode
  void InitializeWASMBackend() {
    // Initialize WebAssembly module structure
    EmitWASMHeader();
  }

  // WebAssembly instruction emitters
  void local_get(uint32_t index) { EmitByte(0x20); EmitLEB128(index); }
  void local_set(uint32_t index) { EmitByte(0x21); EmitLEB128(index); }
  void local_tee(uint32_t index) { EmitByte(0x22); EmitLEB128(index); }
  
  void i32_const(int32_t value) { EmitByte(0x41); EmitSLEB128(value); }
  void i64_const(int64_t value) { EmitByte(0x42); EmitSLEB128(value); }
  void f32_const(float value) { EmitByte(0x43); EmitFloat32(value); }
  void f64_const(double value) { EmitByte(0x44); EmitFloat64(value); }

  // Memory operations
  void i32_load(uint32_t align, uint32_t offset) {
    EmitByte(0x28);
    EmitLEB128(align);
    EmitLEB128(offset);
  }
  
  void i32_store(uint32_t align, uint32_t offset) {
    EmitByte(0x36);
    EmitLEB128(align);
    EmitLEB128(offset);
  }

  // Arithmetic operations
  void i32_add() { EmitByte(0x6A); }
  void i32_sub() { EmitByte(0x6B); }
  void i32_mul() { EmitByte(0x6C); }
  void i32_div_s() { EmitByte(0x6D); }
  void i32_div_u() { EmitByte(0x6E); }

  // Control flow
  void call(uint32_t function_index) {
    EmitByte(0x10);
    EmitLEB128(function_index);
  }
  
  void call_indirect(uint32_t type_index) {
    EmitByte(0x11);
    EmitLEB128(type_index);
    EmitByte(0x00); // table index
  }

  void wasm_return() { EmitByte(0x0F); }
  void unreachable() { EmitByte(0x00); }
  void nop() { EmitByte(0x01); }

  // Block structure
  void block(uint8_t block_type) {
    EmitByte(0x02);
    EmitByte(block_type);
  }
  
  void loop(uint8_t block_type) {
    EmitByte(0x03);
    EmitByte(block_type);
  }
  
  void if_(uint8_t block_type) {
    EmitByte(0x04);
    EmitByte(block_type);
  }
  
  void else_() { EmitByte(0x05); }
  void end() { EmitByte(0x0B); }

  // Branch instructions
  void br(uint32_t depth) {
    EmitByte(0x0C);
    EmitLEB128(depth);
  }
  
  void br_if(uint32_t depth) {
    EmitByte(0x0D);
    EmitLEB128(depth);
  }

  // Comparison operations
  void i32_eq() { EmitByte(0x46); }
  void i32_ne() { EmitByte(0x47); }
  void i32_lt_s() { EmitByte(0x48); }
  void i32_lt_u() { EmitByte(0x49); }
  void i32_gt_s() { EmitByte(0x4A); }
  void i32_gt_u() { EmitByte(0x4B); }

  // Stack manipulation
  void drop() { EmitByte(0x1A); }
  void select() { EmitByte(0x1B); }

  // Label binding for WebAssembly blocks
  void bind(Label* label) {
    DCHECK(!label->is_bound());
    label->bind_to(pc_offset());
  }

  // Required overrides from AssemblerBase
  void AbortedCodeGeneration() override {}
  int InstructionSize() const { return 1; }  // WebAssembly uses variable-length encoding

 private:
  void EmitWASMHeader() {
    // WebAssembly magic number and version
    EmitByte(0x00); EmitByte(0x61); EmitByte(0x73); EmitByte(0x6D);  // \0asm
    EmitByte(0x01); EmitByte(0x00); EmitByte(0x00); EmitByte(0x00);  // version 1
  }

  void EmitByte(uint8_t byte) {
    buffer_->WriteByte(byte);
  }

  void EmitLEB128(uint32_t value) {
    do {
      uint8_t byte = value & 0x7F;
      value >>= 7;
      if (value != 0) byte |= 0x80;
      EmitByte(byte);
    } while (value != 0);
  }

  void EmitSLEB128(int32_t value) {
    bool more = true;
    while (more) {
      uint8_t byte = value & 0x7F;
      value >>= 7;
      if ((value == 0 && (byte & 0x40) == 0) ||
          (value == -1 && (byte & 0x40) != 0)) {
        more = false;
      } else {
        byte |= 0x80;
      }
      EmitByte(byte);
    }
  }

  void EmitFloat32(float value) {
    union { float f; uint32_t i; } u = {value};
    EmitByte(u.i & 0xFF);
    EmitByte((u.i >> 8) & 0xFF);
    EmitByte((u.i >> 16) & 0xFF);
    EmitByte((u.i >> 24) & 0xFF);
  }

  void EmitFloat64(double value) {
    union { double d; uint64_t i; } u = {value};
    for (int i = 0; i < 8; i++) {
      EmitByte((u.i >> (i * 8)) & 0xFF);
    }
  }
};

using Assembler = AssemblerWASM32;

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_ASSEMBLER_WASM32_H_