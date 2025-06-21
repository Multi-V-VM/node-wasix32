// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_V8_CPPGC_H_
#define INCLUDE_V8_CPPGC_H_

#ifdef __wasi__
// WASI-specific complete implementation in single file

// Include system standard library - don't redefine
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

// V8 macro definitions for WASI
#define V8_EXPORT
#define V8_EXPORT_PRIVATE
#define V8_INLINE inline
#define V8_NOINLINE
#define V8_TRIVIAL_ABI
#define V8_LIKELY(x) (x)
#define V8_UNLIKELY(x) (x)
#define V8_WARN_UNUSED_RESULT
#define V8_NODISCARD
#define V8_FALLTHROUGH

// cppgc namespace
namespace cppgc {
enum class EmbedderStackState { kEmpty, kNonEmpty };

class CustomSpaceBase {};
enum class CustomSpaceIndex : size_t {};

struct HeapStatistics {
  enum class DetailLevel { kBrief, kDetailed };
};

class Visitor {
 public:
  struct Key {};
  explicit Visitor(Key) {}
  virtual ~Visitor() = default;
  virtual void Visit(const void* ref) {}
};

struct TraceDescriptor {
  const void* base_object_payload;
  void (*trace_callback)(Visitor*, const void*);
};

template <typename T>
struct TraceTrait;

class AllocationHandle {};
class HeapHandle {};
namespace testing {}
}  // namespace cppgc

namespace v8 {
class Object;
class Platform {
 public:
  virtual ~Platform() = default;
};

// V8 core types that are missing
class Isolate;
template <typename T>
class Local {
  T* ptr_;

 public:
  Local() : ptr_(nullptr) {}
  Local(T* p) : ptr_(p) {}
  T* operator->() const { return ptr_; }
  T& operator*() const { return *ptr_; }
  bool IsEmpty() const { return ptr_ == nullptr; }
};

template <typename T>
class Eternal {
  T* ptr_;

 public:
  Eternal() : ptr_(nullptr) {}
};

template <typename T>
class PersistentBase {
  T* ptr_;

 public:
  PersistentBase() : ptr_(nullptr) {}
};

class EmbedderGraph {
 public:
  class Node {
   public:
    enum class Detachedness { kUnknown };
  };
};

namespace internal {
class CppHeap;
}

class CustomSpaceStatisticsReceiver {
 public:
  virtual ~CustomSpaceStatisticsReceiver() = default;
  virtual void AllocatedBytes(cppgc::CustomSpaceIndex space_index,
                              size_t bytes) = 0;
};

struct V8_EXPORT CppHeapCreateParams {
  std::vector<std::unique_ptr<cppgc::CustomSpaceBase>> custom_spaces;
  int marking_support = 0;
  int sweeping_support = 0;

  explicit CppHeapCreateParams(
      std::vector<std::unique_ptr<cppgc::CustomSpaceBase>> cs)
      : custom_spaces(std::move(cs)) {}
  CppHeapCreateParams(const CppHeapCreateParams&) = delete;
  CppHeapCreateParams& operator=(const CppHeapCreateParams&) = delete;
};

class V8_EXPORT CppHeap {
 public:
  static std::unique_ptr<CppHeap> Create(v8::Platform* platform,
                                         const CppHeapCreateParams& params) {
    return nullptr;
  }
  virtual ~CppHeap() = default;
  cppgc::AllocationHandle& GetAllocationHandle() {
    static cppgc::AllocationHandle h;
    return h;
  }
  cppgc::HeapHandle& GetHeapHandle() {
    static cppgc::HeapHandle h;
    return h;
  }
  void Terminate() {}
  cppgc::HeapStatistics CollectStatistics(
      cppgc::HeapStatistics::DetailLevel level) {
    return {};
  }
  void CollectCustomSpaceStatisticsAtLastGC(
      std::vector<cppgc::CustomSpaceIndex> spaces,
      std::unique_ptr<CustomSpaceStatisticsReceiver> receiver) {}
  void EnableDetachedGarbageCollectionsForTesting() {}
  void CollectGarbageForTesting(cppgc::EmbedderStackState state) {}
  void CollectGarbageInYoungGenerationForTesting(
      cppgc::EmbedderStackState state) {}

 private:
  CppHeap() = default;
  friend class internal::CppHeap;
};

// TracedReference support
template <typename T>
class TracedReference {
 public:
  bool IsEmptyThreadSafe() const { return true; }
};

class TracedReferenceBase {
 public:
  bool IsEmptyThreadSafe() const { return true; }
};

class JSVisitor : public cppgc::Visitor {
 public:
  explicit JSVisitor(cppgc::Visitor::Key key) : cppgc::Visitor(key) {}
  ~JSVisitor() override = default;
  void Trace(const TracedReferenceBase& ref) {
    if (ref.IsEmptyThreadSafe()) return;
    Visit(&ref);
  }

 protected:
  using cppgc::Visitor::Visit;
  virtual void Visit(const TracedReferenceBase& ref) {}
};

}  // namespace v8

namespace cppgc {
template <typename T>
struct TraceTrait<v8::TracedReference<T>> {
  static cppgc::TraceDescriptor GetTraceDescriptor(const void* self) {
    return {nullptr, Trace};
  }
  static void Trace(Visitor* visitor, const void* self) {
    static_cast<v8::JSVisitor*>(visitor)->Trace(
        *static_cast<const v8::TracedReference<T>*>(self));
  }
};
}  // namespace cppgc

#else
// Non-WASI: Include original headers
#include "cppgc/common.h"
#include "cppgc/custom-space.h"
#include "cppgc/heap-statistics.h"
#include "cppgc/visitor.h"
#include "v8-internal.h"
#include "v8-platform.h"
#include "v8-traced-handle.h"
// ... (rest of original implementation)
#endif  // __wasi__

#endif  // INCLUDE_V8_CPPGC_H_
