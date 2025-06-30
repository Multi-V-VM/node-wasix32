// WASI stub for TracingController to avoid namespace conflicts
#include "src/libplatform/tracing/tracing-controller.h"
#include "src/base/platform/mutex.h"

namespace v8 {
namespace platform {
namespace tracing {

TracingController::TracingController() {
  // Stub implementation for WASI
}

TracingController::~TracingController() {
  // Stub implementation for WASI
}

void TracingController::Initialize(TraceBuffer* trace_buffer) {
  // Stub implementation for WASI
}

int64_t TracingController::CurrentTimestampMicroseconds() {
  return 0; // Stub implementation for WASI
}

int64_t TracingController::CurrentCpuTimestampMicroseconds() {
  return 0; // Stub implementation for WASI
}

uint64_t TracingController::AddTraceEvent(
    char phase, const uint8_t* category_enabled_flag, const char* name,
    const char* scope, uint64_t id, uint64_t bind_id, int num_args,
    const char** arg_names, const uint8_t* arg_types,
    const uint64_t* arg_values, std::unique_ptr<ConvertableToTraceFormat>* arg_convertables,
    unsigned int flags) {
  return 0; // Stub implementation for WASI
}

void TracingController::UpdateTraceEventDuration(
    const uint8_t* category_enabled_flag, const char* name, uint64_t handle) {
  // Stub implementation for WASI
}

void TracingController::AddTraceStateObserver(TraceStateObserver* observer) {
  // Stub implementation for WASI
}

void TracingController::RemoveTraceStateObserver(TraceStateObserver* observer) {
  // Stub implementation for WASI
}

void TracingController::StartTracing(TraceConfig* trace_config) {
  // Stub implementation for WASI
}

void TracingController::StopTracing() {
  // Stub implementation for WASI
}

}  // namespace tracing
}  // namespace platform
}  // namespace v8
