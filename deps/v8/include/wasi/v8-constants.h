#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_CONSTANTS_H_
#define V8_WASI_CONSTANTS_H_

#ifdef __wasi__

// Size constants
constexpr size_t KB = 1024;
constexpr size_t MB = 1024 * KB;
constexpr size_t GB = 1024 * MB;

// Missing API constants for WASI
constexpr int kApiTaggedSize = 4;  // Same as kTaggedSize for WASI
constexpr int kApiSystemPointerSize = sizeof(void*);
constexpr int kApiDoubleSize = 8;

#endif // __wasi__

#endif // V8_WASI_CONSTANTS_H_
