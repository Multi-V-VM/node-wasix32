#ifdef __wasi__
#include "wasi/cpp20-compat.h"
#endif

// Copyright 2017 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_TEMPLATE_UTILS_H_
#define V8_BASE_TEMPLATE_UTILS_H_

#include <type_traits>
#include <utility>
#include <tuple>
#include <array>

#include "src/base/compiler-specific.h"
#include <ostream>

namespace v8 {
namespace base {

// WASI-compatible implementation without concepts
#ifdef __wasi__

// Simple type trait for output operator checking
template <typename T, typename TStream>
struct has_output_operator : std::false_type {};

template <typename T>
struct has_output_operator<T, std::ostream>
    : std::integral_constant<bool,
                             std::is_arithmetic<T>::value || 
                             std::is_same<T, std::string>::value ||
                             std::is_convertible_v<T, const char*>> {};

#else

// Original concepts-based implementation for non-WASI
template <typename T, typename TStream>
concept has_output_operator = requires(T t, TStream stream) { stream << t; };

#endif

// Forward declarations for tuple helpers
namespace detail {
template <size_t N, typename Tuple>
struct NIsNotGreaterThanTupleSize : std::bool_constant<(N <= std::tuple_size_v<Tuple>)> {};
} // namespace detail

// Make tuple access WASI-compatible
template <size_t N, typename Tuple>
#ifdef __wasi__
typename std::enable_if_t<detail::NIsNotGreaterThanTupleSize<N, Tuple>::value,
                          std::tuple_element_t<N, Tuple>&>
#else
  requires(detail::NIsNotGreaterThanTupleSize<N, Tuple>)
std::tuple_element_t<N, Tuple>&
#endif
get(Tuple& tuple) {
  return ::std::get<N>(tuple);
}

// Rest of template utilities...
template<typename T>
struct remove_cvref {
  using type = typename std::remove_cv_t<std::remove_reference_t<T>>;
};

template<typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

// Provide simple meta utilities used by discriminated-union and others.
// index_of_type_v<T, Ts...> gives the index of T in the parameter pack Ts...
template <typename T, typename... Ts>
struct index_of_type;

template <typename T, typename U, typename... Ts>
struct index_of_type<T, U, Ts...> {
  static constexpr size_t value = 1 + index_of_type<T, Ts...>::value;
};

template <typename T, typename... Ts>
struct index_of_type<T, T, Ts...> {
  static constexpr size_t value = 0;
};

template <typename T>
struct index_of_type<T>;

template <typename T, typename... Ts>
inline constexpr size_t index_of_type_v = index_of_type<T, Ts...>::value;

// nth_type_t<N, Ts...> gives the Nth type in Ts...
template <size_t N, typename T, typename... Ts>
struct nth_type {
  using type = typename nth_type<N - 1, Ts...>::type;
};

template <typename T, typename... Ts>
struct nth_type<0, T, Ts...> {
  using type = T;
};

template <size_t N, typename... Ts>
using nth_type_t = typename nth_type<N, Ts...>::type;

// ---------------------------------------------------------------------------
// Utility: base::make_array<N>(F)
// Creates a std::array by invoking the generator F for indices [0, N).
// This is used by wasm/value-type.h and others.
template <size_t N, typename F>
constexpr auto make_array(F&& f) {
  using T = decltype(f(static_cast<size_t>(0)));
  std::array<T, N> out{};
  for (size_t i = 0; i < N; ++i) out[i] = f(i);
  return out;
}

}  // namespace base
}  // namespace v8

// Bridge make_array into v8::internal::base for callers that use
// v8::internal::base::make_array without including globals.h.
namespace v8 { namespace internal { namespace base {
using ::v8::base::make_array;
} } }

// Bridge the meta-programming utilities namespace for code that refers to
// v8::internal::base::tmp.
// namespace v8 { namespace internal { namespace base {
// namespace tmp = ::v8::base::tmp;
// } } }


#endif  // V8_BASE_TEMPLATE_UTILS_H_
