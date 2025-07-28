#ifndef V8_BASE_BITS_NAMESPACE_FIX_H_
#define V8_BASE_BITS_NAMESPACE_FIX_H_

#include <stdint.h>

namespace v8 {
namespace base {
namespace bits {

// Rotation functions
inline uint32_t RotateRight32(uint32_t value, uint32_t shift) {
  return (value >> shift) | (value << ((32 - shift) & 31));
}

inline uint32_t RotateLeft32(uint32_t value, uint32_t shift) {
  return (value << shift) | (value >> ((32 - shift) & 31));
}

inline uint64_t RotateRight64(uint64_t value, uint32_t shift) {
  return (value >> shift) | (value << ((64 - shift) & 63));
}

inline uint64_t RotateLeft64(uint64_t value, uint32_t shift) {
  return (value << shift) | (value >> ((64 - shift) & 63));
}

}  // namespace bits
}  // namespace base
}  // namespace v8

#endif  // V8_BASE_BITS_NAMESPACE_FIX_H_