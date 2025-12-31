#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_MACRO_FIXES_H_
#define V8_WASI_MACRO_FIXES_H_

#ifdef __wasi__

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
// NON_EXPORTED_BASE is already defined in compiler-specific.h
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

// Sanitizer disable macros - no-op for WASI
#ifndef DISABLE_ASAN
#define DISABLE_ASAN
#endif

#ifndef DISABLE_HWASAN
#define DISABLE_HWASAN
#endif

#ifndef DISABLE_TSAN
#define DISABLE_TSAN
#endif

// Permission macros for memory protection - no-op for WASI
#ifndef PERMISSION_MUTABLE_SECTION
#define PERMISSION_MUTABLE_SECTION
#endif

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

// Don't redefine V8_PRESERVE_MOST or V8_CONSTINIT here — both are defined
// appropriately in v8config.h. Defining V8_CONSTINIT too early can cause
// it to be applied to extern declarations (e.g., TLS variables), which is
// ill-formed. Defer to v8config.h for the correct definition.

#ifndef V8_CONST
#if __cplusplus >= 201402L
#define V8_CONST constexpr
#else
#define V8_CONST const
#endif
#endif

// V8_PRETTY_FUNCTION macro - use fallback value on WASI
#ifndef V8_PRETTY_FUNCTION_VALUE_OR
#define V8_PRETTY_FUNCTION_VALUE_OR(fallback) (fallback)
#endif

// Deprecation macros
#ifndef V8_DEPRECATE_SOON
#define V8_DEPRECATE_SOON(message)
#endif

#ifndef V8_DEPRECATED
#define V8_DEPRECATED(message)
#endif

// V8_DEBUGGING_EXPORT is used for debug helper functions
// On WASI, we define it as nothing (no export needed)
#ifndef V8_DEBUGGING_EXPORT
#define V8_DEBUGGING_EXPORT
#endif

#endif  // __wasi__

#endif  // V8_WASI_MACRO_FIXES_H_
