#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_MACRO_FIXES_H_
#define V8_WASI_MACRO_FIXES_H_

#ifdef __wasi__
#include <cstdint>

// Comprehensive macro fixes for WASI - FINAL VERSION

// Only define macros if they haven't been defined yet
#ifndef V8_BASE_EXPORT
#define V8_BASE_EXPORT
#endif
#ifndef V8_EXPORT
#define V8_EXPORT
#endif
#ifndef V8_EXPORT_PRIVATE
#define V8_EXPORT_PRIVATE
#endif
#ifndef V8_PLATFORM_EXPORT
#define V8_PLATFORM_EXPORT
#endif
#ifndef V8_NOINLINE
#define V8_NOINLINE
#endif
#ifndef V8_WARN_UNUSED_RESULT
#define V8_WARN_UNUSED_RESULT
#endif
#ifndef NON_EXPORTED_BASE
#define NON_EXPORTED_BASE(x) x
#endif
#ifndef V8_PLATFORM_NON_EXPORTED_BASE
#define V8_PLATFORM_NON_EXPORTED_BASE(x) \
 public                                  \
  x
#endif
#ifndef V8_GLIBC_PREREQ
#define V8_GLIBC_PREREQ(maj, min) 0
#endif
#ifndef DISABLE_CFI_ICALL
#define DISABLE_CFI_ICALL
#endif

// Additional platform-specific types
using Address = uintptr_t;

// V8 Platform specific macro overrides for WASI
#ifndef V8_OS_WASI
#define V8_OS_WASI 1
#endif

#ifndef V8_TARGET_OS_WASI
#define V8_TARGET_OS_WASI 1
#endif

#ifndef V8_CC_CLANG
#define V8_CC_CLANG 1
#endif

#ifndef V8_HAS_BUILTIN_POPCOUNT
#define V8_HAS_BUILTIN_POPCOUNT 1
#endif

#ifndef V8_HAS_BUILTIN_CLZ
#define V8_HAS_BUILTIN_CLZ 1
#endif

#ifndef V8_HAS_BUILTIN_CTZ
#define V8_HAS_BUILTIN_CTZ 1
#endif

#ifndef PRINTF_FORMAT
#define PRINTF_FORMAT(format_param, dots_param)
#endif

#ifndef SCANF_FORMAT
#define SCANF_FORMAT(format_param, dots_param)
#endif

#ifndef NO_SANITIZE_CFI_ICALL
#define NO_SANITIZE_CFI_ICALL
#endif

// Additional compiler attribute macros
#ifndef V8_PRESERVE_MOST
#if __has_attribute(preserve_most)
#define V8_PRESERVE_MOST __attribute__((preserve_most))
#else
#define V8_PRESERVE_MOST
#endif
#endif

#ifndef V8_CONSTINIT
#if defined(__cpp_constinit) && __cpp_constinit >= 201907L
#define V8_CONSTINIT constinit
#elif __has_attribute(require_constant_initialization)
#define V8_CONSTINIT __attribute__((require_constant_initialization))
#else
#define V8_CONSTINIT
#endif
#endif

#ifndef V8_CONST
#if __cplusplus >= 201402L
#define V8_CONST constexpr
#else
#define V8_CONST const
#endif
#endif

#endif  // __wasi__

#endif  // V8_WASI_MACRO_FIXES_H_
