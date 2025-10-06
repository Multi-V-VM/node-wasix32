// Copyright 2024 the V8 project authors. All rights reserved.
// WASI platform implementation for V8

#include "src/base/platform/platform.h"

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifdef __wasi__
extern "C" {
// WASI API functions
int32_t __wasi_clock_time_get(uint32_t id, uint64_t precision, uint64_t* time);
int32_t __wasi_random_get(uint8_t* buf, size_t buf_len);
void __wasi_proc_exit(uint32_t rval);
int32_t __wasi_sched_yield(void);

}

#define __WASI_CLOCKID_REALTIME 0
#define __WASI_CLOCKID_MONOTONIC 1
#define __WASI_ERRNO_SUCCESS 0
#endif

#include <cstdarg>
#include <cstddef>
#include <memory>
#include <vector>

#include "src/base/lazy-instance.h"
#include "src/base/macros.h"
#include "src/base/platform/platform-posix.h"
#include "src/base/platform/platform-posix-time.h"
#include "src/base/platform/wrappers.h"
#include "src/base/utils/random-number-generator.h"

namespace v8 {
namespace base {

namespace {

// WASI doesn't support traditional memory mapping, so we simulate it
class WASIMemoryMappedFile : public OS::MemoryMappedFile {
 public:
  WASIMemoryMappedFile(void* memory, size_t size)
      : memory_(memory), size_(size) {}
  
  ~WASIMemoryMappedFile() override {
    if (memory_) {
      free(memory_);
    }
  }
  
  void* memory() const override { return memory_; }
  size_t size() const override { return size_; }

 private:
  void* memory_;
  size_t size_;
};

}  // namespace

// Thread implementation for WASI (single-threaded)
class Thread::PlatformData {
 public:
  PlatformData() : thread_id_(0) {}
  
  pthread_t thread_id_;
};

Thread::Thread(const Options& options)
    : data_(new PlatformData()),
      stack_size_(options.stack_size()),
      priority_(options.priority()),
      start_semaphore_(nullptr) {
  // WASI is single-threaded, so we can't create real threads
  set_name(options.name());
}

Thread::~Thread() {
  delete data_;
}

void Thread::Start() {
  // WASI doesn't support threads, run synchronously
  if (start_semaphore_) {
    start_semaphore_->Signal();
  }
  Run();
}

void Thread::Join() {
  // No-op for WASI
}

Thread::LocalStorageKey Thread::CreateThreadLocalKey() {
  // WASI doesn't have thread-local storage
  return static_cast<LocalStorageKey>(0);
}

void Thread::DeleteThreadLocalKey(LocalStorageKey key) {
  // No-op for WASI
}

void* Thread::GetThreadLocal(LocalStorageKey key) {
  // WASI doesn't have thread-local storage
  return nullptr;
}

void Thread::SetThreadLocal(LocalStorageKey key, void* value) {
  // No-op for WASI
}

// Stack trace implementation (not available in WASI)
int OS::GetStackTrace(void** trace, int size) {
  return 0;  // No stack traces in WASI
}

// Memory allocation
void* OS::Allocate(void* hint, size_t size, size_t alignment,
                   MemoryPermission access) {
  // WASI doesn't support memory protection, just use aligned allocation
  void* result = nullptr;
  if (posix_memalign(&result, alignment, size) == 0) {
    return result;
  }
  return nullptr;
}

bool OS::Free(void* address, size_t size) {
  free(address);
  return true;
}

bool OS::Release(void* address, size_t size) {
  // WASI doesn't support releasing memory back to OS
  return true;
}

bool OS::SetPermissions(void* address, size_t size, MemoryPermission access) {
  // WASI doesn't support memory permissions
  return true;
}

bool OS::RecommitPages(void* address, size_t size, MemoryPermission access) {
  // WASI doesn't support memory permissions
  return true;
}

bool OS::DiscardSystemPages(void* address, size_t size) {
  // WASI doesn't support discarding pages
  return true;
}

bool OS::DecommitPages(void* address, size_t size) {
  // WASI doesn't support decommitting pages
  return true;
}

bool OS::CanReserveAddressSpace() {
  return false;  // WASI can't reserve address space
}

std::optional<AddressSpaceReservation> OS::CreateAddressSpaceReservation(
    void* hint, size_t size, size_t alignment,
    MemoryPermission max_permission) {
  return std::nullopt;  // Not supported in WASI
}

void OS::FreeAddressSpaceReservation(AddressSpaceReservation reservation) {
  // No-op for WASI
}

bool OS::SetRandomMmapSeed(int64_t seed) {
  return false;  // Not supported in WASI
}

void* OS::GetRandomMmapAddr() {
  // Return a pseudo-random address
  static uint32_t counter = 0x40000000;
  counter += 0x10000;
  return reinterpret_cast<void*>(static_cast<uintptr_t>(counter));
}

// File operations
OS::MemoryMappedFile* OS::MemoryMappedFile::open(const char* name,
                                                  FileMode mode) {
  // WASI file operations would go here
  // For now, return nullptr as WASI file API is complex
  return nullptr;
}

OS::MemoryMappedFile* OS::MemoryMappedFile::create(const char* name,
                                                    size_t size, void* initial) {
  void* memory = malloc(size);
  if (!memory) return nullptr;
  
  if (initial) {
    memcpy(memory, initial, size);
  } else {
    memset(memory, 0, size);
  }
  
  return new WASIMemoryMappedFile(memory, size);
}

// Time functions
double OS::TimeCurrentMillis() {
#ifdef __wasi__
  uint64_t timestamp;
  int32_t err = __wasi_clock_time_get(
    __WASI_CLOCKID_REALTIME,
    1000000,  // 1ms precision
    &timestamp
  );
  
  if (err != __WASI_ERRNO_SUCCESS) {
    return 0.0;
  }
  
  return static_cast<double>(timestamp) / 1000000.0;
#else
  return 0.0;
#endif
}

int64_t OS::TimeCurrentMillis(bool is_monotonic) {
#ifdef __wasi__
  uint32_t clock_id = is_monotonic 
    ? __WASI_CLOCKID_MONOTONIC 
    : __WASI_CLOCKID_REALTIME;
  
  uint64_t timestamp;
  int32_t err = __wasi_clock_time_get(
    clock_id,
    1000000,  // 1ms precision
    &timestamp
  );
  
  if (err != __WASI_ERRNO_SUCCESS) {
    return 0;
  }
  
  return timestamp / 1000000;  // Convert nanoseconds to milliseconds
#else
  return 0;
#endif
}

// System information
int OS::GetUserTime(uint32_t* secs, uint32_t* usecs) {
  // WASI doesn't provide user time information
  *secs = 0;
  *usecs = 0;
  return -1;
}

int OS::GetCurrentProcessId() {
  // WASI doesn't have process IDs
  return 1;
}

int OS::GetCurrentThreadId() {
  // WASI is single-threaded
  return 1;
}

void OS::ExitProcess(int exit_code) {
#ifdef __wasi__
  __wasi_proc_exit(exit_code);
#else
  exit(exit_code);
#endif
}

// Abort and debugging
void OS::Abort() {
  // WASI abort
#ifdef __wasi__
  __wasi_proc_exit(134);  // SIGABRT exit code
#else
  abort();
#endif
}

void OS::DebugBreak() {
  // No debugger support in WASI
  Abort();
}

// Sleep
void OS::Sleep(TimeDelta interval) {
  // WASI doesn't have sleep, but we can yield
#ifdef __wasi__
  __wasi_sched_yield();
#endif
}

// CPU information
int OS::NumberOfWorkerThreads() {
  // WASI is single-threaded
  return 1;
}

size_t OS::AllocatePageSize() {
  return 4096;  // Standard page size
}

size_t OS::CommitPageSize() {
  return 4096;  // Standard page size
}

void OS::SetRandomSeed(int64_t seed) {
  // Initialize random number generator
  srandom(static_cast<unsigned int>(seed));
}

void OS::SignalCodeMovingGC() {
  // No-op for WASI
}

void OS::AdjustSchedulingParams() {
  // No-op for WASI
}

// Memory protection key support (not available in WASI)
bool OS::MemoryProtectionKeysEnabled() {
  return false;
}

int OS::AllocateMemoryProtectionKey() {
  return -1;
}

void OS::FreeMemoryProtectionKey(int key) {
  // No-op
}

bool OS::SetMemoryProtectionKey(void* address, size_t size, int key) {
  return false;
}

// POSIX-specific functions that need WASI implementation
FILE* OS::FOpen(const char* path, const char* mode) {
  // WASI doesn't have standard FILE*, return nullptr
  return nullptr;
}

bool OS::Remove(const char* path) {
  // WASI doesn't support file removal directly
  return false;
}

char OS::DirectorySeparator() {
  return '/';
}

bool OS::isDirectorySeparator(const char ch) {
  return ch == '/';
}

// Random entropy
bool OS::GetRandomBytes(void* buffer, size_t size) {
  // Use WASI random_get for entropy
#ifdef __wasi__
  int32_t err = __wasi_random_get(static_cast<uint8_t*>(buffer), size);
  return err == __WASI_ERRNO_SUCCESS;
#else
  return false;
#endif
}

std::unique_ptr<OS::SharedLibraryAddresses> OS::GetSharedLibraryAddresses() {
  // WASI doesn't support shared libraries
  return std::make_unique<SharedLibraryAddresses>();
}

void OS::GetSharedLibraryAddresses(SharedLibraryAddresses* addresses) {
  // No shared libraries in WASI
}

int OS::GetActivationFrameCount() {
  // No stack walking in WASI
  return 0;
}

}  // namespace base
}  // namespace v8
