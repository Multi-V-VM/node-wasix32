#ifndef V8_LIBPLATFORM_V8_TRACING_BASE_H_
#define V8_LIBPLATFORM_V8_TRACING_BASE_H_

// This header is a placeholder - the actual tracing classes are defined
// in v8-tracing.h. This header exists to support files that only need
// forward declarations of tracing types.

namespace v8 {
namespace platform {
namespace tracing {

// Forward declarations only - full definitions are in v8-tracing.h
class TraceObject;
class TraceWriter;
class TraceBufferChunk;
class TraceBuffer;
class TraceConfig;
class TracingController;

} // namespace tracing
} // namespace platform
} // namespace v8

#endif  // V8_LIBPLATFORM_V8_TRACING_BASE_H_
