// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_V8_CPPGC_H_
#define INCLUDE_V8_CPPGC_H_

#include <memory>
#include "v8-local-handle.h"
#include "v8config.h"
#include "cppgc/heap.h"

// Extra forward declarations to accommodate varying include orders
namespace cppgc {
enum class CollectionType;
enum class StackState;
struct HeapStatistics;
class HeapHandle;
class AllocationHandle;
}

namespace v8 { class Platform; }

namespace v8 {

class V8_EXPORT CppHeap {
 public:
  static std::unique_ptr<CppHeap> Create(v8::Platform* platform);
  static std::unique_ptr<CppHeap> Create(
      v8::Platform* platform,
      const ::cppgc::HeapOptions& options);

  virtual ~CppHeap() = default;

  ::cppgc::HeapHandle& GetHeapHandle();

  ::cppgc::HeapStatistics CollectStatistics(
      ::cppgc::HeapStatistics::DetailLevel detail_level);

  void CollectGarbageForTesting(::cppgc::CollectionType collection_type,
                                ::cppgc::StackState stack_state);

  void EnableDetachedGarbageCollectionsForTesting();

#ifdef __wasi__
  // WASI-specific: Provide access to allocation handle for cppgc allocations
  virtual ::cppgc::AllocationHandle& GetAllocationHandle() = 0;
#endif

 protected:
  CppHeap() = default;

 private:
  CppHeap(const CppHeap&) = delete;
  CppHeap& operator=(const CppHeap&) = delete;
};

}  // namespace v8


#endif  // INCLUDE_V8_CPPGC_H_
