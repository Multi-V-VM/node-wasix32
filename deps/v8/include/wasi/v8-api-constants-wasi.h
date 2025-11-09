#ifndef V8_WASI_API_CONSTANTS_H_
#define V8_WASI_API_CONSTANTS_H_

#ifdef __wasi__

#include <cstdint>

namespace v8 {

// Forward declarations
class Isolate;

namespace internal {

// Isolate UseCounterFeature enum - WebAssembly and other feature counters
enum class UseCounterFeature {
  kWasmRefTypes,
  kWasmGC,
  kWasmSimdOpcodes,
  kWasmThreadOpcodes,
  kWasmExceptionHandling,
  kWasmMultiValue,
  kWasmMultiMemory,
  kWasmMemory64,
  kWasmRelaxedSimd,
  kWasmExtendedConst,
  kWasmGCExternRefStrings,
  kWasmImportedStrings,
  kWasmJSPI,
  kWasmTypeReflection,
  kWasmModuleCompilation,
  kWasmStreamingCompilation,
  kWasmBulkMemory,
  kWasmNonTrappingConversions,
  kWasmSignExtension,
  kWasmReferenceTypes,
  kWasmTailCall,
  kWasmMemory32,
  kWasmCustomSections,
  kWasmNameSection,
  kWasmProducersSection,
  kWasmSourceMappingURL,
  kWasmExternalRefAPI,
  kWasmInternalMemoryAPI,
  kWasmGlobalAPI,
  kWasmModuleAPI,
  kWasmInstanceAPI,
  kWasmMemoryAPI,
  kWasmTableAPI,
  kWasmExceptionAPI,
  kWasmTagAPI,
  kWasmCompilationHints,
  kWasmBranchHinting,
  kWasmDynamicTiering,
  kWasmLazyCompilation,
  kWasmCodeCaching,
  kWasmDeserialize,
  kWasmTierUp,
  kWasmInlining,
  kWasmSpeculativeInlining,
  kWasmLiftoff,
  kWasmTurbofan,
  kWasmTurboshaft,
  kMaxValue  // Must be last
};

// Isolate protector invalidation reasons
enum class InvalidatedArrayBufferDetachingProtector {
  kInvalidated = 0
};

// HeapProfiler constants
constexpr uint32_t kUnknownObjectId = 0;

// HeapProfiler sampling flags
enum SamplingFlags {
  kSamplingIncludeObjectsCollectedByMajorGC = 1 << 0,
  kSamplingIncludeObjectsCollectedByMinorGC = 1 << 1,
  kSamplingForceGC = 1 << 2
};

// TracedReference storage modes - used to track how traced handles are stored
enum class TracedReferenceStoreMode {
  kInitializingStore,  // Initial store, skip write barrier
  kAssigningStore      // Assignment store, needs write barrier
};

// TracedReference handling modes - controls GC behavior
enum class TracedReferenceHandling {
  kDefault,   // Normal traced reference
  kDroppable  // Can be dropped by GC if object is unreachable
};

// CppHeap allocation modes
enum class AllocationMode {
  kNormal,
  kAtomic
};

// Embedder heap tracer flags
enum class EmbedderHeapTracerFlags {
  kNone = 0,
  kForced = 1 << 0,
  kReduceMemory = 1 << 1,
  kCollectAllGarbage = 1 << 2
};

// V8 compilation cache modes
enum class CompilationCacheMode {
  kNormal,
  kBypassCache,
  kSkipCache
};

// V8 compilation hints
enum class CompilationHintTier {
  kDefault,
  kBaseline,
  kOptimized
};

// V8 code kind enumeration
enum class CodeKind {
  kOptimizedFunction,
  kBytecodeHandler,
  kForTesting,
  kBuiltin,
  kRegExp,
  kWasmFunction,
  kWasmToJsFunction,
  kJsToWasmFunction,
  kWasmInterpreterEntry,
  kC2WasmFunction,
  kCWasmEntry
};

// V8 builtin tier
enum class BuiltinTier {
  kInterpreter,
  kSparkplug,
  kMaglev,
  kTurbofan
};

// Memory pressure level
enum class MemoryPressureLevel {
  kNone,
  kModerate,
  kCritical
};

// RAILMode for performance optimization hints
enum class RAILMode {
  kDefault,
  kResponse,
  kAnimation,
  kIdle,
  kLoad
};

// Promise hook types
enum class PromiseHookType {
  kInit,
  kResolve,
  kBefore,
  kAfter
};

// Import attributes (formerly import assertions)
enum class ImportAttributesType {
  kJSON,
  kWasm,
  kCSS,
  kUnknown
};

// Module status
enum class ModuleStatus {
  kUninstantiated,
  kInstantiating,
  kInstantiated,
  kEvaluating,
  kEvaluated,
  kErrored
};

// Script type
enum class ScriptType {
  kClassic,
  kModule
};

// Script compilation type
enum class ScriptCompilationType {
  kEager,
  kLazy
};

// Array buffer allocation mode
enum class ArrayBufferAllocationMode {
  kNormal,
  kReservation
};

// Backing store initialization mode
enum class BackingStoreInitializationMode {
  kZeroInitialize,
  kUninitialized
};

// Shared array buffer constructability
enum class SharedArrayBufferConstructionEnabledCallback {
  kEnabled,
  kDisabled
};

// V8 GC callback flags
enum GCCallbackFlags {
  kNoGCCallbackFlags = 0,
  kGCCallbackFlagCompacted = 1 << 0,
  kGCCallbackFlagConstructRetainedObjectInfos = 1 << 1,
  kGCCallbackFlagForced = 1 << 2,
  kGCCallbackFlagSynchronousPhantomCallbackProcessing = 1 << 3,
  kGCCallbackFlagCollectAllAvailableGarbage = 1 << 4,
  kGCCallbackFlagCollectAllExternalMemory = 1 << 5,
  kGCCallbackScheduleIdleGarbageCollection = 1 << 6
};

// V8 GC type
enum class GCType {
  kGCTypeScavenge = 1 << 0,
  kGCTypeMarkSweepCompact = 1 << 1,
  kGCTypeIncrementalMarking = 1 << 2,
  kGCTypeProcessWeakCallbacks = 1 << 3,
  kGCTypeAll = kGCTypeScavenge | kGCTypeMarkSweepCompact |
               kGCTypeIncrementalMarking | kGCTypeProcessWeakCallbacks
};

// Interceptor result types
enum class Intercepted {
  kNo,
  kYes
};

// Property attribute
enum PropertyAttribute {
  None = 0,
  ReadOnly = 1 << 0,
  DontEnum = 1 << 1,
  DontDelete = 1 << 2
};

// Access control
enum AccessControl {
  DEFAULT = 0,
  ALL_CAN_READ = 1 << 0,
  ALL_CAN_WRITE = 1 << 1,
  PROHIBITS_OVERWRITING = 1 << 2
};

// Side effects type for API functions
enum class SideEffectType {
  kHasSideEffect,
  kHasNoSideEffect,
  kHasSideEffectToReceiver
};

// Constructor behavior
enum class ConstructorBehavior {
  kThrow,
  kAllow
};

// String resource types
enum class StringResourceType {
  kOneByte,
  kTwoByte
};

// Integrity level
enum class IntegrityLevel {
  kSealed,
  kFrozen
};

// Key collection mode
enum class KeyCollectionMode {
  kOwnOnly,
  kIncludePrototypes
};

// Index filter
enum class IndexFilter {
  kIncludeIndices,
  kSkipIndices
};

// Key conversion mode
enum class KeyConversionMode {
  kConvertToString,
  kKeepNumbers,
  kNoNumbers
};

// Promise reject event
enum class PromiseRejectEvent {
  kPromiseRejectWithNoHandler,
  kPromiseHandlerAddedAfterReject,
  kPromiseRejectAfterResolved,
  kPromiseResolveAfterResolved
};

// Microtask queue priority
enum class MicrotaskQueuePriority {
  kDefault,
  kHigh
};

// WebAssembly compilation mode
enum class WasmCompilationMode {
  kSynchronous,
  kAsync,
  kAsyncStreaming
};

// WebAssembly compilation tier
enum class WasmTier {
  kLiftoff,
  kTurbofan,
  kTurboshaft
};

// WebAssembly feature flags
struct WasmFeatures {
  bool has_gc = false;
  bool has_typed_funcref = false;
  bool has_memory64 = false;
  bool has_multi_memory = false;
  bool has_relaxed_simd = false;
  bool has_extended_const = false;
  bool has_imported_strings = false;
  bool has_jspi = false;
  bool has_type_reflection = false;
  bool has_exnref = false;
};

}  // namespace internal

// Public Isolate constants
class Isolate {
 public:
  // Use counter feature enumeration
  using UseCounterFeature = internal::UseCounterFeature;

  // Message error level
  enum MessageErrorLevel {
    kMessageLog = 1 << 0,
    kMessageDebug = 1 << 1,
    kMessageInfo = 1 << 2,
    kMessageError = 1 << 3,
    kMessageWarning = 1 << 4,
    kMessageAll = kMessageLog | kMessageDebug | kMessageInfo |
                  kMessageError | kMessageWarning
  };

  // Use counter features for tracking API/feature usage
  enum UseCounterFeatureEnum {
    kVarRedeclaredCatchBinding = 0,
    kUseCounterFeatureCount  // Must be last
  };
};

}  // namespace v8

#endif  // __wasi__

#endif  // V8_WASI_API_CONSTANTS_H_
