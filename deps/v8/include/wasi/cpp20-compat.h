#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_CPP20_COMPAT_H_
#define V8_WASI_CPP20_COMPAT_H_

#ifdef __wasi__

#include <type_traits>
#include <utility>
#include <functional>
#include <bit>

// IMPORTANT: This header should be included before any V8 namespace is opened.
// Only install std:: polyfills at translation unit global scope.

// Only install std:: variable template shims when explicitly enabled at
// translation unit global scope via V8_WASI_STD_POLYFILLS_ENABLED.
#ifdef V8_WASI_STD_POLYFILLS_ENABLED
namespace std {
#ifndef __cpp_lib_type_trait_variable_templates
template <typename T>
constexpr bool is_unsigned_v = std::is_unsigned<T>::value;
template <typename T>
constexpr bool is_integral_v = std::is_integral<T>::value;
template <typename T>
constexpr bool is_scalar_v = std::is_scalar<T>::value;
template <typename... Args>
constexpr bool conjunction_v = std::conjunction<Args...>::value;
#endif

// Ensure other commonly used templates are available
#ifndef __cpp_lib_trivially_copyable
template <typename T>
using is_trivially_copyable = std::is_trivially_copy_constructible<T>;
#endif

}  // namespace std
#endif  // V8_WASI_STD_POLYFILLS_ENABLED

#endif  // __wasi__

#endif  // V8_WASI_CPP20_COMPAT_H_
