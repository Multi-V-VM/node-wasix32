#ifndef WASI_V8_FLAGS_FIX_H_
#define WASI_V8_FLAGS_FIX_H_

#ifdef __wasi__
#include "deps/v8/src/base/flags.h"
#include "wasi-v8-essential-constants.h"
#endif  // __wasi__

#endif  // WASI_V8_FLAGS_FIX_H_
