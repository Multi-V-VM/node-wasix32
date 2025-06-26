#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_LIBPLATFORM_TRACING_TRACING_CONTROLLER_H_
#define V8_LIBPLATFORM_TRACING_TRACING_CONTROLLER_H_

#include <memory>

#include "v8config.h"

#ifdef __wasi__

namespace v8 {
namespace platform {
namespace tracing {

class TraceObject;
class TraceConfig;
class TraceBuffer;

class V8_PLATFORM_EXPORT TracingController {
 public:
  TracingController();
  virtual ~TracingController();

  void Initialize(TraceBuffer* trace_buffer);
  
  static int64_t CurrentTimestampMicroseconds();
  static int64_t CurrentCpuTimestampMicroseconds();
  
  uint64_t AddTraceEvent(
      char phase, const uint8_t* category_group_enabled, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values, void** arg_convertables,
      unsigned int flags);
      
  uint64_t AddTraceEventWithTimestamp(
      char phase, const uint8_t* category_group_enabled, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values, void** arg_convertables,
      unsigned int flags, int64_t timestamp);
      
  void UpdateTraceEventDuration(const uint8_t* category_group_enabled,
                                const char* name, uint64_t handle);
                                
  const char* GetCategoryGroupName(const uint8_t* category_group_enabled);
  
  void StartTracing(TraceConfig* trace_config);
  void StopTracing();
  
  void UpdateCategoryGroupEnabledFlag(size_t category_index);
  void UpdateCategoryGroupEnabledFlags();
  
  const uint8_t* GetCategoryGroupEnabled(const char* category_group);

 private:
  std::unique_ptr<TraceBuffer> trace_buffer_;
};

} // namespace tracing
} // namespace platform

using TracingController = platform::tracing::TracingController;

} // namespace v8

#endif // __wasi__

#endif  // V8_LIBPLATFORM_TRACING_TRACING_CONTROLLER_H_
