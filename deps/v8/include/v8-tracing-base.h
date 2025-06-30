#ifndef V8_V8_TRACING_BASE_H_
#define V8_V8_TRACING_BASE_H_

#include <memory>

namespace v8 {

// ConvertableToTraceFormat interface
class ConvertableToTraceFormat {
 public:
  virtual ~ConvertableToTraceFormat() = default;
  virtual void AppendAsTraceFormat(std::string* out) const = 0;
};

// TracingController base class
class TracingController {
 public:
  virtual ~TracingController() = default;
  
  // TraceStateObserver interface
  class TraceStateObserver {
   public:
    virtual ~TraceStateObserver() = default;
    virtual void OnTraceEnabled() = 0;
    virtual void OnTraceDisabled() = 0;
  };
  
  virtual const uint8_t* GetCategoryGroupEnabled(const char* name) = 0;
  virtual uint64_t AddTraceEvent(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<v8::ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags) = 0;
  virtual uint64_t AddTraceEventWithTimestamp(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<v8::ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags, int64_t timestamp) = 0;
  virtual void UpdateTraceEventDuration(
      const uint8_t* category_enabled_flag, const char* name, uint64_t handle) = 0;
      
  virtual void AddTraceStateObserver(TraceStateObserver* observer) = 0;
  virtual void RemoveTraceStateObserver(TraceStateObserver* observer) = 0;
  
  // Static helper method
  static const char* GetCategoryGroupName(const uint8_t* category_enabled_flag) {
    return "v8";
  }
};

}  // namespace v8

#endif  // V8_V8_TRACING_BASE_H_