#ifdef __wasi__
// LSAN not supported on WASI
#ifndef V8_BASE_SANITIZER_LSAN_VIRTUAL_ADDRESS_SPACE_H_
#define V8_BASE_SANITIZER_LSAN_VIRTUAL_ADDRESS_SPACE_H_
// Empty header for WASI
#endif
#else
// Copyright 2021 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_SANITIZER_LSAN_VIRTUAL_ADDRESS_SPACE_H_
#define V8_BASE_SANITIZER_LSAN_VIRTUAL_ADDRESS_SPACE_H_

#include "include/v8-platform.h"
#include "src/base/base-export.h"
#include "src/base/compiler-specific.h"

namespace v8 {
namespace base {

using Address = uintptr_t;

// This is a v8::VirtualAddressSpace implementation that decorates provided page
// allocator object with leak sanitizer notifications when LEAK_SANITIZER is
// defined.
class V8_BASE_EXPORT LsanVirtualAddressSpace final
    : public v8::VirtualAddressSpace {
 public:
  explicit LsanVirtualAddressSpace(
      std::unique_ptr<v8::VirtualAddressSpace> vas);
  ~LsanVirtualAddressSpace() override = default;

  void SetRandomSeed(int64_t seed) override {
    return vas_->SetRandomSeed(seed);
  }

  Address RandomPageAddress() override { return vas_->RandomPageAddress(); }

  Address AllocatePages(Address hint, size_t size, size_t alignment,
                        PagePermissions permissions) override;

  void FreePages(Address address, size_t size) override;

  Address AllocateSharedPages(Address hint, size_t size,
                              PagePermissions permissions,
                              PlatformSharedMemoryHandle handle,
                              uint64_t offset) override;

  void FreeSharedPages(Address address, size_t size) override;

  bool SetPagePermissions(Address address, size_t size,
                          PagePermissions permissions) override {
    return vas_->SetPagePermissions(address, size, permissions);
  }

  bool RecommitPages(Address address, size_t size,
                     PagePermissions permissions) override {
    return vas_->RecommitPages(address, size, permissions);
  }

  bool AllocateGuardRegion(Address address, size_t size) override {
    return vas_->AllocateGuardRegion(address, size);
  }

  void FreeGuardRegion(Address address, size_t size) override {
    vas_->FreeGuardRegion(address, size);
  }

  bool CanAllocateSubspaces() override { return vas_->CanAllocateSubspaces(); }

  std::unique_ptr<VirtualAddressSpace> AllocateSubspace(
      Address hint, size_t size, size_t alignment,
      PagePermissions max_page_permissions) override;

  bool DiscardSystemPages(Address address, size_t size) override {
    return vas_->DiscardSystemPages(address, size);
  }

  bool DecommitPages(Address address, size_t size) override {
    return vas_->DecommitPages(address, size);
  }

  // Required VirtualAddressSpace interface methods
  Address base() const override { return vas_->base(); }
  size_t size() const override { return vas_->size(); }

 private:
  std::unique_ptr<v8::VirtualAddressSpace> vas_;
};

}  // namespace base
}  // namespace v8
#endif  // V8_BASE_SANITIZER_LSAN_VIRTUAL_ADDRESS_SPACE_H_
#endif // __wasi__
