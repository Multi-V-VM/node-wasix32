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
  kWasmImportedStringsUtf8,
  kWasmJSPI,
  kWasmTypeReflection,
  kWasmModuleCompilation,
  kWasmStreamingCompilation,
  kWasmBulkMemory,
  kWasmNonTrappingConversions,
  kWasmSignExtension,
  kWasmReferenceTypes,
  kWasmTailCall,
  kWasmReturnCall,
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
  kWasmExnRef,
  kWasmTypedFuncRef,
  kWasmJavaScriptPromiseIntegration,
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

// V8 code kind enumeration - only define if the real one from code-kind.h
// hasn't been included yet
#ifndef V8_OBJECTS_CODE_KIND_H_
#ifndef V8_WASI_CODEKIND_STUB_DEFINED
#define V8_WASI_CODEKIND_STUB_DEFINED
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
#endif  // V8_WASI_CODEKIND_STUB_DEFINED
#endif  // V8_OBJECTS_CODE_KIND_H_

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

// Condition codes for comparisons (unified cross-platform)
// Defined in wasi-v8-essential-constants.h - don't redefine here
#ifndef V8_CONDITION_ENUM_DEFINED
#define V8_CONDITION_ENUM_DEFINED
enum Condition : int {
  overflow = 0,
  no_overflow = 1,
  below = 2,
  above_equal = 3,
  equal = 4,
  not_equal = 5,
  below_equal = 6,
  above = 7,
  negative = 8,
  positive = 9,
  parity_even = 10,
  parity_odd = 11,
  less = 12,
  greater_equal = 13,
  less_equal = 14,
  greater = 15,

  // aliases
  carry = below,
  not_carry = above_equal,
  zero = equal,
  not_zero = not_equal,
  sign = negative,
  not_sign = positive,

  // Unified cross-platform condition names/aliases
  kEqual = equal,
  kNotEqual = not_equal,
  kLessThan = less,
  kGreaterThan = greater,
  kLessThanEqual = less_equal,
  kGreaterThanEqual = greater_equal,
  kUnsignedLessThan = below,
  kUnsignedGreaterThan = above,
  kUnsignedLessThanEqual = below_equal,
  kUnsignedGreaterThanEqual = above_equal,
  kOverflow = overflow,
  kNoOverflow = no_overflow,
  kZero = equal,
  kNotZero = not_equal
};

// Returns the equivalent of !cc
inline Condition NegateCondition(Condition cc) {
  return static_cast<Condition>(cc ^ 1);
}
#endif  // V8_CONDITION_ENUM_DEFINED

// Foreign object tags for type identification (ExternalPointerTag)
// These are defined in nuclear-fix.h as ExternalPointerTag enum values
// Use those definitions instead of duplicating here

// Cpp heap pointer constants - defined in nuclear-fix.h
// Isolate class - defined in v8-isolate-wasi-stub.h

}  // namespace internal

}  // namespace v8

#endif  // __wasi__

#endif  // V8_WASI_API_CONSTANTS_H_
