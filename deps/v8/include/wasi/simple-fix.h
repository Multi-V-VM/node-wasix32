#ifndef WASI_SIMPLE_FIX_H
#define WASI_SIMPLE_FIX_H

#ifdef __wasi__

// Only include what we absolutely need
#include <cstddef>
#include <cstdint>
#include <memory>

// Disable problematic filesystem features
#define _LIBCPP_HAS_NO_FILESYSTEM_LIBRARY 1
#define _LIBCPP_HAS_NO_INCOMPLETE_CHRONO 1

// Define only the missing bit operations in the correct namespace
namespace v8 {
namespace base {
namespace bits {

inline uint32_t RotateRight32(uint32_t value, uint8_t shift) {
  return (value >> shift) | (value << (32 - shift));
}

inline bool IsPowerOfTwo(uint32_t value) {
  return value && !(value & (value - 1));
}

template<typename T, typename U>
inline T bit_cast(const U& from) {
  static_assert(sizeof(T) == sizeof(U), "Types must be same size");
  T result;
  memcpy(&result, &from, sizeof(T));
  return result;
}

} // namespace bits

// Missing platform functions
inline bool MainThreadIsCurrentThread() { return true; }
inline int64_t ClockNow(int) { return 0; }

} // namespace base

namespace internal {
// Only define missing V8 internal constants
constexpr int kSmiTagSize = 1;
constexpr int kSmiShiftSize = 0;
constexpr int kSmiValueSize = 31;
constexpr int kSystemPointerSize = sizeof(void*);
constexpr int kTaggedSize = sizeof(void*);
} // namespace internal

} // namespace v8

// Simple debug macro stubs
#ifndef DCHECK
#define DCHECK(condition) ((void)0)
#endif
#ifndef DCHECK_NOT_NULL
#define DCHECK_NOT_NULL(ptr) (ptr)
#endif
#ifndef CHECK
#define CHECK(condition) ((void)0)
#endif

#endif // __wasi__
#endif // WASI_SIMPLE_FIX_H