#include "src/codegen/wasm32/wasm32-encoder.h"

#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
  if (argc != 2) return 2;
  v8::internal::wasm32::WasmModuleBuilder module;
  v8::internal::wasm32::WasmFunctionBuilder& fn =
      module.AddFunction("return_7", v8::internal::wasm32::WasmValueType::kI32);
  fn.I32Const(7);
  fn.Return();
  std::vector<uint8_t> bytes = module.FinishModule();
  std::ofstream out(argv[1], std::ios::binary);
  if (!out) {
    std::cerr << "failed to open " << argv[1] << "\n";
    return 1;
  }
  out.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
  if (!out) {
    std::cerr << "failed to write " << argv[1] << "\n";
    return 1;
  }
  out.close();
  if (!out) {
    std::cerr << "failed to close " << argv[1] << "\n";
    return 1;
  }
  std::cout << bytes.size() << "\n";
  return bytes.size() > 8 ? 0 : 1;
}
