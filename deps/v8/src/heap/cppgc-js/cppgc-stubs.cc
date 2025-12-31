// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Minimal stub implementations for cppgc symbols when building for WASM32
// Most cppgc functionality is disabled for WASM32 builds

#if V8_TARGET_ARCH_WASM32

#include "src/base/platform/mutex.h"
#include "src/base/platform/platform.h"
#include "src/base/platform/time.h"
#include "src/heap/cppgc/process-heap.h"
#include "src/api/api.h"
#include "src/heap/cppgc-js/cpp-heap.h"
#include "include/v8-microtask-queue.h"
#include "include/v8-unwinder.h"
#include "include/v8-unwinder-state.h"

namespace cppgc {
namespace internal {

// ProcessGlobalLock mutex - required by process-heap.h
v8::base::LazyMutex ProcessGlobalLock::process_mutex_ = LAZY_MUTEX_INITIALIZER;

}  // namespace internal
}  // namespace cppgc

namespace v8 {

// MicrotasksScope stubs
MicrotasksScope::MicrotasksScope(Local<Context> context, Type type)
    : MicrotasksScope(context->GetIsolate(), context->GetMicrotaskQueue(),
                      type) {}

// String::Utf8Value stubs
String::Utf8Value::Utf8Value(Isolate* v8_isolate, Local<v8::Value> obj,
                             WriteOptions options)
    : str_(nullptr), length_(0) {}

String::Utf8Value::~Utf8Value() {}

// Value stubs
bool Value::IsTrue() const { return false; }
bool Value::IsFalse() const { return false; }

namespace internal {

// HandleScopeImplementer stubs for WASM32
void HandleScopeImplementer::Iterate(RootVisitor* v) {
  // No-op for WASM32
}

char* HandleScopeImplementer::Iterate(RootVisitor* v, char* storage) {
  return storage + ArchiveSpacePerThread();
}

int HandleScopeImplementer::ArchiveSpacePerThread() {
  return sizeof(HandleScopeImplementer);
}

// CppHeap stubs
void CppHeap::InitializeOncePerProcess() {
  // No-op for WASM32
}

// GetPlatformVirtualAddressSpace stub
VirtualAddressSpace* GetPlatformVirtualAddressSpace() {
  return nullptr;
}

}  // namespace internal

namespace base {

// OS stubs for WASM32
std::vector<OS::SharedLibraryAddress> OS::GetSharedLibraryAddresses() {
  return std::vector<SharedLibraryAddress>();
}

void OS::SignalCodeMovingGC() {
  // No-op for WASM32
}

TimezoneCache* OS::CreateTimezoneCache() {
  return nullptr;
}

}  // namespace base

// RegisterState stubs
RegisterState::RegisterState()
    : pc(nullptr), sp(nullptr), fp(nullptr), lr(nullptr), callee_saved(nullptr) {}
RegisterState::RegisterState(const RegisterState& other)
    : pc(other.pc), sp(other.sp), fp(other.fp), lr(other.lr),
      callee_saved(other.callee_saved
                       ? std::make_unique<CalleeSavedRegisters>(*other.callee_saved)
                       : nullptr) {}
RegisterState::~RegisterState() = default;

// MicrotasksScope stubs
void MicrotasksScope::PerformCheckpoint(Isolate* v8_isolate) {
  // No-op for WASM32
}

}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
