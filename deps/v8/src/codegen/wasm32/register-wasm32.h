// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_REGISTER_WASM32_H_
#define V8_CODEGEN_WASM32_REGISTER_WASM32_H_

// Define that we have proper Register and DoubleRegister class definitions
#define V8_WASM32_REGISTER_DEFINED
#define V8_WASM32_DOUBLEREGISTER_DEFINED

#include "src/codegen/register-base.h"
#include "src/common/globals.h"

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

// Alias for register-configuration.cc compatibility
#define ALLOCATABLE_GENERAL_REGISTERS ALLOCATABLE_GENERAL_REGISTER_LIST

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

// Double precision registers share the same physical registers as floats but
// expose distinct names d0..d15 for clarity in call descriptors
#define DOUBLE_REGISTER_LIST(V) \
  V(d0)  V(d1)  V(d2)  V(d3)  \
  V(d4)  V(d5)  V(d6)  V(d7)  \
  V(d8)  V(d9)  V(d10) V(d11) \
  V(d12) V(d13) V(d14) V(d15)

// Alias for register-configuration.cc compatibility
#define ALLOCATABLE_DOUBLE_REGISTERS DOUBLE_REGISTER_LIST

// SIMD registers (128-bit)
#define SIMD128_REGISTER_LIST(V) \
  V(s0)  V(s1)  V(s2)  V(s3)  \
  V(s4)  V(s5)  V(s6)  V(s7)  \
  V(s8)  V(s9)  V(s10) V(s11) \
  V(s12) V(s13) V(s14) V(s15)

// Combined register list for RegisterName function
#define ALL_WASM32_REGISTERS(V) \
  GENERAL_REGISTER_LIST(V) \
  SPECIAL_REGISTER_LIST(V)

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

  // Constexpr constructors
  constexpr Register() : RegisterBase(-1) {}

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

// Provide double-name aliases for float register codes for compatibility.
static constexpr int kFloatCode_d0 = kFloatCode_f0;
static constexpr int kFloatCode_d1 = kFloatCode_f1;
static constexpr int kFloatCode_d2 = kFloatCode_f2;
static constexpr int kFloatCode_d3 = kFloatCode_f3;
static constexpr int kFloatCode_d4 = kFloatCode_f4;
static constexpr int kFloatCode_d5 = kFloatCode_f5;
static constexpr int kFloatCode_d6 = kFloatCode_f6;
static constexpr int kFloatCode_d7 = kFloatCode_f7;
static constexpr int kFloatCode_d8 = kFloatCode_f8;
static constexpr int kFloatCode_d9 = kFloatCode_f9;
static constexpr int kFloatCode_d10 = kFloatCode_f10;
static constexpr int kFloatCode_d11 = kFloatCode_f11;
static constexpr int kFloatCode_d12 = kFloatCode_f12;
static constexpr int kFloatCode_d13 = kFloatCode_f13;
static constexpr int kFloatCode_d14 = kFloatCode_f14;
static constexpr int kFloatCode_d15 = kFloatCode_f15;

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

  // Constexpr constructors
  constexpr DoubleRegister() : RegisterBase(-1) {}

  // Map double register names d0..d15 to the same underlying codes as
  // float registers f0..f15, since they are the same physical registers.
  static constexpr DoubleRegister d0()  { return DoubleRegister(kFloatCode_f0); }
  static constexpr DoubleRegister d1()  { return DoubleRegister(kFloatCode_f1); }
  static constexpr DoubleRegister d2()  { return DoubleRegister(kFloatCode_f2); }
  static constexpr DoubleRegister d3()  { return DoubleRegister(kFloatCode_f3); }
  static constexpr DoubleRegister d4()  { return DoubleRegister(kFloatCode_f4); }
  static constexpr DoubleRegister d5()  { return DoubleRegister(kFloatCode_f5); }
  static constexpr DoubleRegister d6()  { return DoubleRegister(kFloatCode_f6); }
  static constexpr DoubleRegister d7()  { return DoubleRegister(kFloatCode_f7); }
  static constexpr DoubleRegister d8()  { return DoubleRegister(kFloatCode_f8); }
  static constexpr DoubleRegister d9()  { return DoubleRegister(kFloatCode_f9); }
  static constexpr DoubleRegister d10() { return DoubleRegister(kFloatCode_f10); }
  static constexpr DoubleRegister d11() { return DoubleRegister(kFloatCode_f11); }
  static constexpr DoubleRegister d12() { return DoubleRegister(kFloatCode_f12); }
  static constexpr DoubleRegister d13() { return DoubleRegister(kFloatCode_f13); }
  static constexpr DoubleRegister d14() { return DoubleRegister(kFloatCode_f14); }
  static constexpr DoubleRegister d15() { return DoubleRegister(kFloatCode_f15); }

  static constexpr DoubleRegister no_reg() { return DoubleRegister(kInvalid); }
  static constexpr DoubleRegister from_code(int code) {
    DCHECK(is_valid_code(code));
    return DoubleRegister(code);
  }

  // Compatibility aliases: allow referencing fN as DoubleRegister names.
  static constexpr DoubleRegister f0()  { return d0(); }
  static constexpr DoubleRegister f1()  { return d1(); }
  static constexpr DoubleRegister f2()  { return d2(); }
  static constexpr DoubleRegister f3()  { return d3(); }
  static constexpr DoubleRegister f4()  { return d4(); }
  static constexpr DoubleRegister f5()  { return d5(); }
  static constexpr DoubleRegister f6()  { return d6(); }
  static constexpr DoubleRegister f7()  { return d7(); }
  static constexpr DoubleRegister f8()  { return d8(); }
  static constexpr DoubleRegister f9()  { return d9(); }
  static constexpr DoubleRegister f10() { return d10(); }
  static constexpr DoubleRegister f11() { return d11(); }
  static constexpr DoubleRegister f12() { return d12(); }
  static constexpr DoubleRegister f13() { return d13(); }
  static constexpr DoubleRegister f14() { return d14(); }
  static constexpr DoubleRegister f15() { return d15(); }

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

// Required ArgumentPaddingSlots function
constexpr int ArgumentPaddingSlots(int argument_count) {
  return 0;  // No padding needed for WASM
}

// Placeholder definitions for missing types
constexpr bool kPadArguments = false;
constexpr bool kSimpleFPAliasing = true;
constexpr bool kSimdMaskRegisters = false;

// FP aliasing kind for register configuration (uses AliasingKind from globals.h)
// WASM32 uses overlap aliasing where float/double registers overlap
constexpr AliasingKind kFPAliasing = AliasingKind::kOverlap;

// Define special register constants
constexpr Register kRootRegister = Register::r13();  // Root pointer register
constexpr Register kScratchRegister = Register::r12();  // Scratch register
constexpr Register kStackPointerRegister = Register::sp();
constexpr Register kFramePointerRegister = Register::fp();
constexpr Register kLinkRegister = Register::link();
constexpr DoubleRegister kScratchDoubleReg = DoubleRegister::d15();
constexpr Simd128Register kScratchSimd128Reg = Simd128Register::s15();

// Pointer compression cage base register alias.
constexpr Register kPtrComprCageBaseRegister = kRootRegister;
// Interpreter accumulator register alias.
constexpr Register kInterpreterAccumulatorRegister = Register::r0();

// Zero register (always returns 0 when read, writes are ignored)
constexpr Register kZeroRegister = Register::r15();

// JavaScript argument registers
constexpr Register kJSFunctionRegister = Register::r1();
constexpr Register kContextRegister = Register::r2();
constexpr Register kAllocateSizeRegister = Register::r0();

// Return value registers
constexpr Register kReturnRegister0 = Register::r0();
constexpr Register kReturnRegister1 = Register::r1();

// Stream operators for debugging and logging
inline std::ostream& operator<<(std::ostream& os, const Register& r) {
  return os << Register::Mnemonic(r.code());
}
inline std::ostream& operator<<(std::ostream& os, const DoubleRegister& r) {
  return os << DoubleRegister::Mnemonic(r.code());
}
inline std::ostream& operator<<(std::ostream& os, const FloatRegister& r) {
  return os << FloatRegister::Mnemonic(r.code());
}
inline std::ostream& operator<<(std::ostream& os, const Simd128Register& r) {
  return os << Simd128Register::Mnemonic(r.code());
}
constexpr Register kReturnRegister2 = Register::r2();

// Calling convention argument registers
constexpr Register kArgumentRegister0 = Register::r0();
constexpr Register kArgumentRegister1 = Register::r1();
constexpr Register kArgumentRegister2 = Register::r3();
constexpr Register kArgumentRegister3 = Register::r4();

// C calling convention argument registers
constexpr Register kCArgument0 = Register::r0();
constexpr Register kCArgument1 = Register::r1();
constexpr Register kCArgument2 = Register::r3();
constexpr Register kCArgument3 = Register::r4();

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

// JavaScript call convention helpers
constexpr Register kJavaScriptCallArgCountRegister = kArgumentRegister0;
constexpr Register kJavaScriptCallCodeStartRegister = Register::r8();
constexpr Register kJavaScriptCallTargetRegister = kJSFunctionRegister;
constexpr Register kJavaScriptCallNewTargetRegister = Register::r5();
constexpr Register kJavaScriptCallExtraArg1Register = Register::r6();
constexpr Register kJavaScriptCallDispatchHandleRegister = Register::r9();

// Runtime call convention helpers
constexpr Register kRuntimeCallFunctionRegister = Register::r4();
constexpr Register kRuntimeCallArgCountRegister = kArgumentRegister0;
constexpr Register kRuntimeCallArgvRegister = Register::r7();

// WebAssembly specific aliases
constexpr Register kWasmImplicitArgRegister = kContextRegister;
constexpr Register kWasmCompileLazyFuncIndexRegister = Register::r6();
constexpr Register kWasmTrapHandlerFaultAddressRegister = Register::r10();

// Define RegisterName functions inline
inline const char* RegisterName(Register reg) {
  static constexpr const char* Names[] = {
    #define REGISTER_NAME(R) #R,
    GENERAL_REGISTER_LIST(REGISTER_NAME)
    SPECIAL_REGISTER_LIST(REGISTER_NAME)
    #undef REGISTER_NAME
  };
  static_assert(sizeof(Names) / sizeof(Names[0]) == Register::kNumRegisters);
  return reg.is_valid() ? Names[reg.code()] : "invalid";
}

inline const char* RegisterName(FloatRegister reg) {
  static constexpr const char* Names[] = {
    "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
    "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15"
  };
  return reg.is_valid() && reg.code() < 16 ? Names[reg.code()] : "invalid";
}

inline const char* RegisterName(DoubleRegister reg) {
  static constexpr const char* Names[] = {
    "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
    "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15"
  };
  return reg.is_valid() && reg.code() < 16 ? Names[reg.code()] : "invalid";
}

inline const char* RegisterName(Simd128Register reg) {
  static constexpr const char* Names[] = {
    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
    "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15"
  };
  return reg.is_valid() && reg.code() < 16 ? Names[reg.code()] : "invalid";
}

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_REGISTER_WASM32_H_
