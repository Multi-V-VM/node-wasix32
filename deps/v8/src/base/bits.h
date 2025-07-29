#ifndef V8_SRC_BASE_BITS_H_
#define V8_SRC_BASE_BITS_H_

#include <cstdint>
#include <type_traits>
#include <climits>

#ifdef __wasi__
#include "../../include/wasi/nuclear-fix.h"
#endif

namespace v8 {
namespace base {
namespace bits {

#ifdef __wasi__
// WASI implementations
inline uint32_t RoundUpToPowerOfTwo32(uint32_t value) {
  if (value <= 1) return 1;
  value--;
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  return value + 1;
}

inline uint32_t RoundUpToPowerOfTwo(uint32_t value) {
  return RoundUpToPowerOfTwo32(value);
}

inline uint64_t RoundUpToPowerOfTwo64(uint64_t value) {
  if (value <= 1) return 1;
  value--;
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  value |= value >> 32;
  return value + 1;
}

inline uint64_t RoundUpToPowerOfTwo(uint64_t value) {
  return RoundUpToPowerOfTwo64(value);
}

// Overload for size_t
inline size_t RoundUpToPowerOfTwo(size_t value) {
  if constexpr (sizeof(size_t) == sizeof(uint32_t)) {
    return RoundUpToPowerOfTwo32(static_cast<uint32_t>(value));
  } else {
    return RoundUpToPowerOfTwo64(static_cast<uint64_t>(value));
  }
}

// SignedSaturatedAdd64 and SignedSaturatedSub64 are defined in nuclear-fix.h

#else
// Original bits.h content would go here for non-WASI builds
// Non-WASI implementations would be here
#endif

}  // namespace bits
}  // namespace base
}  // namespace v8

#endif  // V8_SRC_BASE_BITS_H_
