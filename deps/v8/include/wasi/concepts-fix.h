#ifndef WASI_CONCEPTS_FIX_H
#define WASI_CONCEPTS_FIX_H

// Use standard library implementations when available
#include <type_traits>

// Only define what's missing, avoid conflicts
namespace std {
  // Only define if not already present
  #if !defined(_LIBCPP_HAS_REMOVE_CVREF) && !defined(__cpp_lib_remove_cvref)
  template<typename T>
  struct remove_cvref {
    using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
  };
  
  template<typename T>
  using remove_cvref_t = typename remove_cvref<T>::type;
  #endif
  
  // Don't redefine is_same - it exists in standard library
  
  // Concepts emulation - use template variables instead of concepts
  template<typename T, typename U>
  static constexpr bool same_as_v = std::is_same<T, U>::value && std::is_same<U, T>::value;
  
  template<typename T, typename... Args>
  static constexpr bool constructible_from_v = std::is_constructible<T, Args...>::value;
}

#endif // WASI_CONCEPTS_FIX_H
