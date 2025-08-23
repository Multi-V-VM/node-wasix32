#ifndef V8_WASM_FEATURES_FIX_H_
#define V8_WASM_FEATURES_FIX_H_

namespace v8 {
namespace internal {
namespace wasm {

class WasmEnabledFeatures {
 public:
  bool contains(int feature) const { return true; }
  static WasmEnabledFeatures All() { return WasmEnabledFeatures(); }
  static WasmEnabledFeatures None() { return WasmEnabledFeatures(); }
};

} // namespace wasm
} // namespace internal
} // namespace v8

#endif
