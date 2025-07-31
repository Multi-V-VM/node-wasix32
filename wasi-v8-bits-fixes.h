#ifndef WASI_V8_BITS_FIXES_H_
#define WASI_V8_BITS_FIXES_H_

#ifdef __wasi__

#include <stdint.h>
#include <type_traits>

namespace v8 {
namespace base {

// BitField template for WASI builds - only define if not already defined
// NOTE: Check if BitField is already defined to avoid conflicts
#ifndef V8_BASE_BIT_FIELD_DEFINED
#define V8_BASE_BIT_FIELD_DEFINED
template<typename T, int start, int size, typename StorageType = uint32_t>
class BitField {
 public:
  static_assert(std::is_unsigned<StorageType>::value, "StorageType must be unsigned");
  static_assert(start >= 0 && size > 0, "Invalid bit field parameters");
  static_assert(start + size <= sizeof(StorageType) * 8, "Bit field exceeds storage size");
  
  static constexpr StorageType kMask = ((static_cast<StorageType>(1) << size) - 1) << start;
  static constexpr int kShift = start;
  static constexpr int kSize = size;
  static constexpr int kMax = (1 << size) - 1;
  
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
#endif // V8_BASE_BIT_FIELD_DEFINED

} // namespace base
} // namespace v8

// Also fix std namespace issues
namespace v8 {
namespace std {
using ::std::move_backward;
using ::std::move;
using ::std::forward;
using ::std::add_const;
} // namespace std
} // namespace v8

// Fix for namespace issues with Platform and other types
namespace v8 {
namespace internal {
// Define Address type for internal use
using Address = uintptr_t;

namespace v8 {
using ::v8::Platform;
using ::v8::PageAllocator; 
using ::v8::VirtualAddressSpace;
} // namespace v8
} // namespace internal
} // namespace v8

#endif // __wasi__

#endif // WASI_V8_BITS_FIXES_H_
