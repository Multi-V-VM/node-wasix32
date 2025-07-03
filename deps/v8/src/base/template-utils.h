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

#include "src/base/compiler-specific.h"

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
  return std::get<N>(tuple);
}

// Rest of template utilities...
template<typename T>
struct remove_cvref {
  using type = typename std::remove_cv_t<std::remove_reference_t<T>>;
};

template<typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

}  // namespace base
}  // namespace v8

#endif  // V8_BASE_TEMPLATE_UTILS_H_
