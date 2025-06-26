#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef WASI_COMPAT_H_
#define WASI_COMPAT_H_

#ifdef __wasi__

// Only define things that are truly missing, avoid conflicts with libc++
#ifndef _LIBCPP_VERSION
// Only define if we're not using libc++

namespace std {
// Only provide missing C++20 features
template <typename T>
struct remove_cvref {
  typedef T type;
};
template <typename T>
struct remove_cvref<const T> {
  typedef T type;
};
template <typename T>
struct remove_cvref<volatile T> {
  typedef T type;
};
template <typename T>
struct remove_cvref<const volatile T> {
  typedef T type;
};
template <typename T>
struct remove_cvref<T&> {
  typedef T type;
};
template <typename T>
struct remove_cvref<const T&> {
  typedef T type;
};
template <typename T>
struct remove_cvref<volatile T&> {
  typedef T type;
};
template <typename T>
struct remove_cvref<const volatile T&> {
  typedef T type;
};
template <typename T>
struct remove_cvref<T&&> {
  typedef T type;
};
template <typename T>
struct remove_cvref<const T&&> {
  typedef T type;
};
template <typename T>
struct remove_cvref<volatile T&&> {
  typedef T type;
};
template <typename T>
struct remove_cvref<const volatile T&&> {
  typedef T type;
};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;
template <typename T>
using __remove_cvref_t = typename remove_cvref<T>::type;

// Provide span if missing
template <typename T>
struct span {
  T* data_;
  size_t size_;
  span(T* data, size_t size) : data_(data), size_(size) {}
  T* data() const { return data_; }
  size_t size() const { return size_; }
  T* begin() const { return data_; }
  T* end() const { return data_ + size_; }
};

// Byte type for simdutf compatibility
enum class byte : unsigned char {};
}  // namespace std

#endif  // _LIBCPP_VERSION

// C++20 concepts fallback - always define these as they're newer
#ifndef __cpp_concepts
#define concept bool
#define requires(...) true
#endif

// Function attributes for WASI
#ifndef simdutf_really_inline
#define simdutf_really_inline inline
#endif

#ifndef simdutf_warn_unused
#define simdutf_warn_unused
#endif

// Compiler-specific workarounds
#if defined(__clang__) && defined(__wasi__)
// Disable problematic optimizations for WASI
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif

#endif  // __wasi__

#endif  // WASI_COMPAT_H_
