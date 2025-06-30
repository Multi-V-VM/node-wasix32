#ifndef V8_BASE_PLATFORM_THREAD_H_
#define V8_BASE_PLATFORM_THREAD_H_

namespace v8 {
namespace base {

class Thread {
 public:
  enum class Priority {
    kDefault,
    kUserBlocking,
    kUserVisible,
    kBestEffort
  };
  
  Thread() = default;
  virtual ~Thread() = default;
};

}  // namespace base
}  // namespace v8

#endif  // V8_BASE_PLATFORM_THREAD_H_