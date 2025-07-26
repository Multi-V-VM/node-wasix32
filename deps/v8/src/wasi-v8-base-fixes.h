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
  Vector(typename std::enable_if<!std::is_const<U>::value, U>::type* data, size_t length) 
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
    typename std::remove_const<T>::type*,
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

template<typename T, typename U>
class EnumSet {
 public:
  EnumSet() = default;
};

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

struct AllocationResult {
  void* ptr;
  size_t size;
  AllocationResult() : ptr(nullptr), size(0) {}
  AllocationResult(void* p, size_t s) : ptr(p), size(s) {}
};

// Function stubs
inline void* Malloc(size_t size) { return malloc(size); }
inline void Free(void* ptr) { free(ptr); }
inline AllocationResult AllocatePages(size_t size) {
  return AllocationResult(malloc(size), size);
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
}

// Add iterator template for missing std::iterator usage
template<typename Category, typename T, typename Distance = std::ptrdiff_t, 
         typename Pointer = T*, typename Reference = T&>
using iterator = std::iterator<Category, T, Distance, Pointer, Reference>;

}  // namespace base

}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // SRC_WASI_V8_BASE_FIXES_H_