// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/codegen/wasm32/wasm32-builtin-module.h"

#include <fstream>
#include <mutex>
#include <utility>

#include "src/codegen/wasm32/wasm32-encoder.h"

namespace v8 {
namespace internal {
namespace wasm32 {
namespace {

std::mutex& ModuleMutex() {
  static std::mutex mutex;
  return mutex;
}

constexpr uint8_t kWasmFunctionType = 0x60;
constexpr uint8_t kWasmExportFunction = 0x00;

enum class WasmSection : uint8_t {
  kType = 1,
  kFunction = 3,
  kExport = 7,
  kCode = 10,
};

void AppendSection(WasmByteWriter* module, WasmSection section,
                   const WasmByteWriter& payload) {
  module->U8(static_cast<uint8_t>(section));
  module->U32Leb(static_cast<uint32_t>(payload.data().size()));
  module->Bytes(payload.data());
}

std::vector<uint8_t> BuildRawModule(
    const std::vector<GeneratedBuiltinBody>& builtins) {
  WasmByteWriter module;
  module.U8(0x00);
  module.U8(0x61);
  module.U8(0x73);
  module.U8(0x6d);
  module.U8(0x01);
  module.U8(0x00);
  module.U8(0x00);
  module.U8(0x00);

  WasmByteWriter type_section;
  type_section.U32Leb(1);
  type_section.U8(kWasmFunctionType);
  type_section.U32Leb(0);
  type_section.U32Leb(0);
  AppendSection(&module, WasmSection::kType, type_section);

  WasmByteWriter function_section;
  function_section.U32Leb(static_cast<uint32_t>(builtins.size()));
  for (size_t i = 0; i < builtins.size(); ++i) {
    function_section.U32Leb(0);
  }
  AppendSection(&module, WasmSection::kFunction, function_section);

  WasmByteWriter export_section;
  export_section.U32Leb(static_cast<uint32_t>(builtins.size()));
  for (uint32_t i = 0; i < builtins.size(); ++i) {
    export_section.String(builtins[i].symbol_name);
    export_section.U8(kWasmExportFunction);
    export_section.U32Leb(i);
  }
  AppendSection(&module, WasmSection::kExport, export_section);

  WasmByteWriter code_section;
  code_section.U32Leb(static_cast<uint32_t>(builtins.size()));
  for (const GeneratedBuiltinBody& builtin : builtins) {
    code_section.Bytes(builtin.body);
  }
  AppendSection(&module, WasmSection::kCode, code_section);

  return module.Release();
}

bool WriteBytes(const char* path, const std::vector<uint8_t>& bytes) {
  if (path == nullptr || path[0] == '\0') return false;
  std::ofstream output(path, std::ios::binary);
  if (!output) return false;
  output.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
  return output.good();
}

bool WriteManifest(const char* path,
                   const std::vector<GeneratedBuiltinBody>& builtins) {
  if (path == nullptr || path[0] == '\0') return true;
  std::ofstream output(path);
  if (!output) return false;
  for (const GeneratedBuiltinBody& builtin : builtins) {
    output << static_cast<int>(builtin.builtin) << " " << builtin.symbol_name
           << " " << builtin.body.size() << "\n";
  }
  return output.good();
}

}  // namespace

GeneratedBuiltinModule& GeneratedBuiltinModule::Get() {
  static GeneratedBuiltinModule module;
  return module;
}

void GeneratedBuiltinModule::AddBuiltin(Builtin builtin,
                                        const std::string& symbol_name,
                                        std::vector<uint8_t> body) {
  std::lock_guard<std::mutex> lock(ModuleMutex());
  builtins_.push_back({builtin, symbol_name, std::move(body)});
}

bool GeneratedBuiltinModule::WriteObjectAndManifest(
    const char* object_path, const char* manifest_path) const {
  std::vector<GeneratedBuiltinBody> builtins;
  {
    std::lock_guard<std::mutex> lock(ModuleMutex());
    builtins = builtins_;
  }
  std::vector<uint8_t> module = BuildRawModule(builtins);
  return WriteBytes(object_path, module) && WriteManifest(manifest_path, builtins);
}

}  // namespace wasm32
}  // namespace internal
}  // namespace v8
