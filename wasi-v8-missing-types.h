// Thin forwarding shim to the canonical WASI missing-types definitions under
// deps/v8/. Use a distinct include guard so we don't block inclusion of the
// actual header which uses a different guard name.
#ifndef WASI_V8_MISSING_TYPES_SHIM_H_
#define WASI_V8_MISSING_TYPES_SHIM_H_

#ifdef __wasi__

#include "deps/v8/include/wasi/wasi-v8-missing-types.h"

#endif  // __wasi__

#endif  // WASI_V8_MISSING_TYPES_SHIM_H_
