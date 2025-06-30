#ifndef V8_LIBPLATFORM_LIBPLATFORM_FIXED_H_
#define V8_LIBPLATFORM_LIBPLATFORM_FIXED_H_

#ifdef __wasi__

#include <memory>
#include "v8-platform-full.h"
#include "v8-tracing.h"
#include "v8-task-full.h"
#include "v8-isolate.h"
#include "v8-thread-isolated-allocator.h"

namespace v8 {

enum class IdleTaskSupport { kDisabled, kEnabled };
enum class InProcessStackDumping { kDisabled, kEnabled };

namespace platform {

// Use the existing TracingController from v8-tracing.h
using tracing::TracingController;

// Factory function
std::unique_ptr<Platform> NewDefaultPlatform(
    int thread_pool_size = 0,
    IdleTaskSupport idle_task_support = IdleTaskSupport::kDisabled,
    InProcessStackDumping in_process_stack_dumping = InProcessStackDumping::kDisabled,
    std::unique_ptr<v8::TracingController> tracing_controller = {});

}  // namespace platform
}  // namespace v8

#else
#include "libplatform-original.h"
#endif  // __wasi__

#endif  // V8_LIBPLATFORM_LIBPLATFORM_FIXED_H_
