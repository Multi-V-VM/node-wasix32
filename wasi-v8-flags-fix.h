#ifndef WASI_V8_FLAGS_FIX_H_
#define WASI_V8_FLAGS_FIX_H_

// Only include V8 internal headers when compiling V8 source code itself
// (detected by V8_EXPORT_PRIVATE being defined). External code including
// v8.h won't have the internal include paths set up correctly.
#if defined(__wasi__) && defined(V8_EXPORT_PRIVATE)
#include "deps/v8/src/base/flags.h"
#include "wasi-v8-essential-constants.h"
#endif  // __wasi__ && V8_EXPORT_PRIVATE

#endif  // WASI_V8_FLAGS_FIX_H_
