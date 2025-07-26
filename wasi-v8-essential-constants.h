#ifndef WASI_V8_ESSENTIAL_CONSTANTS_H_
#define WASI_V8_ESSENTIAL_CONSTANTS_H_

// Essential constants for V8 WASI build

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

}  // namespace base

}  // namespace internal
}  // namespace v8

#endif  // WASI_V8_ESSENTIAL_CONSTANTS_H_
