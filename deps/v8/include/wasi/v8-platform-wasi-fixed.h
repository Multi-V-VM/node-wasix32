#ifndef V8_PLATFORM_WASI_FIXED_H_
#define V8_PLATFORM_WASI_FIXED_H_

#include <memory>
#include <string>

// Forward declarations for types that may not be defined yet
class TraceObject;
class TraceBuffer;
class TraceConfig;
class TraceStateObserver;
class ConvertableToTraceFormat;

// WASI-specific stubs for types not provided by v8-tracing.h

#ifndef V8_PLATFORM_TRACEOBJECT_H_
#define V8_PLATFORM_TRACEOBJECT_H_
// TraceObject stub
class TraceObject {
 public:
  struct ArgValue {
    union {
      bool as_bool;
      uint64_t as_uint;
      int64_t as_int;
      double as_double;
      const void* as_pointer;
      const char* as_string;
    };
  };
  
  TraceObject() = default;
  ~TraceObject() = default;
};
#endif  // V8_PLATFORM_TRACEOBJECT_H_

#ifndef V8_PLATFORM_TRACECONFIG_H_
#define V8_PLATFORM_TRACECONFIG_H_
// TraceConfig stub
class TraceConfig {
 public:
  TraceConfig() = default;
  ~TraceConfig() = default;
  
  bool IsCategoryGroupEnabled(const char* category_group) const { return true; }
};
#endif  // V8_PLATFORM_TRACECONFIG_H_

#ifndef V8_PLATFORM_TRACESTATEOBSERVER_H_
#define V8_PLATFORM_TRACESTATEOBSERVER_H_
// TraceStateObserver stub
class TraceStateObserver {
 public:
  virtual ~TraceStateObserver() = default;
  virtual void OnTraceEnabled() = 0;
  virtual void OnTraceDisabled() = 0;
};
#endif  // V8_PLATFORM_TRACESTATEOBSERVER_H_

#ifndef V8_PLATFORM_CONVERTABLE_H_
#define V8_PLATFORM_CONVERTABLE_H_
// ConvertableToTraceFormat stub
class ConvertableToTraceFormat {
 public:
  virtual ~ConvertableToTraceFormat() = default;
  virtual void AppendAsTraceFormat(std::string* out) const = 0;
};
#endif  // V8_PLATFORM_CONVERTABLE_H_

#ifndef V8_PLATFORM_TRACEBUFFER_H_
#define V8_PLATFORM_TRACEBUFFER_H_
// TraceBuffer stub
class TraceBuffer {
 public:
  virtual ~TraceBuffer() = default;
  virtual uint64_t AddTraceEvent(uint64_t* handle) = 0;
  virtual TraceObject* GetEventByHandle(uint64_t handle) = 0;
  virtual bool Flush(uint32_t timeout) = 0;
};
#endif  // V8_PLATFORM_TRACEBUFFER_H_

namespace v8 {
namespace platform {
namespace tracing {

// Any WASI-specific extensions can go here

}  // namespace tracing
}  // namespace platform
}  // namespace v8

#endif  // V8_PLATFORM_WASI_FIXED_H_