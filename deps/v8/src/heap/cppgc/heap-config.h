// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_HEAP_CPPGC_HEAP_CONFIG_H_
#define V8_HEAP_CPPGC_HEAP_CONFIG_H_

#include <vector>

#include "include/cppgc/heap.h"
#include <optional>

namespace cppgc {
namespace internal {

// MarkingConfig for internal use
struct MarkingConfig {
  using MarkingType = cppgc::MarkingType;
  using StackState = cppgc::StackState;
  
  enum class IsForcedGC : uint8_t {
    kNotForced,
    kForced,
  };

  static constexpr MarkingConfig Default() {
    return {CollectionType::kMajor, StackState::kMayContainHeapPointers,
            MarkingType::kIncrementalAndConcurrent, IsForcedGC::kNotForced};
  }

  CollectionType collection_type = CollectionType::kMajor;
  StackState stack_state = StackState::kMayContainHeapPointers;
  MarkingType marking_type = MarkingType::kIncrementalAndConcurrent;
  IsForcedGC is_forced_gc = IsForcedGC::kNotForced;
};

// SweepingConfig for internal use
struct SweepingConfig {
  using SweepingType = cppgc::SweepingType;
  
  enum class CompactableSpaceHandling { kSweep, kIgnore };
  enum class FreeMemoryHandling { kDoNotDiscard, kDiscardWherePossible };

  SweepingType sweeping_type = SweepingType::kIncrementalAndConcurrent;
  CompactableSpaceHandling compactable_space_handling =
      CompactableSpaceHandling::kSweep;
  FreeMemoryHandling free_memory_handling = FreeMemoryHandling::kDoNotDiscard;
};

// GCConfig combines marking and sweeping configs
struct GCConfig {
  using MarkingType = MarkingConfig::MarkingType;
  using SweepingType = SweepingConfig::SweepingType;
  using StackState = MarkingConfig::StackState;
  using IsForcedGC = MarkingConfig::IsForcedGC;

  static constexpr GCConfig Default() {
    return {MarkingConfig::Default(), SweepingConfig{}};
  }

  MarkingConfig marking_config;
  SweepingConfig sweeping_config;
};

}  // namespace internal
}  // namespace cppgc

#endif  // V8_HEAP_CPPGC_HEAP_CONFIG_H_
