#ifndef V8_V8_THREAD_ISOLATED_ALLOCATOR_H_
#define V8_V8_THREAD_ISOLATED_ALLOCATOR_H_

#include <cstddef>  // for size_t

namespace v8 {

class ThreadIsolatedAllocator {
 public:
  virtual ~ThreadIsolatedAllocator() = default;

  // Type enum - compatible with libplatform expectations
  enum Type {
    kPkey
  };

  virtual void* Allocate(size_t size) = 0;
  virtual void Free(void* object) = 0;
  virtual enum Type Type() const = 0;
  virtual int Pkey() const = 0;
};

}  // namespace v8

#endif  // V8_V8_THREAD_ISOLATED_ALLOCATOR_H_
