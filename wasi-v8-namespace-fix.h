#ifndef WASI_V8_NAMESPACE_FIX_H_
#define WASI_V8_NAMESPACE_FIX_H_
#include <cstdint>

// Fix namespace issues for V8 internal types

// Forward declare LockGuard template
namespace v8 {
namespace base {
template <typename Mutex> class LockGuard;
class Mutex;
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

template <typename T, int shift, int size>
using BitField64 = ::v8::base::BitField64<T, shift, size>;

template <class T, int shift, int size, class U = uint32_t>
using BitField = ::v8::base::BitField<T, shift, size, U>;

// AddressRegion is defined in v8::internal::base namespace in wasi-v8-base-fixes.h
class AddressRegion;

// Add bits namespace functions - forward declarations only to avoid conflicts
namespace bits {
// Forward declarations - actual implementations are in wasi-v8-base-fixes.h or nuclear-fix.h
unsigned CountPopulation(uint32_t value);
unsigned CountPopulation(uint16_t value);
unsigned CountLeadingZeros(uint32_t value);
unsigned CountTrailingZerosNonZero(uint64_t value);
unsigned CountTrailingZeros(uint32_t value);
unsigned CountTrailingZeros(uint64_t value);
uint32_t RoundUpToPowerOfTwo32(uint32_t value);
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

// Add ConditionVariable - it's already defined in v8::base namespace

} // namespace base
} // namespace internal
} // namespace v8

// AtomicWord is already defined in nuclear-fix.h and atomicops.h
// Don't redefine it here

#endif // WASI_V8_NAMESPACE_FIX_H_