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

class V8_EXPORT Visitor {
 public:
  explicit Visitor(HeapHandle& heap_handle) : heap_handle_(heap_handle) {}
  virtual ~Visitor() = default;

  // Pull the function type into the class for convenience in derived code.
  using WeakCallback = ::cppgc::WeakCallback;

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
  
  virtual void Visit(const void* object, TraceDescriptor desc) = 0;
  
  virtual void VisitWeak(const void* object, TraceDescriptor desc,
                         void* weak_callback, const void* weak_callback_parameter) = 0;

  virtual void RegisterWeakCallback(void* callback, const void* object) {}

 protected:
  HeapHandle& heap_handle_;

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
