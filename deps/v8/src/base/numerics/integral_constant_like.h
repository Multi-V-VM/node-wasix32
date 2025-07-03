// Copyright 2022 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_NUMERICS_INTEGRAL_CONSTANT_LIKE_H_
#define V8_BASE_NUMERICS_INTEGRAL_CONSTANT_LIKE_H_

#include <type_traits>

namespace v8::base::internal::numerics {

// Replace C++20 concept with C++14 compatible trait
template <typename T, typename = void>
struct is_integral_constant_like : std::false_type {};

template <typename T>
struct is_integral_constant_like<T, 
    typename std::enable_if<
        std::is_integral<decltype(T::value)>::value &&
        std::is_convertible<T, decltype(T::value)>::value &&
        std::is_copy_constructible<T>::value &&
        std::is_copy_assignable<T>::value &&
        std::is_destructible<T>::value
    >::type> : std::true_type {};

// Helper for SFINAE
template <typename T>
using enable_if_integral_constant_like = 
    typename std::enable_if<is_integral_constant_like<T>::value>::type;

}  // namespace v8::base::internal::numerics

#endif  // V8_BASE_NUMERICS_INTEGRAL_CONSTANT_LIKE_H_
