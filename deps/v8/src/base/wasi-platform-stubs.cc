#ifdef __wasi__

#include "src/base/page-allocator.h"
#include <cstdlib>

namespace v8 {
namespace base {

// Minimal WASI PageAllocator implementation
void PageAllocator::SetRandomMmapSeed(int64_t seed) {
  // No-op for WASI
}

void* PageAllocator::GetRandomMmapAddr() {
  return nullptr;
}

void* PageAllocator::AllocatePages(void* address, size_t size, size_t alignment,
                                   PageAllocator::Permission access) {
  return std::aligned_alloc(alignment, size);
}

bool PageAllocator::CanAllocateSharedPages() {
  return false;
}

std::unique_ptr<PageAllocator::SharedMemory> PageAllocator::AllocateSharedPages(
    size_t size, const void* original_address) {
  return nullptr;
}

bool PageAllocator::FreePages(void* address, size_t size) {
  if (address) {
    std::free(address);
    return true;
  }
  return false;
}

bool PageAllocator::ReleasePages(void* address, size_t size, size_t new_size) {
  return true; // No-op for WASI
}

bool PageAllocator::SetPermissions(void* address, size_t size,
                                   PageAllocator::Permission access) {
  return true; // No-op for WASI
}

bool PageAllocator::RecommitPages(void* address, size_t size,
                                  PageAllocator::Permission access) {
  return true; // No-op for WASI
}

bool PageAllocator::DiscardSystemPages(void* address, size_t size) {
  return true; // No-op for WASI
}

bool PageAllocator::DecommitPages(void* address, size_t size) {
  return true; // No-op for WASI
}

bool PageAllocator::SealPages(void* address, size_t size) {
  return true; // No-op for WASI
}

}  // namespace base
}  // namespace v8

#endif // __wasi__
