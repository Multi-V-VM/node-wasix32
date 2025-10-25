#ifdef __wasi__
#include "wasi/std-preinclude.h"
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
struct has_output_operator : ::std::false_type {};

template <typename T>
struct has_output_operator<T, ::std::ostream>
    : ::std::integral_constant<bool,
                               ::std::is_arithmetic<T>::value ||
                               ::std::is_same<T, ::std::string>::value ||
                               ::std::is_convertible_v<T, const char*>> {};

#else

// Original concepts-based implementation for non-WASI
template <typename T, typename TStream>
concept has_output_operator = requires(T t, TStream stream) { stream << t; };

#endif

// Forward declarations for tuple helpers
namespace detail {
template <::std::size_t N, typename Tuple>
struct NIsNotGreaterThanTupleSize
    : ::std::bool_constant<(N <= ::std::tuple_size_v<Tuple>)> {};
} // namespace detail

// Make tuple access WASI-compatible
template <::std::size_t N, typename Tuple>
#ifdef __wasi__
typename ::std::enable_if_t<detail::NIsNotGreaterThanTupleSize<N, Tuple>::value,
                            ::std::tuple_element_t<N, Tuple>&>
#else
  requires(detail::NIsNotGreaterThanTupleSize<N, Tuple>)
::std::tuple_element_t<N, Tuple>&
#endif
get(Tuple& tuple) {
  return ::std::get<N>(tuple);
}

// Rest of template utilities...
template<typename T>
struct remove_cvref {
  using type = typename ::std::remove_cv_t<::std::remove_reference_t<T>>;
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

// Tuple meta utilities used by Turboshaft and other internals.
// tuple_head<std::tuple<Ts...>> -> first type
template <typename Tuple>
struct tuple_head;
template <typename T, typename... Ts>
struct tuple_head<::std::tuple<T, Ts...>> { using type = T; };
template <typename Tuple>
using tuple_head_t = typename tuple_head<Tuple>::type;

// prepend_tuple_type<T, std::tuple<Ts...>> -> std::tuple<T, Ts...>
template <typename T, typename Tuple>
struct prepend_tuple_type_impl;
template <typename T, typename... Ts>
struct prepend_tuple_type_impl<T, ::std::tuple<Ts...>> { using type = ::std::tuple<T, Ts...>; };
template <typename T, typename Tuple>
using prepend_tuple_type = typename prepend_tuple_type_impl<T, Tuple>::type;

// tuple_drop<N, std::tuple<...>> drops first N types
template <::std::size_t N, typename Tuple>
struct tuple_drop_impl;
template <::std::size_t N, typename T, typename... Ts>
struct tuple_drop_impl<N, ::std::tuple<T, Ts...>> { using type = typename tuple_drop_impl<N-1, ::std::tuple<Ts...>>::type; };
template <typename T, typename... Ts>
struct tuple_drop_impl<0, ::std::tuple<T, Ts...>> { using type = ::std::tuple<T, Ts...>; };
template <::std::size_t N>
struct tuple_drop_impl<N, ::std::tuple<>> { using type = ::std::tuple<>; };
template <::std::size_t N, typename Tuple>
using tuple_drop = typename tuple_drop_impl<N, Tuple>::type;

// Runtime helpers for tuple manipulation. Use distinct names to avoid
// colliding with type aliases tuple_head_t / tuple_drop.
template <::std::size_t... I, typename Tuple>
auto tuple_take_rt_impl(const Tuple& t, ::std::index_sequence<I...>) {
  return ::std::make_tuple(::std::get<I>(t)...);
}

template <::std::size_t N, typename Tuple>
auto base_tuple_head_rt(const Tuple& t) {
  return tuple_take_rt_impl(t, ::std::make_index_sequence<N>{});
}

template <::std::size_t N, typename Tuple, ::std::size_t... I>
auto tuple_drop_rt_impl(const Tuple& t, ::std::index_sequence<I...>) {
  return ::std::make_tuple(::std::get<I + N>(t)...);
}

template <::std::size_t N, typename Tuple>
auto base_tuple_drop_rt(const Tuple& t) {
  constexpr ::std::size_t S = ::std::tuple_size_v<::std::remove_reference_t<Tuple>>;
  static_assert(N <= S, "base_tuple_drop_rt out of range");
  return tuple_drop_rt_impl<N>(t, ::std::make_index_sequence<S - N>{});
}

// ---------------------------------------------------------------------------
// Utility: base::make_array<N>(F)
// Creates a std::array by invoking the generator F for indices [0, N).
// This is used by wasm/value-type.h and others.
template <::std::size_t N, typename F>
constexpr auto make_array(F&& f) {
  using T = decltype(f(static_cast<::std::size_t>(0)));
  ::std::array<T, N> out{};
  for (::std::size_t i = 0; i < N; ++i) out[i] = f(i);
  return out;
}

// Tuple utilities used by Turboshaft and others
namespace detail {
template <class Tuple, class F, ::std::size_t... I>
auto tuple_map_impl(Tuple& t, F&& f, ::std::index_sequence<I...>) {
  return ::std::make_tuple(::std::forward<F>(f)(::std::get<I>(t))...);
}

template <class Tuple1, class Tuple2, class F, ::std::size_t... I>
auto tuple_map2_impl(Tuple1& t1, Tuple2& t2, F&& f, ::std::index_sequence<I...>) {
  return ::std::make_tuple(::std::forward<F>(f)(::std::get<I>(t1), ::std::get<I>(t2))...);
}

template <class Acc, class Tuple, class F, ::std::size_t... I>
auto tuple_fold_impl(Acc acc, Tuple& t, F&& f, ::std::index_sequence<I...>) {
  // Fold left in order
  auto fold_one = [&](auto&& a, auto&& x) { return ::std::forward<F>(f)(::std::forward<decltype(a)>(a), ::std::forward<decltype(x)>(x)); };
  // Expand manually to preserve order
  (void)::std::initializer_list<int>{(acc = fold_one(acc, ::std::get<I>(t)), 0)...};
  return acc;
}
}  // namespace detail

template <class Tuple, class F>
auto tuple_map(Tuple& t, F&& f) {
  return detail::tuple_map_impl(t, ::std::forward<F>(f), ::std::make_index_sequence<::std::tuple_size_v<::std::remove_reference_t<Tuple>>>{});
}

template <class Tuple1, class Tuple2, class F>
auto tuple_map2(Tuple1& t1, Tuple2& t2, F&& f) {
  constexpr ::std::size_t N = ::std::tuple_size_v<::std::remove_reference_t<Tuple1>>;
  static_assert(N == ::std::tuple_size_v<::std::remove_reference_t<Tuple2>>);
  return detail::tuple_map2_impl(t1, t2, ::std::forward<F>(f), ::std::make_index_sequence<N>{});
}

template <class Acc, class Tuple, class F>
auto tuple_fold(Acc acc, Tuple& t, F&& f) {
  return detail::tuple_fold_impl(::std::move(acc), t, ::std::forward<F>(f), ::std::make_index_sequence<::std::tuple_size_v<::std::remove_reference_t<Tuple>>>{});
}

}  // namespace base
}  // namespace v8

// Intentionally avoid re-opening nested namespaces here. Some translation
// units include this header from within a `namespace v8` block, and re-opening
// `namespace v8` here would create an unintended `v8::v8::...` scope.


#endif  // V8_BASE_TEMPLATE_UTILS_H_
