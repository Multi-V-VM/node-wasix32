#ifndef V8_V8_THREAD_ISOLATED_ALLOCATOR_H_
#define V8_V8_THREAD_ISOLATED_ALLOCATOR_H_

namespace v8 {

class ThreadIsolatedAllocator {
 public:
  virtual ~ThreadIsolatedAllocator() = default;
  
  enum class Type {
    kPkey
  };
  
  virtual void* Allocate(size_t size) = 0;
  virtual void Free(void* ptr) = 0;
  virtual Type GetType() const = 0;
};

}  // namespace v8

#endif  // V8_V8_THREAD_ISOLATED_ALLOCATOR_H_
