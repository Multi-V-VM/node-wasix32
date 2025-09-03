// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_CPPGC_HEAP_H_
#define INCLUDE_CPPGC_HEAP_H_

#include <memory>
#include <vector>

#include "cppgc/common.h"
#include "cppgc/custom-space.h"
#include "cppgc/platform.h"
#include "v8config.h"

// Check if v8-cppgc.h has already defined these types
#ifndef INCLUDE_V8_CPPGC_H_

namespace cppgc {

class AllocationHandle;
class HeapHandle;

// These enums and structs should match v8-cppgc.h definitions
enum class CollectionType {
  kMinor,
  kMajor,
  kFull,
};

enum class MarkingType { 
  kAtomic, 
  kIncremental, 
  kIncrementalAndConcurrent 
};

enum class SweepingType { 
  kAtomic, 
  kIncremental, 
  kIncrementalAndConcurrent 
};

enum class StackState {
  kMayContainHeapPointers,
  kNoHeapPointers,
};

// Support level for stack scanning on the current platform/embedder.
// Matches usage in V8's cppgc GC invoker.
enum class StackSupport {
  kNoConservativeStackScan,
  kSupportsConservativeStackScan,
};

struct HeapStatistics {
  enum class DetailLevel : uint8_t {
    kBrief,
    kDetailed,
  };
  
  size_t used_size_bytes = 0;
  size_t physical_size_bytes = 0;
  size_t committed_size_bytes = 0;
};

struct HeapOptions {
  MarkingType marking_type = MarkingType::kIncrementalAndConcurrent;
  SweepingType sweeping_type = SweepingType::kIncrementalAndConcurrent;
};

// Forward declare Heap class if not already defined
class V8_EXPORT Heap {
 public:
  using StackState = cppgc::StackState;

  static std::unique_ptr<Heap> Create(
      v8::Platform* platform,
      const HeapOptions& options = {});

  virtual ~Heap() = default;

  HeapHandle& GetHeapHandle();
  AllocationHandle& GetAllocationHandle();

  void CollectGarbage(CollectionType collection_type);

  HeapStatistics CollectStatistics(HeapStatistics::DetailLevel detail_level);

  void EnableDetachedGarbageCollectionsForTesting();

  void CollectGarbageForTesting(CollectionType collection_type, StackState stack_state);

 protected:
  Heap() = default;

 private:
  Heap(const Heap&) = delete;
  Heap& operator=(const Heap&) = delete;
};

}  // namespace cppgc

#else // INCLUDE_V8_CPPGC_H_ is defined

// v8-cppgc.h has already defined everything, just use those definitions
#include "v8-cppgc.h"

#endif // INCLUDE_V8_CPPGC_H_

#endif  // INCLUDE_CPPGC_HEAP_H_
