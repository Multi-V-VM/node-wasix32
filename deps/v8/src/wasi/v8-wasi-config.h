// V8 WASI Configuration - Interpreter-only mode
#ifndef V8_WASI_CONFIG_H_
#define V8_WASI_CONFIG_H_

#ifdef __wasi__

// Disable JIT compilation for WASI
#define V8_INTERPRETED_REGEXP
#define V8_JITLESS
#define V8_LITE_MODE

// Disable features not supported in WASI
#define V8_DISABLE_WRITE_BARRIERS
#define V8_CONCURRENT_MARKING 0
#define V8_ENABLE_LAZY_SOURCE_POSITIONS 0
#define V8_ENABLE_REGEXP_INTERPRETER_THREADED_DISPATCH 0

// WASI platform settings
#define V8_OS_WASI 1
#define V8_TARGET_OS_WASI 1
#define V8_HOST_ARCH_WASM32 1
#define V8_TARGET_ARCH_WASM32 1

// Memory configuration
#define V8_COMPRESS_POINTERS 0
#define V8_31BIT_SMIS_ON_64BIT_ARCH 0
#define V8_ENABLE_SANDBOX 0

// Disable threading
#define V8_ENABLE_CONCURRENT_MARKING 0
#define V8_ENABLE_PARALLEL_MARKING 0
#define V8_ENABLE_CONCURRENT_ALLOCATION 0

#endif // __wasi__

#endif // V8_WASI_CONFIG_H_