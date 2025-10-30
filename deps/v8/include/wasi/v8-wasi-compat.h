#ifndef V8_INCLUDE_WASI_V8_WASI_COMPAT_H_
#define V8_INCLUDE_WASI_V8_WASI_COMPAT_H_

// Do not include base-preinclude from widely included public headers; it pulls
// in standard library headers and base headers which may be parsed from within
// namespace v8 in some translation units. Instead, include base-preinclude at
// true TU global scope from v8.h or platform entry points.
// Include standard namespace fixes next
#include "wasi/std-namespace-fix.h"

// Then include other wasi compatibility headers (except embedder-graph-stub.h)
#include "wasi/nuclear-fix.h"
#include "wasi/v8-wasi-helpers.h"
#include "wasi/chrono-compat.h"
// Provide missing API constants like kApiSystemPointerSize
#include "wasi/v8-constants.h"

// Define V8_INLINE if not already defined
#ifndef V8_INLINE
#define V8_INLINE inline
#endif

// Note: FixedArray is defined in v8-fixed-array-stub.h which should be included
// after v8-data.h to ensure proper type definitions

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
