#ifndef WASI_V8_TEMPLATE_UTILS_H_
#define WASI_V8_TEMPLATE_UTILS_H_

// Template utilities for V8 WASI build
#include <type_traits>
#include <cstddef>

namespace v8 {
namespace base {

// Helper to get the Nth type from a parameter pack
template <size_t N, typename... Ts>
struct nth_type;

template <typename T, typename... Ts>
struct nth_type<0, T, Ts...> {
  using type = T;
};

template <size_t N, typename T, typename... Ts>
struct nth_type<N, T, Ts...> : nth_type<N - 1, Ts...> {};

template <size_t N, typename... Ts>
using nth_type_t = typename nth_type<N, Ts...>::type;

// Helper to find the index of a type in a parameter pack
template <typename T, typename... Ts>
struct index_of_type;

template <typename T, typename U, typename... Us>
struct index_of_type<T, U, Us...> {
  static constexpr size_t value = std::is_same_v<T, U> ? 0 : 1 + index_of_type<T, Us...>::value;
};

template <typename T>
struct index_of_type<T> {
  static constexpr size_t value = 0;
};

template <typename T, typename... Ts>
inline constexpr size_t index_of_type_v = index_of_type<T, Ts...>::value;

// Flags template for bit manipulation
template <typename T>
class Flags {
 public:
  using flag_type = T;
  using mask_type = T;
  
  constexpr Flags() : value_(0) {}
  constexpr explicit Flags(T value) : value_(value) {}
  
  constexpr T value() const { return value_; }
  constexpr operator T() const { return value_; }
  
  constexpr Flags operator|(Flags other) const {
    return Flags(value_ | other.value_);
  }
  
  constexpr Flags operator&(Flags other) const {
    return Flags(value_ & other.value_);
  }
  
  constexpr Flags operator^(Flags other) const {
    return Flags(value_ ^ other.value_);
  }
  
  constexpr Flags operator~() const {
    return Flags(~value_);
  }
  
  constexpr Flags& operator|=(Flags other) {
    value_ |= other.value_;
    return *this;
  }
  
  constexpr Flags& operator&=(Flags other) {
    value_ &= other.value_;
    return *this;
  }
  
  constexpr Flags& operator^=(Flags other) {
    value_ ^= other.value_;
    return *this;
  }
  
  constexpr bool operator==(Flags other) const {
    return value_ == other.value_;
  }
  
  constexpr bool operator!=(Flags other) const {
    return value_ != other.value_;
  }
  
 private:
  T value_;
};

}  // namespace base
}  // namespace v8

#endif  // WASI_V8_TEMPLATE_UTILS_H_