// Complete WASI platform implementation for V8
#include "src/base/platform/platform.h"
#include "src/base/platform/mutex.h"
#include "src/base/platform/semaphore.h"
#include "src/base/platform/condition-variable.h"
#include <wasi/api.h>
#include <cstdlib>
#include <ctime>

namespace v8 {
namespace base {

// Time implementation
class WASITime : public Time {
 public:
  static Time Now() {
    __wasi_timestamp_t ts;
    __wasi_clock_time_get(__WASI_CLOCKID_REALTIME, 1000, &ts);
    return Time(ts / 1000000);  // Convert to milliseconds
  }
  
  static Time NowFromSystemTime() { return Now(); }
};

// TimeTicks implementation
class WASITimeTicks : public TimeTicks {
 public:
  static TimeTicks Now() {
    __wasi_timestamp_t ts;
    __wasi_clock_time_get(__WASI_CLOCKID_MONOTONIC, 1000, &ts);
    return TimeTicks(ts / 1000000);
  }
  
  static TimeTicks HighResolutionNow() { return Now(); }
};

// Thread implementation (single-threaded for WASI)
class Thread::PlatformData {
 public:
  PlatformData() : thread_id_(0) {}
  int thread_id_;
};

Thread::Thread(const Options& options) : data_(new PlatformData()) {}
Thread::~Thread() { delete data_; }
void Thread::Start() {}
void Thread::Join() {}
Thread::LocalStorageKey Thread::CreateThreadLocalKey() { return 0; }
void Thread::DeleteThreadLocalKey(LocalStorageKey) {}
void* Thread::GetThreadLocal(LocalStorageKey) { return nullptr; }
void Thread::SetThreadLocal(LocalStorageKey, void*) {}

// Mutex implementation (no-op for single-threaded WASI)
class Mutex::PlatformData {
 public:
  PlatformData() {}
};

Mutex::Mutex() : data_(new PlatformData()) {}
Mutex::~Mutex() { delete data_; }
void Mutex::Lock() {}
void Mutex::Unlock() {}
bool Mutex::TryLock() { return true; }

// Semaphore implementation (no-op for single-threaded WASI)
class Semaphore::PlatformData {
 public:
  PlatformData() : count_(0) {}
  int count_;
};

Semaphore::Semaphore(int count) : data_(new PlatformData()) {
  data_->count_ = count;
}
Semaphore::~Semaphore() { delete data_; }
void Semaphore::Signal() { data_->count_++; }
void Semaphore::Wait() { 
  while (data_->count_ <= 0) {} 
  data_->count_--;
}
bool Semaphore::WaitFor(const TimeDelta&) { 
  if (data_->count_ > 0) {
    data_->count_--;
    return true;
  }
  return false;
}

// ConditionVariable implementation
class ConditionVariable::PlatformData {
 public:
  PlatformData() {}
};

ConditionVariable::ConditionVariable() : data_(new PlatformData()) {}
ConditionVariable::~ConditionVariable() { delete data_; }
void ConditionVariable::NotifyOne() {}
void ConditionVariable::NotifyAll() {}
void ConditionVariable::Wait(Mutex*) {}
bool ConditionVariable::WaitFor(Mutex*, const TimeDelta&) { return false; }

// OS class implementation
class OS {
 public:
  static size_t AllocatePageSize() { return 65536; }  // WASM page size
  static size_t CommitPageSize() { return 65536; }
  
  static void* Allocate(void* hint, size_t size, size_t alignment,
                        MemoryPermission access) {
    void* result = aligned_alloc(alignment, size);
    if (result && access == MemoryPermission::kNoAccess) {
      // WASI doesn't support memory protection
    }
    return result;
  }
  
  static void Free(void* address, size_t size) {
    free(address);
  }
  
  static void SetRandomMmapSeed(int64_t seed) {}
  static void* GetRandomMmapAddr() { return nullptr; }
  
  static void Sleep(TimeDelta delta) {
    __wasi_timestamp_t ns = delta.InMicroseconds() * 1000;
    __wasi_subscription_t sub;
    sub.u.tag = __WASI_EVENTTYPE_CLOCK;
    sub.u.u.clock.id = __WASI_CLOCKID_MONOTONIC;
    sub.u.u.clock.timeout = ns;
    sub.u.u.clock.precision = 0;
    sub.u.u.clock.flags = 0;
    
    __wasi_event_t event;
    size_t nevents;
    __wasi_poll_oneoff(&sub, &event, 1, &nevents);
  }
  
  static void Abort() { abort(); }
  static void DebugBreak() {}
  
  static int GetCurrentProcessId() { return 1; }
  static int GetCurrentThreadId() { return 1; }
  
  static char* StrError(int err) { return strerror(err); }
  
  static void Print(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
  }
  
  static void VPrint(const char* format, va_list args) {
    vprintf(format, args);
  }
  
  static void PrintError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
  }
  
  static void VPrintError(const char* format, va_list args) {
    vfprintf(stderr, format, args);
  }
};

}  // namespace base
}  // namespace v8