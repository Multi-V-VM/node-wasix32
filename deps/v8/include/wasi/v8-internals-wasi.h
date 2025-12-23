#ifndef WASI_V8_INTERNALS_H_
#define WASI_V8_INTERNALS_H_

#if defined(__wasi__) || defined(V8_USING_WASI_SHIMS)

#include <cstdint>

namespace v8 {
namespace internal {

// Additional Internals members for WASI compatibility
class InternalsWASI {
 public:
  // Smi operations for WASM32
  static constexpr intptr_t IntToSmi(int value) {
    return static_cast<intptr_t>(value) << 1;
  }

  static constexpr int SmiValue(intptr_t smi) {
    return static_cast<int>(smi >> 1);
  }

  // GC constants
  static constexpr int kDisallowGarbageCollectionAlign = 8;
  static constexpr int kDisallowGarbageCollectionSize = 16;

  // Node state constants for TracedHandles
  static constexpr uint8_t kNodeStateMask = 0x0f;
  static constexpr uint8_t kNodeStateIsWeakValue = 2;

  // Root indices - these map to specific slots in the root table (must match RootIndex enum)
  static constexpr int kUndefinedValueRootIndex = 4;
  static constexpr int kNullValueRootIndex = 6;
  static constexpr int kTrueValueRootIndex = 7;
  static constexpr int kFalseValueRootIndex = 8;
  static constexpr int kEmptyStringRootIndex = 9;

  // Pointer compression cage base alignment
  static constexpr size_t kPtrComprCageBaseAlignment = 4 * 1024 * 1024 * 1024ULL;  // 4GB
};

}  // namespace internal
}  // namespace v8

// Inject missing members into v8::internal::Internals
namespace v8 {
namespace internal {

#ifndef V8_INTERNALS_WASI_INJECTED
#define V8_INTERNALS_WASI_INJECTED

// Make InternalsWASI members available through Internals
inline constexpr intptr_t InternalsSmiValue(intptr_t smi) {
  return InternalsWASI::SmiValue(smi);
}

inline constexpr intptr_t InternalsIntToSmi(int value) {
  return InternalsWASI::IntToSmi(value);
}

#endif  // V8_INTERNALS_WASI_INJECTED

}  // namespace internal
}  // namespace v8

#endif  // defined(__wasi__) || defined(V8_USING_WASI_SHIMS)

#endif  // WASI_V8_INTERNALS_H_
