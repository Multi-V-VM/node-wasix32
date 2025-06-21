#ifndef SIMDUTF_PREPROCESSED_H_
#define SIMDUTF_PREPROCESSED_H_

#ifdef __wasi__
// Preprocessor-based concept replacement for WASI
#define SIMDUTF_CONCEPT_REPLACEMENT(name, condition) \
  template<typename T> constexpr bool name = condition;

// Replace problematic concept definitions
#define concept bool
#define requires(...) true ||

// Disable CPU-specific optimizations
#ifndef SIMDUTF_IMPLEMENTATION_HASWELL
#define SIMDUTF_IMPLEMENTATION_HASWELL 0
#endif
#ifndef SIMDUTF_IMPLEMENTATION_WESTMERE  
#define SIMDUTF_IMPLEMENTATION_WESTMERE 0
#endif
#ifndef SIMDUTF_IMPLEMENTATION_ICELAKE
#define SIMDUTF_IMPLEMENTATION_ICELAKE 0
#endif
#ifndef SIMDUTF_IMPLEMENTATION_ARM64
#define SIMDUTF_IMPLEMENTATION_ARM64 0
#endif

#endif // __wasi__

// Include the original simdutf header
#include "simdutf.h.orig"

#endif // SIMDUTF_PREPROCESSED_H_
