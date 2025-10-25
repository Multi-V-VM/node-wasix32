#ifndef WASI_V8_NAMESPACE_FIX_H_
#define WASI_V8_NAMESPACE_FIX_H_

#ifdef __wasi__
// No-op: avoid introducing any nested namespace aliases that could
// inadvertently create v8::v8::* or v8::std::* resolutions. The in-tree
// deps/v8/include/wasi/v8-namespace-fix.h is intentionally empty as well.
#include "deps/v8/include/wasi/v8-namespace-fix.h"
#endif  // __wasi__

#endif  // WASI_V8_NAMESPACE_FIX_H_
