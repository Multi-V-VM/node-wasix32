#ifndef WASI_V8_NAMESPACE_FIX_H_
#define WASI_V8_NAMESPACE_FIX_H_
#include <cstdint>
#include <vector>
#include <cstddef>

// Check if Vector class is already defined by wasi-v8-base-fixes.h
#ifdef WASI_V8_VECTOR_CLASS_DEFINED
// Skip Vector definition since it's already defined as a class
#else
#define WASI_V8_VECTOR_AS_USING_ALIAS
#endif

// Fix namespace issues for V8 internal types

// Forward declare LockGuard template and Mutex classes
// These are implemented in wasi-v8-base-fixes.h
namespace v8 {
namespace base {
class Mutex;
template <typename MutexType> class LockGuard;
using MutexGuard = LockGuard<Mutex>;

template <class T, int shift, int size, class U>
class BitField;

// BitField64 is just BitField with uint64_t
template <class T, int shift, int size>
using BitField64 = BitField<T, shift, size, uint64_t>;
}
}

namespace v8 {
namespace internal {
namespace base {

// Add missing namespace aliases
using Mutex = ::v8::base::Mutex;
using MutexGuard = ::v8::base::MutexGuard;

// BitField types - import from v8::base namespace
// BitField64 is already defined in wasi-v8-bits-fixes.h
using ::v8::base::BitField;

// ReadUnalignedValue is provided by other WASI compatibility files if needed

// AddressRegion is defined in v8::internal::base namespace in wasi-v8-base-fixes.h
class AddressRegion;

// Vector is defined as a class in wasi-v8-base-fixes.h
// Don't define it here to avoid conflicts

// Add bit_cast function template
template <typename To, typename From>
inline To bit_cast(const From& from) {
  static_assert(sizeof(To) == sizeof(From), "bit_cast requires same size types");
  static_assert(std::is_trivially_copyable_v<To>, "To must be trivially copyable");
  static_assert(std::is_trivially_copyable_v<From>, "From must be trivially copyable");
  
#if __has_builtin(__builtin_bit_cast)
  return __builtin_bit_cast(To, from);
#else
  // Fallback implementation using union
  union {
    From f;
    To t;
  } u;
  u.f = from;
  return u.t;
#endif
}

// Add bits namespace functions - provide implementations here
namespace bits {
// Note: CountPopulation and CountLeadingZeros functions are already defined in wasi-v8-base-fixes.h
// Don't redefine them here to avoid conflicts

// CountTrailingZeros functions are already defined in wasi-v8-base-fixes.h
// Don't redefine them here to avoid conflicts

inline int CountTrailingZerosNonZero(uint32_t value) {
  return __builtin_ctz(value);
}

inline unsigned CountTrailingZerosNonZero(uint64_t value) {
  return __builtin_ctzll(value);
}

// RoundUpToPowerOfTwo functions are already defined in wasi-v8-base-fixes.h
// Don't redefine them here to avoid conflicts

// Note: uint64_t RoundUpToPowerOfTwo and RoundUpToPowerOfTwo64 are defined in wasi-v8-base-fixes.h
// to avoid redefinition conflicts
} // namespace bits

// Add iterator template - std::iterator is deprecated in C++17
// Define a custom iterator struct instead
template<typename Category, typename T, typename Distance = std::ptrdiff_t,
         typename Pointer = T*, typename Reference = T&>
struct iterator {
  using iterator_category = Category;
  using value_type = T;
  using difference_type = Distance;
  using pointer = Pointer;
  using reference = Reference;
};

// Add IsInRange function
template <typename T>
inline bool IsInRange(T value, T lower_limit, T upper_limit) {
  return lower_limit <= value && value <= upper_limit;
}

// Import external pointer constants from internal namespace
static constexpr uintptr_t kExternalPointerTagShift = 48;

// Add ConditionVariable - it's already defined in v8::base namespace

} // namespace base
} // namespace internal
} // namespace v8

// AtomicWord is already defined in nuclear-fix.h and atomicops.h
// Don't redefine it here

#endif // WASI_V8_NAMESPACE_FIX_H_