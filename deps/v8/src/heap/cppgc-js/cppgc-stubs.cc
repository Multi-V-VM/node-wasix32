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
#include "include/v8-message.h"
#include "include/cppgc/heap.h"
#include "include/cppgc/platform.h"
#include "include/v8-external-memory-accounter.h"
#include "include/cppgc/allocation.h"
#include "include/cppgc/internal/gc-info.h"
#include "include/cppgc/internal/logging.h"
#include "include/cppgc/internal/persistent-node.h"
#include "include/cppgc/internal/pointer-policies.h"
#include "src/deoptimizer/deoptimizer.h"
#include "src/execution/isolate-inl.h"
#include "src/execution/v8threads.h"
#include "src/handles/handles.h"
#include "src/roots/roots-inl.h"
#include "src/base/virtual-address-space.h"
#include <cstdlib>
#include <atomic>

namespace cppgc {

// cppgc::InitializeProcess stub - use weak linkage for mksnapshot compatibility
__attribute__((weak))
void InitializeProcess(PageAllocator* page_allocator, size_t desired_heap_size) {
  // No-op for WASM32
}

// cppgc::ShutdownProcess stub
__attribute__((weak))
void ShutdownProcess() {
  // No-op for WASM32
}

namespace internal {

// ProcessGlobalLock mutex - required by process-heap.h
v8::base::LazyMutex ProcessGlobalLock::process_mutex_ = LAZY_MUTEX_INITIALIZER;

}  // namespace internal
}  // namespace cppgc

namespace v8 {

__attribute__((weak))
std::unique_ptr<MicrotaskQueue> MicrotaskQueue::New(Isolate* v8_isolate,
                                                    MicrotasksPolicy policy) {
  class NoopMicrotaskQueue final : public MicrotaskQueue {
   public:
    void EnqueueMicrotask(Isolate* isolate, Local<Function> microtask) override {
    }
    void EnqueueMicrotask(Isolate* isolate,
                          MicrotaskCallback callback,
                          void* data = nullptr) override {}
    void AddMicrotasksCompletedCallback(
        MicrotasksCompletedCallbackWithData callback,
        void* data = nullptr) override {}
    void RemoveMicrotasksCompletedCallback(
        MicrotasksCompletedCallbackWithData callback,
        void* data = nullptr) override {}
    void PerformCheckpoint(Isolate* isolate) override {}
    bool IsRunningMicrotasks() const override { return false; }
    int GetMicrotasksScopeDepth() const override { return 0; }
  };
  return std::unique_ptr<MicrotaskQueue>(new NoopMicrotaskQueue());
}

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
bool Value::IsTrue() const {
  internal::Isolate* isolate = internal::Isolate::TryGetCurrent();
  if (isolate == nullptr) return false;
  return reinterpret_cast<internal::Address>(this) ==
         internal::ReadOnlyRoots(isolate).true_value().ptr();
}

bool Value::IsFalse() const {
  internal::Isolate* isolate = internal::Isolate::TryGetCurrent();
  if (isolate == nullptr) return false;
  return reinterpret_cast<internal::Address>(this) ==
         internal::ReadOnlyRoots(isolate).false_value().ptr();
}

namespace internal {

// HandleScopeImplementer stubs for WASM32
__attribute__((weak)) const size_t
    HandleScopeImplementer::kEnteredContextsOffset = 0;

void HandleScopeImplementer::Iterate(RootVisitor* v) {
  // No-op for WASM32
}

char* HandleScopeImplementer::Iterate(RootVisitor* v, char* storage) {
  return storage + ArchiveSpacePerThread();
}

int HandleScopeImplementer::ArchiveSpacePerThread() {
  return sizeof(HandleScopeImplementer);
}

std::unique_ptr<PersistentHandles> HandleScopeImplementer::DetachPersistent(
    Address* first_block) {
  return nullptr;
}

// CppHeap stub: cpp-heap.cc is excluded for wasm32 (see v8.gyp sources!) so
// this stub is the only definition.
void CppHeap::InitializeOncePerProcess() {
  // No-op for WASM32
}

// Real platform virtual address space, backed by the WASI mmap emulation in
// platform-posix.cc. The old stub returned nullptr, which made
// ExternalEntityTable (JS dispatch table / leaptiering) trap with
// "uninitialized element" on the first virtual call during Genesis.
VirtualAddressSpace* GetPlatformVirtualAddressSpace() {
  static ::v8::base::VirtualAddressSpace vas;
  return &vas;
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
// Context::New is provided by the real implementation in api.cc (WASI branch
// after NewRemoteContext), which delegates to the full NewContext machinery.
// The old always-empty stub here made Node fail CreateMainEnvironment with
// "Assertion failed: !context.IsEmpty()".

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

#if !V8_ENABLE_TURBOFAN
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
#endif  // !V8_ENABLE_TURBOFAN

// Full compiler builds provide CodeAssembler and Turbofan/Turboshaft symbols.

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

// CppHeap stub: cpp-heap.cc is excluded for wasm32 (see v8.gyp sources!) so
// this stub is the only definition.
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

// Exception::Error/TypeError/RangeError/CreateMessage are now provided by the
// real implementations in api.cc (carved out of the !__wasi__ exclusion).
// The old always-empty stubs here were silent-null landmines: every thrown
// error became an empty Local that crashed far from the throw site.

// CFunctionInfo stub
CFunctionInfo::CFunctionInfo(const CTypeInfo& return_info,
                             unsigned int arg_count,
                             const CTypeInfo* arg_info,
                             Int64Representation repr)
    : return_info_(return_info), repr_(repr), arg_count_(arg_count),
      arg_info_(arg_info) {}

// CFunction stub
CFunction::CFunction(const void* address, const CFunctionInfo* type_info)
    : address_(address), type_info_(type_info) {}

// Object::PreviewEntries stub
MaybeLocal<Array> Object::PreviewEntries(bool* is_key_value) {
  if (is_key_value) *is_key_value = false;
  return MaybeLocal<Array>();
}

// Note: EscapableHandleScope, EscapableHandleScopeBase, and SealHandleScope
// are provided by wasi/v8-handlescope-fix.h for WASI builds

// ExternalMemoryAccounter stubs - use weak linkage for mksnapshot compatibility
__attribute__((weak))
ExternalMemoryAccounter::~ExternalMemoryAccounter() {
  // No-op for WASM32
}

ExternalMemoryAccounter::ExternalMemoryAccounter(ExternalMemoryAccounter&&) = default;
ExternalMemoryAccounter& ExternalMemoryAccounter::operator=(ExternalMemoryAccounter&&) = default;

__attribute__((weak))
void ExternalMemoryAccounter::Increase(Isolate* isolate, size_t size) {
  // No-op for WASM32
}

__attribute__((weak))
void ExternalMemoryAccounter::Decrease(Isolate* isolate, size_t size) {
  // No-op for WASM32
}

void ExternalMemoryAccounter::Update(Isolate* isolate, int64_t delta) {
  // No-op for WASM32
}

int64_t ExternalMemoryAccounter::GetTotalAmountOfExternalAllocatedMemoryForTesting(
    const Isolate* isolate) {
  return 0;
}

// MicrotasksScope stubs with full signature - use weak linkage
__attribute__((weak))
MicrotasksScope::MicrotasksScope(Isolate* isolate, MicrotaskQueue* queue, Type type)
    : i_isolate_(reinterpret_cast<internal::Isolate*>(isolate)),
      microtask_queue_(reinterpret_cast<internal::MicrotaskQueue*>(queue)),
      run_(type == kRunMicrotasks) {
}

__attribute__((weak))
MicrotasksScope::~MicrotasksScope() {
  // No-op for WASM32
}

namespace internal {

// StrongRootAllocatorBase stubs - use weak linkage
__attribute__((weak))
Address* StrongRootAllocatorBase::allocate_impl(size_t n) {
  // Return a valid pointer for WASM32 - use malloc
  return static_cast<Address*>(malloc(n * sizeof(Address)));
}

__attribute__((weak))
void StrongRootAllocatorBase::deallocate_impl(Address* p, size_t n) noexcept {
  // Free the allocated memory
  free(p);
}

// Function callback stubs - use weak linkage
__attribute__((weak))
void InvokeFunctionCallbackGeneric(const FunctionCallbackInfo<Value>& info) {
  // No-op for WASM32
}

__attribute__((weak))
void InvokeFunctionCallbackOptimized(const FunctionCallbackInfo<Value>& info) {
  // No-op for WASM32
}

__attribute__((weak))
void InvokeAccessorGetterCallback(v8::Local<v8::Name> property,
                                   const v8::PropertyCallbackInfo<v8::Value>& info) {
  // No-op for WASM32
}

// Deoptimizer stub - use weak linkage
__attribute__((weak))
void Deoptimizer::PatchToJump(Address pc, Address target) {
  // No-op for WASM32 - no JIT
}

// HandleScopeImplementer stub - use weak linkage
__attribute__((weak))
char* HandleScopeImplementer::ArchiveThread(char* storage) {
  return storage + ArchiveSpacePerThread();
}

__attribute__((weak))
char* HandleScopeImplementer::RestoreThread(char* storage) {
  return storage + ArchiveSpacePerThread();
}

__attribute__((weak))
void HandleScopeImplementer::FreeThreadResources() {
  // No-op for WASM32
}

}  // namespace internal

namespace base {

// OS::AdjustSchedulingParams stub - use weak linkage
__attribute__((weak))
void OS::AdjustSchedulingParams() {
  // No-op for WASM32
}

}  // namespace base

}  // namespace v8

// cppgc stubs
namespace cppgc {

__attribute__((weak))
bool IsInitialized() {
  return false;
}

namespace internal {

// EnsureGCInfoIndexTrait stub - use weak linkage
__attribute__((weak))
unsigned short EnsureGCInfoIndexTrait::EnsureGCInfoIndex(
    std::atomic<unsigned short>& registered_index,
    void (*trace)(Visitor*, const void*)) {
  return 0;
}

__attribute__((weak))
unsigned short EnsureGCInfoIndexTrait::EnsureGCInfoIndex(
    std::atomic<unsigned short>& registered_index,
    void (*trace)(Visitor*, const void*),
    void (*finalize)(void*),
    HeapObjectName (*name)(const void*, HeapObjectNameForUnnamedObject)) {
  return 0;
}

__attribute__((weak))
PersistentRegion& WeakPersistentPolicy::GetPersistentRegion(
    const void* object) {
  static PersistentRegion* region = nullptr;
  return *region;
}

__attribute__((weak))
bool PersistentRegion::IsCreationThread() {
  return true;
}

__attribute__((weak))
PersistentNode* PersistentRegionBase::RefillFreeListAndAllocateNode(
    void* owner,
    void (*trace)(RootVisitor&, const void*)) {
  return nullptr;
}

__attribute__((weak))
void FatalImpl(const char* message, const v8::SourceLocation& loc) {
  abort();
}

// MakeGarbageCollectedTraitInternal stub - use weak linkage
__attribute__((weak))
void* MakeGarbageCollectedTraitInternal::Allocate(
    AllocationHandle& handle,
    size_t size,
    unsigned short gc_info_index,
    size_t alignment) {
  // Return nullptr - GC not supported in WASM32
  return nullptr;
}

}  // namespace internal
}  // namespace cppgc

// GlobalGCInfoTable static member stub - needed by concurrent-marking.o and mark-compact.o
// Include the header that declares the class and just define the static member
#include "src/heap/cppgc/gc-info-table.h"

namespace cppgc {
namespace internal {
__attribute__((weak))
GCInfoTable* GlobalGCInfoTable::global_table_ = nullptr;
}  // namespace internal
}  // namespace cppgc

// CppHeap WriteBarrier stub
namespace v8 {
namespace internal {

__attribute__((weak))
void CppHeap::WriteBarrier(void* object) {
  // No-op for WASM32
}

}  // namespace internal
}  // namespace v8

// EscapableHandleScope stubs - these are needed for V8 inspector code
// For V8 source builds with V8_EXPORT_PRIVATE, we need explicit implementations
// Include the handlescope-fix header to get class declarations
#include "include/wasi/v8-handlescope-fix.h"

// EscapableHandleScopeBase constructor implementation
__attribute__((weak))
v8::EscapableHandleScopeBase::EscapableHandleScopeBase(v8::Isolate* isolate) {
  Initialize(isolate);
  escape_slot_ = nullptr;
}

// EscapableHandleScope constructor implementation
__attribute__((weak))
v8::EscapableHandleScope::EscapableHandleScope(v8::Isolate* isolate)
    : EscapableHandleScopeBase(isolate) {
  // No-op for WASM32
}

#endif  // V8_TARGET_ARCH_WASM32
