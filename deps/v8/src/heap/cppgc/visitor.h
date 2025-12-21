#ifndef V8_HEAP_CPPGC_VISITOR_H_
#define V8_HEAP_CPPGC_VISITOR_H_

#include "include/cppgc/visitor.h"
#include "include/cppgc/source-location.h"
#include "include/cppgc/persistent.h"  // For cppgc::internal::RootVisitor

namespace cppgc {
namespace internal {

// Base class for cppgc Visitors used inside V8 internals.
// Extends cppgc::Visitor and provides a virtual destructor so derived
// classes in marking visitors can safely use 'override'.
class VisitorBase : public cppgc::Visitor {
 public:
  explicit VisitorBase(HeapHandle& heap_handle) : cppgc::Visitor(heap_handle) {}
  explicit VisitorBase(cppgc::internal::VisitorFactory::Key key) : cppgc::Visitor(key) {}
  virtual ~VisitorBase() = default;

  // Default implementation of pure virtual Visit - can be overridden
  void Visit(const void* object, TraceDescriptor desc) override {
    if (desc.callback) {
      desc.callback(this, desc.base_object_payload);
    }
  }
};

class RootVisitorBase : public cppgc::Visitor, public RootVisitor {
 public:
  explicit RootVisitorBase(HeapHandle& heap_handle) : cppgc::Visitor(heap_handle) {}
  explicit RootVisitorBase(cppgc::internal::VisitorFactory::Key key) : cppgc::Visitor(key) {}
  // Default constructor for subclasses that manage their own initialization
  RootVisitorBase() : cppgc::Visitor(cppgc::internal::VisitorFactory::CreateKey()) {}
  virtual ~RootVisitorBase() = default;

  // Default implementation of pure virtual Visit - can be overridden
  void Visit(const void* object, TraceDescriptor desc) override {
    if (desc.callback) {
      desc.callback(this, desc.base_object_payload);
    }
  }

  // Override RootVisitor::Trace for persistent iteration
  void Trace(const void* object) {
    // Default trace implementation - delegates to Visit if needed
  }

  virtual void VisitRoot(const void* object, TraceDescriptor desc, const SourceLocation& loc) {
    if (desc.callback) {
      desc.callback(this, desc.base_object_payload);
    }
  }

  virtual void VisitWeakRoot(const void* object, TraceDescriptor desc,
                             void* weak_callback, const void* weak_callback_data,
                             const SourceLocation& loc) {
    if (desc.callback) {
      desc.callback(this, desc.base_object_payload);
    }
    if (weak_callback) {
      RegisterWeakCallback(reinterpret_cast<WeakCallback>(weak_callback), weak_callback_data);
    }
  }
};

}  // namespace internal
}  // namespace cppgc

// Conservative tracing support used by cppgc marking visitors.
namespace cppgc {
namespace internal {

class HeapBase;
class HeapObjectHeader;
class PageBackend;

// Callback type used to conservatively trace an in-construction object.
using TraceConservativelyCallback = void (*)(class ConservativeTracingVisitor*,
                                             const HeapObjectHeader&);

class ConservativeTracingVisitor {
 public:
  ConservativeTracingVisitor(HeapBase& heap, PageBackend& page_backend,
                             cppgc::Visitor& visitor);
  virtual ~ConservativeTracingVisitor() = default;

  void TraceConservatively(const HeapObjectHeader& header);
  void TryTracePointerConservatively(const void* address);
  virtual void TraceConservativelyIfNeeded(const void* address);
  virtual void TraceConservativelyIfNeeded(HeapObjectHeader& header);

  // Overridables used by specific marking visitors.
  virtual void VisitFullyConstructedConservatively(HeapObjectHeader& header);
  virtual void VisitInConstructionConservatively(HeapObjectHeader& header,
                                         TraceConservativelyCallback callback);
  virtual void VisitPointer(const void*) {}

 protected:
  HeapBase& heap_;
  PageBackend& page_backend_;
  cppgc::Visitor& visitor_;
};

}  // namespace internal
}  // namespace cppgc

#endif  // V8_HEAP_CPPGC_VISITOR_H_
