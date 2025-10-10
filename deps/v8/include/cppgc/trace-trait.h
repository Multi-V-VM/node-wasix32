#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_CPPGC_TRACE_TRAIT_H_
#define INCLUDE_CPPGC_TRACE_TRAIT_H_

#include <type_traits>

#include "cppgc/type-traits.h"
#include "v8config.h"  // NOLINT(build/include_directory)

// Forward declaration to avoid pulling heavy V8 headers.
namespace v8 { template <typename T> class TracedReference; }

namespace cppgc {

class Visitor;

namespace internal {

class RootVisitor;

using TraceRootCallback = void (*)(RootVisitor&, const void* object);

// Implementation of the default TraceTrait handling GarbageCollected and
// GarbageCollectedMixin.
template <typename T,
          bool =
              IsGarbageCollectedMixinTypeV<typename std::remove_const<T>::type>>
struct TraceTraitImpl;

}  // namespace internal

/**
 * Callback for invoking tracing on a given object.
 *
 * \param visitor The visitor to dispatch to.
 * \param object The object to invoke tracing on.
 */
using TraceCallback = void (*)(Visitor* visitor, const void* object);

/**
 * Describes how to trace an object, i.e., how to visit all Oilpan-relevant
 * fields of an object.
 */
struct TraceDescriptor {
  /**
   * Adjusted base pointer, i.e., the pointer to the class inheriting directly
   * from GarbageCollected, of the object that is being traced.
   */
  const void* base_object_payload;
  /**
   * Callback for tracing the object.
   */
  TraceCallback callback;
};

/**
 * Callback for getting a TraceDescriptor for a given address.
 *
 * \param address Possibly inner address of an object.
 * \returns a TraceDescriptor for the provided address.
 */
using TraceDescriptorCallback = TraceDescriptor (*)(const void* address);

namespace internal {

struct V8_EXPORT TraceTraitFromInnerAddressImpl {
  static TraceDescriptor GetTraceDescriptor(const void* address);
};

/**
 * Trait specifying how the garbage collector processes an object of type T.
 *
 * Advanced users may override handling by creating a specialization for their
 * type.
 */
template <typename T>
struct TraceTraitBase {
  static_assert(internal::IsTraceableV<T>, "T must have a Trace() method");

  /**
   * Accessor for retrieving a TraceDescriptor to process an object of type T.
   *
   * \param self The object to be processed.
   * \returns a TraceDescriptor to process the object.
   */
  static TraceDescriptor GetTraceDescriptor(const void* self) {
    return internal::TraceTraitImpl<T>::GetTraceDescriptor(
        static_cast<const T*>(self));
  }

  /**
   * Function invoking the tracing for an object of type T.
   *
   * \param visitor The visitor to dispatch to.
   * \param self The object to invoke tracing on.
   */
  static void Trace(Visitor* visitor, const void* self) {
    static_cast<const T*>(self)->Trace(visitor);
  }
};

}  // namespace internal

template <typename T>
struct TraceTrait : public internal::TraceTraitBase<T> {};

namespace internal {

template <typename T>
struct TraceTraitImpl<T, false> {
  static_assert(IsGarbageCollectedTypeV<T>,
                "T must be of type GarbageCollected or GarbageCollectedMixin");
  static TraceDescriptor GetTraceDescriptor(const void* self) {
    return {self, TraceTrait<T>::Trace};
  }
};

template <typename T>
struct TraceTraitImpl<T, true> {
  static TraceDescriptor GetTraceDescriptor(const void* self) {
    return internal::TraceTraitFromInnerAddressImpl::GetTraceDescriptor(self);
  }
};

}  // namespace internal

#if defined(__wasi__)
// Treat v8::TracedReference as a traceable holder without requiring a
// user-defined Trace() method. This sidesteps cppgc's default static_assert
// for types it doesn't recognize in this WASI configuration.
template <typename T>
struct TraceTrait<::v8::TracedReference<T>> {
  static void Trace(Visitor* visitor, const ::v8::TracedReference<T>* ref) {
    (void)visitor;
    (void)ref;
  }
  static TraceDescriptor GetTraceDescriptor(const void* self) {
    return {self, &TraceThunk};
  }

 private:
  static void TraceThunk(Visitor* v, const void* obj) {
    const auto* ref = static_cast<const ::v8::TracedReference<T>*>(obj);
    Trace(v, ref);
  }
};
#endif  // defined(__wasi__)

}  // namespace cppgc

#endif  // INCLUDE_CPPGC_TRACE_TRAIT_H_
