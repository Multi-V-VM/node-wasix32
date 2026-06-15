#include "src/codegen/wasm32/wasm32-encoder.h"

#include <utility>

namespace v8 {
namespace internal {
namespace wasm32 {
namespace {

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

void EmitValueType(WasmByteWriter* writer, WasmValueType type) {
  writer->U8(static_cast<uint8_t>(type));
}

}  // namespace

void WasmByteWriter::U8(uint8_t value) { data_.push_back(value); }

void WasmByteWriter::U32Leb(uint32_t value) {
  do {
    uint8_t byte = value & 0x7f;
    value >>= 7;
    if (value != 0) byte |= 0x80;
    U8(byte);
  } while (value != 0);
}

void WasmByteWriter::I32Leb(int32_t value) {
  int64_t remaining = value;
  bool more = true;
  while (more) {
    int64_t remainder = remaining % 128;
    if (remainder < 0) remainder += 128;
    uint8_t byte = static_cast<uint8_t>(remainder);
    if (remaining < 0) {
      remaining = -((-remaining + 127) / 128);
    } else {
      remaining /= 128;
    }
    const bool sign_bit_set = (byte & 0x40) != 0;
    more = !((remaining == 0 && !sign_bit_set) ||
             (remaining == -1 && sign_bit_set));
    if (more) byte |= 0x80;
    U8(byte);
  }
}

void WasmByteWriter::Bytes(const std::vector<uint8_t>& bytes) {
  data_.insert(data_.end(), bytes.begin(), bytes.end());
}

void WasmByteWriter::String(const std::string& value) {
  U32Leb(static_cast<uint32_t>(value.size()));
  data_.insert(data_.end(), value.begin(), value.end());
}

WasmFunctionBuilder::WasmFunctionBuilder(std::string name,
                                         WasmValueType result)
    : name_(std::move(name)), result_(result) {}

uint32_t WasmFunctionBuilder::AddLocal(WasmValueType type) {
  locals_.push_back(type);
  return static_cast<uint32_t>(locals_.size() - 1);
}

void WasmFunctionBuilder::Opcode(WasmOpcode opcode) {
  body_.U8(static_cast<uint8_t>(opcode));
}

void WasmFunctionBuilder::I32Const(int32_t value) {
  Opcode(WasmOpcode::kI32Const);
  body_.I32Leb(value);
}

void WasmFunctionBuilder::LocalGet(uint32_t index) {
  Opcode(WasmOpcode::kLocalGet);
  body_.U32Leb(index);
}

void WasmFunctionBuilder::LocalSet(uint32_t index) {
  Opcode(WasmOpcode::kLocalSet);
  body_.U32Leb(index);
}

void WasmFunctionBuilder::Load(WasmOpcode opcode, uint32_t align_log2,
                               uint32_t offset) {
  Opcode(opcode);
  body_.U32Leb(align_log2);
  body_.U32Leb(offset);
}

void WasmFunctionBuilder::Store(WasmOpcode opcode, uint32_t align_log2,
                                uint32_t offset) {
  Opcode(opcode);
  body_.U32Leb(align_log2);
  body_.U32Leb(offset);
}

void WasmFunctionBuilder::Load32(uint32_t align_log2, uint32_t offset) {
  Load(WasmOpcode::kI32Load, align_log2, offset);
}

void WasmFunctionBuilder::Store32(uint32_t align_log2, uint32_t offset) {
  Store(WasmOpcode::kI32Store, align_log2, offset);
}

void WasmFunctionBuilder::Call(uint32_t function_index) {
  Opcode(WasmOpcode::kCall);
  body_.U32Leb(function_index);
}

void WasmFunctionBuilder::CallIndirect(uint32_t type_index) {
  Opcode(WasmOpcode::kCallIndirect);
  body_.U32Leb(type_index);
  body_.U32Leb(0);
}

void WasmFunctionBuilder::Return() {
  Opcode(WasmOpcode::kReturn);
}

std::vector<uint8_t> WasmFunctionBuilder::FinishBody() const {
  WasmByteWriter body;
  body.U32Leb(static_cast<uint32_t>(locals_.size()));
  for (WasmValueType local : locals_) {
    body.U32Leb(1);
    EmitValueType(&body, local);
  }
  body.Bytes(body_.data());
  body.U8(static_cast<uint8_t>(WasmOpcode::kEnd));

  WasmByteWriter sized_body;
  sized_body.U32Leb(static_cast<uint32_t>(body.data().size()));
  sized_body.Bytes(body.data());
  return sized_body.Release();
}

WasmFunctionBuilder& WasmModuleBuilder::AddFunction(const std::string& name,
                                                    WasmValueType result) {
  functions_.emplace_back(name, result);
  return functions_.back();
}

std::vector<uint8_t> WasmModuleBuilder::FinishModule() const {
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
  type_section.U32Leb(static_cast<uint32_t>(functions_.size()));
  for (const WasmFunctionBuilder& function : functions_) {
    type_section.U8(kWasmFunctionType);
    type_section.U32Leb(0);
    if (function.result() == WasmValueType::kVoid) {
      type_section.U32Leb(0);
    } else {
      type_section.U32Leb(1);
      EmitValueType(&type_section, function.result());
    }
  }
  AppendSection(&module, WasmSection::kType, type_section);

  WasmByteWriter function_section;
  function_section.U32Leb(static_cast<uint32_t>(functions_.size()));
  for (uint32_t i = 0; i < functions_.size(); ++i) {
    function_section.U32Leb(i);
  }
  AppendSection(&module, WasmSection::kFunction, function_section);

  WasmByteWriter export_section;
  export_section.U32Leb(static_cast<uint32_t>(functions_.size()));
  for (uint32_t i = 0; i < functions_.size(); ++i) {
    export_section.String(functions_[i].name());
    export_section.U8(kWasmExportFunction);
    export_section.U32Leb(i);
  }
  AppendSection(&module, WasmSection::kExport, export_section);

  WasmByteWriter code_section;
  code_section.U32Leb(static_cast<uint32_t>(functions_.size()));
  for (const WasmFunctionBuilder& function : functions_) {
    code_section.Bytes(function.FinishBody());
  }
  AppendSection(&module, WasmSection::kCode, code_section);

  return module.Release();
}

}  // namespace wasm32
}  // namespace internal
}  // namespace v8
