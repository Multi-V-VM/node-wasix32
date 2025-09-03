// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_HEAP_CPPGC_MARKING_VISITOR_H_
#define V8_HEAP_CPPGC_MARKING_VISITOR_H_

#include "include/cppgc/trace-trait.h"
#include "src/base/macros.h"
#include "src/heap/base/stack.h"
#include "src/heap/cppgc/visitor.h"

namespace cppgc {
namespace internal {

class HeapBase;
class HeapObjectHeader;
class Marker;
class BasicMarkingState;
class MutatorMarkingState;
class ConcurrentMarkingState;

class V8_EXPORT_PRIVATE MarkingVisitorBase : public cppgc::Visitor {
 public:
  MarkingVisitorBase(HeapBase&, BasicMarkingState&);
  ~MarkingVisitorBase() = default;

 protected:
  void Visit(const void*, TraceDescriptor);
  void VisitMultipleUncompressedMember(const void*, size_t,
                                       TraceDescriptorCallback);
#if defined(CPPGC_POINTER_COMPRESSION)
  void VisitMultipleCompressedMember(const void*, size_t,
                                     TraceDescriptorCallback);
#endif  // defined(CPPGC_POINTER_COMPRESSION)
  void VisitWeak(const void*, TraceDescriptor, void*, const void*);
  void VisitEphemeron(const void*, const void*, TraceDescriptor);
  void VisitWeakContainer(const void* object, TraceDescriptor strong_desc,
                          TraceDescriptor weak_desc, WeakCallback callback,
                          const void* data);
  void RegisterWeakCallback(WeakCallback, const void*);
  void HandleMovableReference(const void**);

  BasicMarkingState& marking_state_;
};

class V8_EXPORT_PRIVATE MutatorMarkingVisitor : public MarkingVisitorBase {
 public:
  MutatorMarkingVisitor(HeapBase&, MutatorMarkingState&);
  ~MutatorMarkingVisitor() = default;
};

class V8_EXPORT_PRIVATE ConcurrentMarkingVisitor
    : public MarkingVisitorBase {
 public:
  ConcurrentMarkingVisitor(HeapBase&, ConcurrentMarkingState&);
  ~ConcurrentMarkingVisitor() = default;

 protected:
  bool DeferTraceToMutatorThreadIfConcurrent(const void*, TraceCallback,
                                             size_t);
};

class V8_EXPORT_PRIVATE RootMarkingVisitor : public RootVisitorBase {
 public:
  explicit RootMarkingVisitor(MutatorMarkingState&);
  ~RootMarkingVisitor() = default;

 protected:
  void VisitRoot(const void*, TraceDescriptor, const SourceLocation&);
  void VisitWeakRoot(const void*, TraceDescriptor, void*, const void*,
                     const SourceLocation&);

  MutatorMarkingState& mutator_marking_state_;
};

class ConservativeMarkingVisitor : public ConservativeTracingVisitor,
                                   public heap::base::StackVisitor {
 public:
  ConservativeMarkingVisitor(HeapBase&, MutatorMarkingState&, cppgc::Visitor&);
  ~ConservativeMarkingVisitor() = default;

 private:
  void VisitFullyConstructedConservatively(HeapObjectHeader&);
  void VisitInConstructionConservatively(HeapObjectHeader&,
                                         TraceConservativelyCallback);
  void VisitPointer(const void*) override;

  MutatorMarkingState& marking_state_;
};

}  // namespace internal
}  // namespace cppgc

#endif  // V8_HEAP_CPPGC_MARKING_VISITOR_H_
