#ifndef V8_PLATFORM_WASI_FIXED_H_
#define V8_PLATFORM_WASI_FIXED_H_

// Use canonical libplatform tracing base types; avoid duplicating or
// redefining classes that exist in V8's public headers.
#include "libplatform/v8-tracing-base.h"

namespace v8 {
namespace platform {
namespace tracing {
// Intentionally empty: this header previously attempted to provide stubs
// for tracing types, which conflicted with real definitions. Consumers
// should rely on the declarations in v8-tracing-base.h and
// include/libplatform/v8-tracing.h instead.
}  // namespace tracing
}  // namespace platform
}  // namespace v8

#endif  // V8_PLATFORM_WASI_FIXED_H_
