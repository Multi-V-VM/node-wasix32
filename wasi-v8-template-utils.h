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
// Match the signature used in v8/src/base/flags.h
template <typename EnumT, typename BitfieldT = int,
          typename BitfieldStorageT = BitfieldT>
// class Flags {
//  public:
//   using flag_type = EnumT;
//   using mask_type = BitfieldT;
//   
//   constexpr Flags() : value_(0) {}
//   constexpr explicit Flags(EnumT value) : value_(static_cast<BitfieldT>(value)) {}
//   constexpr explicit Flags(BitfieldT value) : value_(value) {}
//   
//   constexpr BitfieldT value() const { return value_; }
//   constexpr operator BitfieldT() const { return value_; }
//   
//   constexpr Flags operator|(Flags other) const {
//     return Flags(value_ | other.value_);
//   }
//   
//   constexpr Flags operator&(Flags other) const {
//     return Flags(value_ & other.value_);
//   }
//   
//   constexpr Flags operator^(Flags other) const {
//     return Flags(value_ ^ other.value_);
//   }
//   
//   constexpr Flags operator~() const {
//     return Flags(~value_);
//   }
//   
//   constexpr Flags& operator|=(Flags other) {
//     value_ |= other.value_;
//     return *this;
//   }
//   
//   constexpr Flags& operator&=(Flags other) {
//     value_ &= other.value_;
//     return *this;
//   }
//   
//   constexpr Flags& operator^=(Flags other) {
//     value_ ^= other.value_;
//     return *this;
//   }
//   
//   constexpr bool operator==(Flags other) const {
//     return value_ == other.value_;
//   }
//   
//   constexpr bool operator!=(Flags other) const {
//     return value_ != other.value_;
//   }
//   
//  private:
//   BitfieldT value_;
// };

}  // namespace base
}  // namespace v8

#endif  // WASI_V8_TEMPLATE_UTILS_H_