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
#define V8_ENABLE_MAGLEV 0
#define V8_ENABLE_TURBOFAN 0
#define V8_COMPRESS_POINTERS 0
#define V8_31BIT_SMIS_ON_64BIT_ARCH 0
#define V8_ENABLE_POINTER_COMPRESSION 0

// Compiler attributes
#define V8_EXPORT
#define V8_EXPORT_PRIVATE  
#define V8_INLINE inline
#define V8_NOINLINE
#define V8_TRIVIAL_ABI
#define V8_LIKELY(x) (x)
#define V8_UNLIKELY(x) (x)
#define V8_WARN_UNUSED_RESULT
#define V8_NODISCARD
#define V8_FALLTHROUGH

// Compiler detection
#define V8_CC_CLANG 1

#else
// Non-WASI standard configuration
#endif

#endif // V8_CONFIG_H_
