#ifndef V8_LIBPLATFORM_DEFAULT_THREAD_ISOLATED_ALLOCATOR_H_
#define V8_LIBPLATFORM_DEFAULT_THREAD_ISOLATED_ALLOCATOR_H_

#include "include/libplatform/libplatform-export.h"
#include "include/v8-thread-isolated-allocator.h"

namespace v8 {
namespace platform {

class V8_PLATFORM_EXPORT DefaultThreadIsolatedAllocator
    : public v8::ThreadIsolatedAllocator {
 public:
  DefaultThreadIsolatedAllocator() = default;
  ~DefaultThreadIsolatedAllocator() override = default;

  void* Allocate(size_t size) override;
  void Free(void* ptr) override;
  
  enum Type { kPkey };
  Type GetType() const override { return kPkey; }
};

}  // namespace platform
}  // namespace v8

#endif
