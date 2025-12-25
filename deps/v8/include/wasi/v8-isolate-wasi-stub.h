#ifndef WASI_V8_ISOLATE_WASI_STUB_H_
#define WASI_V8_ISOLATE_WASI_STUB_H_

#ifdef __wasi__

#include "v8-wasi-compat.h"
#include "v8-api-stubs.h"
#include "../v8-local-handle.h"
#include "../v8-maybe-local.h"
#include "../v8-maybe.h"  // For Maybe<T> and Just() helper
#include "../v8-callbacks.h"  // Ensure canonical callback/GC typedefs and forward decls
#include <cstddef>   // size_t
#include <cstring>  // for memset

// Undefine any WASI compatibility macros that would hijack method names
// when this header provides the class member declarations.
#ifdef AddNearHeapLimitCallback
#undef AddNearHeapLimitCallback
#endif
#ifdef RemoveNearHeapLimitCallback
#undef RemoveNearHeapLimitCallback
#endif
#ifdef SetCaptureStackTraceForUncaughtExceptions
#undef SetCaptureStackTraceForUncaughtExceptions
#endif

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

// SnapshotBlobRef is no longer used - use v8::StartupData instead

namespace v8 {

// StartupData struct for snapshot data - guard against redefinition
#ifndef V8_STARTUP_DATA_DEFINED
#define V8_STARTUP_DATA_DEFINED
class V8_EXPORT StartupData {
 public:
  // Methods required by Node.js - implementations in api.cc
  bool CanBeRehashed() const;
  bool IsValid() const;

  const char* data;
  int raw_size;
};
#endif

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
class CppHeap;  // Forward declaration for CppHeap
class Private;  // Forward declaration for Private (used by SetPrivate)

namespace internal {
class MicrotaskQueue;
}  // namespace internal

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
// We are already inside namespace v8 at this point in the header.
enum GCType { kGCTypeAll = 0 };
enum GCCallbackFlags { kNoGCCallbackFlags = 0 };
#endif  // INCLUDE_V8_ISOLATE_CALLBACKS_H_

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
  void ConfigureDefaults(uint64_t /*physical_memory*/, uint64_t /*virtual_memory_limit*/) {}
  void ConfigureDefaultsFromHeapSize(size_t /*initial_heap_size_in_bytes*/,
                                     size_t /*maximum_heap_size_in_bytes*/) {}
  void set_stack_limit(uint32_t*) {}
  void set_max_young_generation_size_in_bytes(size_t) {}
  size_t max_young_generation_size_in_bytes() const { return 0; }
  void set_max_old_generation_size_in_bytes(size_t) {}
  void set_initial_young_generation_size_in_bytes(size_t) {}
  size_t initial_young_generation_size_in_bytes() const { return 0; }
  void set_initial_old_generation_size_in_bytes(size_t) {}
  size_t initial_old_generation_size_in_bytes() const { return 0; }
  void set_code_range_size_in_bytes(size_t) {}
  size_t code_range_size_in_bytes() const { return 0; }
};

// Type definitions that need to be before Isolate class
using AbortOnUncaughtExceptionCallback = bool (*)(Isolate*);

// Enums for ArrayBuffer - guard against redefinition
#ifndef V8_ARRAY_BUFFER_ENUMS_DEFINED
#define V8_ARRAY_BUFFER_ENUMS_DEFINED
enum class ArrayBufferCreationMode { kInternalized, kExternalized };
enum class BackingStoreInitializationMode { kZeroInitialized, kUninitialized };
enum class BackingStoreOnFailureMode { kReturnNull, kOutOfMemory };
#endif

// BackingStore class stub - declarations only, implementations in api.cc
#ifndef V8_BACKING_STORE_DEFINED
#define V8_BACKING_STORE_DEFINED

// Define internal::BackingStoreBase if not already defined
// (already defined in nuclear-fix.h with guard V8_WASI_BACKINGSTORE_BASE_DEFINED)
namespace internal {
#ifndef V8_WASI_BACKINGSTORE_BASE_DEFINED
#define V8_WASI_BACKINGSTORE_BASE_DEFINED 1
class BackingStoreBase {
 public:
  virtual ~BackingStoreBase() = default;
};
#endif
}  // namespace internal

class V8_EXPORT BackingStore : public internal::BackingStoreBase {
 public:
  ~BackingStore();
  void* Data() const;
  size_t ByteLength() const;
  size_t MaxByteLength() const;
  bool IsShared() const;
  bool IsResizableByUserJavaScript() const;

  using DeleterCallback = void (*)(void* data, size_t length, void* deleter_data);
  static void EmptyDeleter(void* data, size_t length, void* deleter_data);

  void operator delete(void* ptr) { ::operator delete(ptr); }

 private:
  BackingStore();
};
#endif

// ArrayBuffer class stub for WASI - doesn't inherit from Object
// This avoids the complex V8 internal dependencies
// Declarations only, implementations in api.cc
#ifndef V8_ARRAY_BUFFER_DEFINED
#define V8_ARRAY_BUFFER_DEFINED
class V8_EXPORT ArrayBuffer {
 public:
  static constexpr size_t kMaxByteLength = 0x7FFFFFFF;
  static constexpr int kInternalFieldCount = 2;
  static constexpr int kEmbedderFieldCount = kInternalFieldCount;

  class V8_EXPORT Allocator {
   public:
    virtual ~Allocator() = default;
    virtual void* Allocate(size_t length) = 0;
    virtual void* AllocateUninitialized(size_t length) = 0;
    virtual void Free(void* data, size_t length) = 0;
    virtual size_t MaxAllocationSize() const { return kMaxByteLength; }
    enum class AllocationMode { kNormal, kReservation };
    static Allocator* NewDefaultAllocator();
  };

  size_t ByteLength() const;
  size_t MaxByteLength() const;

  static MaybeLocal<ArrayBuffer> MaybeNew(
      Isolate* isolate, size_t byte_length,
      BackingStoreInitializationMode initialization_mode =
          BackingStoreInitializationMode::kZeroInitialized);

  static Local<ArrayBuffer> New(
      Isolate* isolate, size_t byte_length,
      BackingStoreInitializationMode initialization_mode =
          BackingStoreInitializationMode::kZeroInitialized);

  static Local<ArrayBuffer> New(Isolate* isolate,
                                std::shared_ptr<BackingStore> backing_store);

  static std::unique_ptr<BackingStore> NewBackingStore(
      Isolate* isolate, size_t byte_length,
      BackingStoreInitializationMode initialization_mode =
          BackingStoreInitializationMode::kZeroInitialized,
      BackingStoreOnFailureMode on_failure =
          BackingStoreOnFailureMode::kOutOfMemory);

  static std::unique_ptr<BackingStore> NewBackingStore(
      void* data, size_t byte_length, BackingStore::DeleterCallback deleter,
      void* deleter_data);

  static std::unique_ptr<BackingStore> NewResizableBackingStore(
      size_t byte_length, size_t max_byte_length);

  bool IsDetachable() const;
  bool WasDetached() const;
  void Detach();
  V8_WARN_UNUSED_RESULT Maybe<bool> Detach(Local<Value> key);
  void SetDetachKey(Local<Value> key);
  std::shared_ptr<BackingStore> GetBackingStore();
  bool IsResizableByUserJavaScript() const;
  void* Data() const;

  // Methods from Object base class (inline stub implementations for WASI)
  V8_WARN_UNUSED_RESULT Maybe<bool> SetPrivate(Local<Context> context,
                                                Local<Private> key,
                                                Local<Value> value) {
    return Just(true);  // Stub - always succeed
  }
  MaybeLocal<Value> GetPrivate(Local<Context> context, Local<Private> key) {
    return MaybeLocal<Value>();  // Stub - return empty
  }
  V8_WARN_UNUSED_RESULT Maybe<bool> HasPrivate(Local<Context> context,
                                                Local<Private> key) {
    return Just(false);  // Stub - key not found
  }
  V8_WARN_UNUSED_RESULT Maybe<bool> DeletePrivate(Local<Context> context,
                                                   Local<Private> key) {
    return Just(true);  // Stub - always succeed
  }

  V8_INLINE static ArrayBuffer* Cast(Value* value) {
#ifdef __wasi__
    return reinterpret_cast<ArrayBuffer*>(value);
#else
    return static_cast<ArrayBuffer*>(value);
#endif
  }

 private:
  ArrayBuffer();
  static void CheckCast(Value* obj);
};
#endif

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
    // Additional features referenced by Node/V8 sources
    kFunctionPrototypeArguments,
    kFunctionPrototypeCaller,
    kArrayBufferTransfer,
    kCallSiteAPIGetThisSloppyCall,
    kCallSiteAPIGetFunctionSloppyCall,
    kErrorCaptureStackTrace,
    kErrorIsError,
    kFunctionConstructorReturnedUndefined,
    kDefineGetterOrSetterWouldThrow,
    kRegExpPrototypeToString,
    kRegExpEscape,
    kAtomicsWaitAsync,
    kLegacyDateParser,
    // Features referenced by parser web-compat hack
    kAssigmentExpressionLHSIsCallInStrict,
    kAssigmentExpressionLHSIsCallInSloppy,
    // TurboFan OSR compilation tracking
    kTurboFanOsrCompileStarted,
    // Parser/language feature tracking
    kSloppyModeBlockScopedFunctionRedefinition,
    kVarRedeclaredCatchBinding,
    kForInInitializer,
    kHtmlComment,
    kHtmlCommentInExternalScript,
    kCompileHintsMagicAll,
    kSourceMappingUrlMagicCommentAtSign,
    kSloppyMode,
    kStrictMode,
    kUseAsm,
    kAsyncStackTaggingCreateTaskCall,
    kErrorPrepareStackTrace,
    // WASM feature tracking
    kWasmRefTypes,
    kWasmGC,
    kWasmSimdOpcodes,
    kWasmThreadOpcodes,
    kWasmExceptionHandling,
    kWasmMemory64,
    kWasmMultiMemory,
    kWasmImportedStrings,
    kWasmImportedStringsUtf8,
    kWasmReturnCall,
    kWasmExtendedConst,
    kWasmRelaxedSimd,
    kWasmTypeReflection,
    kWasmExnRef,
    kWasmTypedFuncRef,
    kWasmRefTag,
    kWasmI8,
    kWasmImportDataTag,
    // Additional features
    kTemporalObject,
    kIndexAccessor,
    kErrorStackTraceLimit,
    // Prototype element tracking
    kObjectPrototypeHasElements,
    kArrayPrototypeHasElements,
    // GC counter feature used by heap
    kForcedGC,
    // Prototype read-only override attempts
    kAttemptOverrideReadOnlyOnPrototypeStrict,
    kAttemptOverrideReadOnlyOnPrototypeSloppy,
    // Reserve space for unknown future features.
    kUseCounterFeatureCount = 256
  };

  // Invalidated protector enumeration
  enum InvalidatedProtector {
    kInvalidatedArrayBufferDetachingProtector = 0,
    kInvalidatedArrayConstructorProtector = 1,
    kInvalidatedArrayIteratorLookupChainProtector = 2,
    kInvalidatedArraySpeciesLookupChainProtector = 3,
    kInvalidatedIsConcatSpreadableLookupChainProtector = 4,
    kInvalidatedMapIteratorLookupChainProtector = 5,
    kInvalidatedMegaDOMProtector = 6,
    kInvalidatedNoElementsProtector = 7,
    kInvalidatedNoProfilingProtector = 8,
    kInvalidatedNoUndetectableObjectsProtector = 9,
    kInvalidatedNumberStringNotRegexpLikeProtector = 10,
    kInvalidatedPromiseHookProtector = 11,
    kInvalidatedPromiseResolveLookupChainProtector = 12,
    kInvalidatedPromiseSpeciesLookupChainProtector = 13,
    kInvalidatedPromiseThenLookupChainProtector = 14,
    kInvalidatedRegExpSpeciesLookupChainProtector = 15,
    kInvalidatedSetIteratorLookupChainProtector = 16,
    kInvalidatedStringIteratorLookupChainProtector = 17,
    kInvalidatedStringLengthOverflowLookupChainProtector = 18,
    kInvalidatedStringWrapperToPrimitiveProtector = 19,
    kInvalidatedTypedArrayLengthLookupChainProtector = 20,
    kInvalidatedTypedArraySpeciesLookupChainProtector = 21
  };

  // GC type enumeration
  enum GCType {
    kScavengeGC = 1 << 0,
    kMinorMarkSweepGC = 1 << 1,
    kMarkSweepCompactGC = 1 << 2,
    kIncrementalMarkingGC = 1 << 3,
    kProcessWeakCallbacksGC = 1 << 4,
    kFullGarbageCollection = kMarkSweepCompactGC | kIncrementalMarkingGC
  };

  // GC callback flags
  enum GCCallbackFlags {
    kNoGCCallbackFlags = 0,
    kGCCallbackFlagConstructRetainedObjectInfos = 1 << 1,
    kGCCallbackFlagForced = 1 << 2,
    kGCCallbackFlagSynchronousPhantomCallbackProcessing = 1 << 3,
    kGCCallbackFlagCollectAllAvailableGarbage = 1 << 4,
    kGCCallbackFlagCollectAllExternalMemory = 1 << 5,
    kGCCallbackScheduleIdleGarbageCollection = 1 << 6
  };

  // Callback types
  using UseCounterCallback = void (*)(Isolate*, UseCounterFeature);
  using ReleaseCppHeapCallback = void (*)(std::unique_ptr<CppHeap>);
  using GCCallback = void (*)(Isolate*, GCType, GCCallbackFlags);
  using GCCallbackWithData = void (*)(Isolate*, GCType, GCCallbackFlags, void*);

  // CreateParams stub with Node.js required fields
  struct CreateParams {
    CreateParams() = default;

    // Node.js required fields
    ResourceConstraints constraints;

    int embedder_wrapper_object_index = -1;
    int embedder_wrapper_type_index = -1;

    const StartupData* snapshot_blob = nullptr;

    // Additional fields for Node.js
    const intptr_t* external_references = nullptr;
    CppHeap* cpp_heap = nullptr;
    ArrayBuffer::Allocator* array_buffer_allocator = nullptr;
    std::shared_ptr<ArrayBuffer::Allocator> array_buffer_allocator_shared;
  };
  
  static Isolate* GetCurrent() { return nullptr; }
  static Isolate* TryGetCurrent() { return nullptr; }
  static Isolate* New(const CreateParams& params) { return new Isolate(); }
  
  void Enter() {}
  void Exit() {}
  void Dispose() {}
  void SetIdle(bool idle) { /* No-op for WASI */ }

  // Usage counting (no-op on WASI)
  void CountUsage(UseCounterFeature) {}
  void CountUsage(InvalidatedProtector) {}
  
  // Exception handling stubs for WASI builds
  bool HasPendingException() const { return pending_exception_; }
  void ClearPendingException() { pending_exception_ = false; }
  void ThrowError(const char* /*message*/) { pending_exception_ = true; }
  
  // Additional methods as needed
  void* GetData(uint32_t slot) { return nullptr; }
  void SetData(uint32_t slot, void* data) {}
  Local<Context> GetCurrentContext() { return Local<Context>(); }
  Local<Context> GetEnteredOrMicrotaskContext() { return Local<Context>(); }
  bool InContext() const { return false; }
  
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

  // Near-heap-limit callbacks
  using NearHeapLimitCallback = size_t (*)(void* data, size_t current_limit,
                                          size_t initial_limit);
  void AddNearHeapLimitCallback(NearHeapLimitCallback, void*, size_t initial_limit = 0) {}
  void RemoveNearHeapLimitCallback(NearHeapLimitCallback, void*) {}

  // Stack trace capture configuration
  void SetCaptureStackTraceForUncaughtExceptions(bool, int = 0) {}
  
  // Additional methods for Node.js worker support (no-op)
  // The 3-arg AddNearHeapLimitCallback above, with a default third parameter,
  // already covers calls that pass 2 arguments.
  
  void SetStackLimit(uintptr_t stack_limit) {
    // WASI stub - no-op
  }
  
  // Use the v8::MeasureMemoryDelegate from v8-statistics.h
  void MeasureMemory(std::unique_ptr<v8::MeasureMemoryDelegate> delegate,
                     v8::MeasureMemoryExecution execution = v8::MeasureMemoryExecution::kDefault) {
    // WASI stub - no-op
    // Can't call delegate methods without proper implementation
  }

  // Restore original heap limit (used by inspector for OOM breakpoints)
  void RestoreOriginalHeapLimit() {}

  // GetExternallyAllocatedMemoryInBytesCallback
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

  // Message error levels nested in Isolate for call sites like
  // v8::Isolate::kMessageError
  enum MessageErrorLevel {
    kMessageLog,
    kMessageDebug,
    kMessageInfo,
    kMessageError,
    kMessageWarning
  };

  
  // MessageErrorLevel - use the one from v8:: namespace
  
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

  // Minimal usage counter support (already defined above)
  
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

  // Microtasks suppression scope used by MicrotaskQueue; no-op RAII on WASI
  class SuppressMicrotaskExecutionScope {
   public:
    explicit SuppressMicrotaskExecutionScope(Isolate*) {}
    SuppressMicrotaskExecutionScope(Isolate*, internal::MicrotaskQueue*) {}
  };
  
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

  // Call-completed callbacks (stubs)
  using CallCompletedCallback = void (*)(Isolate* isolate);
  void AddCallCompletedCallback(CallCompletedCallback /*callback*/) {}
  void RemoveCallCompletedCallback(CallCompletedCallback /*callback*/) {}
  
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
  
  // Testing GC method
  void RequestGarbageCollectionForTesting(GCType gc_type,
                                          cppgc::StackState stack_state = cppgc::StackState::kMayContainHeapPointers) {
    // WASI stub - no-op
  }

  void LowMemoryNotification() {
    // WASI stub - no-op
  }
 
 private:
  bool pending_exception_ = false;

  // MessageErrorLevel type is provided via alias above (no redefinition)
};

// StackTracePrinter function type
using StackTracePrinter = void (*)();

// MeasureMemoryDelegate is already defined in v8-statistics.h
// Don't redefine it here

}  // namespace v8

#endif // __wasi__

#endif // WASI_V8_ISOLATE_WASI_STUB_H_
