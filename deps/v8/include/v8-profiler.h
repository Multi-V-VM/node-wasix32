#ifndef V8_V8_PROFILER_H_
#define V8_V8_PROFILER_H_

#ifdef __wasi__
// For WASI builds, include the script API and profiler stubs
#include "v8-script.h"
#include "wasi/embedder-graph-stub.h"
#include "wasi/v8-profiler-stubs.h"
#else
// Include the real V8 profiler header for non-WASI builds
// This would be where the actual V8 profiler definitions go
#endif // __wasi__

#endif // V8_V8_PROFILER_H_
