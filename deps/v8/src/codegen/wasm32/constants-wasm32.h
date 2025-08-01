// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_CONSTANTS_WASM32_H_
#define V8_CODEGEN_WASM32_CONSTANTS_WASM32_H_

#include <stdint.h>

#include "src/base/macros.h"
#include "src/common/globals.h"

namespace v8 {
namespace internal {

// WASM32 instruction encoding constants
constexpr int kWasm32InstrSize = 4;      // Size of WASM32 instruction in bytes
constexpr int kWasm32LoadStoreOffsetBits = 12;  // Bits for load/store offset
constexpr int kWasm32MaxLoadStoreOffset = (1 << kWasm32LoadStoreOffsetBits) - 1;

// Stack alignment
constexpr int kWasm32StackAlignment = 16;  // Stack must be 16-byte aligned

// Pointer size
constexpr int kWasm32PointerSize = 4;  // 32-bit pointers

// Code range constants
constexpr size_t kMaxPCRelativeCodeRangeInMB = 128;  // Maximum PC-relative range

// Root register bias
constexpr int32_t kRootRegisterBias = 128;  // Bias for root register access

// Instruction encoding helpers
class Instruction {
 public:
  enum InstructionType {
    kLoadStore = 0,
    kArithmetic = 1,
    kBranch = 2,
    kSystem = 3
  };

  static constexpr int kOpcodeShift = 0;
  static constexpr int kOpcodeBits = 8;
  static constexpr int kRdShift = 8;
  static constexpr int kRdBits = 4;
  static constexpr int kRs1Shift = 12;
  static constexpr int kRs1Bits = 4;
  static constexpr int kRs2Shift = 16;
  static constexpr int kRs2Bits = 4;
  static constexpr int kImmShift = 20;
  static constexpr int kImmBits = 12;

  // Opcode masks
  static constexpr uint32_t kOpcodeMask = ((1 << kOpcodeBits) - 1) << kOpcodeShift;
  static constexpr uint32_t kRdMask = ((1 << kRdBits) - 1) << kRdShift;
  static constexpr uint32_t kRs1Mask = ((1 << kRs1Bits) - 1) << kRs1Shift;
  static constexpr uint32_t kRs2Mask = ((1 << kRs2Bits) - 1) << kRs2Shift;
  static constexpr uint32_t kImmMask = ((1 << kImmBits) - 1) << kImmShift;
};

// Opcodes for WASM32 instructions
enum Opcode : uint32_t {
  // Load/Store operations
  kLoad32 = 0x00,
  kLoad16 = 0x01,
  kLoad8 = 0x02,
  kStore32 = 0x03,
  kStore16 = 0x04,
  kStore8 = 0x05,
  kLoadF32 = 0x06,
  kLoadF64 = 0x07,
  kStoreF32 = 0x08,
  kStoreF64 = 0x09,
  kLoadSimd128 = 0x0A,
  kStoreSimd128 = 0x0B,

  // Arithmetic operations
  kAdd = 0x10,
  kSub = 0x11,
  kMul = 0x12,
  kDiv = 0x13,
  kRem = 0x14,
  kAnd = 0x15,
  kOr = 0x16,
  kXor = 0x17,
  kShl = 0x18,
  kShr = 0x19,
  kSar = 0x1A,
  kAddi = 0x1B,  // Add immediate

  // Floating point operations
  kAddF32 = 0x20,
  kSubF32 = 0x21,
  kMulF32 = 0x22,
  kDivF32 = 0x23,
  kAddF64 = 0x24,
  kSubF64 = 0x25,
  kMulF64 = 0x26,
  kDivF64 = 0x27,

  // Branch operations
  kBranch = 0x30,
  kBranchEq = 0x31,
  kBranchNe = 0x32,
  kBranchLt = 0x33,
  kBranchGe = 0x34,
  kBranchLtu = 0x35,
  kBranchGeu = 0x36,
  kCall = 0x37,
  kCallIndirect = 0x38,
  kReturn = 0x39,
  kJump = 0x3A,
  kJumpIndirect = 0x3B,

  // Compare operations
  kCmpEq = 0x40,
  kCmpNe = 0x41,
  kCmpLt = 0x42,
  kCmpLe = 0x43,
  kCmpGt = 0x44,
  kCmpGe = 0x45,

  // System operations
  kNop = 0x50,
  kBreakpoint = 0x51,
  kHalt = 0x52,
  kFence = 0x53,

  // Move operations
  kMov = 0x60,
  kMovi = 0x61,  // Move immediate
  kMovF32 = 0x62,
  kMovF64 = 0x63,

  // Conversion operations
  kI32ToF32 = 0x70,
  kI32ToF64 = 0x71,
  kF32ToI32 = 0x72,
  kF64ToI32 = 0x73,
  kF32ToF64 = 0x74,
  kF64ToF32 = 0x75,

  // SIMD operations
  kSimdAdd = 0x80,
  kSimdSub = 0x81,
  kSimdMul = 0x82,
  kSimdDiv = 0x83,

  // Stack operations
  kPush = 0x90,
  kPop = 0x91,
  kPushMultiple = 0x92,
  kPopMultiple = 0x93,
};

// Condition codes for branches
enum Condition : uint32_t {
  kEqual = 0,
  kNotEqual = 1,
  kLessThan = 2,
  kGreaterThanOrEqual = 3,
  kLessThanUnsigned = 4,
  kGreaterThanOrEqualUnsigned = 5,
  kAlways = 6,
  kNever = 7,

  // Floating point conditions
  kFloatEqual = 8,
  kFloatNotEqual = 9,
  kFloatLessThan = 10,
  kFloatLessThanOrEqual = 11,
  kFloatGreaterThan = 12,
  kFloatGreaterThanOrEqual = 13,
  kFloatUnordered = 14,
  kFloatOrdered = 15,

  // Aliases
  eq = kEqual,
  ne = kNotEqual,
  lt = kLessThan,
  ge = kGreaterThanOrEqual,
  lo = kLessThanUnsigned,
  hs = kGreaterThanOrEqualUnsigned,
  al = kAlways,
  nv = kNever,
};

// Memory barrier types
enum BarrierType {
  kNoBarrier = 0,
  kLoadBarrier = 1,
  kStoreBarrier = 2,
  kFullBarrier = 3,
};

// Trap codes for WASM
enum TrapCode {
  kTrapUnreachable = 0,
  kTrapMemOutOfBounds = 1,
  kTrapDivByZero = 2,
  kTrapDivUnrepresentable = 3,
  kTrapRemByZero = 4,
  kTrapFloatUnrepresentable = 5,
  kTrapFuncInvalid = 6,
  kTrapFuncSigMismatch = 7,
  kTrapTypeError = 8,
  kTrapUnalignedAccess = 9,
  kTrapDataSegmentDropped = 10,
  kTrapElemSegmentDropped = 11,
  kTrapTableOutOfBounds = 12,
  kTrapIndirectCallToNull = 13,
  kTrapRethrowNull = 14,
  kTrapNullDereference = 15,
  kTrapIllegalCast = 16,
  kTrapArrayOutOfBounds = 17,
  kTrapArrayTooLarge = 18,
  kTrapStringOffsetOutOfBounds = 19,
  kTrapCount
};

// Helper functions for instruction encoding
inline constexpr uint32_t EncodeRType(Opcode op, int rd, int rs1, int rs2) {
  return static_cast<uint32_t>(op) |
         (rd << Instruction::kRdShift) |
         (rs1 << Instruction::kRs1Shift) |
         (rs2 << Instruction::kRs2Shift);
}

inline constexpr uint32_t EncodeIType(Opcode op, int rd, int rs1, int32_t imm) {
  return static_cast<uint32_t>(op) |
         (rd << Instruction::kRdShift) |
         (rs1 << Instruction::kRs1Shift) |
         ((imm & 0xFFF) << Instruction::kImmShift);
}

inline constexpr uint32_t EncodeBType(Opcode op, int rs1, int rs2, int32_t offset) {
  // Branch encoding: offset is in units of instructions (4 bytes)
  int32_t imm = offset >> 2;
  return static_cast<uint32_t>(op) |
         (rs1 << Instruction::kRs1Shift) |
         (rs2 << Instruction::kRs2Shift) |
         ((imm & 0xFFF) << Instruction::kImmShift);
}

// Stack frame constants
constexpr int kStackFrameExtraParamSlot = 0;
constexpr int kStackFrameTypeOffset = 1 * kWasm32PointerSize;
constexpr int kStackFrameSPOffset = 0 * kWasm32PointerSize;
constexpr int kStackFrameFPOffset = 1 * kWasm32PointerSize;
constexpr int kStackFrameLROffset = 2 * kWasm32PointerSize;
constexpr int kStackFrameContextOffset = 3 * kWasm32PointerSize;

// Safepoint constants
constexpr int kSafepointTableAlignment = 8;
constexpr int kSafepointTableMagic = 0x5AFE0000;

// Call instruction constants
constexpr int kCallInstructionSize = 2 * kWasm32InstrSize;  // Two instructions for far calls
constexpr int kShortCallInstructionSize = kWasm32InstrSize;  // One instruction for near calls

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_CONSTANTS_WASM32_H_