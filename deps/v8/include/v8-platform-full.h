#ifndef V8_V8_PLATFORM_COMPLETE_H_
#define V8_V8_PLATFORM_COMPLETE_H_

#include <memory>
#include "v8-task-full.h"
#include "v8-tracing-base.h"
#include "v8-isolate.h"
#include "v8-thread-isolated-allocator.h"
#include "wasi/nuclear-fix.h"

namespace v8 {

// Job priorities
enum class TaskPriority {
  kUserVisible,
  kUserBlocking,
  kBestEffort,
  kMaxPriority = kBestEffort
};

// JobHandle interface
class JobHandle {
 public:
  virtual ~JobHandle() = default;
  virtual void Join() = 0;
  virtual void Cancel() = 0;
  virtual bool IsActive() = 0;
};

// JobDelegate interface
class JobDelegate {
 public:
  virtual ~JobDelegate() = default;
  virtual bool ShouldYield() = 0;
  virtual void NotifyConcurrencyIncrease() = 0;
  virtual uint8_t GetTaskId() = 0;
  virtual bool IsJoiningThread() const = 0;
};

// JobTask interface
class JobTask {
 public:
  virtual ~JobTask() = default;
  virtual void Run(JobDelegate* delegate) = 0;
  virtual size_t GetMaxConcurrency(size_t worker_count) const = 0;
};

// PageAllocator interface
class PageAllocator {
 public:
  // Permission enum for backwards compatibility
  enum Permission {
    kNoAccess,
    kRead,
    kReadWrite,
    kReadWriteExecute,
    kReadExecute,
    kNoAccessWillJitLater
  };
  
  virtual ~PageAllocator() = default;
  
  virtual size_t AllocatePageSize() = 0;
  virtual size_t CommitPageSize() = 0;
  virtual void SetRandomMmapSeed(int64_t seed) = 0;
  virtual void* GetRandomMmapAddr() = 0;
  
  virtual void* AllocatePages(
      void* address, size_t length, size_t alignment,
      PagePermissions permissions) = 0;
  virtual bool FreePages(void* address, size_t length) = 0;
  virtual bool ReleasePages(void* address, size_t length) = 0;
  virtual bool SetPermissions(void* address, size_t length,
                            PagePermissions permissions) = 0;
  virtual bool RecommitPages(void* address, size_t length,
                           PagePermissions permissions) = 0;
  virtual bool DecommitPages(void* address, size_t length) = 0;
  virtual bool DiscardSystemPages(void* address, size_t size) = 0;
  virtual bool SealPages(void* address, size_t size) = 0;
  
  // Shared memory support
  class SharedMemory {
   public:
    virtual ~SharedMemory() = default;
  };
  
  class SharedMemoryMapping {
   public:
    virtual ~SharedMemoryMapping() = default;
    virtual void* GetMemory() const = 0;
    virtual void Remap(void* new_address) = 0;
  };
};

// Platform interface
class Platform {
 public:
  virtual ~Platform() = default;
  
  virtual PageAllocator* GetPageAllocator() = 0;
  virtual int NumberOfWorkerThreads() = 0;
  virtual std::shared_ptr<TaskRunner> GetForegroundTaskRunner(Isolate* isolate) = 0;
  virtual void CallOnWorkerThread(std::unique_ptr<Task> task) = 0;
  virtual void CallDelayedOnWorkerThread(std::unique_ptr<Task> task, double delay_in_seconds) = 0;
  virtual bool IdleTasksEnabled(Isolate* isolate) = 0;
  virtual double CurrentClockTimeMillis() = 0;
  virtual double MonotonicallyIncreasingTime() = 0;
  virtual std::unique_ptr<JobHandle> PostJob(TaskPriority priority, std::unique_ptr<JobTask> job_task) = 0;
  virtual void PostTaskOnWorkerThread(TaskPriority priority, std::unique_ptr<Task> task) = 0;
  virtual TracingController* GetTracingController() = 0;
  virtual StackTracePrinter GetStackTracePrinter() = 0;
  
  // Deprecated methods with default implementations
  virtual void CallOnForegroundThread(Isolate* isolate, std::unique_ptr<Task> task) {}
  virtual void CallDelayedOnForegroundThread(Isolate* isolate, std::unique_ptr<Task> task, double delay_in_seconds) {}
  virtual void CallIdleOnForegroundThread(Isolate* isolate, std::unique_ptr<IdleTask> task) {}
};

// Message loop behavior
enum class MessageLoopBehavior { kDoNotWait, kWaitForWork };

// Priority mode
enum class PriorityMode { kDontApply, kApply };

// Time function type
using TimeFunction = double (*)();

// Nestability
enum class Nestability { kNestable, kNonNestable };

// Platform shared memory handle
#ifndef __wasi__
using PlatformSharedMemoryHandle = void*;
#else
using PlatformSharedMemoryHandle = int;  // WASI uses file descriptors
#endif

// Stack trace printer
using StackTracePrinter = void (*)(void);

// Forward declarations to avoid redefinition
class TracingController;
class ConvertableToTraceFormat;

}  // namespace v8

#endif  // V8_V8_PLATFORM_COMPLETE_H_

