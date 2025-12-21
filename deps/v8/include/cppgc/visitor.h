// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_CPPGC_VISITOR_H_
#define INCLUDE_CPPGC_VISITOR_H_

#include "cppgc/common.h"
#include "cppgc/trace-trait.h"
#include "cppgc/source-location.h"
#include "v8config.h"
#include "v8-traced-handle.h"  // Ensure v8::TracedReference is visible

namespace cppgc {

class HeapHandle;

// Public WeakCallback function type available to internal visitors.
using WeakCallback = void(*)(const void*);

// Use existing TraceDescriptor from trace-trait.h
// WeakCallback is defined in v8-cppgc.h - don't redefine it here
// SpaceTrait is defined in wasi-cppgc-stubs.h - don't redefine it here

namespace internal {

// Factory for creating visitor keys (opaque tokens for visitor construction)
class VisitorFactory {
 public:
  class Key {
   private:
    Key() = default;
    friend class VisitorFactory;
    friend class ::cppgc::Visitor;
  };

  static Key CreateKey() { return Key(); }
};

}  // namespace internal

class V8_EXPORT Visitor {
 public:
  explicit Visitor(HeapHandle& heap_handle) : heap_handle_(&heap_handle) {}
  virtual ~Visitor() = default;

  // Pull the function type into the class for convenience in derived code.
  using WeakCallback = ::cppgc::WeakCallback;

  // Public Trace methods for external use (e.g., from cppgc_helpers.h)
  template <typename T>
  void Trace(const T& t) {
    TraceImpl(&t);
  }

  // Special handling for v8::TracedReference
  template <typename T>
  void Trace(const v8::TracedReference<T>& ref) {
    // TracedReference has its own special handling
    if (!ref.IsEmpty()) {
      // Just mark it as traced - actual implementation would be in V8
    }
  }

  // Core visiting methods - virtual for derived classes to override
  virtual void Visit(const void* object, TraceDescriptor desc) = 0;

 protected:
  // Protected constructor for internal visitors that don't have a HeapHandle
  explicit Visitor(internal::VisitorFactory::Key) : heap_handle_(nullptr) {}

  virtual void VisitWeak(const void* object, TraceDescriptor desc,
                         WeakCallback weak_callback, const void* weak_callback_parameter) {}

  // Multiple member visiting for batch processing
  virtual void VisitMultipleUncompressedMember(const void* start, size_t len,
                                               TraceDescriptorCallback get_descriptor) {}
#if defined(CPPGC_POINTER_COMPRESSION)
  virtual void VisitMultipleCompressedMember(const void* start, size_t len,
                                             TraceDescriptorCallback get_descriptor) {}
#endif  // defined(CPPGC_POINTER_COMPRESSION)

  // Ephemeron support (weak key-value pairs)
  virtual void VisitEphemeron(const void* key, const void* value, TraceDescriptor value_desc) {}

  // Weak container support
  virtual void VisitWeakContainer(const void* self, TraceDescriptor strong_desc,
                                  TraceDescriptor weak_desc, WeakCallback callback,
                                  const void* data) {}

  // Callback registration
  virtual void RegisterWeakCallback(WeakCallback callback, const void* object) {}

  // Movable reference handling
  virtual void HandleMovableReference(const void** slot) {}

  // TracedReference visiting (for V8 integration)
  virtual void Visit(const v8::TracedReferenceBase& ref) {}

  // Concurrent tracing support
  virtual bool DeferTraceToMutatorThreadIfConcurrent(const void* parameter,
                                                     TraceCallback callback,
                                                     size_t deferred_size) {
    return false;
  }

  HeapHandle* heap_handle_;

 private:
  template <typename T>
  void TraceImpl(const T* t) {
    if (t) {
      TraceTrait<T>::Trace(this, t);
    }
  }
};

}  // namespace cppgc

#endif  // INCLUDE_CPPGC_VISITOR_H_
