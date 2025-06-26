#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_TRACING_H_
#define V8_TRACING_H_

#ifdef __wasi__

#include <memory>
#include <string>
#include <cstdint>

namespace v8 {

// Forward declarations
class Platform;

// Tracing API stubs for WASI
class ConvertableToTraceFormat {
 public:
  virtual ~ConvertableToTraceFormat() = default;
  virtual void AppendAsTraceFormat(std::string* out) const = 0;
};

class TracingController {
 public:
  class TraceStateObserver {
   public:
    virtual ~TraceStateObserver() = default;
    virtual void OnTraceEnabled() = 0;
    virtual void OnTraceDisabled() = 0;
  };

  virtual ~TracingController() = default;
  
  virtual const uint8_t* GetCategoryGroupEnabled(const char* category_group) {
    static const uint8_t enabled = 0;
    return &enabled;
  }
  
  virtual uint64_t AddTraceEvent(
      char phase, const uint8_t* category_enabled, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags) { return 0; }
      
  virtual uint64_t AddTraceEventWithTimestamp(
      char phase, const uint8_t* category_enabled, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags, int64_t timestamp) { return 0; }
      
  virtual void UpdateTraceEventDuration(const uint8_t* category_enabled,
                                        const char* name, uint64_t handle) {}
                                        
  virtual void AddTraceStateObserver(TraceStateObserver* observer) {}
  virtual void RemoveTraceStateObserver(TraceStateObserver* observer) {}
};

}  // namespace v8

#endif // __wasi__

#endif // V8_TRACING_H_
