// This legacy shim is superseded by deps/v8/include/wasi/wasi-v8-missing-types.h.
// Keep it as a thin forwarding header to avoid duplicate definitions and
// accidental nested namespace v8 blocks when included from within namespace v8.
#ifndef WASI_V8_MISSING_TYPES_H_
#define WASI_V8_MISSING_TYPES_H_

#ifdef __wasi__

#include "deps/v8/include/wasi/wasi-v8-missing-types.h"

#endif  // __wasi__

#endif  // WASI_V8_MISSING_TYPES_H_
