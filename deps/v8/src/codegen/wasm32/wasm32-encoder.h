#ifndef V8_CODEGEN_WASM32_WASM32_ENCODER_H_
#define V8_CODEGEN_WASM32_WASM32_ENCODER_H_

#include <cstdint>
#include <deque>
#include <string>
#include <vector>

namespace v8 {
namespace internal {
namespace wasm32 {

enum class WasmValueType : uint8_t {
  kVoid = 0x40,
  kI32 = 0x7f,
  kI64 = 0x7e,
  kF32 = 0x7d,
  kF64 = 0x7c,
};

enum class WasmOpcode : uint8_t {
  kUnreachable = 0x00,
  kBlock = 0x02,
  kLoop = 0x03,
  kIf = 0x04,
  kElse = 0x05,
  kEnd = 0x0b,
  kBr = 0x0c,
  kBrIf = 0x0d,
  kBrTable = 0x0e,
  kReturn = 0x0f,
  kCall = 0x10,
  kCallIndirect = 0x11,
  kDrop = 0x1a,
  kSelect = 0x1b,
  kLocalGet = 0x20,
  kLocalSet = 0x21,
  kLocalTee = 0x22,
  kGlobalGet = 0x23,
  kGlobalSet = 0x24,
  kI32Load = 0x28,
  kI64Load = 0x29,
  kF32Load = 0x2a,
  kF64Load = 0x2b,
  kI32Load8S = 0x2c,
  kI32Load8U = 0x2d,
  kI32Load16S = 0x2e,
  kI32Load16U = 0x2f,
  kI32Store = 0x36,
  kI64Store = 0x37,
  kF32Store = 0x38,
  kF64Store = 0x39,
  kI32Store8 = 0x3a,
  kI32Store16 = 0x3b,
  kI32Const = 0x41,
  kI64Const = 0x42,
  kF32Const = 0x43,
  kF64Const = 0x44,
  kI32Eqz = 0x45,
  kI32Eq = 0x46,
  kI32Ne = 0x47,
  kI32LtS = 0x48,
  kI32LtU = 0x49,
  kI32GtS = 0x4a,
  kI32GtU = 0x4b,
  kI32LeS = 0x4c,
  kI32LeU = 0x4d,
  kI32GeS = 0x4e,
  kI32GeU = 0x4f,
  kI32Add = 0x6a,
  kI32Sub = 0x6b,
  kI32Mul = 0x6c,
  kI32DivS = 0x6d,
  kI32DivU = 0x6e,
  kI32RemS = 0x6f,
  kI32RemU = 0x70,
  kI32And = 0x71,
  kI32Or = 0x72,
  kI32Xor = 0x73,
  kI32Shl = 0x74,
  kI32ShrS = 0x75,
  kI32ShrU = 0x76,
};

class WasmByteWriter {
 public:
  void U8(uint8_t value);
  void U32Leb(uint32_t value);
  void I32Leb(int32_t value);
  void Bytes(const std::vector<uint8_t>& bytes);
  void String(const std::string& value);
  const std::vector<uint8_t>& data() const { return data_; }
  std::vector<uint8_t> Release() { return std::move(data_); }

 private:
  std::vector<uint8_t> data_;
};

class WasmFunctionBuilder {
 public:
  WasmFunctionBuilder(std::string name, WasmValueType result);
  uint32_t AddLocal(WasmValueType type);
  void Opcode(WasmOpcode opcode);
  void I32Const(int32_t value);
  void LocalGet(uint32_t index);
  void LocalSet(uint32_t index);
  // Raw body emitters. Standalone modules using memory/table operations need
  // WasmModuleBuilder support for the matching sections before they validate.
  void Load32(uint32_t align_log2, uint32_t offset);
  void Store32(uint32_t align_log2, uint32_t offset);
  void Call(uint32_t function_index);
  void CallIndirect(uint32_t type_index);
  void Return();
  std::vector<uint8_t> FinishBody() const;
  const std::string& name() const { return name_; }
  WasmValueType result() const { return result_; }

 private:
  std::string name_;
  WasmValueType result_;
  std::vector<WasmValueType> locals_;
  WasmByteWriter body_;
};

class WasmModuleBuilder {
 public:
  WasmFunctionBuilder& AddFunction(const std::string& name,
                                   WasmValueType result);
  std::vector<uint8_t> FinishModule() const;

 private:
  std::deque<WasmFunctionBuilder> functions_;
};

}  // namespace wasm32
}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_WASM32_ENCODER_H_
