// Copyright 2024 the V8 project authors. All rights reserved.
// Platform-specific code for WASI

#include "src/base/platform/platform.h"
#include <cstdlib>
#include <cstring>

namespace v8 {
namespace base {

// Stub implementations for WASI platform
void OS::Initialize(bool hard_abort, const char* const gc_fake_mmap) {
  // WASI initialization
}

int OS::GetCurrentProcessId() {
  return 1;  // WASI doesn't have process IDs
}

int OS::GetCurrentThreadId() {
  return 1;  // WASI doesn't have thread IDs
}

void OS::ExitProcess(int exit_code) {
  exit(exit_code);
}

size_t OS::AllocatePageSize() {
  return 65536;  // 64KB pages for WASI
}

size_t OS::CommitPageSize() {
  return 65536;  // 64KB pages for WASI
}

void OS::SetRandomMmapSeed(int64_t seed) {
  // Not supported on WASI
}

void* OS::Allocate(void* hint, size_t size, size_t alignment,
                   OS::MemoryPermission access) {
  void* result = aligned_alloc(alignment, size);
  if (result == nullptr) return nullptr;
  memset(result, 0, size);
  return result;
}

void OS::Free(void* address, size_t size) {
  free(address);
}

void OS::Release(void* address, size_t size) {
  // WASI doesn't support releasing memory
}

void OS::SetPermissions(void* address, size_t size,
                       OS::MemoryPermission access) {
  // WASI doesn't support changing memory permissions
}

void OS::DiscardSystemPages(void* address, size_t size) {
  // WASI doesn't support discarding pages
}

void OS::DecommitPages(void* address, size_t size) {
  // WASI doesn't support decommitting pages
}

bool OS::RecommitPages(void* address, size_t size,
                      OS::MemoryPermission access) {
  // WASI doesn't support recommitting pages
  return true;
}

bool OS::HasLazyCommits() {
  return false;
}

std::vector<OS::SharedLibraryAddress> OS::GetSharedLibraryAddresses() {
  return std::vector<OS::SharedLibraryAddress>();
}

void OS::SignalCodeMovingGC() {
  // Not supported on WASI
}

void OS::AdjustSchedulingParams() {
  // Not supported on WASI
}

bool OS::DiscardSystemPages(void* address, size_t size, size_t* ret) {
  if (ret) *ret = 0;
  return true;
}

// Stack trace support
int OS::StackWalk(const OS::StackWalker& walker) {
  return 0;  // No stack walking on WASI
}

}  // namespace base
}  // namespace v8
