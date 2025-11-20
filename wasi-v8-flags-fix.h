#ifndef WASI_V8_FLAGS_FIX_H_
#define WASI_V8_FLAGS_FIX_H_

// Disable this file - the includes cause issues with relative paths in V8 headers
// V8 WASI stubs should provide necessary definitions
#if 0

#ifdef __wasi__
#include "deps/v8/src/base/flags.h"
#include "wasi-v8-essential-constants.h"
#endif  // __wasi__

#endif  // #if 0

#endif  // WASI_V8_FLAGS_FIX_H_
