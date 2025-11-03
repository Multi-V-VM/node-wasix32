#ifndef V8_V8_PLATFORM_COMPLETE_H_
#define V8_V8_PLATFORM_COMPLETE_H_


#include <memory>
#include "v8-task-full.h"
#include "v8-tracing-base.h"
#include "v8-isolate.h"
#include "v8-thread-isolated-allocator.h"
#ifdef __wasi__
#include "wasi/base-preinclude.h"
#endif
#ifdef __wasi__
#include "wasi/nuclear-fix.h"
#endif

namespace v8 {

// Forward declarations of type aliases needed by Platform interface
using StackTracePrinter = void (*)(void);

// Job priorities
enum class TaskPriority {
  kUserVisible,
  kUserBlocking,
  kBestEffort,
  kMaxPriority = kBestEffort
};

// Priority mode
enum class PriorityMode { kDontApply, kApply };

// Forward-declare PriorityMode before use
enum class PriorityMode;

// JobHandle interface
class JobHandle {
 public:
  virtual ~JobHandle() = default;
  virtual void Join() = 0;
  virtual void Cancel() = 0;
  virtual bool IsActive() = 0;
  // Additional controls used by various V8 subsystems; default no-ops.
  virtual bool UpdatePriorityEnabled() { return false; }
  virtual void UpdatePriority(TaskPriority) {}
  virtual void CancelAndDetach() { Cancel(); }
  virtual bool IsValid() { return IsActive(); }
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

#ifndef V8_PAGE_ALLOCATOR_INTERFACE_DEFINED
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
  using PagePermissions = Permission;
  
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
#endif  // V8_PAGE_ALLOCATOR_INTERFACE_DEFINED

// Public PageAllocator interface is available either from this header or
// via an early skeleton in wasi/nuclear-fix.h. Mark as defined for users.
#ifndef V8_PAGE_ALLOCATOR_INTERFACE_DEFINED
#define V8_PAGE_ALLOCATOR_INTERFACE_DEFINED 1
#endif

// Expose PagePermissions at v8::PagePermissions for convenience
using PagePermissions = PageAllocator::PagePermissions;

// Platform interface
class Platform {
 public:
  virtual ~Platform() = default;
  // Scoped blocking helper used by some GC synchronization paths.
  enum class BlockingType { kWillBlock, kMayBlock };
  class BlockingScope {
   public:
    BlockingScope() = default;
    ~BlockingScope() = default;
  };
  
  virtual PageAllocator* GetPageAllocator() = 0;
  virtual int NumberOfWorkerThreads() = 0;
  virtual std::shared_ptr<TaskRunner> GetForegroundTaskRunner(Isolate* isolate) = 0;
  virtual void CallOnWorkerThread(std::unique_ptr<Task> task) = 0;
  virtual void CallDelayedOnWorkerThread(std::unique_ptr<Task> task, double delay_in_seconds) = 0;
  virtual bool IdleTasksEnabled(Isolate* isolate) = 0;
  // Primary time API expected by most V8 internals
  virtual double CurrentClockTimeMillis() = 0;
  // Compatibility shims for call sites expecting "Milliseconds" spelling.
  // Provide default implementations in terms of CurrentClockTimeMillis().
  virtual int64_t CurrentClockTimeMilliseconds() {
    return static_cast<int64_t>(CurrentClockTimeMillis());
  }
  virtual double CurrentClockTimeMillisecondsHighResolution() {
    return CurrentClockTimeMillis();
  }
  // Legacy static accessor used by some internal code paths.
  static double SystemClockTimeMillis();
  virtual double MonotonicallyIncreasingTime() = 0;
  virtual std::unique_ptr<JobHandle> PostJob(TaskPriority priority, std::unique_ptr<JobTask> job_task) = 0;
  // Compatibility shim for internal callers expecting CreateJob; forward to PostJob.
  virtual std::unique_ptr<JobHandle> CreateJob(TaskPriority priority,
                                               std::unique_ptr<JobTask> job_task,
                                               PriorityMode /*mode*/) {
    return PostJob(priority, std::move(job_task));
  }
  virtual std::unique_ptr<JobHandle> CreateJob(TaskPriority priority,
                                               std::unique_ptr<JobTask> job_task) {
    return PostJob(priority, std::move(job_task));
  }
  virtual void PostTaskOnWorkerThread(TaskPriority priority, std::unique_ptr<Task> task) = 0;
  // Some paths post delayed worker tasks; provide a default forwarding impl.
  virtual void PostDelayedTaskOnWorkerThread(TaskPriority priority,
                                             std::unique_ptr<Task> task,
                                             double delay_in_seconds) {
    // WASI default: ignore delay and post immediately.
    PostTaskOnWorkerThread(priority, std::move(task));
  }
  virtual TracingController* GetTracingController() = 0;
  virtual StackTracePrinter GetStackTracePrinter() = 0;
  // Crash reporting hook used by some error paths.
  virtual void DumpWithoutCrashing() {}
  // Thread isolated allocator accessor used by sandbox code paths.
  virtual v8::ThreadIsolatedAllocator* GetThreadIsolatedAllocator() { return nullptr; }
  
  // HighAllocationThroughputObserver shims for WASI builds.
  class HighAllocationThroughputObserver {
   public:
    virtual ~HighAllocationThroughputObserver() = default;
    virtual void EnterSection() {}
    virtual void LeaveSection() {}
  };
  
  virtual HighAllocationThroughputObserver* GetHighAllocationThroughputObserver() {
    static HighAllocationThroughputObserver observer;
    return &observer;
  }
  
  // Deprecated methods with default implementations
  virtual void CallOnForegroundThread(Isolate* isolate, std::unique_ptr<Task> task) {}
  virtual void CallDelayedOnForegroundThread(Isolate* isolate, std::unique_ptr<Task> task, double delay_in_seconds) {}
  virtual void CallIdleOnForegroundThread(Isolate* isolate, std::unique_ptr<IdleTask> task) {}
  // Default no-op blocking scope creator for WASI builds.
  virtual BlockingScope CreateBlockingScope(BlockingType) { return BlockingScope{}; }
};

// Message loop behavior
enum class MessageLoopBehavior { kDoNotWait, kWaitForWork };

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

// Minimal VirtualAddressSpace interface for WASI builds.
class VirtualAddressSpace {
 public:
  using Address = uintptr_t;
  static constexpr Address kNoHint = 0;

  virtual ~VirtualAddressSpace() = default;

  virtual void SetRandomSeed(int64_t) {}
  virtual Address RandomPageAddress() { return 0; }

  virtual Address AllocatePages(Address /*hint*/, size_t /*size*/, size_t /*alignment*/,
                                PagePermissions /*perms*/) { return 0; }
  virtual void FreePages(Address /*address*/, size_t /*size*/) {}

  virtual bool SetPagePermissions(Address /*address*/, size_t /*size*/,
                                  PagePermissions /*perms*/) { return true; }
  virtual bool AllocateGuardRegion(Address /*address*/, size_t /*size*/) { return false; }
  virtual void FreeGuardRegion(Address /*address*/, size_t /*size*/) {}

  virtual bool CanAllocateSubspaces() { return false; }
  virtual std::unique_ptr<VirtualAddressSpace> AllocateSubspace(
      Address /*hint*/, size_t /*size*/, size_t /*alignment*/,
      PagePermissions /*max_perms*/) { return nullptr; }

  virtual bool RecommitPages(Address /*address*/, size_t /*size*/,
                             PagePermissions /*perms*/) { return false; }
  virtual bool DiscardSystemPages(Address /*address*/, size_t /*size*/) { return false; }
  virtual bool DecommitPages(Address /*address*/, size_t /*size*/) { return false; }

  // Introspection helpers used by segmented tables.
  virtual Address base() const { return 0; }
  virtual size_t size() const { return 0; }
  virtual size_t page_size() const { return 4096; }
  virtual size_t allocation_granularity() const { return 4096; }
  virtual PagePermissions max_page_permissions() const { return PagePermissions::kReadWrite; }
};

}  // namespace v8

#endif  // V8_V8_PLATFORM_COMPLETE_H_
