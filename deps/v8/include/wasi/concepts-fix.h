#ifndef WASI_CONCEPTS_FIX_H
#define WASI_CONCEPTS_FIX_H

#include <type_traits>

// Avoid injecting into std unless we are guaranteed to be at global scope.
// Base prelude defines V8_WASI_STD_POLYFILLS_GLOBAL_SCOPE before including us
// to install polyfills safely. When this file is included from nested
// namespace contexts, the macro will be unset and we will not modify std::.
#ifdef V8_WASI_STD_POLYFILLS_GLOBAL_SCOPE
namespace std {
#if !defined(_LIBCPP_HAS_REMOVE_CVREF) && !defined(__cpp_lib_remove_cvref)
template <typename T>
struct remove_cvref {
  using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;
#endif
}  // namespace std
#endif  // V8_WASI_STD_POLYFILLS_GLOBAL_SCOPE

#endif // WASI_CONCEPTS_FIX_H
