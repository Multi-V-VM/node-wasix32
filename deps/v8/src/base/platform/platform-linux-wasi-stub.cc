// Copyright 2026 the V8 project authors. All rights reserved.
// WASI replacement for Linux-only V8 base platform entry points.

#include "src/base/platform/platform-linux.h"

#include <stdio.h>

#include <vector>

#include "src/base/platform/platform-posix-time.h"

namespace v8 {
namespace base {

TimezoneCache* OS::CreateTimezoneCache() {
  return new PosixDefaultTimezoneCache();
}

void OS::SignalCodeMovingGC() {}

void OS::AdjustSchedulingParams() {}

void* OS::RemapShared(void* old_address, void* new_address, size_t size) {
  return nullptr;
}

std::vector<OS::MemoryRange> OS::GetFreeMemoryRangesWithin(
    OS::Address boundary_start, OS::Address boundary_end, size_t minimum_size,
    size_t alignment) {
  return {};
}

base::Optional<MemoryRegion> MemoryRegion::FromMapsLine(const char* line) {
  return base::nullopt;
}

std::vector<OS::SharedLibraryAddress> GetSharedLibraryAddresses(FILE* fp) {
  return {};
}

std::vector<OS::SharedLibraryAddress> OS::GetSharedLibraryAddresses() {
  return {};
}

bool OS::RemapPages(const void* address, size_t size, void* new_address,
                    MemoryPermission access) {
  return false;
}

}  // namespace base
}  // namespace v8
