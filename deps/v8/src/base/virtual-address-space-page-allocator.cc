// Copyright 2018 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/base/virtual-address-space-page-allocator.h"

namespace v8 {
namespace base {

VirtualAddressSpacePageAllocator::VirtualAddressSpacePageAllocator(
    v8::VirtualAddressSpace* vas)
    : vas_(vas) {}

void* VirtualAddressSpacePageAllocator::AllocatePages(
    void* hint, size_t size, size_t alignment,
    PagePermissions access) {
  return reinterpret_cast<void*>(
      vas_->AllocatePages(reinterpret_cast<Address>(hint), size, alignment,
                          access));
}

bool VirtualAddressSpacePageAllocator::FreePages(void* ptr, size_t size) {
  MutexGuard guard(&mutex_);
  Address address = reinterpret_cast<Address>(ptr);
  // Was this allocation resized previously? If so, use the original size.
  auto result = resized_allocations_.find(address);
  if (result != resized_allocations_.end()) {
    size = result->second;
    resized_allocations_.erase(result);
  }
  vas_->FreePages(address, size);
  return true;
}

bool VirtualAddressSpacePageAllocator::ReleasePages(void* ptr, size_t size) {
  // The VirtualAddressSpace class doesn't support this method because it can't
  // be properly implemented on top of Windows placeholder mappings (they cannot
  // be partially freed or resized while being allocated). Instead, we emulate
  // this behaviour by decommitting the released pages, which in effect achieves
  // exactly what ReleasePages would normally do as well. However, we still need
  // to pass the original size to FreePages eventually, so we'll need to keep
  // track of that.
  DCHECK_LE(new_size, size);

  MutexGuard guard(&mutex_);
  // For WASI, we don't support partial release, so just return true
  return true;
}

bool VirtualAddressSpacePageAllocator::SetPermissions(
    void* address, size_t size, PagePermissions access) {
  return vas_->SetPagePermissions(reinterpret_cast<Address>(address), size,
                                  access);
}

bool VirtualAddressSpacePageAllocator::RecommitPages(
    void* address, size_t size, PagePermissions access) {
  return vas_->RecommitPages(reinterpret_cast<Address>(address), size,
                             access);
}

bool VirtualAddressSpacePageAllocator::DiscardSystemPages(void* address,
                                                          size_t size) {
  return vas_->DiscardSystemPages(reinterpret_cast<Address>(address), size);
}

bool VirtualAddressSpacePageAllocator::DecommitPages(void* address,
                                                     size_t size) {
  return vas_->DecommitPages(reinterpret_cast<Address>(address), size);
}

bool VirtualAddressSpacePageAllocator::SealPages(void* address, size_t size) {
  return false;
}

}  // namespace base
}  // namespace v8
