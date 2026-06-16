// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_WASM32_BUILTIN_MODULE_WRITER_H_
#define V8_CODEGEN_WASM32_WASM32_BUILTIN_MODULE_WRITER_H_

#if V8_TARGET_ARCH_WASM32

namespace v8 {
namespace internal {
namespace wasm32 {

bool WriteGeneratedBuiltinModule(const char* object_path,
                                 const char* manifest_path,
                                 const char* registry_source_path);

}  // namespace wasm32
}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32

#endif  // V8_CODEGEN_WASM32_WASM32_BUILTIN_MODULE_WRITER_H_
