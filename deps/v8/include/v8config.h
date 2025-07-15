#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_CONFIG_H_
#define V8_CONFIG_H_

#ifdef __wasi__
// WASI-specific V8 configuration

// Architecture and OS detection
#define V8_WASI 1
#define V8_HOST_ARCH_32_BIT 1
#define V8_TARGET_ARCH_WASM 1
#define V8_OS_WASI 1

// Disable problematic features
#define V8_ENABLE_SANDBOX 0
#ifndef V8_ENABLE_MAGLEV
#define V8_ENABLE_MAGLEV 0
#endif
#define V8_COMPRESS_POINTERS 0
#define V8_31BIT_SMIS_ON_64BIT_ARCH 0
#define V8_ENABLE_POINTER_COMPRESSION 0

// Compiler attributes
#define V8_EXPORT
#define V8_EXPORT_PRIVATE  
#define V8_INLINE inline
#define V8_NOINLINE __attribute__((noinline))
#define V8_TRIVIAL_ABI
#define V8_LIKELY(x) __builtin_expect(!!(x), 1)
#define V8_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define V8_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define V8_NODISCARD [[nodiscard]]
#define V8_FALLTHROUGH [[fallthrough]]
#define V8_NO_UNIQUE_ADDRESS [[no_unique_address]]
#define V8_HAS_ATTRIBUTE_NO_UNIQUE_ADDRESS 1

// Additional compiler attributes
#if __has_attribute(preserve_most)
#define V8_PRESERVE_MOST __attribute__((preserve_most))
#else
#define V8_PRESERVE_MOST
#endif

// C++20 constinit support
#if defined(__cpp_constinit) && __cpp_constinit >= 201907L
#define V8_CONSTINIT constinit
#elif __has_attribute(require_constant_initialization)
#define V8_CONSTINIT __attribute__((require_constant_initialization))
#else
#define V8_CONSTINIT
#endif

// Constexpr support
#if __cplusplus >= 201402L
#define V8_CONST constexpr
#else
#define V8_CONST const
#endif

// Compiler detection
#define V8_CC_CLANG 1

#else
// Non-WASI standard configuration
#endif

#endif // V8_CONFIG_H_
