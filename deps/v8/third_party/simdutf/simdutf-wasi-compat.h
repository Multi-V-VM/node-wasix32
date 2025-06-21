#ifndef SIMDUTF_WASI_COMPAT_H_
#define SIMDUTF_WASI_COMPAT_H_

#ifdef __wasi__
#include <stddef.h>
#include <stdint.h>
// Simdutf WASI compatibility layer
// Only define what's needed for simdutf specifically

// C++20 concepts fallback
#ifndef __cpp_concepts
#define concept bool
#define requires(...) true ||
#endif

// Function attributes
#ifndef simdutf_really_inline
#define simdutf_really_inline inline
#endif

#ifndef simdutf_warn_unused
#define simdutf_warn_unused
#endif

// Minimal std additions for simdutf
namespace std {
// Only add things that are missing and needed by simdutf
#ifndef _LIBCPP_HAS_NO_CONCEPTS
template <typename T>
using remove_cvref_t = T;  // Simple fallback
#endif

// Byte type compatibility
// Span fallback if needed
#if !defined(__cpp_lib_span)
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
#endif
}  // namespace std

// Disable problematic simdutf features for WASI
#define SIMDUTF_IMPLEMENTATION_HASWELL 0
#define SIMDUTF_IMPLEMENTATION_WESTMERE 0
#define SIMDUTF_IMPLEMENTATION_ICELAKE 0
#define SIMDUTF_IMPLEMENTATION_ARM64 0

#endif  // __wasi__

#endif  // SIMDUTF_WASI_COMPAT_H_