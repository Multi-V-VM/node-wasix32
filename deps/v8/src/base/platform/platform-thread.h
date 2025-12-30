#ifndef V8_BASE_PLATFORM_THREAD_H_
#define V8_BASE_PLATFORM_THREAD_H_

#include <cstdint>
#include "src/base/platform/semaphore.h"

namespace v8 {
namespace base {

class Semaphore;  // fwd-decl to avoid heavy include

// Check if Thread was already defined in platform.h
// If so, skip this definition to avoid redefinition errors
#ifndef V8_BASE_THREAD_DEFINED
#define V8_BASE_THREAD_DEFINED

// Minimal public declaration of base::Thread used across platform backends.
class Thread {
 public:
#if V8_OS_STARBOARD
  using LocalStorageKey = SbThreadLocalKey;
#elif V8_OS_ZOS
  using LocalStorageKey = pthread_key_t;
#else
  using LocalStorageKey = int32_t;
#endif

  enum class Priority { kBestEffort, kUserVisible, kUserBlocking, kDefault };

  class Options {
   public:
    Options() : Options("v8:<unknown>") {}
    explicit Options(const char* name, int stack_size = 0)
        : Options(name, Priority::kDefault, stack_size) {}
    Options(const char* name, Priority priority, int stack_size = 0)
        : name_(name), priority_(priority), stack_size_(stack_size) {}

    const char* name() const { return name_; }
    int stack_size() const { return stack_size_; }
    Priority priority() const { return priority_; }

   private:
    const char* name_;
    const Priority priority_;
    const int stack_size_;
  };

  explicit Thread(const Options& options);
  Thread(const Thread&) = delete;
  Thread& operator=(const Thread&) = delete;
  virtual ~Thread();

  // Start new thread by calling Run() on the new thread. On WASI this may run
  // synchronously; return true on success.
  bool Start();
  bool StartSynchronously() {
    start_semaphore_ = new Semaphore(0);
    if (!Start()) return false;
    start_semaphore_->Wait();
    delete start_semaphore_;
    start_semaphore_ = nullptr;
    return true;
  }

  // Wait until thread terminates.
  void Join();

  inline const char* name() const { return name_; }

  // Abstract method for run handler.
  virtual void Run() = 0;

  // Thread-local storage.
  static LocalStorageKey CreateThreadLocalKey();
  static void DeleteThreadLocalKey(LocalStorageKey key);
  static void* GetThreadLocal(LocalStorageKey key);
  static void SetThreadLocal(LocalStorageKey key, void* value);
  static bool HasThreadLocal(LocalStorageKey key) {
    return GetThreadLocal(key) != nullptr;
  }

  static const int kMaxThreadNameLength = 16;

  class PlatformData;
  PlatformData* data() { return data_; }
  Priority priority() const { return priority_; }

  void NotifyStartedAndRun() {
    if (start_semaphore_) start_semaphore_->Signal();
    Run();
  }

 private:
  void set_name(const char* name);

  PlatformData* data_ = nullptr;
  char name_[kMaxThreadNameLength] = {0};
  int stack_size_ = 0;
  Priority priority_ = Priority::kDefault;
  Semaphore* start_semaphore_ = nullptr;
};

#endif  // V8_BASE_THREAD_DEFINED

}  // namespace base
}  // namespace v8

#endif  // V8_BASE_PLATFORM_THREAD_H_
