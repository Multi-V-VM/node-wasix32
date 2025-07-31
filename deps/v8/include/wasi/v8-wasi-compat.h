#ifndef V8_INCLUDE_WASI_V8_WASI_COMPAT_H_
#define V8_INCLUDE_WASI_V8_WASI_COMPAT_H_

// Include standard namespace fixes first
#include "wasi/std-namespace-fix.h"

// Then include other wasi compatibility headers (except embedder-graph-stub.h)
#include "wasi/nuclear-fix.h"
#include "wasi/v8-wasi-helpers.h"
#include "wasi/chrono-compat.h"

// Include embedder-graph-stub.h last to ensure V8 types are defined
// This needs to be included after v8-value.h and v8-primitive.h have been included
// by the main v8 headers
#ifndef V8_INCLUDE_V8_VALUE_H_
// Only include if v8-value.h hasn't been included yet
// embedder-graph-stub.h will be included later by v8-profiler.h
#else
#include "wasi/embedder-graph-stub.h"
#endif

#endif  // V8_INCLUDE_WASI_V8_WASI_COMPAT_H_
