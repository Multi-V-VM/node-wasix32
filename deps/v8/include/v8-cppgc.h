// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_V8_CPPGC_H_
#define INCLUDE_V8_CPPGC_H_

#include <memory>
#include <vector>
#include <string>

#include "v8-local-handle.h"  
#include "v8config.h"

namespace v8 {
class Platform;
}

namespace cppgc {

class HeapHandle;
class AllocationHandle;
class PageStatistics;
class SpaceStatistics;
class ObjectStatsEntry;

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

// Support level for stack scanning used by GC invoker and heap configuration
enum class StackSupport {
  kNoConservativeStackScan,
  kSupportsConservativeStackScan,
};

// Define HeapStatistics with nested stats types matching Node's usage
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
    std::vector<uint32_t> bucket_size;
    std::vector<uint32_t> free_count;
    std::vector<uint32_t> free_size;
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

// Don't define WeakCallback as a class - use function pointer type
using WeakCallback = void(*)(const void*);

class V8_EXPORT Heap {
 public:
  // Define StackState here to avoid conflicts
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

namespace v8 {

class V8_EXPORT CppHeap {
 public:
  static std::unique_ptr<CppHeap> Create(
      v8::Platform* platform,
      const ::cppgc::HeapOptions& options = {});

  virtual ~CppHeap() = default;

  ::cppgc::HeapHandle& GetHeapHandle();

  ::cppgc::HeapStatistics CollectStatistics(
      ::cppgc::HeapStatistics::DetailLevel detail_level);

  void CollectGarbageForTesting(::cppgc::CollectionType collection_type,
                                ::cppgc::StackState stack_state);

  void EnableDetachedGarbageCollectionsForTesting();

 protected:
  CppHeap() = default;

 private:
  CppHeap(const CppHeap&) = delete;
  CppHeap& operator=(const CppHeap&) = delete;
};

}  // namespace v8

#endif  // INCLUDE_V8_CPPGC_H_
