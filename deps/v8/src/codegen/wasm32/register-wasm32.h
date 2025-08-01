// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_REGISTER_WASM32_H_
#define V8_CODEGEN_WASM32_REGISTER_WASM32_H_

#include "src/codegen/register-base.h"

namespace v8 {
namespace internal {

// Invalid register code constant
static constexpr int kInvalid = -1;

// WASM32 register definitions
// Note: WASM32 is a virtual ISA, so these are virtual registers
// that will be mapped to actual registers by the WASM runtime

// General purpose registers
#define GENERAL_REGISTER_LIST(V) \
  V(r0)  V(r1)  V(r2)  V(r3)  \
  V(r4)  V(r5)  V(r6)  V(r7)  \
  V(r8)  V(r9)  V(r10) V(r11) \
  V(r12) V(r13) V(r14) V(r15)

// Allocatable general purpose registers (excluding special ones)
#define ALLOCATABLE_GENERAL_REGISTER_LIST(V) \
  V(r0)  V(r1)  V(r2)  V(r3)  \
  V(r4)  V(r5)  V(r6)  V(r7)  \
  V(r8)  V(r9)  V(r10) V(r11) \
  V(r12)

// Special purpose registers
#define SPECIAL_REGISTER_LIST(V) \
  V(sp)   /* Stack pointer */    \
  V(fp)   /* Frame pointer */    \
  V(link) /* Link register */

// Floating point registers (32-bit)
#define FLOAT_REGISTER_LIST(V) \
  V(f0)  V(f1)  V(f2)  V(f3)  \
  V(f4)  V(f5)  V(f6)  V(f7)  \
  V(f8)  V(f9)  V(f10) V(f11) \
  V(f12) V(f13) V(f14) V(f15)

// Double precision registers share the same codes as float registers
// since they're the same physical registers in WASM32
#define DOUBLE_REGISTER_LIST(V) \
  V(f0)  V(f1)  V(f2)  V(f3)  \
  V(f4)  V(f5)  V(f6)  V(f7)  \
  V(f8)  V(f9)  V(f10) V(f11) \
  V(f12) V(f13) V(f14) V(f15)

// SIMD registers (128-bit)
#define SIMD128_REGISTER_LIST(V) \
  V(s0)  V(s1)  V(s2)  V(s3)  \
  V(s4)  V(s5)  V(s6)  V(s7)  \
  V(s8)  V(s9)  V(s10) V(s11) \
  V(s12) V(s13) V(s14) V(s15)

// Define register codes
enum RegisterCode {
#define REGISTER_CODE(R) kRegCode_##R,
  GENERAL_REGISTER_LIST(REGISTER_CODE)
  SPECIAL_REGISTER_LIST(REGISTER_CODE)
#undef REGISTER_CODE
  kRegAfterLast
};

class Register : public RegisterBase<Register, kRegAfterLast> {
 public:
  static constexpr int kNumRegisters = kRegAfterLast;
  
#define DEFINE_REGISTER(R) \
  static constexpr Register R() { return Register(kRegCode_##R); }
  GENERAL_REGISTER_LIST(DEFINE_REGISTER)
  SPECIAL_REGISTER_LIST(DEFINE_REGISTER)
#undef DEFINE_REGISTER

  // Aliases for special registers
  static constexpr Register no_reg() { return Register(kInvalid); }
  static constexpr Register from_code(int code) {
    DCHECK(is_valid_code(code));
    return Register(code);
  }

  static const char* AllocationIndexToString(int index);
  static const char* Mnemonic(int code);

 private:
  explicit constexpr Register(int code) : RegisterBase(code) {}
  static constexpr bool is_valid_code(int code) {
    return 0 <= code && code < kNumRegisters;
  }
};

// Define floating point register codes
enum FloatRegisterCode {
#define FLOAT_REGISTER_CODE(R) kFloatCode_##R,
  FLOAT_REGISTER_LIST(FLOAT_REGISTER_CODE)
#undef FLOAT_REGISTER_CODE
  kFloatAfterLast
};

// Float registers - 32-bit
class FloatRegister : public RegisterBase<FloatRegister, kFloatAfterLast> {
 public:
  static constexpr int kNumRegisters = kFloatAfterLast;

#define DEFINE_FLOAT_REGISTER(R) \
  static constexpr FloatRegister R() { return FloatRegister(kFloatCode_##R); }
  FLOAT_REGISTER_LIST(DEFINE_FLOAT_REGISTER)
#undef DEFINE_FLOAT_REGISTER

  static constexpr FloatRegister no_reg() { return FloatRegister(kInvalid); }
  static constexpr FloatRegister from_code(int code) {
    DCHECK(is_valid_code(code));
    return FloatRegister(code);
  }

  static const char* AllocationIndexToString(int index);
  static const char* Mnemonic(int code);

 private:
  explicit constexpr FloatRegister(int code) : RegisterBase(code) {}
  static constexpr bool is_valid_code(int code) {
    return 0 <= code && code < kNumRegisters;
  }
};

// Double registers - 64-bit
class DoubleRegister : public RegisterBase<DoubleRegister, kFloatAfterLast> {
 public:
  static constexpr int kNumRegisters = kFloatAfterLast;

#define DEFINE_DOUBLE_REGISTER(R) \
  static constexpr DoubleRegister R() { \
    return DoubleRegister(kFloatCode_##R); \
  }
  DOUBLE_REGISTER_LIST(DEFINE_DOUBLE_REGISTER)
#undef DEFINE_DOUBLE_REGISTER

  static constexpr DoubleRegister no_reg() { return DoubleRegister(kInvalid); }
  static constexpr DoubleRegister from_code(int code) {
    DCHECK(is_valid_code(code));
    return DoubleRegister(code);
  }

  static const char* AllocationIndexToString(int index);
  static const char* Mnemonic(int code);

  // Conversion to float register (same index)
  FloatRegister to_float() const { return FloatRegister::from_code(code()); }

 private:
  explicit constexpr DoubleRegister(int code) : RegisterBase(code) {}
  static constexpr bool is_valid_code(int code) {
    return 0 <= code && code < kNumRegisters;
  }
};

// Define SIMD128 register codes
enum Simd128RegisterCode {
#define SIMD128_REGISTER_CODE(R) kSimd128Code_##R,
  SIMD128_REGISTER_LIST(SIMD128_REGISTER_CODE)
#undef SIMD128_REGISTER_CODE
  kSimd128AfterLast
};

// SIMD128 registers
class Simd128Register : public RegisterBase<Simd128Register, kSimd128AfterLast> {
 public:
  static constexpr int kNumRegisters = kSimd128AfterLast;

#define DEFINE_SIMD128_REGISTER(R) \
  static constexpr Simd128Register R() { \
    return Simd128Register(kSimd128Code_##R); \
  }
  SIMD128_REGISTER_LIST(DEFINE_SIMD128_REGISTER)
#undef DEFINE_SIMD128_REGISTER

  static constexpr Simd128Register no_reg() { 
    return Simd128Register(kInvalid); 
  }
  static constexpr Simd128Register from_code(int code) {
    DCHECK(is_valid_code(code));
    return Simd128Register(code);
  }

  static const char* AllocationIndexToString(int index);
  static const char* Mnemonic(int code);

 private:
  explicit constexpr Simd128Register(int code) : RegisterBase(code) {}
  static constexpr bool is_valid_code(int code) {
    return 0 <= code && code < kNumRegisters;
  }
};

// Type aliases for V8 compatibility
using FPRegister = DoubleRegister;
using VRegister = Simd128Register;

// Required constants for frame layout
constexpr int kStackFrameExtraParamSlot = 0;
const int kStackFrameSPAdjustment = 0;
const int kStackFrameExtraActualArgSlot = 0;

// Required ArgumentPaddingSlots function
constexpr int ArgumentPaddingSlots(int argument_count) {
  return 0;  // No padding needed for WASM
}

// Placeholder definitions for missing types
constexpr bool kPadArguments = false;
constexpr bool kSimpleFPAliasing = true;
constexpr bool kSimdMaskRegisters = false;

// Define special register constants
constexpr Register kRootRegister = Register::r13();  // Root pointer register
constexpr Register kScratchRegister = Register::r12();  // Scratch register
constexpr Register kStackPointerRegister = Register::sp();
constexpr Register kFramePointerRegister = Register::fp();
constexpr Register kLinkRegister = Register::link();

// Zero register (always returns 0 when read, writes are ignored)
constexpr Register kZeroRegister = Register::r15();  

// JavaScript argument registers
constexpr Register kJSFunctionRegister = Register::r1();
constexpr Register kContextRegister = Register::r2();
constexpr Register kAllocateSizeRegister = Register::r0();

// Return value registers
constexpr Register kReturnRegister0 = Register::r0();
constexpr Register kReturnRegister1 = Register::r1();
constexpr Register kReturnRegister2 = Register::r2();

// Calling convention argument registers
constexpr Register kArgumentRegister0 = Register::r0();
constexpr Register kArgumentRegister1 = Register::r1();
constexpr Register kArgumentRegister2 = Register::r2();
constexpr Register kArgumentRegister3 = Register::r3();

// C calling convention argument registers
constexpr Register kCArgument0 = Register::r0();
constexpr Register kCArgument1 = Register::r1();
constexpr Register kCArgument2 = Register::r2();
constexpr Register kCArgument3 = Register::r3();

// Floating point return and argument registers
constexpr DoubleRegister kFPReturnRegister0 = DoubleRegister::d0();
constexpr DoubleRegister kFPReturnRegister1 = DoubleRegister::d1();
constexpr DoubleRegister kFPArgumentRegister0 = DoubleRegister::d0();
constexpr DoubleRegister kFPArgumentRegister1 = DoubleRegister::d1();
constexpr DoubleRegister kFPArgumentRegister2 = DoubleRegister::d2();
constexpr DoubleRegister kFPArgumentRegister3 = DoubleRegister::d3();

// SIMD return and argument registers
constexpr Simd128Register kSimd128ReturnRegister0 = Simd128Register::s0();
constexpr Simd128Register kSimd128ArgumentRegister0 = Simd128Register::s0();
constexpr Simd128Register kSimd128ArgumentRegister1 = Simd128Register::s1();
constexpr Simd128Register kSimd128ArgumentRegister2 = Simd128Register::s2();
constexpr Simd128Register kSimd128ArgumentRegister3 = Simd128Register::s3();

// Register aliases
constexpr Register no_reg = Register::no_reg();
constexpr FloatRegister no_freg = FloatRegister::no_reg();
constexpr DoubleRegister no_dreg = DoubleRegister::no_reg();
constexpr Simd128Register no_vreg = Simd128Register::no_reg();

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_REGISTER_WASM32_H_