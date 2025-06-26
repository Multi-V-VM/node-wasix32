#ifndef WASI_PAGE_ALLOCATOR_BASE_H
#define WASI_PAGE_ALLOCATOR_BASE_H

#include <cstddef>
#include <cstdint>
#include <memory>

namespace v8 {

// Forward declaration
using PlatformSharedMemoryHandle = void*;

// Base PageAllocator interface
class PageAllocator {
 public:
  virtual ~PageAllocator() = default;
  
  enum Permission {
    kNoAccess,
    kRead,
    kReadWrite,
    kReadWriteExecute,
    kReadExecute
  };
  
  using Address = uintptr_t;
  using PagePermissions = Permission;
  
  virtual void* AllocatePages(void* hint, size_t size, size_t alignment,
                              Permission access) = 0;
  virtual void FreePages(void* address, size_t size) = 0;
  virtual void ReleasePages(void* address, size_t size, size_t free_size) = 0;
  virtual void SetPermissions(void* address, size_t size, Permission access) = 0;
  virtual void DiscardSystemPages(void* address, size_t size) {}
  virtual void DecommitPages(void* address, size_t size) = 0;
  virtual void RecommitPages(void* address, size_t size, Permission access) = 0;
  
  // SharedMemory support
  virtual Address AllocateSharedPages(Address hint, size_t size, PagePermissions permissions, 
                                     PlatformSharedMemoryHandle handle, uint64_t offset) = 0;
  
  // Additional methods
  virtual bool CanAllocateSharedPages() { return false; }
};

}  // namespace v8

#endif // WASI_PAGE_ALLOCATOR_BASE_H
