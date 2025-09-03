// Minimal WASI stub for wasm features when building without full platform
#ifndef V8_WASM_FEATURES_FIX_H_
#define V8_WASM_FEATURES_FIX_H_

namespace v8 {
namespace internal {
namespace wasm {

// Provide minimal feature toggles as false for WASI stub builds
struct Features {
  static bool threads_enabled() { return false; }
  static bool simd_enabled() { return false; }
  static bool gc_enabled() { return false; }
};

}  // namespace wasm
}  // namespace internal
}  // namespace v8

#endif  // V8_WASM_FEATURES_FIX_H_

