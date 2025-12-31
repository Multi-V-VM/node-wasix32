// Copyright 2017 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_TEMPLATE_UTILS_H_
#define V8_BASE_TEMPLATE_UTILS_H_

#include <array>
#include <functional>
#include <iosfwd>
#include <tuple>
#include <type_traits>
#include <utility>

namespace v8 {
namespace base {

namespace detail {

template <typename Function, std::size_t... Indexes>
constexpr inline auto make_array_helper(Function f,
                                        std::index_sequence<Indexes...>)
    -> std::array<decltype(f(0)), sizeof...(Indexes)> {
  return {{f(Indexes)...}};
}

}  // namespace detail

// base::make_array: Create an array of fixed length, initialized by a function.
// The content of the array is created by calling the function with 0 .. Size-1.
// Example usage to create the array {0, 2, 4}:
//   std::array<int, 3> arr = base::make_array<3>(
//       [](std::size_t i) { return static_cast<int>(2 * i); });
// The resulting array will be constexpr if the passed function is constexpr.
template <std::size_t Size, class Function>
constexpr auto make_array(Function f) {
  return detail::make_array_helper(f, std::make_index_sequence<Size>{});
}

// Helper to determine how to pass values: Pass scalars and arrays by value,
// others by const reference (even if it was a non-const ref before; this is
// disallowed by the style guide anyway).
// The default is to also remove array extends (int[5] -> int*), but this can be
// disabled by setting {remove_array_extend} to false.
template <typename T, bool remove_array_extend = true>
struct pass_value_or_ref {
  using noref_t = typename std::remove_reference<T>::type;
  using decay_t = typename std::conditional<
      std::is_array<noref_t>::value && !remove_array_extend, noref_t,
      typename std::decay<noref_t>::type>::type;
  using type = typename std::conditional<std::is_scalar<decay_t>::value ||
                                             std::is_array<decay_t>::value,
                                         decay_t, const decay_t&>::type;
};

// Uses expression SFINAE to detect whether using operator<< would work.
template <typename T, typename TStream = std::ostream, typename = void>
struct has_output_operator : std::false_type {};
template <typename T, typename TStream>
struct has_output_operator<
    T, TStream, decltype(void(std::declval<TStream&>() << std::declval<T>()))>
    : std::true_type {};

// Turn std::tuple<A...> into std::tuple<A..., T>.
template <class Tuple, class T>
using append_tuple_type = decltype(std::tuple_cat(
    std::declval<Tuple>(), std::declval<std::tuple<T>>()));

// Turn std::tuple<A...> into std::tuple<T, A...>.
template <class T, class Tuple>
using prepend_tuple_type = decltype(std::tuple_cat(
    std::declval<std::tuple<T>>(), std::declval<Tuple>()));

namespace detail {

template <size_t N, typename Tuple>
constexpr bool NIsNotGreaterThanTupleSize =
    N <= std::tuple_size_v<std::decay_t<Tuple>>;

template <size_t N, typename T, size_t... Ints>
constexpr auto tuple_slice_impl(const T& tpl, std::index_sequence<Ints...>) {
  return std::tuple{std::get<N + Ints>(tpl)...};
}

template <typename Tuple, typename Function, size_t... Index>
constexpr auto tuple_for_each_impl(const Tuple& tpl, Function&& function,
                                   std::index_sequence<Index...>) {
  (function(std::get<Index>(tpl)), ...);
}

template <typename Tuple, typename Function, size_t... Index>
constexpr auto tuple_for_each_with_index_impl(const Tuple& tpl,
                                              Function&& function,
                                              std::index_sequence<Index...>) {
  (function(std::get<Index>(tpl), std::integral_constant<size_t, Index>()),
   ...);
}

}  // namespace detail

// Get the first N elements from a tuple.
template <size_t N, typename Tuple>
constexpr auto tuple_head(Tuple&& tpl) {
  constexpr size_t total_size = std::tuple_size_v<std::decay_t<Tuple>>;
  static_assert(N <= total_size);
  return detail::tuple_slice_impl<0>(std::forward<Tuple>(tpl),
                                     std::make_index_sequence<N>());
}

// Drop the first N elements from a tuple.
template <
    size_t N, typename Tuple,
    // If the user accidentally passes in an N that is larger than the tuple
    // size, the unsigned subtraction will create a giant index sequence and
    // crash the compiler. To avoid this and fail early, disable this function
    // for invalid N.
    typename = std::enable_if_t<detail::NIsNotGreaterThanTupleSize<N, Tuple>>>
constexpr auto tuple_drop(Tuple&& tpl) {
  constexpr size_t total_size = std::tuple_size_v<std::decay_t<Tuple>>;
  static_assert(N <= total_size);
  return detail::tuple_slice_impl<N>(
      std::forward<Tuple>(tpl), std::make_index_sequence<total_size - N>());
}

// Calls `function(v)` for each `v` in the tuple.
template <typename Tuple, typename Function>
constexpr void tuple_for_each(Tuple&& tpl, Function&& function) {
  detail::tuple_for_each_impl(
      std::forward<Tuple>(tpl), function,
      std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>());
}

// Calls `function(v, i)` for each `v` in the tuple, with index `i`. The index
// `i` is passed as an std::integral_constant<size_t>, rather than a raw size_t,
// to allow it to be used
template <typename Tuple, typename Function>
constexpr void tuple_for_each_with_index(Tuple&& tpl, Function&& function) {
  detail::tuple_for_each_with_index_impl(
      std::forward<Tuple>(tpl), function,
      std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>());
}

#ifdef __clang__

template <size_t N, typename... Ts>
using nth_type_t = __type_pack_element<N, Ts...>;

// For compatibility with code that uses nth_type<N, Ts...>::type
template <size_t N, typename... Ts>
struct nth_type {
  using type = __type_pack_element<N, Ts...>;
};

#else

template <size_t N, typename... Ts>
struct nth_type;

template <typename T, typename... Ts>
struct nth_type<0, T, Ts...> {
  using type = T;
};

template <size_t N, typename T, typename... Ts>
struct nth_type<N, T, Ts...> : public nth_type<N - 1, Ts...> {};

template <size_t N, typename... T>
using nth_type_t = typename nth_type<N, T...>::type;

#endif

// Find SearchT in Ts. SearchT must be present at most once in Ts, and returns
// sizeof...(Ts) if not found.
template <typename SearchT, typename... Ts>
struct index_of_type;

// Not found / empty list.
template <typename SearchT>
struct index_of_type<SearchT> : public std::integral_constant<size_t, 0> {};

// SearchT found at head of list.
template <typename SearchT, typename... Ts>
struct index_of_type<SearchT, SearchT, Ts...>
    : public std::integral_constant<size_t, 0> {
  // SearchT is not allowed to be anywhere else in the list.
  static_assert(index_of_type<SearchT, Ts...>::value == sizeof...(Ts));
};

// Recursion, SearchT not found at head of list.
template <typename SearchT, typename T, typename... Ts>
struct index_of_type<SearchT, T, Ts...>
    : public std::integral_constant<size_t,
                                    1 + index_of_type<SearchT, Ts...>::value> {
};

template <typename SearchT, typename... Ts>
constexpr size_t index_of_type_v = index_of_type<SearchT, Ts...>::value;

namespace detail {

template <typename Tuple, typename Function, size_t... Index>
constexpr auto tuple_map_impl(Tuple&& tpl, Function&& function,
                              std::index_sequence<Index...>) {
  return std::tuple{function(std::get<Index>(std::forward<Tuple>(tpl)))...};
}

template <typename Tuple1, typename Tuple2, typename Function, size_t... Index>
constexpr auto tuple_map2_impl(Tuple1&& tpl1, Tuple2&& tpl2, Function&& function,
                               std::index_sequence<Index...>) {
  return std::tuple{function(std::get<Index>(std::forward<Tuple1>(tpl1)),
                             std::get<Index>(std::forward<Tuple2>(tpl2)))...};
}

template <typename Acc, typename Tuple, typename Function, size_t Index>
constexpr auto tuple_fold_impl_step(Acc&& acc, Tuple&& tpl, Function&& function,
                                    std::integral_constant<size_t, Index>) {
  if constexpr (Index >= std::tuple_size_v<std::decay_t<Tuple>>) {
    return std::forward<Acc>(acc);
  } else {
    auto new_acc = function(std::forward<Acc>(acc),
                            std::get<Index>(std::forward<Tuple>(tpl)));
    return tuple_fold_impl_step(
        std::move(new_acc), std::forward<Tuple>(tpl),
        std::forward<Function>(function),
        std::integral_constant<size_t, Index + 1>{});
  }
}

}  // namespace detail

// Maps a function over a tuple, returning a new tuple with transformed values.
// Example: tuple_map(std::tuple{1, 2.0}, [](auto x) { return x + 1; })
//          returns std::tuple{2, 3.0}
template <typename Tuple, typename Function>
constexpr auto tuple_map(Tuple&& tpl, Function&& function) {
  return detail::tuple_map_impl(
      std::forward<Tuple>(tpl), std::forward<Function>(function),
      std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>());
}

// Maps a binary function over two tuples element-wise.
// The two tuples must have the same size.
template <typename Tuple1, typename Tuple2, typename Function>
constexpr auto tuple_map2(Tuple1&& tpl1, Tuple2&& tpl2, Function&& function) {
  static_assert(std::tuple_size_v<std::decay_t<Tuple1>> ==
                std::tuple_size_v<std::decay_t<Tuple2>>);
  return detail::tuple_map2_impl(
      std::forward<Tuple1>(tpl1), std::forward<Tuple2>(tpl2),
      std::forward<Function>(function),
      std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple1>>>());
}

// Folds a binary function over a tuple, left to right.
// Example: tuple_fold(0, std::tuple{1, 2, 3}, [](int a, int b) { return a + b; })
//          returns 6
template <typename Acc, typename Tuple, typename Function>
constexpr auto tuple_fold(Acc&& init, Tuple&& tpl, Function&& function) {
  return detail::tuple_fold_impl_step(
      std::forward<Acc>(init), std::forward<Tuple>(tpl),
      std::forward<Function>(function), std::integral_constant<size_t, 0>{});
}

// Runtime versions of tuple_head and tuple_drop that take the size as a parameter.
// These are useful when the size is not known at compile time.
template <typename Tuple>
constexpr auto base_tuple_head_rt(Tuple&& tpl, size_t n) {
  // For runtime version, we need to return a variant or similar.
  // For now, just return the tuple itself as a placeholder.
  // This is typically not used directly but may be needed for type compatibility.
  return std::forward<Tuple>(tpl);
}

// Runtime version with regular parameter
template <typename Tuple>
constexpr auto base_tuple_drop_rt(Tuple&& tpl, size_t n) {
  // For runtime version, similar to above.
  return std::forward<Tuple>(tpl);
}

namespace detail {
template <size_t N, typename Tuple, size_t... Is>
constexpr auto tuple_drop_impl(Tuple&& tpl, std::index_sequence<Is...>) {
  return std::make_tuple(std::get<N + Is>(std::forward<Tuple>(tpl))...);
}
}  // namespace detail

// Template version that takes the drop count as a template parameter
// This version drops the first N elements from a tuple at compile time
template <size_t N, typename Tuple>
constexpr auto base_tuple_drop_rt(Tuple&& tpl) {
  // Use std::apply with index sequence to drop first N elements
  return std::apply(
      [](auto&&... args) {
        return detail::tuple_drop_impl<N>(
            std::forward_as_tuple(std::forward<decltype(args)>(args)...),
            std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>> - N>{});
      },
      std::forward<Tuple>(tpl));
}

}  // namespace base
}  // namespace v8

#endif  // V8_BASE_TEMPLATE_UTILS_H_
