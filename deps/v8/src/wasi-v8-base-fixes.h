#ifndef SRC_WASI_V8_BASE_FIXES_H_
#define SRC_WASI_V8_BASE_FIXES_H_

#ifdef __wasi__

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <mutex>

// Forward declare v8::internal types
namespace v8 {
namespace internal {
// Address type should already be defined
using Address = uintptr_t;
}  // namespace internal
}  // namespace v8

// Add missing base namespace types
namespace v8 {
namespace internal {
namespace base {

// Alias Address from v8::internal
using Address = ::v8::internal::Address;

// Add missing type traits
template<typename T, typename... Ts>
struct has_type;

template<typename T>
struct has_type<T> : std::false_type {};

template<typename T, typename Head, typename... Tail>
struct has_type<T, Head, Tail...> 
    : std::conditional_t<std::is_same_v<T, Head>, 
                         std::true_type, 
                         has_type<T, Tail...>> {};

template<typename T, typename... Ts>
inline constexpr bool has_type_v = has_type<T, Ts...>::value;

// Vector type (simple wrapper that doesn't use std::vector to avoid const issues)
template<typename T>
class Vector {
 public:
  Vector() = default;
  
  // Constructor for non-const types
  template<typename U = T>
  Vector(typename ::std::enable_if<!::std::is_const<U>::value, U>::type* data, size_t length) 
      : data_(data), length_(length) {}
  
  // Constructor for const types - stores as const pointer
  template<typename U = T>
  Vector(typename std::enable_if<std::is_const<U>::value, U>::type* data, size_t length)
      : data_(const_cast<typename std::remove_const<T>::type*>(data)), length_(length) {}
  
  // Implicit conversion from pointer and length (common V8 pattern)
  Vector(T* data, size_t length) : data_(data), length_(length) {}
  
  T* data() const { return data_; }
  size_t length() const { return length_; }
  size_t size() const { return length_; }
  
  T& operator[](size_t i) { return data_[i]; }
  const T& operator[](size_t i) const { return data_[i]; }
  
  T* begin() { return data_; }
  T* end() { return data_ + length_; }
  const T* begin() const { return data_; }
  const T* end() const { return data_ + length_; }
  
  bool IsEmpty() const { return length_ == 0; }
  
 private:
  typename std::conditional<
    std::is_const<T>::value,
    typename ::std::remove_const<T>::type*,
    T*
  >::type data_ = nullptr;
  size_t length_ = 0;
};

// Mutex types
// Note: These are defined in wasi-consolidated-fixes.h now
// using Mutex = std::mutex;
// using MutexGuard = std::lock_guard<std::mutex>;

// Placeholder for BoundedPageAllocator
class BoundedPageAllocator {
 public:
  BoundedPageAllocator() = default;
  ~BoundedPageAllocator() = default;
};

// Add missing template types that cause namespace conflicts
template<typename T>
struct hash : public std::hash<T> {};

template<typename T>
struct bit_hash : public std::hash<T> {};

template<typename T>
struct bit_equal_to : public std::equal_to<T> {};

template<typename T, typename U>
class Flags {
 public:
  Flags() = default;
};

// EnumSet is now defined in v8/src/base/enum-set.h
// Remove duplicate definition to avoid conflicts

class AddressRegion {
 public:
  AddressRegion() = default;
  AddressRegion(Address addr, size_t sz) : address_(addr), size_(sz) {}
  
  Address address() const { return address_; }
  size_t size() const { return size_; }
  
 private:
  Address address_ = 0;
  size_t size_ = 0;
};

// Memory allocation types
enum class PageInitializationMode { kUninitialized, kZeroInitialized };
enum class PageFreeingMode { kMakeInaccessible, kDiscard };

// AllocationResult is already defined in memory.h - don't redefine
// template <class Pointer>
// struct AllocationResult {
//   Pointer ptr = nullptr;
//   size_t count = 0;
// };

// Function stubs
inline void* Malloc(size_t size) { return malloc(size); }
inline void Free(void* ptr) { free(ptr); }

// Use a simple allocation result structure
template<typename T>
struct AllocationResult {
  T* ptr;
  size_t count;
};

template<typename T>
inline AllocationResult<T*> AllocatePages(size_t size) {
  T* ptr = static_cast<T*>(malloc(size * sizeof(T)));
  return {ptr, ptr ? size : 0};
}

inline void FreePages(void* address, size_t size) { free(address); }

inline size_t hash_combine(size_t seed, size_t hash) {
  return seed ^ (hash + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

template<typename T>
using is_trivially_copyable = std::is_trivially_copyable<T>;

namespace bits {
  inline unsigned CountLeadingZeros(uint32_t value) {
    return value ? __builtin_clz(value) : 32;
  }
  inline unsigned CountPopulation(uint32_t value) {
    return __builtin_popcount(value);
  }
  inline unsigned CountPopulation(uint16_t value) {
    return __builtin_popcount(value);
  }
  // CountTrailingZeros functions removed - using existing V8 implementations
  inline uint32_t RoundUpToPowerOfTwo32(uint32_t value) {
    if (value <= 1) return 1;
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return value + 1;
  }
}

// iterator is already defined in wasi-v8-namespace-fix.h
// Don't redefine it here

}  // namespace base

}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // SRC_WASI_V8_BASE_FIXES_H_