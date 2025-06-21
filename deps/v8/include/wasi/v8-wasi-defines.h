#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
// WASI-specific defines to bypass problematic V8 code
#ifndef V8_INCLUDE_WASI_V8_WASI_DEFINES_H_
#define V8_INCLUDE_WASI_V8_WASI_DEFINES_H_

#ifdef __wasi__

// Disable V8 features that don't work with WASI
#define V8_DISABLE_TORQUE 1
#define V8_DISABLE_TURBOFAN 1
#define V8_DISABLE_MAGLEV 1
#define V8_LITE_MODE 1
#define V8_ENABLE_LITE_MODE 1

// Simplify threading for WASI
#define V8_ATOMIC_OBJECT_FIELD_WRITES 1
#define V8_CONCURRENT_MARKING 0

#endif  // __wasi__

#endif  // V8_INCLUDE_WASI_V8_WASI_DEFINES_H_
