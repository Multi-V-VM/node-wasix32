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

// Condition codes for comparisons (unified cross-platform)
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

// Foreign object tags for type identification (ExternalPointerTag)
constexpr Address kSyntheticModuleTag = 0x1;
constexpr Address kCFunctionTag = 0x2;
constexpr Address kCFunctionInfoTag = 0x3;
constexpr Address kWasmNativeModuleTag = 0x4;
constexpr Address kWasmFuncDataTag = 0x5;
constexpr Address kWasmManagedDataTag = 0x6;
constexpr Address kGenericManagedTag = 0x7;
constexpr Address kWasmImportDataTag = 0x8;
constexpr Address kMessageListenerTag = 0x9;
constexpr Address kDisplayNamesInternalTag = 0xA;
constexpr Address kWasmWasmStreamingTag = 0xB;
constexpr Address kIcuBreakIteratorTag = 0xC;
constexpr Address kIcuCollatorTag = 0xD;
constexpr Address kIcuDateFormatTag = 0xE;
constexpr Address kIcuDecimalFormatTag = 0xF;
constexpr Address kIcuListFormatterTag = 0x10;
constexpr Address kIcuLocaleTag = 0x11;
constexpr Address kIcuNumberFormatterTag = 0x12;
constexpr Address kIcuPluralRulesTag = 0x13;
constexpr Address kIcuRelativeDateTimeFormatterTag = 0x14;
constexpr Address kIcuSegmenterTag = 0x15;
constexpr Address kIcuSegmentIteratorTag = 0x16;
constexpr Address kIcuSimpleDateFormatTag = 0x17;
constexpr Address kIcuUnicodeStringTag = 0x18;

// Cpp heap pointer constants
constexpr uint32_t kAnyCppHeapPointer = 0;
constexpr uint32_t kNullCppHeapPointer = 1;
constexpr uint32_t kMaxCppHeapPointers = 0xFFFFFF;

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
    kSloppyModeBlockScopedFunctionRedefinition = 1,
    kForInInitializer = 2,
    kHtmlComment = 3,
    kHtmlCommentInExternalScript = 4,
    kSourceMappingUrlMagicCommentAtSign = 5,
    kCompileHintsMagicAll = 6,
    kSloppyMode = 7,
    kStrictMode = 8,
    kUseAsm = 9,
    kIndexAccessor = 10,
    kErrorPrepareStackTrace = 11,
    kErrorCaptureStackTrace = 12,
    kTemporalObject = 13,
    kAsyncStackTaggingCreateTaskCall = 14,
    kUseCounterFeatureCount  // Must be last
  };

  // Invalidated protector values
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
    kInvalidatedStringLengthOverflowLookupChainProtector = 18
  };

  // GC-related enums
  enum GCFlags {
    kForcedGC = 1 << 0
  };
};

}  // namespace v8

#endif  // __wasi__

#endif  // V8_WASI_API_CONSTANTS_H_
