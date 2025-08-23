#ifndef V8_TRACING_CONTROLLER_FIX_H_
#define V8_TRACING_CONTROLLER_FIX_H_

namespace v8 {
namespace platform {
namespace tracing {

class TracingController {
 public:
  void StartTracing() {}
  void StopTracing() {}
};

} // namespace tracing
} // namespace platform
} // namespace v8

#endif
