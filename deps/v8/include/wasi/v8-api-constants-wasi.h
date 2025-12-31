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

// TracedReference storage modes are defined in v8-traced-handle.h
// Do not duplicate here to avoid redefinition errors

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

// NOTE: CodeKind enum removed - use the real definition from
// src/objects/code-kind.h to avoid redefinition errors.

// V8 builtin tier
enum class BuiltinTier {
  kInterpreter,
  kSparkplug,
  kMaglev,
  kTurbofan
};

// Memory pressure level - defined in v8:: namespace (see bottom of file)

// RAILMode for performance optimization hints
enum class RAILMode {
  kDefault,
  kResponse,
  kAnimation,
  kIdle,
  kLoad
};

// Promise hook types - defined in v8:: namespace (see bottom of file)

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

// V8 GC callback flags and GC type - defined in v8:: namespace (see bottom of file)

// Interceptor result types - only define if not already defined by v8-template.h
#ifndef INCLUDE_V8_TEMPLATE_H_
enum class Intercepted {
  kNo,
  kYes
};
#endif  // INCLUDE_V8_TEMPLATE_H_

// Property attribute - only define if not already defined by v8-object.h
#ifndef INCLUDE_V8_OBJECT_H_
enum PropertyAttribute {
  None = 0,
  ReadOnly = 1 << 0,
  DontEnum = 1 << 1,
  DontDelete = 1 << 2
};
#endif  // INCLUDE_V8_OBJECT_H_

// Access control
enum AccessControl {
  DEFAULT = 0,
  ALL_CAN_READ = 1 << 0,
  ALL_CAN_WRITE = 1 << 1,
  PROHIBITS_OVERWRITING = 1 << 2
};

// Side effects type for API functions - only define if not already defined by v8-object.h
#ifndef INCLUDE_V8_OBJECT_H_
enum class SideEffectType {
  kHasSideEffect,
  kHasNoSideEffect,
  kHasSideEffectToReceiver
};
#endif  // INCLUDE_V8_OBJECT_H_

// Constructor behavior - only define if not already defined by v8-template.h
#ifndef INCLUDE_V8_TEMPLATE_H_
enum class ConstructorBehavior {
  kThrow,
  kAllow
};
#endif  // INCLUDE_V8_TEMPLATE_H_

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

// KeyCollectionMode, IndexFilter, KeyConversionMode are defined in v8-object.h
// Do not duplicate here to avoid redefinition errors

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
// For WASI, we need unique values for each condition to avoid duplicate case errors
// in switch statements (kZero and kEqual must be different values)
#ifndef V8_CONDITION_ENUM_DEFINED
#define V8_CONDITION_ENUM_DEFINED
enum Condition : int {
  // Base conditions with unique values
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
  // Additional unique values for kZero/kNotZero to avoid duplicate case errors
  zero_cond = 16,
  not_zero_cond = 17,

  // aliases
  carry = below,
  not_carry = above_equal,
  zero = zero_cond,      // Use unique value, not equal
  not_zero = not_zero_cond,  // Use unique value, not not_equal
  sign = negative,
  not_sign = positive,

  // Unified cross-platform condition names/aliases
  kEqual = equal,
  kNotEqual = not_equal,
  kLessThan = less,
  kGreaterThan = greater,
  kLessThanEqual = less_equal,
  kGreaterThanEqual = greater_equal,
  kLessThanOrEqual = less_equal,      // Alternative naming
  kGreaterThanOrEqual = greater_equal, // Alternative naming
  kUnsignedLessThan = below,
  kUnsignedGreaterThan = above,
  kUnsignedLessThanEqual = below_equal,
  kUnsignedGreaterThanEqual = above_equal,
  kUnsignedLessThanOrEqual = below_equal,      // Alternative naming
  kUnsignedGreaterThanOrEqual = above_equal,   // Alternative naming
  kOverflow = overflow,
  kNoOverflow = no_overflow,
  kZero = zero_cond,      // Use unique value to avoid duplicate case
  kNotZero = not_zero_cond   // Use unique value to avoid duplicate case
};

// Returns the equivalent of !cc
inline Condition NegateCondition(Condition cc) {
  // For most conditions, XOR with 1 works (pairs differ by 1)
  // For zero_cond/not_zero_cond, we need special handling
  if (cc == zero_cond) return not_zero_cond;
  if (cc == not_zero_cond) return zero_cond;
  return static_cast<Condition>(cc ^ 1);
}
#endif  // V8_CONDITION_ENUM_DEFINED

// Foreign object tags for type identification (ExternalPointerTag)
// These are defined in nuclear-fix.h as ExternalPointerTag enum values
// Use those definitions instead of duplicating here

// Cpp heap pointer constants - defined in nuclear-fix.h
// Isolate class - defined in v8-isolate-wasi-stub.h

// Import v8:: namespace types into v8::internal:: namespace
// These are defined below in v8:: namespace and need to be visible here
}  // namespace internal

// Types that belong in v8:: namespace (not v8::internal::)
// These match the definitions in v8-callbacks.h, v8-promise.h, v8-isolate.h

// Memory pressure level - only define if not already defined by v8-isolate.h
#ifndef V8_V8_ISOLATE_H_
enum class MemoryPressureLevel {
  kNone,
  kModerate,
  kCritical
};
#endif  // V8_V8_ISOLATE_H_

// Promise hook types - define if v8-promise.h hasn't been included yet
// Also define guard so v8-promise.h knows not to redefine
#ifndef INCLUDE_V8_PROMISE_H_
#define V8_WASI_PROMISE_HOOK_TYPE_DEFINED
enum class PromiseHookType { kInit, kResolve, kBefore, kAfter };
#endif  // INCLUDE_V8_PROMISE_H_

// V8 GC callback flags - only define if not already defined by v8-callbacks.h
// The include guard is INCLUDE_V8_ISOLATE_CALLBACKS_H_ (not V8_CALLBACKS_H_)
#ifndef INCLUDE_V8_ISOLATE_CALLBACKS_H_
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

// V8 GC type - use regular enum (not enum class) to allow bitwise operations
enum GCType {
  kGCTypeScavenge = 1 << 0,
  kGCTypeMinorMarkSweep = 1 << 1,
  kGCTypeMarkSweepCompact = 1 << 2,
  kGCTypeIncrementalMarking = 1 << 3,
  kGCTypeProcessWeakCallbacks = 1 << 4,
  kGCTypeAll = kGCTypeScavenge | kGCTypeMinorMarkSweep |
               kGCTypeMarkSweepCompact | kGCTypeIncrementalMarking |
               kGCTypeProcessWeakCallbacks
};
#endif  // INCLUDE_V8_ISOLATE_CALLBACKS_H_

// Now import these v8:: types into v8::internal:: for internal code
namespace internal {
#ifndef V8_V8_ISOLATE_H_
using v8::MemoryPressureLevel;
#endif
#ifndef INCLUDE_V8_PROMISE_H_
using v8::PromiseHookType;
#endif
#ifndef INCLUDE_V8_ISOLATE_CALLBACKS_H_
using v8::GCCallbackFlags;
using v8::GCType;
using v8::kNoGCCallbackFlags;
using v8::kGCCallbackFlagCompacted;
using v8::kGCCallbackFlagConstructRetainedObjectInfos;
using v8::kGCCallbackFlagForced;
using v8::kGCCallbackFlagSynchronousPhantomCallbackProcessing;
using v8::kGCCallbackFlagCollectAllAvailableGarbage;
using v8::kGCCallbackFlagCollectAllExternalMemory;
using v8::kGCCallbackScheduleIdleGarbageCollection;
using v8::kGCTypeScavenge;
using v8::kGCTypeMinorMarkSweep;
using v8::kGCTypeMarkSweepCompact;
using v8::kGCTypeIncrementalMarking;
using v8::kGCTypeProcessWeakCallbacks;
using v8::kGCTypeAll;
#endif
}  // namespace internal

}  // namespace v8

#endif  // __wasi__

#endif  // V8_WASI_API_CONSTANTS_H_
