// WASI stub for TracingController
#ifdef __wasi__

#include "include/libplatform/tracing/tracing-controller.h"

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
    char phase, const uint8_t* category_group_enabled, const char* name,
    const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
    const char** arg_names, const uint8_t* arg_types,
    const uint64_t* arg_values, void** arg_convertables,
    unsigned int flags) {
  return 0; // Stub implementation for WASI
}

uint64_t TracingController::AddTraceEventWithTimestamp(
    char phase, const uint8_t* category_group_enabled, const char* name,
    const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
    const char** arg_names, const uint8_t* arg_types,
    const uint64_t* arg_values, void** arg_convertables,
    unsigned int flags, int64_t timestamp) {
  return 0; // Stub implementation for WASI
}

void TracingController::UpdateTraceEventDuration(
    const uint8_t* category_group_enabled, const char* name, uint64_t handle) {
  // Stub implementation for WASI
}

const char* TracingController::GetCategoryGroupName(const uint8_t* category_group_enabled) {
  return ""; // Stub implementation for WASI
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

#else
// Original implementation will be compiled when not WASI
#error "Non-WASI TracingController implementation not found"
#endif