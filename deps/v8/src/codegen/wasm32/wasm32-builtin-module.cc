// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/codegen/wasm32/wasm32-builtin-module.h"

#include <fstream>
#include <mutex>
#include <unordered_map>
#include <utility>

#include "src/codegen/wasm32/wasm32-builtin-module-writer.h"
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
constexpr uint8_t kWasmImportMemory = 0x02;
constexpr uint8_t kWasmLimitsMinOnly = 0x00;
constexpr uint32_t kWasmObjectMemoryMinPages = 0;
constexpr uint8_t kWasmLinkingVersion = 2;
constexpr uint8_t kWasmLinkingSymbolTable = 8;
constexpr uint8_t kWasmSymbolFunction = 0;
constexpr uint8_t kWasmSymbolData = 1;
constexpr uint32_t kWasmSymbolUndefined = 0x10;
constexpr uint8_t kWasmRelocFunctionIndexLeb = 0;
constexpr uint8_t kWasmRelocMemoryAddressSleb = 4;
constexpr uint32_t kWasmCodeSectionIndex = 3;
constexpr const char* kLinearMemoryImportModule = "env";
constexpr const char* kLinearMemoryImportName = "__linear_memory";
constexpr const char* kWasmRegsSymbol = "g_wasm_regs";

enum class WasmSection : uint8_t {
  kCustom = 0,
  kType = 1,
  kImport = 2,
  kFunction = 3,
  kCode = 10,
};

void AppendSection(WasmByteWriter* module, WasmSection section,
                   const WasmByteWriter& payload) {
  module->U8(static_cast<uint8_t>(section));
  module->U32Leb(static_cast<uint32_t>(payload.data().size()));
  module->Bytes(payload.data());
}

void AppendCustomSection(WasmByteWriter* module, const std::string& name,
                         const WasmByteWriter& payload) {
  WasmByteWriter section;
  section.String(name);
  section.Bytes(payload.data());
  AppendSection(module, WasmSection::kCustom, section);
}

std::vector<uint8_t> BuildRelocatableObject(
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

  WasmByteWriter import_section;
  import_section.U32Leb(1);
  import_section.String(kLinearMemoryImportModule);
  import_section.String(kLinearMemoryImportName);
  import_section.U8(kWasmImportMemory);
  import_section.U8(kWasmLimitsMinOnly);
  import_section.U32Leb(kWasmObjectMemoryMinPages);
  AppendSection(&module, WasmSection::kImport, import_section);

  WasmByteWriter function_section;
  function_section.U32Leb(static_cast<uint32_t>(builtins.size()));
  for (size_t i = 0; i < builtins.size(); ++i) {
    function_section.U32Leb(0);
  }
  AppendSection(&module, WasmSection::kFunction, function_section);

  WasmByteWriter code_section;
  code_section.U32Leb(static_cast<uint32_t>(builtins.size()));
  struct PendingRelocation {
    WasmRelocationKind kind;
    uint32_t code_offset;
    uint32_t symbol_index;
    int32_t addend;
  };
  std::unordered_map<std::string, uint32_t> function_symbol_indices;
  for (uint32_t i = 0; i < builtins.size(); ++i) {
    function_symbol_indices.emplace(builtins[i].symbol_name, i);
  }
  const uint32_t data_symbol_index = static_cast<uint32_t>(builtins.size());
  std::vector<PendingRelocation> relocations;
  for (const GeneratedBuiltinBody& builtin : builtins) {
    uint32_t body_start = static_cast<uint32_t>(code_section.size());
    code_section.Bytes(builtin.body);
    for (const WasmRelocation& relocation : builtin.relocations) {
      switch (relocation.kind) {
        case WasmRelocationKind::kMemoryAddressSleb:
          CHECK_EQ(relocation.symbol_name, kWasmRegsSymbol);
          relocations.push_back({relocation.kind, body_start + relocation.offset,
                                 data_symbol_index, relocation.addend});
          break;
        case WasmRelocationKind::kFunctionIndexLeb: {
          auto it = function_symbol_indices.find(relocation.symbol_name);
          CHECK_NE(it, function_symbol_indices.end());
          relocations.push_back({relocation.kind, body_start + relocation.offset,
                                 it->second, relocation.addend});
          break;
        }
      }
    }
  }
  AppendSection(&module, WasmSection::kCode, code_section);

  WasmByteWriter symbols;
  symbols.U32Leb(data_symbol_index + 1);
  for (uint32_t i = 0; i < builtins.size(); ++i) {
    symbols.U8(kWasmSymbolFunction);
    symbols.U32Leb(0);
    symbols.U32Leb(i);
    symbols.String(builtins[i].symbol_name);
  }
  symbols.U8(kWasmSymbolData);
  symbols.U32Leb(kWasmSymbolUndefined);
  symbols.String(kWasmRegsSymbol);

  WasmByteWriter linking;
  linking.U32Leb(kWasmLinkingVersion);
  linking.U8(kWasmLinkingSymbolTable);
  linking.U32Leb(static_cast<uint32_t>(symbols.size()));
  linking.Bytes(symbols.data());
  AppendCustomSection(&module, "linking", linking);

  if (!relocations.empty()) {
    WasmByteWriter reloc_code;
    reloc_code.U32Leb(kWasmCodeSectionIndex);
    reloc_code.U32Leb(static_cast<uint32_t>(relocations.size()));
    for (const PendingRelocation& relocation : relocations) {
      switch (relocation.kind) {
        case WasmRelocationKind::kFunctionIndexLeb:
          reloc_code.U8(kWasmRelocFunctionIndexLeb);
          break;
        case WasmRelocationKind::kMemoryAddressSleb:
          reloc_code.U8(kWasmRelocMemoryAddressSleb);
          break;
      }
      reloc_code.U32Leb(relocation.code_offset);
      reloc_code.U32Leb(relocation.symbol_index);
      if (relocation.kind == WasmRelocationKind::kMemoryAddressSleb) {
        reloc_code.I32Leb(relocation.addend);
      }
    }
    AppendCustomSection(&module, "reloc.CODE", reloc_code);
  }

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

bool WriteRegistrySource(const char* path,
                         const std::vector<GeneratedBuiltinBody>& builtins) {
  if (path == nullptr || path[0] == '\0') return true;
  std::ofstream output(path);
  if (!output) return false;

  output << "// Generated by node_mksnapshot for the wasm32 backend.\n";
  output << "extern \"C\" void RegisterWasmBuiltinById(int builtin_id, "
            "void* fnptr);\n";
  output << "\n";
  output << "namespace v8 {\nnamespace internal {\n\n";
  for (const GeneratedBuiltinBody& builtin : builtins) {
    output << "extern \"C\" void " << builtin.symbol_name << "();\n";
  }
  output << "\nvoid RegisterGeneratedWasmBuiltins() {\n";
  for (const GeneratedBuiltinBody& builtin : builtins) {
    output << "  RegisterWasmBuiltinById("
           << static_cast<int>(builtin.builtin)
           << ", reinterpret_cast<void*>(&" << builtin.symbol_name
           << "));\n";
  }
  output << "}\n\n";
  output << "}  // namespace internal\n}  // namespace v8\n\n";
  return output.good();
}

}  // namespace

GeneratedBuiltinModule& GeneratedBuiltinModule::Get() {
  static GeneratedBuiltinModule module;
  return module;
}

void GeneratedBuiltinModule::AddBuiltin(Builtin builtin,
                                        const std::string& symbol_name,
                                        std::vector<uint8_t> body,
                                        std::vector<WasmRelocation> relocations) {
  std::lock_guard<std::mutex> lock(ModuleMutex());
  builtins_.push_back(
      {builtin, symbol_name, std::move(body), std::move(relocations)});
}

bool GeneratedBuiltinModule::WriteObjectAndManifest(
    const char* object_path, const char* manifest_path,
    const char* registry_source_path) const {
  std::vector<GeneratedBuiltinBody> builtins;
  {
    std::lock_guard<std::mutex> lock(ModuleMutex());
    builtins = builtins_;
  }
  std::vector<uint8_t> module = BuildRelocatableObject(builtins);
  return WriteBytes(object_path, module) &&
         WriteManifest(manifest_path, builtins) &&
         WriteRegistrySource(registry_source_path, builtins);
}

bool WriteGeneratedBuiltinModule(const char* object_path,
                                 const char* manifest_path,
                                 const char* registry_source_path) {
  return GeneratedBuiltinModule::Get().WriteObjectAndManifest(object_path,
                                                              manifest_path,
                                                              registry_source_path);
}

}  // namespace wasm32
}  // namespace internal
}  // namespace v8
