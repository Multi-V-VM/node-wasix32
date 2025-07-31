#ifndef WASI_V8_ESSENTIAL_CONSTANTS_H_
#define WASI_V8_ESSENTIAL_CONSTANTS_H_

// Essential constants for V8 WASI build
#include <limits>
#include <functional>

namespace v8 {
namespace internal {

// Only define if not already defined
#ifndef kSmiTag
constexpr int kSmiTag = 0;
#endif

// kSmiTagMask is already defined in wasi-v8-api-stubs.h

#ifndef kHeapObjectTag
constexpr int kHeapObjectTag = 1;
#endif

#ifndef kHeapObjectTagMask
constexpr intptr_t kHeapObjectTagMask = 3;
#endif

#ifndef kWeakHeapObjectTag
constexpr int kWeakHeapObjectTag = 3;
#endif

// For internal::base namespace - minimal additions
namespace base {

// saturated_cast implementation
template <typename To, typename From>
inline To saturated_cast(From from) {
  if (std::numeric_limits<To>::max() < from) {
    return std::numeric_limits<To>::max();
  }
  if (std::numeric_limits<To>::min() > from) {
    return std::numeric_limits<To>::min();
  }
  return static_cast<To>(from);
}

// Add missing hash functions
template<typename T>
inline size_t hash_value(const T& value) {
  return std::hash<T>{}(value);
}

template<typename T>
inline size_t hash_combine(size_t seed, const T& value) {
  return seed ^ (hash_value(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

// Missing helper functions
inline bool IsInRange(int value, int lower_limit, int higher_limit) {
  return lower_limit <= value && value <= higher_limit;
}

inline bool IsUint(int64_t value, int N) {
  return 0 <= value && value < (int64_t{1} << N);
}

constexpr bool is_constant_evaluated() noexcept { return false; }

}  // namespace base

}  // namespace internal

// Also add to v8::base for torque compiler
namespace base {

// Add missing hash functions
template<typename T>
inline size_t hash_value(const T& value) {
  return std::hash<T>{}(value);
}

template<typename T>
inline size_t hash_combine(size_t seed, const T& value) {
  return seed ^ (hash_value(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

}  // namespace base

}  // namespace v8

#endif  // WASI_V8_ESSENTIAL_CONSTANTS_H_
