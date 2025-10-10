// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_CPPGC_HEAP_H_
 #define INCLUDE_CPPGC_HEAP_H_

#include <memory>
#include <vector>
#include <string>

 #include "cppgc/common.h"
 #include "cppgc/custom-space.h"
 #include "cppgc/platform.h"
 #include "v8config.h"

namespace cppgc {

class AllocationHandle;
class HeapHandle;

// These enums and structs define the core cppgc surface types
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
  struct ObjectStatsEntry {
    size_t allocated_bytes = 0;
    size_t object_count = 0;
  };

  struct PageStatistics {
    size_t committed_size_bytes = 0;
    size_t resident_size_bytes = 0;
    size_t used_size_bytes = 0;
    std::vector<ObjectStatsEntry> object_statistics;
  };

  struct FreeListStatistics {
    std::vector<size_t> bucket_size;
    std::vector<size_t> free_count;
    std::vector<size_t> free_size;
  };

  struct SpaceStatistics {
    std::string name;
    size_t committed_size_bytes = 0;
    size_t resident_size_bytes = 0;
    size_t used_size_bytes = 0;
    std::vector<PageStatistics> page_stats;
    FreeListStatistics free_list_stats;
  };

  enum class DetailLevel : uint8_t {
    kBrief,
    kDetailed,
  };

  size_t used_size_bytes = 0;
  size_t physical_size_bytes = 0;
  size_t committed_size_bytes = 0;
  size_t resident_size_bytes = 0;
  std::vector<SpaceStatistics> space_stats;
  std::vector<std::string> type_names;
};

struct HeapOptions {
  MarkingType marking_type = MarkingType::kIncrementalAndConcurrent;
  SweepingType sweeping_type = SweepingType::kIncrementalAndConcurrent;
};

// Forward declare Heap class if not already defined
class V8_EXPORT Heap {
 public:
  using StackState = cppgc::StackState;
  using StackSupport = cppgc::StackSupport;
  using MarkingType = cppgc::MarkingType;
  using SweepingType = cppgc::SweepingType;

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

#endif  // INCLUDE_CPPGC_HEAP_H_
