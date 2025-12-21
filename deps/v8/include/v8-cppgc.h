// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_V8_CPPGC_H_
#define INCLUDE_V8_CPPGC_H_

#include <memory>
#include <vector>
#include "v8-local-handle.h"
#include "v8config.h"
#include "cppgc/heap.h"
#include "cppgc/common.h"
#include "cppgc/custom-space.h"

// Extra forward declarations to accommodate varying include orders
namespace cppgc {
enum class CollectionType;
enum class StackState : uint8_t;  // Must match underlying type in cppgc/heap.h
struct HeapStatistics;
class HeapHandle;
class AllocationHandle;
}

namespace v8 {

class Platform;

// Receiver for custom space statistics
class V8_EXPORT CustomSpaceStatisticsReceiver {
 public:
  virtual ~CustomSpaceStatisticsReceiver() = default;
  virtual void AllocatedBytes(cppgc::CustomSpaceIndex space_index,
                              size_t bytes) = 0;
};

class V8_EXPORT CppHeap {
 public:
  // Collection type for unified heap
  enum class CollectionType : uint8_t {
    kMinor,
    kMajor,
  };

  static std::unique_ptr<CppHeap> Create(v8::Platform* platform);
  static std::unique_ptr<CppHeap> Create(
      v8::Platform* platform,
      const ::cppgc::HeapOptions& options);

  virtual ~CppHeap() = default;

  // Core heap access - virtual for internal::CppHeap override
  virtual ::cppgc::AllocationHandle& GetAllocationHandle();
  virtual ::cppgc::HeapHandle& GetHeapHandle();

  // Terminate the heap - must be called before destruction when attached to isolate
  void Terminate();

  // Statistics collection
  ::cppgc::HeapStatistics CollectStatistics(
      ::cppgc::HeapStatistics::DetailLevel detail_level);

  void CollectCustomSpaceStatisticsAtLastGC(
      std::vector<cppgc::CustomSpaceIndex> custom_spaces,
      std::unique_ptr<CustomSpaceStatisticsReceiver> receiver);

  // Testing APIs
  void EnableDetachedGarbageCollectionsForTesting();

  void CollectGarbageForTesting(::cppgc::EmbedderStackState stack_state);

  void CollectGarbageInYoungGenerationForTesting(
      ::cppgc::EmbedderStackState stack_state);

  // Legacy overload for CollectionType + StackState
  void CollectGarbageForTesting(::cppgc::CollectionType collection_type,
                                ::cppgc::StackState stack_state);

 protected:
  CppHeap() = default;

 private:
  CppHeap(const CppHeap&) = delete;
  CppHeap& operator=(const CppHeap&) = delete;
};

}  // namespace v8


#endif  // INCLUDE_V8_CPPGC_H_
