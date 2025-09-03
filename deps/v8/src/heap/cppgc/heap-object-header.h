// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_HEAP_CPPGC_HEAP_OBJECT_HEADER_H_
#define V8_HEAP_CPPGC_HEAP_OBJECT_HEADER_H_

#include <stdint.h>

#include <atomic>

#include "include/cppgc/internal/gc-info.h"
#include "include/cppgc/internal/member-storage.h"
#include "include/cppgc/internal/name-trait.h"
#include "src/base/atomic-utils.h"
#include "src/base/bit-field.h"
#include "src/base/logging.h"
#include "src/base/macros.h"
#include "src/heap/cppgc/gc-info-table.h"
#include "src/heap/cppgc/globals.h"

#if defined(CPPGC_CAGED_HEAP)
#include "src/heap/cppgc/caged-heap.h"
#endif  // defined(CPPGC_CAGED_HEAP)

namespace cppgc {

class Visitor;

namespace internal {

// HeapObjectHeader contains meta data per object and is prepended to each
// object.
//
// +-----------------+------+------------------------------------------+
// | name            | bits |                                          |
// +-----------------+------+------------------------------------------+
// | padding         |   32 | Only present on 64-bit platform.         |
// +-----------------+------+------------------------------------------+
// | GCInfoIndex     |   14 |                                          |
// | unused          |    1 |                                          |
// | in construction |    1 | In construction encoded as |false|.      |
// +-----------------+------+------------------------------------------+
// | size            |   15 | 17 bits because allocations are aligned. |
// | mark bit        |    1 |                                          |
// +-----------------+------+------------------------------------------+
//
// Notes:
// - See |GCInfoTable| for constraints on GCInfoIndex.
// - |size| for regular objects is encoded with 15 bits but can actually
//   represent sizes up to |kBlinkPageSize| (2^17) because allocations are
//   always 4 byte aligned (see kAllocationGranularity) on 32bit. 64bit uses
//   8 byte aligned allocations which leaves 1 bit unused.
// - |size| for large objects is encoded as 0. The size of a large object is
//   stored in |LargeObjectPage::PayloadSize()|.
// - |mark bit| and |in construction| bits are located in separate 16-bit halves
//    to allow potentially accessing them non-atomically.
class HeapObjectHeader {
 public:
  static constexpr size_t kSizeLog2 = 17;
  static constexpr size_t kMaxSize = (size_t{1} << kSizeLog2) - 1;
  static constexpr uint16_t kLargeObjectSizeInHeader = 0;

  inline static HeapObjectHeader& FromObject(void* address);
  inline static const HeapObjectHeader& FromObject(const void* address);

  inline HeapObjectHeader(size_t size, GCInfoIndex gc_info_index);

  // The object starts directly after the HeapObjectHeader.
  inline Address ObjectStart() const;
  template <AccessMode mode = AccessMode::kNonAtomic>
  inline Address ObjectEnd() const;

  template <AccessMode mode = AccessMode::kNonAtomic>
  inline GCInfoIndex GetGCInfoIndex() const;

  template <AccessMode mode = AccessMode::kNonAtomic>
  inline size_t AllocatedSize() const;
  inline void SetAllocatedSize(size_t size);

  template <AccessMode mode = AccessMode::kNonAtomic>
  inline size_t ObjectSize() const;

  template <AccessMode mode = AccessMode::kNonAtomic>
  inline bool IsLargeObject() const;

  template <AccessMode = AccessMode::kNonAtomic>
  bool IsInConstruction() const;
  V8_EXPORT_PRIVATE void MarkAsFullyConstructed();
  // Use MarkObjectAsFullyConstructed() to mark an object as being constructed.

  template <AccessMode = AccessMode::kNonAtomic>
  bool IsMarked() const;
  template <AccessMode = AccessMode::kNonAtomic>
  void Unmark();
  inline bool TryMarkAtomic();

  inline void MarkNonAtomic();

  template <AccessMode = AccessMode::kNonAtomic>
  bool IsYoung() const;

  template <AccessMode = AccessMode::kNonAtomic>
  bool IsFree() const;

  inline bool IsFinalizable() const;
  void Finalize();

#if defined(CPPGC_CAGED_HEAP)
  inline void SetNextUnfinalized(HeapObjectHeader* next);
  inline HeapObjectHeader* GetNextUnfinalized(uintptr_t cage_base) const;
#endif  // defined(CPPGC_CAGED_HEAP)

  // Default version will retrieve `HeapObjectNameForUnnamedObject` as it is
  // configured at runtime.
  V8_EXPORT_PRIVATE HeapObjectName GetName() const;
  // Override for verifying and testing where we always want to pass the naming
  // option explicitly.
  V8_EXPORT_PRIVATE HeapObjectName
      GetName(HeapObjectNameForUnnamedObject) const;

  template <AccessMode = AccessMode::kNonAtomic>
  void TraceImpl(Visitor*) const;

 private:
  enum class EncodedHalf : uint8_t { kLow, kHigh };

  // Used in |encoded_low_|.
  using MarkBitField = ::v8::base::BitField<bool, 0, 1, uint16_t>;
  using SizeField = MarkBitField::Next<size_t, 15>;  // Use EncodeSize/DecodeSize instead.
  // Used in |encoded_high_|.
  using FullyConstructedField = ::v8::base::BitField<bool, 0, 1, uint16_t>;
  using UnusedField1 = FullyConstructedField::Next<bool, 1>;
  using GCInfoIndexField = UnusedField1::Next<GCInfoIndex, 14>;

  static constexpr size_t DecodeSize(uint16_t encoded) {
    // Essentially, gets optimized to << 1.
    return SizeField::decode(encoded) * kAllocationGranularity;
  }

  static constexpr uint16_t EncodeSize(size_t size) {
    // Essentially, gets optimized to >> 1.
    return SizeField::encode(size / kAllocationGranularity);
  }

  V8_EXPORT_PRIVATE void CheckApiConstants();

  template <AccessMode, EncodedHalf part,
            std::memory_order memory_order = std::memory_order_seq_cst>
  inline uint16_t LoadEncoded() const;
  template <AccessMode mode, EncodedHalf part,
            std::memory_order memory_order = std::memory_order_seq_cst>
  inline void StoreEncoded(uint16_t bits, uint16_t mask);

#if defined(V8_HOST_ARCH_64_BIT)
  // If cage is enabled, to save on space required by sweeper metadata, we store
  // the list of to-be-finalized objects inlined in HeapObjectHeader.
#if defined(CPPGC_CAGED_HEAP)
  uint32_t next_unfinalized_ = 0;
#else   // !defined(CPPGC_CAGED_HEAP)
  uint32_t padding_ = 0;
#endif  // !defined(CPPGC_CAGED_HEAP)
#endif  // defined(V8_HOST_ARCH_64_BIT)
  uint16_t encoded_high_;
  uint16_t encoded_low_;
};

}  // namespace internal
}  // namespace cppgc

#endif  // V8_HEAP_CPPGC_HEAP_OBJECT_HEADER_H_
