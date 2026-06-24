// Copyright 2018 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_OBJECTS_PROTOTYPE_INL_H_
#define V8_OBJECTS_PROTOTYPE_INL_H_

#include "src/objects/prototype.h"
// Include the non-inl header before the rest of the headers.

#include "src/handles/handles-inl.h"
#include "src/objects/js-proxy.h"
#include "src/objects/map-inl.h"
#if defined(__wasi__)
#include <cstdio>
#endif

namespace v8 {
namespace internal {

PrototypeIterator::PrototypeIterator(Isolate* isolate,
                                     DirectHandle<JSReceiver> receiver,
                                     WhereToStart where_to_start,
                                     WhereToEnd where_to_end)
    : isolate_(isolate),
      handle_(indirect_handle(receiver, isolate)),
      where_to_end_(where_to_end),
      is_at_end_(false),
      seen_proxies_(0) {
  CHECK(!handle_.is_null());
  if (where_to_start == kStartAtPrototype) Advance();
}

PrototypeIterator::PrototypeIterator(Isolate* isolate,
                                     Tagged<JSReceiver> receiver,
                                     WhereToStart where_to_start,
                                     WhereToEnd where_to_end)
    : isolate_(isolate),
      object_(receiver),
      where_to_end_(where_to_end),
      is_at_end_(false),
      seen_proxies_(0) {
  if (where_to_start == kStartAtPrototype) Advance();
}

PrototypeIterator::PrototypeIterator(Isolate* isolate, Tagged<Map> receiver_map,
                                     WhereToEnd where_to_end)
    : isolate_(isolate),
      object_(receiver_map->GetPrototypeChainRootMap(isolate_)->prototype()),
      where_to_end_(where_to_end),
      is_at_end_(IsNull(object_, isolate_)),
      seen_proxies_(0) {
  if (!is_at_end_ && where_to_end_ == END_AT_NON_HIDDEN) {
    DCHECK(IsJSReceiver(object_));
    Tagged<Map> map = Cast<JSReceiver>(object_)->map();
    is_at_end_ = !IsJSGlobalProxyMap(map);
  }
}

PrototypeIterator::PrototypeIterator(Isolate* isolate,
                                     DirectHandle<Map> receiver_map,
                                     WhereToEnd where_to_end)
    : isolate_(isolate),
      handle_(receiver_map->GetPrototypeChainRootMap(isolate_)->prototype(),
              isolate_),
      where_to_end_(where_to_end),
      is_at_end_(IsNull(*handle_, isolate_)),
      seen_proxies_(0) {
  if (!is_at_end_ && where_to_end_ == END_AT_NON_HIDDEN) {
    DCHECK(IsJSReceiver(*handle_));
    Tagged<Map> map = Cast<JSReceiver>(*handle_)->map();
    is_at_end_ = !IsJSGlobalProxyMap(map);
  }
}

bool PrototypeIterator::HasAccess() const {
  // We can only perform access check in the handlified version of the
  // PrototypeIterator.
  DCHECK(!handle_.is_null());
  if (IsAccessCheckNeeded(*handle_)) {
    return isolate_->MayAccess(isolate_->native_context(),
                               Cast<JSObject>(handle_));
  }
  return true;
}

void PrototypeIterator::Advance() {
#if defined(__wasi__)
  Tagged<JSPrototype> current = handle_.is_null() ? object_ : *handle_;
  std::fprintf(stderr,
               "PrototypeIterator::Advance enter handle_null=%d current=0x%zx "
               "is_at_end=%d where_to_end=%d seen=%d\n",
               handle_.is_null(), static_cast<size_t>(current.ptr()),
               is_at_end_, where_to_end_, seen_proxies_);
  std::fflush(stderr);
#endif
  if (handle_.is_null() && IsJSProxy(object_)) {
    is_at_end_ = true;
    object_ = ReadOnlyRoots(isolate_).null_value();
#if defined(__wasi__)
    std::fprintf(stderr, "PrototypeIterator::Advance jsproxy object -> null\n");
    std::fflush(stderr);
#endif
    return;
  } else if (!handle_.is_null() && IsJSProxy(*handle_)) {
    is_at_end_ = true;
    handle_ = isolate_->factory()->null_value();
#if defined(__wasi__)
    std::fprintf(stderr, "PrototypeIterator::Advance jsproxy handle -> null\n");
    std::fflush(stderr);
#endif
    return;
  }
  AdvanceIgnoringProxies();
}

void PrototypeIterator::AdvanceIgnoringProxies() {
  Tagged<JSPrototype> object = handle_.is_null() ? object_ : *handle_;
#if defined(__wasi__)
  std::fprintf(stderr,
               "PrototypeIterator::AdvanceIgnoringProxies before map "
               "handle_null=%d object=0x%zx is_at_end=%d\n",
               handle_.is_null(), static_cast<size_t>(object.ptr()),
               is_at_end_);
  std::fflush(stderr);
#endif
  Tagged<Map> map = object->map();

  Tagged<JSPrototype> prototype = map->prototype();
#if defined(__wasi__)
  std::fprintf(stderr,
               "PrototypeIterator::AdvanceIgnoringProxies after read "
               "object=0x%zx map=0x%zx prototype=0x%zx map_type=%u\n",
               static_cast<size_t>(object.ptr()), static_cast<size_t>(map.ptr()),
               static_cast<size_t>(prototype.ptr()),
               static_cast<unsigned>(map->instance_type()));
  std::fflush(stderr);
#endif
  is_at_end_ = IsNull(prototype, isolate_) ||
               (where_to_end_ == END_AT_NON_HIDDEN && !IsJSGlobalProxyMap(map));

  if (handle_.is_null()) {
    object_ = prototype;
  } else {
    handle_ = handle(prototype, isolate_);
  }
}

V8_WARN_UNUSED_RESULT bool PrototypeIterator::AdvanceFollowingProxies() {
  DCHECK(!(handle_.is_null() && IsJSProxy(object_)));
  if (!HasAccess()) {
    // Abort the lookup if we do not have access to the current object.
    handle_ = isolate_->factory()->null_value();
    is_at_end_ = true;
    return true;
  }
  return AdvanceFollowingProxiesIgnoringAccessChecks();
}

V8_WARN_UNUSED_RESULT bool
PrototypeIterator::AdvanceFollowingProxiesIgnoringAccessChecks() {
  if (handle_.is_null() || !IsJSProxy(*handle_)) {
    AdvanceIgnoringProxies();
    return true;
  }

  // Due to possible __proto__ recursion limit the number of Proxies
  // we visit to an arbitrarily chosen large number.
  seen_proxies_++;
  if (seen_proxies_ > JSProxy::kMaxIterationLimit) {
    isolate_->StackOverflow();
    return false;
  }
  MaybeDirectHandle<JSPrototype> proto =
      JSProxy::GetPrototype(Cast<JSProxy>(handle_));

  // TODO(372390038): This can be again simplified when handle_ migrates to a
  // direct handle.
  DirectHandle<JSPrototype> proto_direct_handle;
  bool ok = proto.ToHandle(&proto_direct_handle);
  handle_ = indirect_handle(proto_direct_handle, isolate_);
  if (!ok) return false;

  is_at_end_ = where_to_end_ == END_AT_NON_HIDDEN || IsNull(*handle_, isolate_);
  return true;
}

}  // namespace internal
}  // namespace v8

#endif  // V8_OBJECTS_PROTOTYPE_INL_H_
