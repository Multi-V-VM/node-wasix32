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
#include "include/v8-context.h"
#include "include/v8-snapshot.h"
#include "include/v8-template.h"
#include "src/compiler/linkage.h"
#include "src/compiler/wasm-compiler-definitions.h"
#include "src/snapshot/snapshot.h"
#include "src/wasm/value-type.h"
#include "src/wasm/canonical-types.h"
#include "src/wasm/wasm-code-manager.h"
#include "src/wasm/compilation-environment.h"
#include "src/wasm/wasm-result.h"
#include "src/wasm/module-instantiate.h"
#include "src/compiler/access-builder.h"
#include "src/compiler/wasm-call-descriptors.h"
#include "src/compiler/wasm-compiler.h"
#include "src/codegen/cpu-features.h"
#include "src/codegen/optimized-compilation-info.h"
#include "src/heap/heap.h"
#include "include/v8-fast-api-calls.h"
#include "include/v8-exception.h"
#include "include/cppgc/heap.h"
#include "include/cppgc/platform.h"
#include "include/v8-external-memory-accounter.h"

namespace cppgc {

// cppgc::InitializeProcess stub
void InitializeProcess(PageAllocator* page_allocator, size_t desired_heap_size) {
  // No-op for WASM32
}

// cppgc::ShutdownProcess stub
void ShutdownProcess() {
  // No-op for WASM32
}
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

// Context::New stub (WASI version uses Local instead of MaybeLocal)
Local<Context> Context::New(
    Isolate* external_isolate, ExtensionConfiguration* extensions,
    Local<ObjectTemplate> global_template,
    Local<Value> global_object,
    DeserializeInternalFieldsCallback internal_fields_deserializer,
    MicrotaskQueue* microtask_queue,
    DeserializeContextDataCallback context_callback_deserializer,
    DeserializeAPIWrapperCallback api_wrapper_deserializer) {
  // WASM32 does not support context creation
  return Local<Context>();
}

namespace api_internal {
// GetFunctionTemplateData stub
V8_EXPORT Local<Value> GetFunctionTemplateData(
    Isolate* isolate, Local<Data> raw_target) {
  // WASM32 does not support function template data
  return Local<Value>();
}
}  // namespace api_internal

namespace internal {

// Snapshot creation stubs
v8::StartupData CreateSnapshotDataBlobInternal(
    v8::SnapshotCreator::FunctionCodeHandling function_code_handling,
    const char* embedded_source,
    Snapshot::SerializerFlags serializer_flags) {
  // WASM32 does not support snapshot creation
  return v8::StartupData{nullptr, 0};
}

v8::StartupData CreateSnapshotDataBlobInternalForInspectorTest(
    v8::SnapshotCreator::FunctionCodeHandling function_code_handling,
    const char* embedded_source) {
  // WASM32 does not support snapshot creation
  return v8::StartupData{nullptr, 0};
}

namespace compiler {

// CallDescriptor stubs
int CallDescriptor::GetOffsetToReturns() const {
  // WASM32 does not support JIT compilation
  return 0;
}

int CallDescriptor::GetStackParameterDelta(
    const CallDescriptor* tail_caller) const {
  // WASM32 does not support JIT compilation
  return 0;
}

// GetWasmCallDescriptor template stubs for WASM32
// These must be here since wasm-compiler-definitions.cc is not linked
template <typename T>
CallDescriptor* GetWasmCallDescriptor(Zone* zone, const Signature<T>* signature,
                                      WasmCallKind kind, bool need_frame_state) {
  return nullptr;
}

// Explicit template instantiations
template CallDescriptor* GetWasmCallDescriptor<wasm::ValueType>(
    Zone*, const Signature<wasm::ValueType>*, WasmCallKind, bool);
template CallDescriptor* GetWasmCallDescriptor<wasm::CanonicalValueType>(
    Zone*, const Signature<wasm::CanonicalValueType>*, WasmCallKind, bool);

// GetI32WasmCallDescriptor stub
CallDescriptor* GetI32WasmCallDescriptor(
    Zone* zone, const CallDescriptor* call_descriptor) {
  // WASM32 does not support 32-bit lowering
  return nullptr;
}

// Linkage::GetStubCallDescriptor stub
CallDescriptor* Linkage::GetStubCallDescriptor(
    Zone* zone, const CallInterfaceDescriptor& descriptor,
    int stack_parameter_count, CallDescriptor::Flags flags,
    Operator::Properties properties, StubCallMode stub_mode) {
  // WASM32 does not support stub calls
  return nullptr;
}

// AccessBuilder stubs
FieldAccess AccessBuilder::ForStringLength() {
  // WASM32 stub - return empty FieldAccess
  return FieldAccess();
}

// WasmCallDescriptors stub
WasmCallDescriptors::WasmCallDescriptors(AccountingAllocator* allocator) {}

// Compiler function stubs
std::unique_ptr<OptimizedCompilationJob> NewJSToWasmCompilationJob(
    Isolate* isolate, const wasm::CanonicalSig* sig) {
  return nullptr;
}

bool IsFastCallSupportedSignature(const v8::CFunctionInfo* sig) {
  return false;
}

wasm::WasmCompilationResult CompileWasmCapiCallWrapper(
    const wasm::CanonicalSig* sig) {
  return wasm::WasmCompilationResult{};
}

wasm::WasmCompilationResult CompileWasmJSFastCallWrapper(
    const wasm::CanonicalSig* sig,
    DirectHandle<JSReceiver> callable) {
  return wasm::WasmCompilationResult{};
}

wasm::WasmCompilationResult CompileWasmImportCallWrapper(
    wasm::ImportCallKind kind, const wasm::CanonicalSig* sig,
    bool source_positions, int expected_arity, wasm::Suspend suspend) {
  return wasm::WasmCompilationResult{};
}

namespace turboshaft {
wasm::WasmCompilationResult ExecuteTurboshaftWasmCompilation(
    wasm::CompilationEnv* env, WasmCompilationData& data,
    wasm::WasmDetectedFeatures* detected, Counters* counters) {
  return wasm::WasmCompilationResult{};
}
}  // namespace turboshaft

}  // namespace compiler

// CodeAssembler stubs are now in src/compiler/code-assembler-stubs-wasm32.cc

// CpuFeatures stub (non-inline version for linkage)
bool CpuFeatures::SupportsWasmSimd128() {
  return false;
}

// Heap stubs
void Heap::AddGCEpilogueCallback(v8::Isolate::GCCallbackWithData callback,
                                 GCType gc_type, void* data) {
  // No-op for WASM32
}

}  // namespace internal

// CFunctionInfo stubs - static CTypeInfo to return reference to
static CTypeInfo kDummyCTypeInfo = CTypeInfo(CTypeInfo::Type::kVoid);

const CTypeInfo& CFunctionInfo::ArgumentInfo(unsigned int index) const {
  return kDummyCTypeInfo;
}

// CppHeap stubs
std::unique_ptr<CppHeap> CppHeap::Create(Platform* platform, const cppgc::HeapOptions& options) {
  return nullptr;
}

// Context::FromSnapshot stub
MaybeLocal<Context> Context::FromSnapshot(
    Isolate* isolate, size_t context_snapshot_index,
    DeserializeInternalFieldsCallback embedder_fields_deserializer,
    ExtensionConfiguration* extensions,
    Local<Value> global_object,
    MicrotaskQueue* microtask_queue,
    DeserializeContextDataCallback context_data_deserializer,
    DeserializeAPIWrapperCallback api_wrapper_deserializer) {
  return MaybeLocal<Context>();
}

// Exception::Error stub
Local<Value> Exception::Error(Local<String> message, Local<Value> options) {
  return Local<Value>();
}

// Exception::TypeError stub
Local<Value> Exception::TypeError(Local<String> message, Local<Value> options) {
  return Local<Value>();
}

// EscapableHandleScopeBase stubs
EscapableHandleScopeBase::EscapableHandleScopeBase(Isolate* isolate)
    : HandleScope(isolate) {
  // No-op for WASM32
}

internal::Address* EscapableHandleScopeBase::EscapeSlot(internal::Address* escape_value) {
  // WASM32 stub - just return the input
  return escape_value;
}

// EscapableHandleScope stubs
EscapableHandleScope::EscapableHandleScope(Isolate* isolate)
    : EscapableHandleScopeBase(isolate) {
  // No-op for WASM32
}

EscapableHandleScope::~EscapableHandleScope() {
  // No-op for WASM32
}

// SealHandleScope stubs
SealHandleScope::SealHandleScope(Isolate* isolate)
    : i_isolate_(nullptr), prev_limit_(nullptr), prev_sealed_level_(0) {
  // No-op for WASM32
}

SealHandleScope::~SealHandleScope() {
  // No-op for WASM32
}

// ExternalMemoryAccounter stubs
ExternalMemoryAccounter::~ExternalMemoryAccounter() {
  // No-op for WASM32
}

ExternalMemoryAccounter::ExternalMemoryAccounter(ExternalMemoryAccounter&&) = default;
ExternalMemoryAccounter& ExternalMemoryAccounter::operator=(ExternalMemoryAccounter&&) = default;

void ExternalMemoryAccounter::Increase(Isolate* isolate, size_t size) {
  // No-op for WASM32
}

void ExternalMemoryAccounter::Update(Isolate* isolate, int64_t delta) {
  // No-op for WASM32
}

void ExternalMemoryAccounter::Decrease(Isolate* isolate, size_t size) {
  // No-op for WASM32
}

int64_t ExternalMemoryAccounter::GetTotalAmountOfExternalAllocatedMemoryForTesting(
    const Isolate* isolate) {
  return 0;
}

}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
