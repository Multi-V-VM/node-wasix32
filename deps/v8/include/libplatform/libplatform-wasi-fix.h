#ifndef V8_LIBPLATFORM_WASI_FIX_H_
#define V8_LIBPLATFORM_WASI_FIX_H_

#include "wasi/v8-platform-wasi-fixed.h"
#include "wasi/nuclear-fix.h"

namespace v8 {
namespace platform {

// Stub implementations for WASI
class TraceBufferChunk {
 public:
  static constexpr size_t kChunkSize = 64 * 1024;
  TraceBufferChunk(uint32_t seq) : seq_(seq) {}
  
 private:
  uint32_t seq_;
};

}  // namespace platform

namespace tracing {
using TracingController = v8::TracingController;
}  // namespace tracing

}  // namespace v8

#endif  // V8_LIBPLATFORM_WASI_FIX_H_
