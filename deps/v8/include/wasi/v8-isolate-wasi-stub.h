#ifndef WASI_V8_ISOLATE_WASI_STUB_H_
#define WASI_V8_ISOLATE_WASI_STUB_H_

#ifdef __wasi__

#include "v8-wasi-compat.h"
#include "v8-api-stubs.h"
#include "../v8-local-handle.h"
#include "../v8-maybe-local.h"
#include "../v8-callbacks.h"  // Ensure canonical callback/GC typedefs and forward decls
#include <cstring>  // for memset
#include <memory>
#include <string>

// V8_EXPORT macro for WASI
#ifndef V8_EXPORT
#define V8_EXPORT
#endif

// Include necessary headers for type definitions BEFORE namespace v8
// IMPORTANT: Include v8-value.h first to ensure Value base class is complete
#include "../v8-value.h"
#include "../v8-microtask.h"
// Note: Do NOT include v8-promise.h here - it creates circular dependency
// because Object isn't defined yet. Forward declare Promise instead.
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

// Forward declarations required by callback typedefs
class Context;
class Message;
class Value;
class StackTrace;
class String;
// Promise forward declaration - full definition not needed yet
class Promise;
class Isolate;
class HeapProfiler;
class HeapStatistics; // Forward declaration for v8::HeapStatistics
class Module;
class Data;
class Object;
class Array;

// Promise reject types - use unique guard to prevent double definition
// This guard prevents redefinition when v8-promise.h is later included
#ifndef V8_WASI_PROMISE_TYPES_DEFINED
#define V8_WASI_PROMISE_TYPES_DEFINED

enum PromiseRejectEvent {
  kPromiseRejectWithNoHandler = 0,
  kPromiseHandlerAddedAfterReject = 1,
  kPromiseRejectAfterResolved = 2,
  kPromiseResolveAfterResolved = 3,
};

// PromiseRejectMessage class (from v8-promise.h)
// Defined here to avoid circular dependency with v8-promise.h
class PromiseRejectMessage {
 public:
  PromiseRejectMessage(Local<Promise> promise, PromiseRejectEvent event,
                       Local<Value> value)
      : promise_(promise), event_(event), value_(value) {}

  V8_INLINE Local<Promise> GetPromise() const { return promise_; }
  V8_INLINE PromiseRejectEvent GetEvent() const { return event_; }
  V8_INLINE Local<Value> GetValue() const { return value_; }

 private:
  Local<Promise> promise_;
  PromiseRejectEvent event_;
  Local<Value> value_;
};

// Promise-related callback typedef (from v8-promise.h)
using PromiseRejectCallback = void (*)(PromiseRejectMessage message);

#endif  // V8_WASI_PROMISE_TYPES_DEFINED

// GC types needed by Isolate class - define in v8 namespace
// Only define if v8-callbacks.h hasn't already provided them
#ifndef INCLUDE_V8_ISOLATE_CALLBACKS_H_
enum GCType { kGCTypeAll = 0 };
enum GCCallbackFlags { kNoGCCallbackFlags = 0 };
#endif

#ifndef V8_WASI_CALLBACK_TYPES_DEFINED
#define V8_WASI_CALLBACK_TYPES_DEFINED
// Only provide fallbacks if the canonical header wasn't included (defensive).
#ifndef INCLUDE_V8_ISOLATE_CALLBACKS_H_
  // Minimal MaybeLocal fallback when v8-maybe-local.h hasn't been included.
  #ifndef INCLUDE_V8_MAYBE_LOCAL_H_
  template <class T>
  class MaybeLocal {
   public:
    MaybeLocal() = default;
    template <class S>
    MaybeLocal(Local<S>) {}
    bool IsEmpty() const { return true; }
    Local<T> ToLocalChecked() const { return Local<T>(); }
    bool ToLocal(Local<T>*) const { return false; }
  };
  #endif  // INCLUDE_V8_MAYBE_LOCAL_H_

  // Minimal enums and callback typedefs to unblock compilation when the real
  // declarations are unavailable. These are guarded to avoid redefinitions
  // once v8-callbacks.h is present.
  // Note: GCType and GCCallbackFlags are defined outside the guards above
  enum class ModuleImportPhase { kSource, kEvaluation };

  using MessageCallback = void (*)(Local<Message> message, Local<Value> data);
  using FatalErrorCallback = void (*)(const char* location, const char* message);
  struct OOMDetails;
  using OOMErrorCallback = void (*)(const char* location, const OOMDetails& details);
  template <typename T> class FunctionCallbackInfo;  // forward decl
  class FixedArray;  // forward decl
  class Promise;     // forward decl
  using HostImportModuleDynamicallyCallback = MaybeLocal<Promise> (*)(
      Local<Context> context, Local<Data> host_defined_options,
      Local<Value> resource_name, Local<String> specifier,
      Local<FixedArray> import_attributes);
  using HostImportModuleWithPhaseDynamicallyCallback = MaybeLocal<Promise> (*)(
      Local<Context> context, Local<Data> host_defined_options,
      Local<Value> resource_name, Local<String> specifier,
      ModuleImportPhase phase, Local<FixedArray> import_attributes);
  using HostInitializeImportMetaObjectCallback = void (*)(Local<Context> context,
                                                         Local<Module> module,
                                                         Local<Object> meta);
  using PrepareStackTraceCallback = MaybeLocal<Value> (*)(Local<Context> context,
                                                         Local<Value> error,
                                                         Local<Array> sites);
  using WasmStreamingCallback = void (*)(const FunctionCallbackInfo<Value>&);
  using HostCreateShadowRealmContextCallback = MaybeLocal<Context> (*)(Local<Context> initiator_context);
#endif  // !INCLUDE_V8_ISOLATE_CALLBACKS_H_
#endif  // V8_WASI_CALLBACK_TYPES_DEFINED

// PromiseRejectCallback is provided by v8-promise.h

// Minimal ResourceConstraints stub for WASI builds
class ResourceConstraints {
 public:
  size_t max_old_generation_size_in_bytes() const { return 0; }
  void ConfigureDefaults(size_t /*physical_memory*/, size_t /*virtual_memory_limit*/) {}
  void set_stack_limit(uint32_t*) {}
  void set_max_young_generation_size_in_bytes(size_t) {}
  size_t max_young_generation_size_in_bytes() const { return 0; }
  void set_max_old_generation_size_in_bytes(size_t) {}
  void set_code_range_size_in_bytes(size_t) {}
  size_t code_range_size_in_bytes() const { return 0; }
};

// Type definitions that need to be before Isolate class
using AbortOnUncaughtExceptionCallback = bool (*)(Isolate*);

// Minimal Isolate stub for WASI
class V8_EXPORT Isolate {
 public:
  using AbortOnUncaughtExceptionCallback = bool (*)(Isolate*);
  // UseCounter feature enumeration (include commonly referenced entries).
  enum UseCounterFeature {
    kExtendingNonExtensibleWithPrivate = 0,
    kDateToLocaleDateString,
    kDateToLocaleString,
    kDateToLocaleTimeString,
    kNumberToLocaleString,
    kResizableArrayBuffer,
    kGrowableSharedArrayBuffer,
    kDocumentAllLegacyCall,
    kDocumentAllLegacyConstruct,
    kStringLocaleCompare,
    kConsoleContext,
    kStringNormalize,
    kDeoptimizerDisableSpeculation,
    kExplicitResourceManagement,
    kDisplayNames,
    kDurationFormat,
    kNumberFormat,
    kDateTimeFormat,
    kListFormat,
    kLocale,
    kLocaleInfoFunctions,
    kLocaleInfoObsoletedGetters,
    kRelativeTimeFormat,
    kPluralRules,
    kCollator,
    kSegmenter,
    kArrayInstanceConstructorModified,
    kArrayPrototypeConstructorModified,
    kArraySpeciesModified,
    kNumberFormatStyleUnit,
    kBreakIteratorTypeLine,
    kBreakIteratorTypeWord,
    kBreakIterator,
    kDateTimeFormatDateTimeStyle,
    kDateTimeFormatRange,
    kSharedArrayBufferConstructed,
    kFunctionTokenOffsetTooLongForToString,
    kWasmSharedMemory,
    kWebAssemblyInstantiation,
    kStringToLocaleLowerCase,
    kDecimalWithLeadingZeroInStrictMode,
    // Features referenced by parser web-compat hack
    kAssigmentExpressionLHSIsCallInStrict,
    kAssigmentExpressionLHSIsCallInSloppy,
    // Reserve space for unknown future features.
    kUseCounterFeatureCount = 256
  };

  // Callback types
  using UseCounterCallback = void (*)(Isolate*, UseCounterFeature);
  using ReleaseCppHeapCallback = void (*)(Isolate*);

  // CreateParams stub with Node.js required fields
  struct CreateParams {
    CreateParams() = default;
    
    // Node.js required fields
    ResourceConstraints constraints;
    
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
  
  // Exception handling stubs for WASI builds
  bool HasPendingException() const { return pending_exception_; }
  void ClearPendingException() { pending_exception_ = false; }
  void ThrowError(const char* /*message*/) { pending_exception_ = true; }
  
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
  template <typename F>
  void SetHostImportModuleDynamicallyCallback(F) {}
  template <typename F>
  void SetHostImportModuleWithPhaseDynamicallyCallback(F) {}
  template <typename F>
  void SetHostInitializeImportMetaObjectCallback(F) {}
  
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
  void MeasureMemory(std::unique_ptr<MeasureMemoryDelegate> delegate,
                     MeasureMemoryExecution execution = MeasureMemoryExecution::kDefault) {
    // WASI stub - no-op
    // Can't call delegate methods without proper implementation
  }

  // Restore original heap limit (used by inspector for OOM breakpoints)
  void RestoreOriginalHeapLimit() {}
  
  // GC callbacks - updated to match V8 API signature
  // Note: The standard V8 signature uses GCType and GCCallbackFlags enums.
  // These are defined in v8-callbacks.h (included at the top of this file)
  using GCCallbackWithData = void (*)(Isolate* isolate, ::v8::GCType,
                                      ::v8::GCCallbackFlags, void* data);
  using GCCallback = void (*)(Isolate* isolate, ::v8::GCType,
                              ::v8::GCCallbackFlags);
  using GetExternallyAllocatedMemoryInBytesCallback = size_t (*)();

  template <typename... Args>
  void AddGCPrologueCallback(Args...) {}
  template <typename... Args>
  void RemoveGCPrologueCallback(Args...) {}
  template <typename... Args>
  void AddGCEpilogueCallback(Args...) {}
  template <typename... Args>
  void RemoveGCEpilogueCallback(Args...) {}
  
  // Scope class
  class Scope {
   public:
    explicit Scope(Isolate* isolate) {}
    ~Scope() {}
  };
  
  // MessageErrorLevel - use the one from v8:: namespace
  using MessageErrorLevel = ::v8::MessageErrorLevel;
  
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

  // Minimal usage counter support (implemented above in class definitions)
  void CountUsage(UseCounterFeature) {}
  
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
  template <typename F>
  void AddMessageListenerWithErrorLevel(F /*callback*/, int /*message_levels*/,
                                        Local<Value> = Local<Value>()) {
    // WASI stub - no-op
  }
  
  void SetAbortOnUncaughtExceptionCallback(AbortOnUncaughtExceptionCallback callback) {
    // WASI stub - no-op
  }
  
  template <typename F>
  void SetFatalErrorHandler(F) {
    // WASI stub - no-op
  }
  
  template <typename F>
  void SetOOMErrorHandler(F) {
    // WASI stub - no-op
  }
  
  template <typename F>
  void SetPrepareStackTraceCallback(F) {
    // WASI stub - no-op
  }
  
  void SetMicrotasksPolicy(MicrotasksPolicy policy) {
    // WASI stub - no-op
  }
  
  template <typename F>
  void SetAllowWasmCodeGenerationCallback(F) {
    // WASI stub - no-op
  }
  
  template <typename F>
  void SetModifyCodeGenerationFromStringsCallback(F) {
    // WASI stub - no-op
  }
  
  template <typename F>
  void SetWasmStreamingCallback(F) {
    // WASI stub - no-op
  }
  
  template <typename F>
  void SetHostCreateShadowRealmContextCallback(F) {
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
    pending_exception_ = true;
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
 
 private:
  bool pending_exception_ = false;
};

// StackTracePrinter function type
using StackTracePrinter = void (*)();

// MeasureMemoryDelegate is already defined in v8-statistics.h
// Don't redefine it here

}  // namespace v8

#endif // __wasi__

#endif // WASI_V8_ISOLATE_WASI_STUB_H_
