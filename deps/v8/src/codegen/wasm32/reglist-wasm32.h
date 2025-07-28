// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_REGLIST_WASM32_H_
#define V8_CODEGEN_WASM32_REGLIST_WASM32_H_

namespace v8 {
namespace internal {

// WebAssembly doesn't have traditional registers like native architectures
// These are stub definitions for compilation compatibility

// Define stub Register class
class Register {
 public:
  Register() = default;
  static Register no_reg() { return Register(); }
};

// Define stub DoubleRegister class  
class DoubleRegister {
 public:
  DoubleRegister() = default;
  static DoubleRegister no_reg() { return DoubleRegister(); }
};

// Define RegList and DoubleRegList for WASM32
class RegList {
 public:
  RegList() = default;
  template<typename... Args>
  RegList(Args...) {}
  
  bool is_empty() const { return true; }
  int Count() const { return 0; }
};

class DoubleRegList {
 public:
  DoubleRegList() = default;
  template<typename... Args>
  DoubleRegList(Args...) {}
  
  bool is_empty() const { return true; }
  int Count() const { return 0; }
};

// Define empty macros for register allocation
#define ALLOCATABLE_GENERAL_REGISTERS(V) 
#define ALLOCATABLE_DOUBLE_REGISTERS(V)

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_REGLIST_WASM32_H_