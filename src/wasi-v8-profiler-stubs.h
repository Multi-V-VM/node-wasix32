#ifndef WASI_V8_PROFILER_STUBS_H_
#define WASI_V8_PROFILER_STUBS_H_

#ifdef __wasi__

// Include the V8 profiler header which includes embedder-graph-stub.h
// That file already has all the profiler type definitions we need
#include "v8-profiler.h"

namespace v8 {

// Forward declarations
class Isolate;

// CpuProfiler stub for WASI builds
class CpuProfiler {
 public:
  static void UseDetailedSourcePositionsForProfiling(Isolate* isolate) {
    // WASI stub - no-op implementation
    (void)isolate;
  }
};

// Additional V8 API method extensions that may be missing in WASI builds
// can be added here if needed
} // namespace v8

#endif // __wasi__

#endif // WASI_V8_PROFILER_STUBS_H_