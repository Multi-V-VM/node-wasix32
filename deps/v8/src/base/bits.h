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

// Count functions inline implementations
constexpr int CountLeadingZeros(uint32_t value) {
  return __builtin_clz(value);
}
constexpr int CountLeadingZeros(uint64_t value) {
  return __builtin_clzll(value);
}
constexpr int CountLeadingZeros(uint8_t value) {
  return __builtin_clz(static_cast<uint32_t>(value)) - (32 - 8);
}
constexpr int CountTrailingZeros(uint32_t value) {
  return __builtin_ctz(value);
}
constexpr int CountTrailingZeros(uint64_t value) {
  return __builtin_ctzll(value);
}
constexpr int CountTrailingZeros(uint8_t value) {
  return __builtin_ctz(static_cast<uint32_t>(value));
}
constexpr int CountPopulation(uint32_t value) {
  return __builtin_popcount(value);
}
constexpr int CountPopulation(uint64_t value) {
  return __builtin_popcountll(value);
}

// CountTrailingZerosNonZero is defined in nuclear-fix.h

// Provide saturated 64-bit add/sub for WASI builds
constexpr int64_t SignedSaturatedAdd64(int64_t a, int64_t b) {
  const int64_t max = std::numeric_limits<int64_t>::max();
  const int64_t min = std::numeric_limits<int64_t>::min();
  if (b > 0 && a > max - b) return max;
  if (b < 0 && a < min - b) return min;
  return a + b;
}

constexpr int64_t SignedSaturatedSub64(int64_t a, int64_t b) {
  const int64_t max = std::numeric_limits<int64_t>::max();
  const int64_t min = std::numeric_limits<int64_t>::min();
  if (b < 0 && a > max + b) return max;
  if (b > 0 && a < min + b) return min;
  return a - b;
}

// IsPowerOfTwo helper used by RNG; accept signed and cast to unsigned
template <typename T>
constexpr bool IsPowerOfTwo(T value) {
  using U = typename ::std::make_unsigned<T>::type;
  return value > 0 && ((U(value) & (U(value) - 1)) == 0);
}

// BitWidth: number of bits needed to represent value (0 => 0)
template <typename T>
constexpr int BitWidth(T value) {
  using U = typename ::std::make_unsigned<T>::type;
  U v = static_cast<U>(value);
  if (v == 0) return 0;
  if constexpr (sizeof(U) == 8) {
    return static_cast<int>(8 * sizeof(U) - __builtin_clzll(v));
  } else {
    return static_cast<int>(8 * sizeof(U) - __builtin_clz(static_cast<uint32_t>(v)));
  }
}

#else
// Original bits.h content would go here for non-WASI builds
// Non-WASI implementations would be here
#endif

}  // namespace bits
}  // namespace base
}  // namespace v8

#endif  // V8_SRC_BASE_BITS_H_
