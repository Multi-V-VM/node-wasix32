#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
/* WebAssembly/WASI config for V8 */
#ifndef V8_INCLUDE_WASI_V8CONFIG_WASI_H_
#define V8_INCLUDE_WASI_V8CONFIG_WASI_H_

#define V8_TARGET_ARCH_WASM 1
#define V8_TARGET_ARCH_32_BIT 1
#define V8_ATOMIC_OBJECT_FIELD_WRITES 1
/* Remove architecture detection errors */
#undef V8_HOST_ARCH_ARM
#undef V8_HOST_ARCH_ARM64
#undef V8_HOST_ARCH_IA32
#undef V8_HOST_ARCH_X64
#define V8_HOST_ARCH_WASM 1
#define V8_HAVE_TARGET_OS 1
#define V8_TARGET_OS_WASI 1

/* Basic V8 macros for WASI */
#define V8_INLINE inline
#define V8_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define V8_EXPORT
#define V8_NOINLINE __attribute__((noinline))
#define V8_BASE_EXPORT

#ifdef __cplusplus
// Include our platform-specific definitions
#include "wasi/v8-platform-wasi.h"
#endif  // __cplusplus

#endif  // V8_INCLUDE_WASI_V8CONFIG_WASI_H_
