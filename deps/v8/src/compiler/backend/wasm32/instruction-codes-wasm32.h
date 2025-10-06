// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_BACKEND_WASM32_INSTRUCTION_CODES_WASM32_H_
#define V8_COMPILER_BACKEND_WASM32_INSTRUCTION_CODES_WASM32_H_

namespace v8 {
namespace internal {
namespace compiler {

// WASM32-specific opcodes that specify which assembly sequence to emit.
// WASM32 uses a minimal set since it compiles to WebAssembly, not native code.

#define TARGET_ARCH_OPCODE_LIST(V) \
  V(Wasm32Add)                     \
  V(Wasm32Sub)                     \
  V(Wasm32Mul)                     \
  V(Wasm32And)                     \
  V(Wasm32Or)                      \
  V(Wasm32Xor)                     \
  V(Wasm32Shl)                     \
  V(Wasm32Shr)                     \
  V(Wasm32Sar)                     \
  V(Wasm32Cmp)                     \
  V(Wasm32Test)                    \
  V(Wasm32Mov)                     \
  V(Wasm32Load)                    \
  V(Wasm32Store)                   \
  V(Wasm32Call)                    \
  V(Wasm32Return)

// Addressing modes
#define TARGET_ADDRESSING_MODE_LIST(V) \
  V(MR)   /* [%r0] */                  \
  V(MRI)  /* [%r0 + K] */              \
  V(M1)   /* [%r1 * 1] */              \
  V(M2)   /* [%r1 * 2] */              \
  V(M4)   /* [%r1 * 4] */              \
  V(M8)   /* [%r1 * 8] */              \
  V(M1I)  /* [%r1 * 1 + K] */          \
  V(M2I)  /* [%r1 * 2 + K] */          \
  V(M4I)  /* [%r1 * 4 + K] */          \
  V(M8I)  /* [%r1 * 8 + K] */          \
  V(MR1)  /* [%r0 + %r1 * 1] */        \
  V(MR2)  /* [%r0 + %r1 * 2] */        \
  V(MR4)  /* [%r0 + %r1 * 4] */        \
  V(MR8)  /* [%r0 + %r1 * 8] */        \
  V(MR1I) /* [%r0 + %r1 * 1 + K] */    \
  V(MR2I) /* [%r0 + %r1 * 2 + K] */    \
  V(MR4I) /* [%r0 + %r1 * 4 + K] */    \
  V(MR8I) /* [%r0 + %r1 * 8 + K] */    \
  V(Root) /* [%r + K] */

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_COMPILER_BACKEND_WASM32_INSTRUCTION_CODES_WASM32_H_
