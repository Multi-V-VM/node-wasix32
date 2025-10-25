#ifndef V8_SRC_BASE_BITS_H_
#define V8_SRC_BASE_BITS_H_

#if 1  // WASI and generic implementations
#include <cstdint>
#include <type_traits>
#include <climits>
#include <limits>

#if 0
// Avoid including large WASI shims here to prevent circular includes.
// #include "wasi/nuclear-fix.h"
#endif
#include "src/base/bits-namespace-fix.h"

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

inline uint32_t RoundDownToPowerOfTwo32(uint32_t value) {
  if (value <= 1) return value;
  // Set all bits below the highest set bit
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  // Remove all bits except the highest
  return value - (value >> 1);
}

// Generic RoundUpToPowerOfTwo for integral types
template <typename T>
inline T RoundUpToPowerOfTwo(T value) {
  static_assert(::std::is_integral<T>::value, "T must be integral");
  if constexpr (sizeof(T) <= sizeof(uint32_t)) {
    return static_cast<T>(RoundUpToPowerOfTwo32(static_cast<uint32_t>(value)));
  } else {
    return static_cast<T>(RoundUpToPowerOfTwo64(static_cast<uint64_t>(value)));
  }
}

// Count functions inline implementations
// Some code expects explicit 32-bit variant.
constexpr int CountLeadingZeros32(uint32_t value) { return __builtin_clz(value); }
constexpr int CountLeadingZeros64(uint64_t value) { return __builtin_clzll(value); }
constexpr int CountLeadingZeros(uint32_t value) { return __builtin_clz(value); }
constexpr int CountLeadingZeros(uint64_t value) { return __builtin_clzll(value); }
// Provide a size_t overload to avoid ambiguous calls where sizeof(size_t)
// differs from builtin overloads.
constexpr int CountLeadingZeros(size_t value) {
  if constexpr (sizeof(size_t) == 8) {
    return __builtin_clzll(static_cast<uint64_t>(value));
  } else {
    return __builtin_clz(static_cast<uint32_t>(value));
  }
}
constexpr int CountLeadingZeros(uint8_t value) {
  return __builtin_clz(static_cast<uint32_t>(value)) - (32 - 8);
}
constexpr int CountTrailingZeros32(uint32_t value) { return __builtin_ctz(value); }
constexpr int CountTrailingZeros64(uint64_t value) { return __builtin_ctzll(value); }
constexpr int CountTrailingZeros(uint32_t value) { return __builtin_ctz(value); }
constexpr int CountTrailingZeros(uint64_t value) { return __builtin_ctzll(value); }
constexpr int CountTrailingZeros(uint8_t value) {
  return __builtin_ctz(static_cast<uint32_t>(value));
}
// Variants that require value != 0. Callers ensure non-zero.
#ifndef V8_BITS_CTZ_NZ_DEFINED
constexpr int CountTrailingZerosNonZero(uint32_t value) { return __builtin_ctz(value); }
constexpr int CountTrailingZerosNonZero(uint64_t value) { return __builtin_ctzll(value); }
template <typename T>
constexpr int CountTrailingZerosNonZero(T value) {
  if constexpr (sizeof(T) <= sizeof(uint32_t)) {
    return CountTrailingZerosNonZero(static_cast<uint32_t>(value));
  } else {
    return CountTrailingZerosNonZero(static_cast<uint64_t>(value));
  }
}
#define V8_BITS_CTZ_NZ_DEFINED 1
#endif

constexpr int CountPopulation(uint32_t value) { return __builtin_popcount(value); }
constexpr int CountPopulation(uint64_t value) { return __builtin_popcountll(value); }

// Template wrappers used by newer V8 call sites (e.g. CountTrailingZeros<uint32_t>)
// Provide these to preserve compatibility with templated usage.
template <typename T>
constexpr int CountTrailingZeros(T value) {
  if constexpr (sizeof(T) <= sizeof(uint32_t)) {
    return CountTrailingZeros(static_cast<uint32_t>(value));
  } else {
    return CountTrailingZeros(static_cast<uint64_t>(value));
  }
}

// Additional helpers used throughout V8
template <typename T>
constexpr int WhichPowerOfTwo(T value) {
  // Caller ensures value is a power of two and non-zero.
  return CountTrailingZerosNonZero(static_cast<std::make_unsigned_t<T>>(value));
}

inline bool SignedAddOverflow32(int32_t a, int32_t b, int32_t* out) {
  long long r = static_cast<long long>(a) + static_cast<long long>(b);
  *out = static_cast<int32_t>(r);
  return r > INT32_MAX || r < INT32_MIN;
}

inline bool SignedSubOverflow32(int32_t a, int32_t b, int32_t* out) {
  long long r = static_cast<long long>(a) - static_cast<long long>(b);
  *out = static_cast<int32_t>(r);
  return r > INT32_MAX || r < INT32_MIN;
}

inline bool SignedMulOverflow32(int32_t a, int32_t b, int32_t* out) {
  long long r = static_cast<long long>(a) * static_cast<long long>(b);
  *out = static_cast<int32_t>(r);
  return r > INT32_MAX || r < INT32_MIN;
}

// Division and modulo operations for compiler use
inline int32_t SignedDiv32(int32_t lhs, int32_t rhs) {
  if (rhs == 0) return 0;
  if (rhs == -1 && lhs == INT32_MIN) return lhs;  // Prevent overflow
  return lhs / rhs;
}

inline int64_t SignedDiv64(int64_t lhs, int64_t rhs) {
  if (rhs == 0) return 0;
  if (rhs == -1 && lhs == INT64_MIN) return lhs;  // Prevent overflow
  return lhs / rhs;
}

inline int32_t SignedMod32(int32_t lhs, int32_t rhs) {
  if (rhs == 0) return 0;
  if (rhs == -1) return 0;  // Prevent overflow
  return lhs % rhs;
}

inline int64_t SignedMod64(int64_t lhs, int64_t rhs) {
  if (rhs == 0) return 0;
  if (rhs == -1) return 0;  // Prevent overflow
  return lhs % rhs;
}

inline uint32_t UnsignedDiv32(uint32_t lhs, uint32_t rhs) {
  if (rhs == 0) return 0;
  return lhs / rhs;
}

inline uint64_t UnsignedDiv64(uint64_t lhs, uint64_t rhs) {
  if (rhs == 0) return 0;
  return lhs / rhs;
}

inline uint32_t UnsignedMod32(uint32_t lhs, uint32_t rhs) {
  if (rhs == 0) return 0;
  return lhs % rhs;
}

inline uint64_t UnsignedMod64(uint64_t lhs, uint64_t rhs) {
  if (rhs == 0) return 0;
  return lhs % rhs;
}

// ReverseBytes helpers for 32/64-bit integers.
inline uint32_t ReverseBytes(uint32_t v) { return __builtin_bswap32(v); }
inline uint64_t ReverseBytes(uint64_t v) { return __builtin_bswap64(v); }
// CountLeadingSignBits: number of leading bits equal to the sign bit.
inline int CountLeadingSignBits(int32_t v) {
  // For non-negative, count leading zeros; for negative, count leading ones as clz(~v).
  return v >= 0 ? CountLeadingZeros(static_cast<uint32_t>(v))
                : CountLeadingZeros(static_cast<uint32_t>(~v));
}
inline int CountLeadingSignBits(int64_t v) {
  return v >= 0 ? CountLeadingZeros(static_cast<uint64_t>(v))
                : CountLeadingZeros(static_cast<uint64_t>(~v));
}

// Overflow detection helpers for 64-bit signed ops.
inline bool SignedAddOverflow64(int64_t a, int64_t b, int64_t* out) {
  return __builtin_add_overflow(a, b, out);
}
inline bool SignedSubOverflow64(int64_t a, int64_t b, int64_t* out) {
  return __builtin_sub_overflow(a, b, out);
}
inline bool SignedMulOverflow64(int64_t a, int64_t b, int64_t* out) {
  return __builtin_mul_overflow(a, b, out);
}

// High-part multiply helpers.
inline int32_t SignedMulHigh32(int32_t a, int32_t b) {
  long long p = static_cast<long long>(a) * static_cast<long long>(b);
  return static_cast<int32_t>(p >> 32);
}
inline int64_t SignedMulHigh64(int64_t a, int64_t b) {
#if defined(__SIZEOF_INT128__)
  __int128 p = static_cast<__int128>(a) * static_cast<__int128>(b);
  return static_cast<int64_t>(p >> 64);
#else
  // Fallback (approximate): not used in critical paths for WASI build.
  return 0;
#endif
}
inline uint32_t UnsignedMulHigh32(uint32_t a, uint32_t b) {
  uint64_t p = static_cast<uint64_t>(a) * static_cast<uint64_t>(b);
  return static_cast<uint32_t>(p >> 32);
}
inline uint64_t UnsignedMulHigh64(uint64_t a, uint64_t b) {
#if defined(__SIZEOF_INT128__)
  unsigned __int128 p = static_cast<unsigned __int128>(a) * static_cast<unsigned __int128>(b);
  return static_cast<uint64_t>(p >> 64);
#else
  return 0;
#endif
}

// Convert signed to unsigned of same width.
template <typename T>
constexpr typename ::std::make_unsigned<T>::type Unsigned(T v) {
  return static_cast<typename ::std::make_unsigned<T>::type>(v);
}

template <typename T>
constexpr int CountPopulation(T value) {
  if constexpr (sizeof(T) <= sizeof(uint32_t)) {
    return CountPopulation(static_cast<uint32_t>(value));
  } else {
    return CountPopulation(static_cast<uint64_t>(value));
  }
}

template <typename T>
constexpr int CountLeadingZeros(T value) {
  if constexpr (sizeof(T) <= sizeof(uint32_t)) {
    return CountLeadingZeros(static_cast<uint32_t>(value));
  } else {
    return CountLeadingZeros(static_cast<uint64_t>(value));
  }
}

// CountTrailingZerosNonZero is defined in nuclear-fix.h

// Provide saturated 64-bit add/sub for WASI builds
constexpr int64_t SignedSaturatedAdd64(int64_t a, int64_t b) {
  const int64_t max = ::std::numeric_limits<int64_t>::max();
  const int64_t min = ::std::numeric_limits<int64_t>::min();
  if (b > 0 && a > max - b) return max;
  if (b < 0 && a < min - b) return min;
  return a + b;
}

constexpr int64_t SignedSaturatedSub64(int64_t a, int64_t b) {
  const int64_t max = ::std::numeric_limits<int64_t>::max();
  const int64_t min = ::std::numeric_limits<int64_t>::min();
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

#endif

}  // namespace bits
}  // namespace base
}  // namespace v8

// Make selected ::v8::base::bits helpers visible under v8::internal::base::bits
// so internal code can refer to base::bits::* without including globals.h.
namespace v8 {
namespace internal {
namespace base {
namespace bits {
using ::v8::base::bits::RoundUpToPowerOfTwo32;
using ::v8::base::bits::RoundUpToPowerOfTwo64;
using ::v8::base::bits::RoundUpToPowerOfTwo;
using ::v8::base::bits::CountTrailingZerosNonZero;
using ::v8::base::bits::CountTrailingZeros;
using ::v8::base::bits::CountTrailingZeros32;
using ::v8::base::bits::CountTrailingZeros64;
using ::v8::base::bits::CountLeadingZeros;
using ::v8::base::bits::CountLeadingZeros32;
using ::v8::base::bits::CountLeadingZeros64;
using ::v8::base::bits::CountPopulation;
using ::v8::base::bits::CountLeadingSignBits;
using ::v8::base::bits::SignedSaturatedAdd64;
using ::v8::base::bits::SignedSaturatedSub64;
using ::v8::base::bits::IsPowerOfTwo;
using ::v8::base::bits::ReverseBytes;
using ::v8::base::bits::SignedAddOverflow64;
using ::v8::base::bits::SignedSubOverflow64;
using ::v8::base::bits::SignedMulOverflow64;
using ::v8::base::bits::SignedMulHigh32;
using ::v8::base::bits::SignedMulHigh64;
using ::v8::base::bits::UnsignedMulHigh32;
using ::v8::base::bits::UnsignedMulHigh64;
using ::v8::base::bits::Unsigned;
using ::v8::base::bits::WhichPowerOfTwo;
using ::v8::base::bits::SignedAddOverflow32;
using ::v8::base::bits::SignedSubOverflow32;
using ::v8::base::bits::SignedMulOverflow32;
using ::v8::base::bits::SignedDiv32;
using ::v8::base::bits::SignedDiv64;
using ::v8::base::bits::SignedMod32;
using ::v8::base::bits::SignedMod64;
using ::v8::base::bits::UnsignedDiv32;
using ::v8::base::bits::UnsignedDiv64;
using ::v8::base::bits::UnsignedMod32;
using ::v8::base::bits::UnsignedMod64;
using ::v8::base::bits::RotateRight32;
using ::v8::base::bits::RotateRight64;
using ::v8::base::bits::RotateLeft32;
using ::v8::base::bits::RotateLeft64;
using ::v8::base::bits::RoundDownToPowerOfTwo32;
}  // namespace bits
}  // namespace base
}  // namespace internal
}  // namespace v8

#endif  // __wasi__
#endif  // V8_SRC_BASE_BITS_H_
