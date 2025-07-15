// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_REGLIST_WASM32_H_
#define V8_CODEGEN_WASM32_REGLIST_WASM32_H_

#include "src/codegen/register-arch.h"
#include "src/codegen/reglist-base.h"

namespace v8 {
namespace internal {

using RegList = RegListBase<Register>;
using FloatRegList = RegListBase<FloatRegister>; 
using DoubleRegList = RegListBase<DoubleRegister>;
using Simd128RegList = RegListBase<Simd128Register>;

// Allocatable register lists
constexpr RegList kAllocatableGeneralRegisters = {
    r0(), r1(), r2(), r3(), r4(), r5(), r6(), r7(),
    r8(), r9(), r10(), r11(), r12()};

// Registers saved by the caller
constexpr RegList kCallerSavedRegisters = {
    r0(), r1(), r2(), r3(), r4(), r5(), r6(), r7()};

// Registers saved by the callee  
constexpr RegList kCalleeSavedRegisters = {
    r8(), r9(), r10(), r11(), r13(), r14()};

// Allocatable float registers
constexpr FloatRegList kAllocatableFloatRegisters = {
    f0(), f1(), f2(), f3(), f4(), f5(), f6(), f7(),
    f8(), f9(), f10(), f11(), f12(), f13(), f14(), f15()};

// Allocatable double registers
constexpr DoubleRegList kAllocatableDoubleRegisters = {
    d0(), d1(), d2(), d3(), d4(), d5(), d6(), d7(),
    d8(), d9(), d10(), d11(), d12(), d13(), d14(), d15()};

// Allocatable SIMD128 registers
constexpr Simd128RegList kAllocatableSimd128Registers = {
    s0(), s1(), s2(), s3(), s4(), s5(), s6(), s7(),
    s8(), s9(), s10(), s11(), s12(), s13(), s14(), s15()};

// Caller saved float registers
constexpr FloatRegList kCallerSavedFloatRegisters = {
    f0(), f1(), f2(), f3(), f4(), f5(), f6(), f7()};

// Caller saved double registers
constexpr DoubleRegList kCallerSavedDoubleRegisters = {
    d0(), d1(), d2(), d3(), d4(), d5(), d6(), d7()};

// Caller saved SIMD128 registers
constexpr Simd128RegList kCallerSavedSimd128Registers = {
    s0(), s1(), s2(), s3(), s4(), s5(), s6(), s7()};

// Architecture-specific helpers
inline constexpr int NumRegs(RegList list) { return list.Count(); }
inline constexpr int NumRegs(FloatRegList list) { return list.Count(); }
inline constexpr int NumRegs(DoubleRegList list) { return list.Count(); }
inline constexpr int NumRegs(Simd128RegList list) { return list.Count(); }

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_REGLIST_WASM32_H_