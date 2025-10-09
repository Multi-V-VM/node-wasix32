#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_CPP20_COMPAT_H_
#define V8_WASI_CPP20_COMPAT_H_

#ifdef __wasi__

// Close any open v8 namespace before including standard library headers
}  // close any v8 namespace if open

#include <type_traits>

namespace std {

// C++20 compatibility for WASI (without enable_if_t to avoid conflicts)
#ifndef __cpp_lib_type_trait_variable_templates
template<typename T>
constexpr bool is_unsigned_v = std::is_unsigned<T>::value;

template<typename T>
constexpr bool is_integral_v = std::is_integral<T>::value;

template<typename T>
constexpr bool is_scalar_v = std::is_scalar<T>::value;

template<typename... Args>
constexpr bool conjunction_v = std::conjunction<Args...>::value;
#endif

}  // namespace std

// Re-open the v8 namespace if it was closed at the beginning
namespace v8 {

#endif // __wasi__

#endif // V8_WASI_CPP20_COMPAT_H_
