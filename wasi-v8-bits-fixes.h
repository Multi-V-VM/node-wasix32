#ifndef WASI_V8_BITS_FIXES_H_
#define WASI_V8_BITS_FIXES_H_

#ifdef __wasi__

// Only define BitField if V8's version hasn't been included yet
#ifndef V8_BASE_BIT_FIELD_H_
#define V8_BASE_BIT_FIELD_H_

#include <stdint.h>
#include <type_traits>

namespace v8 {
namespace base {

// BitField template for WASI builds
template<typename T, int start, int size, typename StorageType = uint32_t>
class BitField final {
 public:
  static_assert(std::is_unsigned<StorageType>::value, "StorageType must be unsigned");
  static_assert(start >= 0 && size > 0, "Invalid bit field parameters");
  static_assert(start + size <= sizeof(StorageType) * 8, "Bit field exceeds storage size");
  
  static constexpr StorageType kMask = ((static_cast<StorageType>(1) << size) - 1) << start;
  static constexpr int kShift = start;
  static constexpr int kSize = size;
  static constexpr int kMax = (1 << size) - 1;
  static constexpr int kLastUsedBit = start + size - 1;
  
  static constexpr T decode(StorageType value) {
    return static_cast<T>((value & kMask) >> kShift);
  }
  
  static constexpr StorageType encode(T value) {
    return (static_cast<StorageType>(value) << kShift) & kMask;
  }
  
  static constexpr StorageType update(StorageType previous, T value) {
    return (previous & ~kMask) | encode(value);
  }
  
  // Tells whether the provided value fits into the bit field.
  static constexpr bool is_valid(T value) {
    return (static_cast<StorageType>(value) & ~kMax) == 0;
  }
  
  // Next template for chaining BitFields
  template<typename NextT, int next_size>
  using Next = BitField<NextT, start + size, next_size, StorageType>;
};

// BitField64 is just BitField with uint64_t storage
template<typename T, int start, int size>
using BitField64 = BitField<T, start, size, uint64_t>;

} // namespace base
} // namespace v8

#endif // V8_BASE_BIT_FIELD_H_

// Also fix std namespace issues - conditionally include std functions
#include <utility>
#include <algorithm>
#include <type_traits>
#include <array>

namespace v8 {
namespace std {
#ifdef __cpp_lib_move_iterator
using ::std::move_backward;
using ::std::move;
#endif
using ::std::forward;
using ::std::add_const;
} // namespace std
} // namespace v8

// Fix for namespace issues with Platform and other types
namespace v8 {
namespace internal {
// Define Address type for internal use
using Address = uintptr_t;

// Create base namespace alias to avoid conflicts
namespace base {
// Re-export BitField from ::v8::base to ::v8::internal::base
using ::v8::base::BitField;

// BitField64 is just BitField with uint64_t storage
template<typename T, int start, int size>
using BitField64 = ::v8::base::BitField<T, start, size, uint64_t>;

// Add make_array utility for WASI builds
template<size_t N, typename F>
constexpr auto make_array(F&& f) -> std::array<decltype(f(0)), N> {
  return []<size_t... I>(F&& f, std::index_sequence<I...>) {
    return std::array<decltype(f(0)), N>{{f(I)...}};
  }(std::forward<F>(f), std::make_index_sequence<N>{});
}

} // namespace base

} // namespace internal
} // namespace v8

#endif // __wasi__

#endif // WASI_V8_BITS_FIXES_H_
