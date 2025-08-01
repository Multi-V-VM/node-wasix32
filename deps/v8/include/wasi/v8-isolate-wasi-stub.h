#ifndef WASI_V8_ISOLATE_WASI_STUB_H_
#define WASI_V8_ISOLATE_WASI_STUB_H_

#ifdef __wasi__

#include "v8-wasi-compat.h"
#include "../v8-local-handle.h"
#include <cstring>  // for memset

// V8_EXPORT macro for WASI
#ifndef V8_EXPORT
#define V8_EXPORT
#endif

// Include necessary headers for type definitions BEFORE namespace v8
#include "../v8-callbacks.h"
#include "../v8-promise.h"
#include "../v8-microtask.h"
#include "../v8-statistics.h"  // For MeasureMemoryMode
// Don't include v8-snapshot.h here to avoid circular dependency

// SnapshotBlobRef stub if not defined elsewhere
#ifndef V8_SNAPSHOT_BLOB_REF_DEFINED
#define V8_SNAPSHOT_BLOB_REF_DEFINED
struct SnapshotBlobRef {
  const uint8_t* data;
  int raw_size;
};
#endif

namespace v8 {

// Forward declarations
class Context;
class Message;
class Value;
class StackTrace;
class String;
class Isolate;
class HeapProfiler;
class HeapStatistics;

// Type definitions that need to be before Isolate class
using AbortOnUncaughtExceptionCallback = bool (*)(Isolate*);

// Minimal Isolate stub for WASI
class V8_EXPORT Isolate {
 public:
  // CreateParams stub with Node.js required fields
  struct CreateParams {
    CreateParams() = default;
    
    // Node.js required fields
    v8::ResourceConstraints constraints;
    
    int embedder_wrapper_object_index = -1;
    int embedder_wrapper_type_index = -1;
    
    const SnapshotBlobRef* snapshot_blob = nullptr;
    
    // Additional fields for Node.js
    const intptr_t* external_references = nullptr;
    void* cpp_heap = nullptr;
    void* array_buffer_allocator = nullptr;
    std::shared_ptr<void> array_buffer_allocator_shared;
  };
  
  static Isolate* GetCurrent() { return nullptr; }
  static Isolate* TryGetCurrent() { return nullptr; }
  static Isolate* New(const CreateParams& params) { return new Isolate(); }
  
  void Enter() {}
  void Exit() {}
  void Dispose() {}
  void SetIdle(bool idle) { /* No-op for WASI */ }
  
  // Additional methods as needed
  void* GetData(uint32_t slot) { return nullptr; }
  void SetData(uint32_t slot, void* data) {}
  Local<Context> GetCurrentContext() { return Local<Context>(); }
  
  // Execution termination methods
  bool IsExecutionTerminating() { return false; }
  void CancelTerminateExecution() {}
  void TerminateExecution() {
    // WASI stub - no-op
  }
  
  // Other missing Isolate methods
  enum class TimeZoneDetection { kSkip, kRedetect };
  
  std::string GetDefaultLocale() { return "en-US"; }
  void DateTimeConfigurationChangeNotification(
      TimeZoneDetection detection = TimeZoneDetection::kRedetect) {}
  
  // Host callback methods
  void SetHostImportModuleDynamicallyCallback(HostImportModuleDynamicallyCallback callback) {}
  void SetHostImportModuleWithPhaseDynamicallyCallback(
      HostImportModuleWithPhaseDynamicallyCallback callback) {}
  void SetHostInitializeImportMetaObjectCallback(
      HostInitializeImportMetaObjectCallback callback) {}
  
  // Heap profiler
  HeapProfiler* GetHeapProfiler() { return nullptr; }
  
  // Additional methods for Node.js worker support
  void AddNearHeapLimitCallback_WASI(size_t (*callback)(void* data, size_t current_heap_limit, size_t initial_heap_limit), void* data) {
    // WASI stub - no-op
  }
  // Provide the original name via inline wrapper to avoid macro conflicts
  inline void AddNearHeapLimitCallback(size_t (*callback)(void* data, size_t current_heap_limit, size_t initial_heap_limit), void* data) {
    AddNearHeapLimitCallback_WASI(callback, data);
  }
  
  void SetStackLimit(uintptr_t stack_limit) {
    // WASI stub - no-op
  }
  
  // Use the v8::MeasureMemoryDelegate from v8-statistics.h
  void MeasureMemory(std::unique_ptr<v8::MeasureMemoryDelegate> delegate,
                     v8::MeasureMemoryExecution execution = v8::MeasureMemoryExecution::kDefault) {
    // WASI stub - no-op
    // Can't call delegate methods without proper implementation
  }
  
  // GC callbacks - updated to match V8 API signature
  // Note: The standard V8 signature uses GCType and GCCallbackFlags enums, 
  // but the callback expects (Isolate*, GCType, GCCallbackFlags, void*)
  using GCCallback = void (*)(Isolate* isolate, v8::GCType gc_type, v8::GCCallbackFlags gc_flags, void* data);
  void AddGCPrologueCallback(GCCallback callback, void* data = nullptr, v8::GCType gc_type = v8::GCType::kGCTypeAll) {
    // WASI stub - no-op
  }
  void RemoveGCPrologueCallback(GCCallback callback, void* data = nullptr) {
    // WASI stub - no-op
  }
  void AddGCEpilogueCallback(GCCallback callback, void* data = nullptr, v8::GCType gc_type = v8::GCType::kGCTypeAll) {
    // WASI stub - no-op
  }
  void RemoveGCEpilogueCallback(GCCallback callback, void* data = nullptr) {
    // WASI stub - no-op
  }
  
  // Scope class
  class Scope {
   public:
    explicit Scope(Isolate* isolate) {}
    ~Scope() {}
  };
  
  // MessageErrorLevel enum
  enum MessageErrorLevel {
    kMessageLog,
    kMessageDebug,
    kMessageInfo,
    kMessageError,
    kMessageWarning
  };
  
  // DisallowJavascriptExecutionScope for WASI
  class DisallowJavascriptExecutionScope {
  public:
    enum OnFailure {
      CRASH_ON_FAILURE,
      THROW_ON_FAILURE
    };
    
    DisallowJavascriptExecutionScope(Isolate* isolate, OnFailure on_failure) {
      // WASI stub - no-op
    }
    
    ~DisallowJavascriptExecutionScope() {
      // WASI stub - no-op
    }
  };
  
  // AllowJavascriptExecutionScope for WASI
  class AllowJavascriptExecutionScope {
  public:
    explicit AllowJavascriptExecutionScope(Isolate* isolate) {
      // WASI stub - no-op
    }
    
    ~AllowJavascriptExecutionScope() {
      // WASI stub - no-op
    }
  };
  
  // Missing methods for Node.js compatibility
  void AddMessageListenerWithErrorLevel(MessageCallback callback,
                                        int message_levels,
                                        Local<Value> data = Local<Value>()) {
    // WASI stub - no-op
  }
  
  void SetAbortOnUncaughtExceptionCallback(AbortOnUncaughtExceptionCallback callback) {
    // WASI stub - no-op
  }
  
  void SetFatalErrorHandler(FatalErrorCallback callback) {
    // WASI stub - no-op
  }
  
  void SetOOMErrorHandler(OOMErrorCallback callback) {
    // WASI stub - no-op
  }
  
  void SetPrepareStackTraceCallback(PrepareStackTraceCallback callback) {
    // WASI stub - no-op
  }
  
  void SetMicrotasksPolicy(MicrotasksPolicy policy) {
    // WASI stub - no-op
  }
  
  void SetAllowWasmCodeGenerationCallback(AllowWasmCodeGenerationCallback callback) {
    // WASI stub - no-op
  }
  
  void SetModifyCodeGenerationFromStringsCallback(ModifyCodeGenerationFromStringsCallback2 callback) {
    // WASI stub - no-op
  }
  
  void SetWasmStreamingCallback(WasmStreamingCallback callback) {
    // WASI stub - no-op
  }
  
  void SetHostCreateShadowRealmContextCallback(HostCreateShadowRealmContextCallback callback) {
    // WASI stub - no-op
  }
  
  void SetPromiseRejectCallback(PromiseRejectCallback callback) {
    // WASI stub - no-op
  }
  
  static Isolate* Allocate() {
    return new Isolate();
  }
  
  // Interrupt handling
  using InterruptCallback = void (*)(Isolate* isolate, void* data);
  void RequestInterrupt(InterruptCallback callback, void* data) {
    // WASI stub - no-op
    // In a real implementation, this would queue the callback to be called
  }
  
  // Heap statistics methods
  size_t NumberOfHeapSpaces() { return 0; }
  
  struct HeapSpaceStatistics {
    const char* space_name() { return ""; }
    size_t space_size() { return 0; }
    size_t space_used_size() { return 0; }
    size_t space_available_size() { return 0; }
    size_t physical_space_size() { return 0; }
  };
  
  bool GetHeapSpaceStatistics(HeapSpaceStatistics* space_statistics, size_t index) {
    // WASI stub - no-op
    return false;
  }
  
  void GetHeapStatistics(HeapStatistics* heap_statistics) {
    // WASI stub - no-op
    // Cannot initialize here as HeapStatistics is not fully defined yet
  }
  
  int64_t AdjustAmountOfExternalAllocatedMemory(int64_t change_in_bytes) {
    // WASI stub - return 0
    return 0;
  }
  
  static void Free(Isolate* isolate) {
    delete isolate;
  }
  
  void Deinitialize() {
    // WASI stub - no-op
  }
  
  // Additional required methods
  Local<Value> GetContinuationPreservedEmbedderData() {
    // WASI stub - return empty
    return Local<Value>();
  }
  
  void SetContinuationPreservedEmbedderData(Local<Value> data) {
    // WASI stub - no-op
  }
  
  // Priority enum
  enum class Priority {
    kBestEffort = 0,
    kUserVisible = 1,
    kUserBlocking = 2,
  };
  
  // Exception handling
  Local<Value> ThrowException(Local<Value> exception) {
    return exception;
  }
  
  // Additional methods for Node.js
  static void Initialize(Isolate* isolate, const CreateParams& params) {
    // WASI stub - no-op
  }
  
  void DumpAndResetStats() {
    // WASI stub - no-op
  }
  
  // GetDataFromSnapshotOnce method
  template<typename T>
  MaybeLocal<T> GetDataFromSnapshotOnce(size_t index) {
    return MaybeLocal<T>();
  }
  
  // Missing methods for heap management
  void AutomaticallyRestoreInitialHeapLimit(double threshold) {
    // WASI stub - no-op
  }
  
  void ClearKeptObjects() {
    // WASI stub - no-op
  }
  
  void LowMemoryNotification() {
    // WASI stub - no-op
  }
};

// StackTracePrinter function type
using StackTracePrinter = void (*)();

// MeasureMemoryDelegate is already defined in v8-statistics.h
// Don't redefine it here

}  // namespace v8

#endif // __wasi__

#endif // WASI_V8_ISOLATE_WASI_STUB_H_