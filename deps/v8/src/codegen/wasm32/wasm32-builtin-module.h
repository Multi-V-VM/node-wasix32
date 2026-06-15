// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_WASM32_BUILTIN_MODULE_H_
#define V8_CODEGEN_WASM32_WASM32_BUILTIN_MODULE_H_

#include <cstdint>
#include <string>
#include <vector>

#include "src/builtins/builtins.h"

namespace v8 {
namespace internal {
namespace wasm32 {

struct GeneratedBuiltinBody {
  Builtin builtin;
  std::string symbol_name;
  std::vector<uint8_t> body;
};

class GeneratedBuiltinModule {
 public:
  static GeneratedBuiltinModule& Get();

  void AddBuiltin(Builtin builtin, const std::string& symbol_name,
                  std::vector<uint8_t> body);
  bool WriteObjectAndManifest(const char* object_path,
                              const char* manifest_path) const;
  const std::vector<GeneratedBuiltinBody>& builtins() const {
    return builtins_;
  }

 private:
  std::vector<GeneratedBuiltinBody> builtins_;
};

}  // namespace wasm32
}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_WASM32_BUILTIN_MODULE_H_
