#ifndef WASI_V8_BITS_FIXES_H_
#define WASI_V8_BITS_FIXES_H_

// Bit manipulation fixes for V8 WASI build
#include <cstdint>

namespace v8 {
namespace base {
namespace bits {

// Add missing CountTrailingZeros functions that complement what's in nuclear-fix.h
template <typename T>
constexpr unsigned CountTrailingZeros(T value) {
  if (value == 0) return sizeof(T) * 8;
  
  unsigned count = 0;
  while ((value & 1) == 0) {
    value >>= 1;
    count++;
  }
  return count;
}

// Specialized versions for better performance
inline constexpr unsigned CountTrailingZeros(uint32_t value) {
  if (value == 0) return 32;
#ifdef __has_builtin
#if __has_builtin(__builtin_ctz)
  return __builtin_ctz(value);
#endif
#endif
  return CountTrailingZeros<uint32_t>(value);
}

inline constexpr unsigned CountTrailingZeros(uint64_t value) {
  if (value == 0) return 64;
#ifdef __has_builtin
#if __has_builtin(__builtin_ctzll)
  return __builtin_ctzll(value);
#endif
#endif
  return CountTrailingZeros<uint64_t>(value);
}

inline constexpr unsigned CountTrailingZeros(uint16_t value) {
  if (value == 0) return 16;
  return CountTrailingZeros(static_cast<uint32_t>(value));
}

inline constexpr unsigned CountTrailingZeros(uint8_t value) {
  if (value == 0) return 8;
  return CountTrailingZeros(static_cast<uint32_t>(value));
}

}  // namespace bits
}  // namespace base
}  // namespace v8

#endif  // WASI_V8_BITS_FIXES_H_