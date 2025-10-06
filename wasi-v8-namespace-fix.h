#ifndef WASI_V8_NAMESPACE_FIX_H_
#define WASI_V8_NAMESPACE_FIX_H_
#include <cstdint>
#include <vector>
#include <cstddef>
#include <type_traits>
#include <cstdarg>
#include <iterator>

// Check if Vector class is already defined by wasi-v8-base-fixes.h
#ifdef WASI_V8_VECTOR_CLASS_DEFINED
// Skip Vector definition since it's already defined as a class
#else
#define WASI_V8_VECTOR_AS_USING_ALIAS
#endif

// Fix namespace issues for V8 internal types

// Forward declare frequently used API types so we can import them into
// v8::internal when necessary.
namespace v8 {
namespace base {
class OS;
class Semaphore;
template <typename T>
class ScopedVector;
template <typename T>
class Vector;
template <typename T>
class AtomicValue;
class Double;  // Forward declare Double class from src/base/numbers/double.h
class ElapsedTimer;  // Forward declare from platform/elapsed-timer.h
class TimeTicks;  // Forward declare from platform/time.h
class TimeDelta;  // Forward declare from platform/time.h

int SNPrintF(Vector<char> str, const char* format, ...);
int VSNPrintF(Vector<char> str, const char* format, va_list args);

template <typename T, size_t N>
inline constexpr Vector<T> ArrayVector(T (&arr)[N]);

template <typename T>
inline constexpr Vector<T> VectorOf(T* start, size_t size);

template <typename Container>
inline constexpr auto VectorOf(Container&& container)
    -> decltype(VectorOf(std::data(container), std::size(container)));
}  // namespace base

class ArrayBuffer;
class BackingStore;
class PageAllocator;
class RegExp;
class TypedArray;
namespace metrics {
struct Recorder;
}  // namespace metrics
namespace internal {
class DoubleRegister;
}  // namespace internal
}  // namespace v8

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

// Surface common platform utilities expected under v8::internal::base.
using ::v8::base::OS;
using ::v8::base::Semaphore;
template <typename T>
using AtomicValue = ::v8::base::AtomicValue<T>;
template <typename T>
using ScopedVector = ::v8::base::ScopedVector<T>;
using ::v8::base::ArrayVector;
using ::v8::base::VectorOf;

// Import additional commonly used types from v8::base
// SNPrintF and related functions
using ::v8::base::SNPrintF;
using ::v8::base::VSNPrintF;

// Import Double class (defined in src/base/numbers/double.h)
using ::v8::base::Double;

// Import timer classes
using ::v8::base::ElapsedTimer;
using ::v8::base::TimeTicks;
using ::v8::base::TimeDelta;

// BitField types - import from v8::base namespace
// BitField64 is already defined in wasi-v8-bits-fixes.h
using ::v8::base::BitField;

// Import BitField8 template alias (defined in src/base/bit-field.h)
template <class T, int shift, int size>
using BitField8 = ::v8::base::BitField<T, shift, size, uint8_t>;

// Character and integer helpers used throughout parser/regexp code.
typedef uint16_t uc16;
typedef uint32_t uc32;

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

namespace v8 {
namespace internal {

// Ensure references to v8::Something inside v8::internal resolve to the API
// namespace rather than recursively to v8::internal::v8::... during lookup.
namespace v8 = ::v8;

// Bring additional namespaces frequently referenced from internal code.
namespace metrics = ::v8::metrics;

}  // namespace internal
}  // namespace v8

// AtomicWord is already defined in nuclear-fix.h and atomicops.h
// Don't redefine it here

#endif // WASI_V8_NAMESPACE_FIX_H_
