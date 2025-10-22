// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_INTERFACE_DESCRIPTORS_WASM32_INL_H_
#define V8_CODEGEN_WASM32_INTERFACE_DESCRIPTORS_WASM32_INL_H_

#if V8_TARGET_ARCH_WASM32

#include "src/codegen/interface-descriptors.h"

namespace v8 {
namespace internal {

// Provide wasm32 defaults for descriptor register arrays. Concrete descriptors
// use these defaults unless overridden elsewhere.
constexpr auto CallInterfaceDescriptor::DefaultRegisterArray() {
  return RegisterArray(Register::r0(), Register::r1(), Register::r2(),
                       Register::r3(), Register::r4());
}

constexpr auto CallInterfaceDescriptor::DefaultDoubleRegisterArray() {
  return DoubleRegisterArray(DoubleRegister::d0(), DoubleRegister::d1(),
                             DoubleRegister::d2(), DoubleRegister::d3(),
                             DoubleRegister::d4());
}

constexpr auto CallInterfaceDescriptor::DefaultReturnRegisterArray() {
  return RegisterArray(Register::r0(), Register::r1());
}

constexpr auto CallInterfaceDescriptor::DefaultReturnDoubleRegisterArray() {
  return DoubleRegisterArray(DoubleRegister::d0(), DoubleRegister::d1());
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32

#endif  // V8_CODEGEN_WASM32_INTERFACE_DESCRIPTORS_WASM32_INL_H_

