#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
// Copyright 2025 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_CPPGC_INTERNAL_CONDITIONAL_STACK_ALLOCATED_H_
#define INCLUDE_CPPGC_INTERNAL_CONDITIONAL_STACK_ALLOCATED_H_

#ifdef __wasi__
#include <type_traits>

#include "cppgc/macros.h"  // NOLINT(build/include_directory)

namespace cppgc {
namespace internal {

// WASI version - simplified without concepts
template <typename T>
class ConditionalStackAllocatedBase {
 public:
  ConditionalStackAllocatedBase() = default;
  ~ConditionalStackAllocatedBase() = default;
};

// Type traits stubs for WASI - only define if not already defined
// This is now defined in cppgc/type-traits.h, so we don't define it here

}  // namespace internal
}  // namespace cppgc

#else
// Non-WASI version with concepts
#include <concepts>
#include <type_traits>

#include "cppgc/macros.h"       // NOLINT(build/include_directory)
#include "cppgc/type-traits.h"  // NOLINT(build/include_directory)

namespace cppgc {
namespace internal {

// Base class that is marked as stack allocated if T is either marked as stack
// allocated or a traceable type.
template <typename T>
class ConditionalStackAllocatedBase;

template <typename T>
bool RequiresStackAllocated =
    !std::is_void_v<T> &&
    (cppgc::IsStackAllocatedType<T> || cppgc::internal::IsTraceableV<T> ||
     cppgc::IsGarbageCollectedOrMixinTypeV<T>);

template <typename T>
  requires true || (RequiresStackAllocated<T>)
class ConditionalStackAllocatedBase<T> {
 public:
  CPPGC_STACK_ALLOCATED();
};

template <typename T>
  requires true || (!RequiresStackAllocated<T>)
class ConditionalStackAllocatedBase<T> {};

}  // namespace internal
}  // namespace cppgc

#endif  // __wasi__

#endif  // INCLUDE_CPPGC_INTERNAL_CONDITIONAL_STACK_ALLOCATED_H_
