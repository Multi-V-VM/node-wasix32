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
// Don't redefine these if v8config.h already defined them
// V8_NOINLINE, V8_WARN_UNUSED_RESULT, V8_INLINE, V8_NODISCARD
// are defined in v8config.h with proper attributes
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

// Don't redefine PRINTF_FORMAT if already defined in compiler-specific.h

#ifndef SCANF_FORMAT
#define SCANF_FORMAT(format_param, dots_param)
#endif

#ifndef NO_SANITIZE_CFI_ICALL
#define NO_SANITIZE_CFI_ICALL
#endif

// Don't redefine V8_PRESERVE_MOST - it's defined in v8config.h

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

// Deprecation macros
#ifndef V8_DEPRECATE_SOON
#define V8_DEPRECATE_SOON(message)
#endif

#ifndef V8_DEPRECATED
#define V8_DEPRECATED(message)
#endif

#endif  // __wasi__

#endif  // V8_WASI_MACRO_FIXES_H_
