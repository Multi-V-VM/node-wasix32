// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/heap/memory-allocator.h"

#include <cinttypes>
#include <optional>

#include "src/base/address-region.h"
#include "src/base/macros.h"
#include "src/common/globals.h"
#include "src/execution/isolate.h"
#include "src/flags/flags.h"
#include "src/heap/gc-tracer-inl.h"
#include "src/heap/gc-tracer.h"
#include "src/heap/heap-inl.h"
#include "src/heap/heap.h"
#include "src/heap/large-page-metadata.h"
#include "src/heap/memory-chunk-metadata.h"
#include "src/heap/mutable-page-metadata.h"
#include "src/heap/page-pool.h"
#include "src/heap/read-only-spaces.h"
#include "src/heap/zapping.h"
#include "src/logging/log.h"
#include "src/sandbox/hardware-support.h"
#include "src/utils/allocation.h"

namespace v8 {
namespace internal {

// -----------------------------------------------------------------------------
// MemoryAllocator
//

size_t MemoryAllocator::commit_page_size_ = 0;
size_t MemoryAllocator::commit_page_size_bits_ = 0;

MemoryAllocator::MemoryAllocator(Isolate* isolate,
                                 ::v8::PageAllocator* code_page_allocator,
                                 ::v8::PageAllocator* trusted_page_allocator,
                                 size_t capacity)
    : isolate_(isolate),
      data_page_allocator_(isolate->page_allocator()),
      code_page_allocator_(code_page_allocator),
      trusted_page_allocator_(trusted_page_allocator),
      capacity_(RoundUp(capacity, PageMetadata::kPageSize)) {
  DCHECK_NOT_NULL(data_page_allocator_);
  DCHECK_NOT_NULL(code_page_allocator_);
  DCHECK_NOT_NULL(trusted_page_allocator_);
  pool_ = isolate_->isolate_group()->page_pool();
}

void MemoryAllocator::TearDown() {
  DCHECK_EQ(pool()->GetCount(isolate_), 0);

  // Check that spaces were torn down before MemoryAllocator.
  DCHECK_EQ(size_, 0u);
  // TODO(gc) this will be true again when we fix FreeMemory.
  // DCHECK_EQ(0, size_executable_);
  capacity_ = 0;

  if (reserved_chunk_at_virtual_memory_limit_) {
    reserved_chunk_at_virtual_memory_limit_->Free();
  }

  code_page_allocator_ = nullptr;
  data_page_allocator_ = nullptr;
  trusted_page_allocator_ = nullptr;
}

size_t MemoryAllocator::GetPooledChunksCount() {
  return pool()->GetCount(isolate_);
}

size_t MemoryAllocator::GetSharedPooledChunksCount() {
  return pool()->GetSharedCount();
}

size_t MemoryAllocator::GetTotalPooledChunksCount() {
  return pool()->GetTotalCount();
}

void MemoryAllocator::ReleasePooledChunksImmediately() {
  pool()->ReleaseImmediately(isolate_);
}

void MemoryAllocator::FreeMemoryRegion(v8::PageAllocator* page_allocator,
                                       Address base, size_t size) {
  FreePages(page_allocator, reinterpret_cast<void*>(base), size);
}

Address MemoryAllocator::AllocateAlignedMemory(
    size_t chunk_size, size_t area_size, size_t alignment,
    AllocationSpace space, Executability executable, void* hint,
    VirtualMemory* controller) {
  DCHECK_EQ(space == CODE_SPACE || space == CODE_LO_SPACE,
            executable == EXECUTABLE);
  ::v8::PageAllocator* page_allocator = this->page_allocator(space);
  DCHECK_LT(area_size, chunk_size);

  PageAllocator::Permission permissions =
      executable == EXECUTABLE
          ? MutablePageMetadata::GetCodeModificationPermission()
          : PageAllocator::kReadWrite;
  VirtualMemory reservation(page_allocator, chunk_size, hint, alignment,
                            permissions);
  if (!reservation.IsReserved()) return HandleAllocationFailure(executable);

  // We cannot use the last chunk in the address space because we would
  // overflow when comparing top and limit if this chunk is used for a
  // linear allocation area.
  if ((reservation.address() + static_cast<Address>(chunk_size)) == 0u) {
    CHECK(!reserved_chunk_at_virtual_memory_limit_);
    reserved_chunk_at_virtual_memory_limit_ = std::move(reservation);
    CHECK(reserved_chunk_at_virtual_memory_limit_);

    // Retry reserve virtual memory.
    reservation =
        VirtualMemory(page_allocator, chunk_size, hint, alignment, permissions);
    if (!reservation.IsReserved()) return HandleAllocationFailure(executable);
  }

  Address base = reservation.address();

  if (executable == EXECUTABLE) {
    ThreadIsolation::RegisterJitPage(base, chunk_size);
  }

  UpdateAllocatedSpaceLimits(base, base + chunk_size, executable);

  *controller = std::move(reservation);
  return base;
}

Address MemoryAllocator::HandleAllocationFailure(Executability executable) {
  Heap* heap = isolate_->heap();
  if (!heap->deserialization_complete()) {
    heap->FatalProcessOutOfMemory(
        executable == EXECUTABLE
            ? "Executable MemoryChunk allocation failed during deserialization."
            : "MemoryChunk allocation failed during deserialization.");
  }
  return kNullAddress;
}

size_t MemoryAllocator::ComputeChunkSize(size_t area_size,
                                         AllocationSpace space) {
  //
  // +----------------------------+<- base aligned at MemoryChunk::kAlignment
  // |          Header            |
  // +----------------------------+<- area_start_ (base + area_start_)
  // |           Area             |
  // +----------------------------+<- area_end_ (area_start + area_size)
  // |  Committed but not used    |
  // +----------------------------+<- base + chunk_size
  //

  return ::RoundUp(
      MemoryChunkLayout::ObjectStartOffsetInMemoryChunk(space) + area_size,
      GetCommitPageSize());
}

std::optional<MemoryAllocator::MemoryChunkAllocationResult>
MemoryAllocator::AllocateUninitializedChunkAt(BaseSpace* space,
                                              size_t area_size,
                                              Executability executable,
                                              Address hint,
                                              PageSize page_size) {
#ifndef V8_COMPRESS_POINTERS
  // When pointer compression is enabled, spaces are expected to be at a
  // predictable address (see mkgrokdump) so we don't supply a hint and rely on
  // the deterministic behaviour of the BoundedPageAllocator.
  if (hint == kNullAddress) {
    hint = reinterpret_cast<Address>(
        AlignedAddress(isolate_->heap()->GetRandomMmapAddr(),
                       MemoryChunk::GetAlignmentForAllocation()));
  }
#endif

  VirtualMemory reservation;
  size_t chunk_size = ComputeChunkSize(area_size, space->identity());
  DCHECK_EQ(chunk_size % GetCommitPageSize(), 0);

  Address base = AllocateAlignedMemory(
      chunk_size, area_size, MemoryChunk::GetAlignmentForAllocation(),
      space->identity(), executable, reinterpret_cast<void*>(hint),
      &reservation);
  if (base == kNullAddress) return {};

  size_ += reservation.size();

  // Update executable memory size.
  if (executable == EXECUTABLE) {
    size_executable_ += reservation.size();
  }

  if (heap::ShouldZapGarbage()) {
    if (executable == EXECUTABLE) {
      CodePageMemoryModificationScopeForDebugging memory_write_scope(
          isolate_->heap(), &reservation,
          base::AddressRegion(base, chunk_size));
      heap::ZapBlock(base, chunk_size, kZapValue);
    } else {
      DCHECK_EQ(executable, NOT_EXECUTABLE);
      // Zap both page header and object area at once. No guard page in-between.
      heap::ZapBlock(base, chunk_size, kZapValue);
    }
  }

  LOG(isolate_,
      NewEvent("MemoryChunk", reinterpret_cast<void*>(base), chunk_size));

  Address area_start = base + MemoryChunkLayout::ObjectStartOffsetInMemoryChunk(
                                  space->identity());
  Address area_end = area_start + area_size;

  return MemoryChunkAllocationResult{
      reinterpret_cast<void*>(base), nullptr, chunk_size, area_start, area_end,
      std::move(reservation),
  };
}

void MemoryAllocator::PartialFreeMemory(MemoryChunkMetadata* chunk,
                                        Address start_free,
                                        size_t bytes_to_free,
                                        Address new_area_end) {
  VirtualMemory* reservation = chunk->reserved_memory();
  DCHECK(reservation->IsReserved());
  chunk->set_size(chunk->size() - bytes_to_free);
  chunk->set_area_end(new_area_end);
  if (chunk->Chunk()->IsFlagSet(MemoryChunk::IS_EXECUTABLE)) {
    // Add guard page at the end.
    size_t page_size = GetCommitPageSize();
    DCHECK_EQ(0, chunk->area_end() % static_cast<Address>(page_size));
    DCHECK_EQ(chunk->ChunkAddress() + chunk->size(), chunk->area_end());

    if ((V8_HEAP_USE_PTHREAD_JIT_WRITE_PROTECT ||
         V8_HEAP_USE_BECORE_JIT_WRITE_PROTECT) &&
        !isolate_->jitless()) {
      DCHECK(isolate_->RequiresCodeRange());
      DiscardSealedMemoryScope discard_scope("Partially free memory.");
      reservation->DiscardSystemPages(chunk->area_end(), page_size);
    } else {
      CHECK(reservation->SetPermissions(chunk->area_end(), page_size,
                                        PageAllocator::kNoAccess));
    }
  }
  // On e.g. Windows, a reservation may be larger than a page and releasing
  // partially starting at |start_free| will also release the potentially
  // unused part behind the current page.
  const size_t released_bytes = reservation->Release(start_free);
  DCHECK_GE(size_, released_bytes);
  size_ -= released_bytes;
}

void MemoryAllocator::UnregisterSharedMemoryChunk(MemoryChunkMetadata* chunk) {
  VirtualMemory* reservation = chunk->reserved_memory();
  const size_t size =
      reservation->IsReserved() ? reservation->size() : chunk->size();
  DCHECK_GE(size_, static_cast<size_t>(size));
  size_ -= size;
}

void MemoryAllocator::UnregisterMemoryChunk(MemoryChunkMetadata* chunk_metadata,
                                            Executability executable) {
  MemoryChunk* chunk = chunk_metadata->Chunk();
  DCHECK(!chunk->IsFlagSet(MemoryChunk::UNREGISTERED));
  VirtualMemory* reservation = chunk_metadata->reserved_memory();
  const size_t size =
      reservation->IsReserved() ? reservation->size() : chunk_metadata->size();
  DCHECK_GE(size_, static_cast<size_t>(size));

  size_ -= size;
  if (executable == EXECUTABLE) {
    DCHECK_GE(size_executable_, size);
    size_executable_ -= size;
#ifdef DEBUG
    UnregisterExecutableMemoryChunk(
        static_cast<MutablePageMetadata*>(chunk_metadata));
#endif  // DEBUG

    ThreadIsolation::UnregisterJitPage(chunk->address(),
                                       chunk_metadata->size());
  }
  chunk->SetFlagSlow(MemoryChunk::UNREGISTERED);
}

void MemoryAllocator::UnregisterMutableMemoryChunk(MutablePageMetadata* chunk) {
  UnregisterMemoryChunk(chunk, chunk->Chunk()->executable());
}

void MemoryAllocator::UnregisterReadOnlyPage(ReadOnlyPageMetadata* page) {
  DCHECK(!page->Chunk()->executable());
  UnregisterMemoryChunk(page, NOT_EXECUTABLE);
}

void MemoryAllocator::FreeReadOnlyPage(ReadOnlyPageMetadata* chunk) {
  DCHECK(!chunk->Chunk()->IsFlagSet(MemoryChunk::PRE_FREED));
  LOG(isolate_, DeleteEvent("MemoryChunk", chunk));

  UnregisterSharedMemoryChunk(chunk);

  ::v8::PageAllocator* allocator = page_allocator(RO_SPACE);
  VirtualMemory* reservation = chunk->reserved_memory();
  if (reservation->IsReserved()) {
    reservation->Free();
  } else {
    // Only read-only pages can have a non-initialized reservation object. This
    // happens when the pages are remapped to multiple locations and where the
    // reservation would therefore be invalid.
    FreeMemoryRegion(allocator, chunk->ChunkAddress(),
                     RoundUp(chunk->size(), allocator->AllocatePageSize()));
  }

  delete chunk;
}

void MemoryAllocator::PreFreeMemory(MutablePageMetadata* chunk_metadata) {
  MemoryChunk* chunk = chunk_metadata->Chunk();
  DCHECK(!chunk->IsFlagSet(MemoryChunk::PRE_FREED));
  LOG(isolate_, DeleteEvent("MemoryChunk", chunk_metadata));
  UnregisterMutableMemoryChunk(chunk_metadata);
  isolate_->heap()->RememberUnmappedPage(
      reinterpret_cast<Address>(chunk_metadata),
      chunk->IsEvacuationCandidate());
  chunk->SetFlagSlow(MemoryChunk::PRE_FREED);
}

void MemoryAllocator::PerformFreeMemory(MutablePageMetadata* chunk_metadata) {
  DCHECK(chunk_metadata->Chunk()->IsFlagSet(MemoryChunk::UNREGISTERED));
  DCHECK(chunk_metadata->Chunk()->IsFlagSet(MemoryChunk::PRE_FREED));
  DCHECK(!chunk_metadata->Chunk()->InReadOnlySpace());

  chunk_metadata->ReleaseAllAllocatedMemory();

  DeleteMemoryChunk(chunk_metadata);
}

void MemoryAllocator::Free(MemoryAllocator::FreeMode mode,
                           MutablePageMetadata* chunk_metadata) {
  MemoryChunk* chunk = chunk_metadata->Chunk();
  RecordMemoryChunkDestroyed(chunk);
  PreFreeMemory(chunk_metadata);

  switch (mode) {
    case FreeMode::kImmediately:
      PerformFreeMemory(chunk_metadata);
      break;
    case FreeMode::kPostpone:
      // Record page to be freed later.
      queued_pages_to_be_freed_.push_back(chunk_metadata);
      break;
    case FreeMode::kPool:
      // Ensure that we only ever put pages with their markbits cleared into the
      // pool. This is necessary because `PreFreeMemory` doesn't clear the
      // marking bitmap and the marking bitmap is reused when this page is taken
      // out of the pool again.
      DCHECK(chunk_metadata->IsLivenessClear());
      DCHECK_EQ(chunk_metadata->size(),
                static_cast<size_t>(MutablePageMetadata::kPageSize));
      DCHECK_EQ(chunk->executable(), NOT_EXECUTABLE);
      // The chunks added to this queue will be cached until memory reducing GC.
      pool()->Add(isolate_, chunk_metadata);
      break;
  }
}

PageMetadata* MemoryAllocator::AllocatePage(
    MemoryAllocator::AllocationMode alloc_mode, Space* space,
    Executability executable) {
  const size_t size =
      MemoryChunkLayout::AllocatableMemoryInMemoryChunk(space->identity());
  std::optional<MemoryChunkAllocationResult> chunk_info;
  if (alloc_mode == AllocationMode::kUsePool) {
    DCHECK_EQ(executable, NOT_EXECUTABLE);
    chunk_info = AllocateUninitializedPageFromPool(space);
  }

  if (!chunk_info) {
    chunk_info =
        AllocateUninitializedChunk(space, size, executable, PageSize::kRegular);
  }

  if (!chunk_info) return nullptr;

  PageMetadata* metadata;
  if (chunk_info->optional_metadata) {
    metadata = new (chunk_info->optional_metadata) PageMetadata(
        isolate_->heap(), space, chunk_info->size, chunk_info->area_start,
        chunk_info->area_end, std::move(chunk_info->reservation));
  } else {
    metadata = new PageMetadata(isolate_->heap(), space, chunk_info->size,
                                chunk_info->area_start, chunk_info->area_end,
                                std::move(chunk_info->reservation));
  }
  MemoryChunk* chunk;
  MemoryChunk::MainThreadFlags flags = metadata->InitialFlags(executable);
  if (v8_flags.black_allocated_pages && space->identity() != NEW_SPACE &&
      space->identity() != NEW_LO_SPACE &&
      isolate_->heap()->incremental_marking()->black_allocation()) {
    // Disable the write barrier for objects pointing to this page. We don't
    // need to trigger the barrier for pointers to old black-allocated pages,
    // since those are never considered for evacuation. However, we have to
    // keep the old->shared remembered set across multiple GCs, so those
    // pointers still need to be recorded.
    if (!IsAnySharedSpace(space->identity())) {
      flags &= ~MemoryChunk::POINTERS_TO_HERE_ARE_INTERESTING;
    }
    // And mark the page as black allocated.
    flags |= MemoryChunk::BLACK_ALLOCATED;
  }
  if (executable) {
    RwxMemoryWriteScope scope("Initialize a new MemoryChunk.");
    chunk = new (chunk_info->chunk) MemoryChunk(flags, metadata);
  } else {
    chunk = new (chunk_info->chunk) MemoryChunk(flags, metadata);
  }

#ifdef DEBUG
  if (chunk->executable()) RegisterExecutableMemoryChunk(metadata);
#endif  // DEBUG

  DCHECK(metadata->IsLivenessClear());
  space->InitializePage(metadata);
  RecordMemoryChunkCreated(chunk);
  return metadata;
}

ReadOnlyPageMetadata* MemoryAllocator::AllocateReadOnlyPage(
    ReadOnlySpace* space, Address hint) {
  DCHECK_EQ(space->identity(), RO_SPACE);
  size_t size = MemoryChunkLayout::AllocatableMemoryInMemoryChunk(RO_SPACE);
  std::optional<MemoryChunkAllocationResult> chunk_info =
      AllocateUninitializedChunkAt(space, size, NOT_EXECUTABLE, hint,
                                   PageSize::kRegular);
  if (!chunk_info) {
    return nullptr;
  }
  CHECK_NULL(chunk_info->optional_metadata);
  ReadOnlyPageMetadata* metadata = new ReadOnlyPageMetadata(
      isolate_->heap(), space, chunk_info->size, chunk_info->area_start,
      chunk_info->area_end, std::move(chunk_info->reservation));

  new (chunk_info->chunk) MemoryChunk(metadata->InitialFlags(), metadata);

  SandboxHardwareSupport::NotifyReadOnlyPageCreated(
      metadata->ChunkAddress(), metadata->size(),
      PageAllocator::Permission::kReadWrite);

  return metadata;
}

std::unique_ptr<::v8::PageAllocator::SharedMemoryMapping>
MemoryAllocator::RemapSharedPage(
    ::v8::PageAllocator::SharedMemory* shared_memory, Address new_address) {
  return shared_memory->RemapTo(reinterpret_cast<void*>(new_address));
}

LargePageMetadata* MemoryAllocator::AllocateLargePage(
    LargeObjectSpace* space, size_t object_size, Executability executable) {
  std::optional<MemoryChunkAllocationResult> chunk_info =
      AllocateUninitializedChunk(space, object_size, executable,
                                 PageSize::kLarge);

  if (!chunk_info) return nullptr;

  LargePageMetadata* metadata;
  if (chunk_info->optional_metadata) {
    metadata = new (chunk_info->optional_metadata) LargePageMetadata(
        isolate_->heap(), space, chunk_info->size, chunk_info->area_start,
        chunk_info->area_end, std::move(chunk_info->reservation), executable);
  } else {
    metadata = new LargePageMetadata(
        isolate_->heap(), space, chunk_info->size, chunk_info->area_start,
        chunk_info->area_end, std::move(chunk_info->reservation), executable);
  }
  MemoryChunk* chunk;
  MemoryChunk::MainThreadFlags flags = metadata->InitialFlags(executable);
  if (executable) {
    RwxMemoryWriteScope scope("Initialize a new MemoryChunk.");
    chunk = new (chunk_info->chunk) MemoryChunk(flags, metadata);
  } else {
    chunk = new (chunk_info->chunk) MemoryChunk(flags, metadata);
  }

#ifdef DEBUG
  if (chunk->executable()) RegisterExecutableMemoryChunk(metadata);
#endif  // DEBUG

  RecordMemoryChunkCreated(chunk);
  return metadata;
}

bool MemoryAllocator::ResizeLargePage(LargePageMetadata* page,
                                      size_t old_object_size,
                                      size_t new_object_size) {
  const size_t old_reservation_size = page->reservation_.size();
  const size_t old_page_end = page->reservation_.end();
  const Address new_area_end = page->area_start() + new_object_size;
  const Address new_page_end = RoundUp(new_area_end, GetCommitPageSize());
  const size_t new_page_size = new_page_end - page->ChunkAddress();

  if (old_page_end == new_page_end) {
    // We were able to grow the object without growing its owning page.
    page->set_area_end(new_area_end);
    return true;
  }

  // Currently we only support growing.
  DCHECK_LT(old_page_end, new_page_end);

  if (!page->reservation_.Resize(page->ChunkAddress(), new_page_size,
                                 PageAllocator::kReadWrite)) {
    return false;
  }

  page->set_area_end(new_area_end);
  page->set_size(new_page_size);

#if DEBUG
  AllocationSpace space = page->owner_identity();
  DCHECK(space == NEW_LO_SPACE || space == LO_SPACE);
#endif  // DEBUG

  UpdateAllocatedSpaceLimits(page->ChunkAddress(), new_page_end,
                             Executability::NOT_EXECUTABLE);

  size_ -= old_reservation_size;
  size_ += page->reservation_.size();

  return true;
}

std::optional<MemoryAllocator::MemoryChunkAllocationResult>
MemoryAllocator::AllocateUninitializedPageFromPool(Space* space) {
  MemoryChunkMetadata* chunk_metadata = pool()->Remove(isolate_);
  if (chunk_metadata == nullptr) return {};
  const int size = MutablePageMetadata::kPageSize;
  const Address start = chunk_metadata->ChunkAddress();
  const Address area_start =
      start +
      MemoryChunkLayout::ObjectStartOffsetInMemoryChunk(space->identity());
  const Address area_end = start + size;
  // Pooled pages are always regular data pages.
  DCHECK_NE(CODE_SPACE, space->identity());
  DCHECK_NE(TRUSTED_SPACE, space->identity());
  VirtualMemory reservation(data_page_allocator(), start, size);
  if (heap::ShouldZapGarbage()) {
    heap::ZapBlock(start, size, kZapValue);
  }
  size_ += size;
  UpdateAllocatedSpaceLimits(start, start + size,
                             Executability::NOT_EXECUTABLE);
  return MemoryChunkAllocationResult{
      chunk_metadata->Chunk(), chunk_metadata, size, area_start, area_end,
      std::move(reservation),
  };
}

void MemoryAllocator::InitializeOncePerProcess() {
  commit_page_size_ = v8_flags.v8_os_page_size > 0
                          ? v8_flags.v8_os_page_size * KB
                          : CommitPageSize();
  CHECK(base::bits::IsPowerOfTwo(commit_page_size_));
  commit_page_size_bits_ = base::bits::WhichPowerOfTwo(commit_page_size_);
}

bool MemoryAllocator::SetPermissionsOnExecutableMemoryChunk(VirtualMemory* vm,
                                                            Address start,
                                                            size_t chunk_size) {
  // All addresses and sizes must be aligned to the commit page size.
  DCHECK(IsAligned(start, GetCommitPageSize()));
  DCHECK_EQ(0, chunk_size % GetCommitPageSize());

  if (isolate_->RequiresCodeRange()) {
    // The pages of the code range are already mapped RWX, we just need to
    // recommit them.
    return vm->RecommitPages(start, chunk_size,
                             PageAllocator::kReadWriteExecute);
  } else {
    return vm->SetPermissions(
        start, chunk_size,
        MutablePageMetadata::GetCodeModificationPermission());
  }
}

const MemoryChunk* MemoryAllocator::LookupChunkContainingAddress(
    Address addr) const {
  base::MutexGuard guard(&chunks_mutex_);
  return LookupChunkContainingAddressInSafepoint(addr);
}

const MemoryChunk* MemoryAllocator::LookupChunkContainingAddressInSafepoint(
    Address addr) const {
  // All threads should be either parked or in a safepoint whenever this method
  // is called, thus pages cannot be allocated or freed at the same time and a
  // mutex is not required here.
  // As the address may not correspond to a valid heap object, the chunk we
  // obtain below is not necessarily a valid chunk.
  MemoryChunk* chunk = MemoryChunk::FromAddress(addr);
  // Check if it corresponds to a known normal or large page.
  if (auto normal_page_it = normal_pages_.find(chunk);
      normal_page_it != normal_pages_.end()) {
    // The chunk is a normal page.
    // auto* normal_page = PageMetadata::cast(chunk);
    DCHECK_LE((*normal_page_it)->address(), addr);
    if (chunk->Metadata()->Contains(addr)) return chunk;
  } else if (auto large_page_it = large_pages_.upper_bound(chunk);
             large_page_it != large_pages_.begin()) {
    // The chunk could be inside a large page.
    DCHECK_IMPLIES(large_page_it != large_pages_.end(),
                   addr < (*large_page_it)->address());
    auto* large_page_chunk = *std::next(large_page_it, -1);
    DCHECK_NOT_NULL(large_page_chunk);
    DCHECK_LE(large_page_chunk->address(), addr);
    if (large_page_chunk->Metadata()->Contains(addr)) return large_page_chunk;
  }
  // Not found in any page.
  return nullptr;
}

void MemoryAllocator::RecordMemoryChunkCreated(const MemoryChunk* chunk) {
  base::MutexGuard guard(&chunks_mutex_);
  if (chunk->IsLargePage()) {
    auto result = large_pages_.insert(chunk);
    USE(result);
    DCHECK(result.second);
  } else {
    auto result = normal_pages_.insert(chunk);
    USE(result);
    DCHECK(result.second);
  }
}

void MemoryAllocator::RecordMemoryChunkDestroyed(const MemoryChunk* chunk) {
  base::MutexGuard guard(&chunks_mutex_);
  if (chunk->IsLargePage()) {
    auto size = large_pages_.erase(chunk);
    USE(size);
    DCHECK_EQ(1u, size);
  } else {
    auto size = normal_pages_.erase(chunk);
    USE(size);
    DCHECK_EQ(1u, size);
  }
}

void MemoryAllocator::ReleaseQueuedPages() {
  for (auto* chunk : queued_pages_to_be_freed_) {
    PerformFreeMemory(chunk);
  }
  queued_pages_to_be_freed_.clear();
}

// static
void MemoryAllocator::DeleteMemoryChunk(MutablePageMetadata* metadata) {
  DCHECK(metadata->reserved_memory()->IsReserved());
  DCHECK(!metadata->Chunk()->InReadOnlySpace());
  // The Metadata contains a VirtualMemory reservation and the destructor will
  // release the MemoryChunk.
  DiscardSealedMemoryScope discard_scope("Deleting a memory chunk");
  if (metadata->IsLargePage()) {
    delete reinterpret_cast<LargePageMetadata*>(metadata);
  } else {
    delete reinterpret_cast<PageMetadata*>(metadata);
  }
}

}  // namespace internal
}  // namespace v8
