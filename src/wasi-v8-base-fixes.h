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

// Vector type (simple wrapper around std::vector)
template<typename T>
class Vector {
 public:
  Vector() = default;
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
  
 private:
  T* data_ = nullptr;
  size_t length_ = 0;
};

// Mutex types
using Mutex = std::mutex;
using MutexGuard = std::lock_guard<std::mutex>;

// Placeholder for BoundedPageAllocator
class BoundedPageAllocator {
 public:
  BoundedPageAllocator() = default;
  ~BoundedPageAllocator() = default;
};

}  // namespace base
}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // SRC_WASI_V8_BASE_FIXES_H_