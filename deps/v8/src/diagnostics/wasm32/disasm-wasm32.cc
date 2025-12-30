// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Minimal WASM32 disassembler stub - provides symbols for linking but
// actual disassembly is not implemented since WASM32 is a virtual ISA.

#if V8_TARGET_ARCH_WASM32

#include "src/codegen/wasm32/register-wasm32.h"
#include "src/diagnostics/disasm.h"

namespace v8 {
namespace internal {

// Register Mnemonic implementations
const char* Register::AllocationIndexToString(int index) {
  static const char* const names[] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
    "r8", "r9", "r10", "r11", "r12", "sp", "fp", "link"
  };
  if (index >= 0 && index < static_cast<int>(sizeof(names) / sizeof(names[0]))) {
    return names[index];
  }
  return "invalid";
}

const char* Register::Mnemonic(int code) {
  return AllocationIndexToString(code);
}

const char* FloatRegister::AllocationIndexToString(int index) {
  static const char* const names[] = {
    "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
    "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15"
  };
  if (index >= 0 && index < static_cast<int>(sizeof(names) / sizeof(names[0]))) {
    return names[index];
  }
  return "invalid";
}

const char* FloatRegister::Mnemonic(int code) {
  return AllocationIndexToString(code);
}

const char* DoubleRegister::AllocationIndexToString(int index) {
  static const char* const names[] = {
    "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
    "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15"
  };
  if (index >= 0 && index < static_cast<int>(sizeof(names) / sizeof(names[0]))) {
    return names[index];
  }
  return "invalid";
}

const char* DoubleRegister::Mnemonic(int code) {
  return AllocationIndexToString(code);
}

const char* Simd128Register::AllocationIndexToString(int index) {
  static const char* const names[] = {
    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
    "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15"
  };
  if (index >= 0 && index < static_cast<int>(sizeof(names) / sizeof(names[0]))) {
    return names[index];
  }
  return "invalid";
}

const char* Simd128Register::Mnemonic(int code) {
  return AllocationIndexToString(code);
}

}  // namespace internal
}  // namespace v8

namespace disasm {

// Minimal NameConverter implementation for WASM32
const char* NameConverter::NameOfCPURegister(int reg) const {
  return v8::internal::Register::Mnemonic(reg);
}

const char* NameConverter::NameOfByteCPURegister(int reg) const {
  return NameOfCPURegister(reg);
}

const char* NameConverter::NameOfXMMRegister(int reg) const {
  return v8::internal::DoubleRegister::Mnemonic(reg);
}

const char* NameConverter::NameOfAddress(uint8_t* addr) const {
  return "";
}

const char* NameConverter::NameOfConstant(uint8_t* addr) const {
  return "";
}

const char* NameConverter::NameInCode(uint8_t* addr) const {
  return "";
}

// Disassembler methods implementation for WASM32
int Disassembler::InstructionDecode(ZoneVector<char> buffer,
                                    uint8_t* instruction) {
  // Minimal stub - just return nop
  if (buffer.size() > 3) {
    buffer[0] = 'n';
    buffer[1] = 'o';
    buffer[2] = 'p';
    buffer[3] = '\0';
  }
  return 4;  // Assume 4-byte instructions
}

int Disassembler::ConstantPoolSizeAt(uint8_t* instruction) {
  return -1;  // No constant pool
}

void Disassembler::Disassemble(FILE* f, uint8_t* begin, uint8_t* end,
                               UnimplementedOpcodeAction unimplemented_action) {
  // Stub - no disassembly for WASM32
}

}  // namespace disasm

#endif  // V8_TARGET_ARCH_WASM32
