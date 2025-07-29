// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_REGLIST_WASM32_H_
#define V8_CODEGEN_WASM32_REGLIST_WASM32_H_

#ifdef __wasi__
#include "wasi-v8-reglist-fix.h"
#endif

namespace v8 {
namespace internal {

// WebAssembly doesn't have traditional registers like native architectures
// These are stub definitions for compilation compatibility

// Define stub Register class
class Register {
 public:
  static constexpr int kNumRegisters = 32;  // Arbitrary value for WASM
  
  Register() : code_(0), valid_(false) {}
  explicit Register(int code) : code_(code), valid_(true) {}
  
  static Register no_reg() { return Register(); }
  static Register from_code(int code) { return Register(code); }
  
  bool is_valid() const { return valid_; }
  int code() const { return code_; }
  
 private:
  int code_;
  bool valid_;
};

// Define stub DoubleRegister class  
class DoubleRegister {
 public:
  static constexpr int kNumRegisters = 32;  // Arbitrary value for WASM
  
  DoubleRegister() : code_(0), valid_(false) {}
  explicit DoubleRegister(int code) : code_(code), valid_(true) {}
  
  static DoubleRegister no_reg() { return DoubleRegister(); }
  static DoubleRegister from_code(int code) { return DoubleRegister(code); }
  
  bool is_valid() const { return valid_; }
  int code() const { return code_; }
  
 private:
  int code_;
  bool valid_;
};

// Define RegList and DoubleRegList using RegListBase
using RegList = RegListBase<Register>;
using DoubleRegList = RegListBase<DoubleRegister>;

// Define empty macros for register allocation
#define ALLOCATABLE_GENERAL_REGISTERS(V) 
#define ALLOCATABLE_DOUBLE_REGISTERS(V)

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_REGLIST_WASM32_H_