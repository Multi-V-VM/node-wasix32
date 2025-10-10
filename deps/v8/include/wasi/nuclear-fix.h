#ifndef V8_INCLUDE_WASI_NUCLEAR_FIX_H_
#define V8_INCLUDE_WASI_NUCLEAR_FIX_H_

#include <cstdint>
#include <cstddef>
#include <limits>
#include <type_traits>

// Provide global SmiTagging template and API size fallback expected by Torque
#ifndef kApiInt32Size
constexpr int kApiInt32Size = sizeof(int32_t);
#endif

template <int Size>
struct SmiTagging {
  static constexpr int kSmiShiftSize = 0;
};

// Mark that nuclear-fix provides SMI helpers so project stubs can avoid redefs
#ifndef V8_WASI_NUCLEAR_PROVIDES_SMI
#define V8_WASI_NUCLEAR_PROVIDES_SMI 1
#endif

// Forward declarations before namespace definitions
namespace v8 {
  class Isolate;
  class CustomSpaceStatisticsReceiver;
}

namespace v8 {
// Forward declare types referenced by stubs
namespace internal {
struct CppHeapPointerTagRange;

// Forward declaration of internal::Isolate
class Isolate;

// Additional constants
#ifndef V8_HAS_ADDRESS_TYPE
#define V8_HAS_ADDRESS_TYPE
using Address = uintptr_t;
#endif

// kNullAddress constant
#ifndef V8_NULL_ADDRESS_DEFINED
#define V8_NULL_ADDRESS_DEFINED
constexpr Address kNullAddress = 0;
#endif

// External pointer tag definitions with guards
#ifndef V8_EXTERNAL_POINTER_TAGS_DEFINED
#define V8_EXTERNAL_POINTER_TAGS_DEFINED

enum ExternalPointerTag : uint64_t {
  kExternalPointerNullTag = 0,
  kExternalPointerFreeEntryTag = 1,
  // Used by external-pointer-table evacuation stubs
  kExternalPointerEvacuationEntryTag = 2,
  kAnyExternalPointerTag = ~0ULL,
  // Common tags referenced across the codebase
  kExternalStringResourceTag = 0x0001000000000000ULL,
  kExternalStringResourceDataTag = 0x0002000000000000ULL,
  kForeignForeignAddressTag = 0x0003000000000000ULL,
  kNativeContextMicrotaskQueueTag = 0x0004000000000000ULL,
  kEmbedderDataSlotPayloadTag = 0x0005000000000000ULL,
  kCodeEntryPointTag = 0x0006000000000000ULL,
  kExternalObjectValueTag = 0x0007000000000000ULL,
  kCallHandlerInfoCallbackTag = 0x0008000000000000ULL,
  kAccessorInfoGetterTag = 0x0009000000000000ULL,
  kAccessorInfoSetterTag = 0x000a000000000000ULL,
  kWasmInternalFunctionCallTargetTag = 0x000b000000000000ULL,
  kWasmTypeInfoNativeTypeTag = 0x000c000000000000ULL,
  kWasmExportedFunctionDataSignatureTag = 0x000d000000000000ULL,
  kWasmContinuationJmpbufTag = 0x000e000000000000ULL,
  kWasmStackMemoryTag = 0x000f000000000000ULL,
  kWasmIndirectFunctionTargetTag = 0x0010000000000000ULL,
  kArrayBufferExtensionTag = 0x0011000000000000ULL,
  kFunctionTemplateInfoCallbackTag = 0x0012000000000000ULL,
  kWasmWrappedObjectTag = 0x0013000000000000ULL,
  kWasmWasmJSFunctionDataSignatureTag = 0x0014000000000000ULL,
  kManagedObjectTag = 0x0015000000000000ULL,
  kWasmImportDataTag = 0x0016000000000000ULL,
  kFunctionDataFunctionTag = 0x0017000000000000ULL,
  kInterpreterDataBytecodeArrayTag = 0x0018000000000000ULL,
  kFunctionTemplateInfoJsCallbackTag = 0x0019000000000000ULL,
  kTrustedPointerTag = 0x001a000000000000ULL,
  kWasmOnFulfilledDataTag = 0x001b000000000000ULL,
  kWasmRefTag = 0x001c000000000000ULL,
  // Used by Atomics waiters list (slots.h/js-atomics-synchronization)
  kWaiterQueueNodeTag = 0x001d000000000000ULL,
  kWasmWasmStreamingTag = 0x0020000000000000ULL,
  kLastExternalPointerTag = kWasmRefTag,
};

#endif // V8_EXTERNAL_POINTER_TAGS_DEFINED

// Trusted pointer tags
#ifndef V8_TRUSTED_POINTER_TAGS_DEFINED
#define V8_TRUSTED_POINTER_TAGS_DEFINED

enum TrustedPointerTag : uint64_t {
  kTrustedPointerNullTag = 0,
  kUnknownTrustedPointerTag = 1,
  kCodeTrustedPointerTag = 2,
  kBytecodeArrayTrustedPointerTag = 3,
  kInterpreterDataTrustedPointerTag = 4,
  kBaselineBytecodeArrayTrustedPointerTag = 5,
  kAttachedCodeDataContainerTrustedPointerTag = 6,
  kNativeContextTrustedPointerTag = 7,
  kProtectedInstructionStreamTrustedPointerTag = 8,
  kProtectedFixedArrayTrustedPointerTag = 9,
  kLastTrustedPointerTag = kProtectedFixedArrayTrustedPointerTag,
};

#endif // V8_TRUSTED_POINTER_TAGS_DEFINED

// Code pointer tags
#ifndef V8_CODE_POINTER_TAGS_DEFINED
#define V8_CODE_POINTER_TAGS_DEFINED

enum CodePointerTag : uint32_t {
  kCodePointerNullTag = 0,
  kBuiltinCodePointerTag = 1,
  kBytecodeHandlerCodePointerTag = 2,
  kJSFunctionCodePointerTag = 3,
  kRegExpCodePointerTag = 4,
  kWasmFunctionCodePointerTag = 5,
  kWasmToCFunctionCodePointerTag = 6,
  kWasmToJSWrapperCodePointerTag = 7,
  kWasmToCapiFunctionCodePointerTag = 8,
  kJSToWasmWrapperCodePointerTag = 9,
  kCEntryCodePointerTag = 10,
  kContinuationJSToWasmCodePointerTag = 11,
  kGenericJSToWasmWrapperCodePointerTag = 12,
  kWasmToOnRejectedCodePointerTag = 13,
  kUnknownCodePointerTag = 14,
  kLastCodePointerTag = kUnknownCodePointerTag,
};

#endif // V8_CODE_POINTER_TAGS_DEFINED

// Type aliases with guards
#ifndef V8_POINTER_HANDLE_TYPES_DEFINED
#define V8_POINTER_HANDLE_TYPES_DEFINED

using ExternalPointerHandle = uint32_t;
using TrustedPointerHandle = uint32_t;
using CodePointerHandle = uint32_t;

#endif // V8_POINTER_HANDLE_TYPES_DEFINED

// Provide external/code pointer table constants for WASI.
// These are marked inline constexpr to allow multiple ODR definitions safely
// across translation units if some .cc files also define them.
inline constexpr size_t kExternalPointerTableReservationSize = 1024 * 1024;  // 1MB
inline constexpr uint32_t kMaxExternalPointers = 65536;
inline constexpr uint32_t kMaxCapacity = 65536;
inline constexpr size_t kExternalPointerTableEntrySize = 8;
inline constexpr ExternalPointerHandle kNullExternalPointerHandle = 0;

inline constexpr size_t kTrustedPointerTableReservationSize = 1024 * 1024;  // 1MB
inline constexpr size_t kCodePointerTableEntrySize = 8;
inline constexpr uint32_t kMaxCodePointers = 65536;
// Trusted pointers capacity sanity check used in table headers
inline constexpr uint32_t kMaxTrustedPointers = 65536;

// Should throw on error function - now with proper forward declaration
inline bool ShouldThrowOnError(v8::internal::Isolate* isolate) {
  return false;  // WASI stub - never throw
}

// WASI-specific smi operations
constexpr int kSmiTagSize = 1;
constexpr int kSmiShiftSize = 0;
constexpr int kSmiValueSize = 31;
#ifndef V8_WASI_SMI_CONSTANTS_DEFINED
#define V8_WASI_SMI_CONSTANTS_DEFINED 1
#endif
#ifndef V8_WASI_SMI_TAG_MASK_DEFINED
#define V8_WASI_SMI_TAG_MASK_DEFINED
constexpr intptr_t kSmiTagMask = (1 << kSmiTagSize) - 1;
#endif

// SmiValue is also provided by project stubs; keep one definition to avoid
// duplicate symbol errors across TUs.
inline int SmiValue(Address value) {
  return static_cast<int32_t>(static_cast<intptr_t>(value)) >> kSmiTagSize;
}

constexpr bool SmiValuesAre32Bits() { return sizeof(void*) == 8; }
constexpr bool SmiValuesAre31Bits() { return !SmiValuesAre32Bits(); }

// Pointer compression flag for WASI
constexpr bool kAllowHandleUnwrapping = false;

// Additional required stub functions
inline bool PointerCompressionIsEnabled() { return false; }
inline bool SandboxIsEnabled() { return false; }

// Function mode stubs
inline bool IsFunctionModeWithPrototypeAllowed(uint32_t) { return false; }
inline bool IsFunctionModeWithWritablePrototype(uint32_t) { return false; }
inline bool IsFunctionModeWithReadonlyPrototype(uint32_t) { return false; }
inline bool IsFunctionModeWithoutPrototype(uint32_t) { return false; }
inline bool IsValidFunctionKind(uint32_t) { return true; }
inline bool IsGeneratorFunction(uint32_t) { return false; }
inline bool IsAsyncFunction(uint32_t) { return false; }
inline bool IsAsyncGeneratorFunction(uint32_t) { return false; }
inline bool IsConciseMethod(uint32_t) { return false; }
inline bool IsStrictFunctionWithoutPrototype(uint32_t) { return false; }
inline bool IsGetterFunction(uint32_t) { return false; }
inline bool IsSetterFunction(uint32_t) { return false; }
inline bool IsArrowFunction(uint32_t) { return false; }
inline bool IsModule(uint32_t) { return false; }
inline bool IsClassConstructor(uint32_t) { return false; }
inline bool IsDefaultConstructor(uint32_t) { return false; }
inline bool IsBaseConstructor(uint32_t) { return false; }
inline bool IsDerivedConstructor(uint32_t) { return false; }
inline bool IsNormalFunction(uint32_t) { return true; }
inline bool IsWrappedFunction(uint32_t) { return false; }
inline bool IsStrictFunction(uint32_t) { return false; }
inline bool IsConstructor(uint32_t) { return false; }

// Additional internal namespace functions
inline ::v8::Isolate* TryGetCurrent() { return nullptr; }
inline void IncrementLongTasksStatsCounter(::v8::Isolate* isolate) {}

// Add ReadExternalPointerField stub
template <bool check_statically_enabled>
inline Address ReadExternalPointerField(Address field_address,
                                       const void* cage_base,
                                       ExternalPointerTag tag) {
  return 0;
}

// Slot and embedder field operations
inline void InternalWriteRawField(Address object, int offset, Address value) {}
inline Address InternalReadRawField(Address object, int offset) { return 0; }

// Tag/untag operations
template<typename T>
inline T* InternalTagged(T* value) { return value; }

template<typename T>  
inline T* InternalUntagged(T* value) { return value; }

// Object slot operations
inline void InternalWriteSlot(Address object, int offset, Address value) {}
inline Address InternalReadSlot(Address object, int offset) { return 0; }

// ---------------------------------------------------------------------------
// Minimal Internals stub for WASI to satisfy public headers usage
// Only define if not already provided elsewhere.
#ifndef V8_INTERNALS_CLASS_DEFINED
#define V8_INTERNALS_CLASS_DEFINED
class Internals {
 public:
  // Commonly referenced layout constants in public API headers
  static constexpr int kNodeClassIdOffset = 4;
  static constexpr int kNodeStateIsWeakValue = 1;
  static constexpr int kEmptyStringRootIndex = 0;
  static constexpr int kUndefinedValueRootIndex = 1;
  static constexpr int kNullValueRootIndex = 2;
  static constexpr int kTrueValueRootIndex = 3;
  static constexpr int kFalseValueRootIndex = 4;
  static constexpr int kTheHoleValueRootIndex = 5;
  static constexpr int kStringResourceOffset = 8;
  // String representation and encoding masks/tags used by public API helpers
  static constexpr int kStringRepresentationAndEncodingMask = 0x0f;
  static constexpr int kStringEncodingMask = 0x08;
  static constexpr int kExternalOneByteRepresentationTag = 0x04;
  static constexpr int kExternalTwoByteRepresentationTag = 0x02;
  static constexpr uint64_t kExternalStringResourceTag = 0x0002000000000000ULL;
  static constexpr uint64_t kEmbedderDataSlotPayloadTag = 0x0005000000000000ULL;
  static constexpr int kJSAPIObjectWithEmbedderSlotsHeaderSize = 16;
  static constexpr int kEmbedderDataSlotSize = 8;
  static constexpr int kEmbedderDataSlotExternalPointerOffset = 0;
  static constexpr int kJSObjectHeaderSize = 12;
  // Offset differs on 32-bit vs 64-bit configurations. On 32-bit (WASI),
  // contexts.h expects 24; on 64-bit it is 32.
  static constexpr int kNativeContextEmbedderDataOffset =
      (sizeof(void*) == 4 ? 24 : 32);
  static constexpr int kEmbedderDataArrayHeaderSize = 8;
  // Number of embedder data slots reserved in IsolateData
  static constexpr int kNumIsolateDataSlots = 4;

  // No-op initialization checks for WASI stubs
  static void CheckInitialized(void*) {}
  template <typename T>
  static void CheckInitialized(T*) {}

  // Root slot access — return address of a static dummy
  template <typename S = Address>
  static S* GetRootSlot(void*, int) {
    static Address dummy = 0;
    return reinterpret_cast<S*>(&dummy);
  }

  // Minimal helpers referenced from v8-primitive.h
  static bool IsExternalTwoByteString(int) { return false; }
  static int GetInstanceType(Address) { return 0; }
  static ::v8::Isolate* GetIsolateForSandbox(Address) { return nullptr; }

  // External pointer field read used by some API accessors
  static Address ReadExternalPointerField(void*, Address /*obj*/, int /*offset*/, uint64_t /*tag*/) {
    return 0;
  }
  template <uint64_t tag>
  static Address ReadExternalPointerField(void* isolate, Address obj, int offset) {
    return ReadExternalPointerField(isolate, obj, offset, tag);
  }

  // Node state helpers for persistent handles
  static int GetNodeState(void*) { return 0; }

  // Instance type predicate used by v8-object.h
  static bool CanHaveInternalField(int) { return true; }

  // Raw field utilities used by some accessors
  template <typename T>
  static T ReadRawField(Address obj, int offset) {
    return *reinterpret_cast<T*>(obj + offset);
  }
  static Address DecompressTaggedField(Address /*obj*/, uint32_t value) {
    return static_cast<Address>(value);
  }
  static uint32_t CompressTagged(Address value) {
    return static_cast<uint32_t>(value);
  }

  // Tagged field helpers
  static Address ReadTaggedPointerField(Address obj, int offset) {
    return *reinterpret_cast<Address*>(obj + offset);
  }
  static bool HasHeapObjectTag(Address value) {
    return (static_cast<intptr_t>(value) & 1) == 1;  // minimal check
  }

  // SMI helpers
  static constexpr bool IsValidSmi(intptr_t value) {
    // kSmiTagSize = 1 -> 31-bit Smi range
    const intptr_t smi_limit = static_cast<intptr_t>(1) << 30;
    return value >= -smi_limit && value <= smi_limit - 1;
  }
  static constexpr Address IntegralToSmi(int value) {
    return (static_cast<Address>(value) << 1) | 0;
  }
  static Address* TryIntegralToSmi(intptr_t value) {
    if (IsValidSmi(value)) {
      static thread_local Address smi = IntegralToSmi(static_cast<int>(value));
      return &smi;
    }
    return nullptr;
  }
  static int SmiValue(Address value) {
    return static_cast<int32_t>(static_cast<intptr_t>(value)) >> kSmiTagSize;
  }

  // Root retrieval for basic values
  static Address GetRoot(v8::Isolate*, int index) {
    switch (index) {
      case kUndefinedValueRootIndex: return 0x1000;
      case kNullValueRootIndex: return 0x1004;
      case kTrueValueRootIndex: return 0x1008;
      case kFalseValueRootIndex: return 0x100C;
      case kTheHoleValueRootIndex: return 0x1014;
      case kEmptyStringRootIndex: return 0x1010;
      default: return 0;
    }
  }

  // Throw mode constants
  static constexpr int kInferShouldThrowMode = -1;
  static constexpr int kDontThrow = 0;
  static constexpr int kThrowOnError = 1;

  // Metrics hook used by v8-metrics.h (no-op on WASI)
  static void IncrementLongTasksStatsCounter(::v8::Isolate*) {}
};
#endif  // V8_INTERNALS_CLASS_DEFINED

// ---------------------------------------------------------------------------
// Core type and size definitions required by src/common/globals.h (WASI)
#ifndef V8_WASI_CORE_SIZES_DEFINED
#define V8_WASI_CORE_SIZES_DEFINED
// Fundamental size constants
constexpr int kSystemPointerSize = sizeof(void*);
#ifndef V8_INTERNAL_KSYSTEMPOINTERSIZE_DEFINED
#define V8_INTERNAL_KSYSTEMPOINTERSIZE_DEFINED 1
#endif
constexpr int kSystemPointerSizeLog2 = (sizeof(void*) == 4 ? 2 : 3);
constexpr int kTaggedSize = 4;  // 32-bit tagged pointers on WASI builds
constexpr int kTaggedSizeLog2 = 2;
// Integer sizes are defined in src/common/globals.h; don't redefine here.

// Storage types for tagged values
using Tagged_t = uintptr_t;
using AtomicTagged_t = uintptr_t;

// External pointer storage types on the heap
using ExternalPointer_t = uint32_t;
using CppHeapPointer_t = uint32_t;
using IndirectPointerHandle = uint32_t;
#ifndef V8_CPPHEAP_POINTER_HANDLE_TYPE_DEFINED
#define V8_CPPHEAP_POINTER_HANDLE_TYPE_DEFINED
using CppHeapPointerHandle = uint32_t;
#endif
#endif  // V8_WASI_CORE_SIZES_DEFINED

// JS dispatch handle shift used in globals.h; for WASI stubs, a value of 0
// ensures expressions like (0xffffffff << kJSDispatchHandleShift) compile.
#ifndef kJSDispatchHandleShift
constexpr int kJSDispatchHandleShift = 0;
#endif

// Define deprecation macro used in some headers if missing
#ifndef V8_DEPRECATE_SOON
#define V8_DEPRECATE_SOON(message)
#endif

// BackingStore base stub used by v8-array-buffer.h
class BackingStoreBase {
 public:
  virtual ~BackingStoreBase() = default;
};

}  // namespace internal
}  // namespace v8

#ifndef V8_BASE_BITS_WASI_HELPERS_DEFINED
#define V8_BASE_BITS_WASI_HELPERS_DEFINED
// Provide minimal implementations expected by src/base/bits.h when building
// for WASI, placed in the v8::base::bits namespace.
namespace v8 {
namespace base {
namespace bits {

inline int CountTrailingZerosNonZero(uint32_t value) {
  // Caller guarantees value != 0
  return __builtin_ctz(value);
}

inline int CountTrailingZerosNonZero(uint64_t value) {
  // Caller guarantees value != 0
  return __builtin_ctzll(value);
}

#if __SIZEOF_SIZE_T__ != __SIZEOF_LONG_LONG__
// Provide a size_t overload only when it is not the same type as uint64_t.
inline int CountTrailingZerosNonZero(size_t value) {
  // Caller guarantees value != 0
  if constexpr (sizeof(size_t) == sizeof(uint32_t)) {
    return __builtin_ctz(static_cast<uint32_t>(value));
  } else {
    return __builtin_ctzll(static_cast<uint64_t>(value));
  }
}
#endif

// (IsPowerOfTwo is provided in src/base/bits.h for WASI)

}  // namespace bits
}  // namespace base
}  // namespace v8
#endif  // V8_BASE_BITS_WASI_HELPERS_DEFINED

#ifndef V8_WASI_BIT_CAST_DEFINED
#define V8_WASI_BIT_CAST_DEFINED
// Provide a global bit_cast implementation usable from legacy call sites.
template <typename To, typename From>
inline To bit_cast(const From& from) {
  static_assert(sizeof(To) == sizeof(From), "bit_cast requires equal sizes");
#if __has_builtin(__builtin_bit_cast)
  return __builtin_bit_cast(To, from);
#else
  To to;
  std::memcpy(&to, &from, sizeof(To));
  return to;
#endif
}
#endif  // V8_WASI_BIT_CAST_DEFINED

// Add selected internal free-function stubs referenced by public headers
namespace v8 {
namespace internal {
inline void* IsolateFromNeverReadOnlySpaceObject(Address /*obj*/) { return nullptr; }

template <typename T>
inline T* ReadCppHeapPointerField(void* /*isolate*/, Address obj, int offset,
                                  const CppHeapPointerTagRange& /*tag_range*/) {
  return reinterpret_cast<T*>(*reinterpret_cast<Address*>(obj + offset));
}

// Safe buffer size for typed arrays when sandbox is enabled. Use full range in WASI.
constexpr size_t kMaxSafeBufferSizeForSandbox = static_cast<size_t>(-1);

// External pointer tag range used by external-pointer-table APIs
struct ExternalPointerTagRange {
  ExternalPointerTag start;
  ExternalPointerTag end;
  constexpr ExternalPointerTagRange(ExternalPointerTag s, ExternalPointerTag e)
      : start(s), end(e) {}
  // Single-tag constructor used by callers passing a tag directly
  constexpr explicit ExternalPointerTagRange(ExternalPointerTag tag)
      : start(tag), end(tag) {}
  // For WASI stubs, treat all tags as acceptable
  constexpr bool Contains(ExternalPointerTag) const { return true; }
  // Default constructor and helpers used by slots.h
  constexpr ExternalPointerTagRange()
      : start(kExternalPointerNullTag), end(kExternalPointerNullTag) {}
  constexpr int Size() const { return (start == end) ? 1 : 2; }
  constexpr ExternalPointerTag first() const { return start; }
};

// Equality helpers used by slots and table utilities
constexpr bool operator==(const ExternalPointerTagRange& range,
                          ExternalPointerTag tag) {
  return range.start == tag && range.end == tag;
}
constexpr bool operator==(ExternalPointerTag tag,
                          const ExternalPointerTagRange& range) {
  return range == tag;
}
constexpr bool operator!=(const ExternalPointerTagRange& range,
                          ExternalPointerTag tag) {
  return !(range == tag);
}
constexpr bool operator!=(ExternalPointerTag tag,
                          const ExternalPointerTagRange& range) {
  return !(range == tag);
}

inline bool ExternalPointerCanBeEmpty(ExternalPointerTagRange) { return true; }

// External pointer tagging masks and mark bit. On WASI, no tagging is applied,
// so choose no-op values to keep operations safe.
constexpr Address kExternalPointerTagMask = static_cast<Address>(0);
constexpr Address kExternalPointerPayloadMask =
    ~static_cast<Address>(0);
constexpr Address kExternalPointerMarkBit = static_cast<Address>(0);

// Tag shift used for external pointer tagging
#ifndef V8_EXTERNAL_POINTER_TAG_SHIFT
#define V8_EXTERNAL_POINTER_TAG_SHIFT
// Use an int for shift counts to match other WASI stubs
constexpr int kExternalPointerTagShift = 48;
#endif

// PerformCastCheck stub
template <typename T>
inline void PerformCastCheck(T) {}

// CppHeap pointer table constants and helpers for WASI
#ifndef V8_CPPHEAP_POINTER_TABLE_SIZE
#define V8_CPPHEAP_POINTER_TABLE_SIZE
constexpr size_t kCppHeapPointerTableReservationSize = 1024 * 1024;  // 1MB
#endif

#ifndef V8_MAX_CPPHEAP_POINTERS
#define V8_MAX_CPPHEAP_POINTERS
constexpr size_t kMaxCppHeapPointers = 65536;
#endif

#ifndef V8_CPPHEAP_POINTER_SHIFTS_DEFINED
#define V8_CPPHEAP_POINTER_SHIFTS_DEFINED
// Keep shifts small to avoid 32-bit overflow/UB
constexpr int kCppHeapPointerIndexShift = 0;
constexpr int kCppHeapPointerPayloadShift = 1;
constexpr int kCppHeapPointerTagShift = 1;
#endif

#ifndef V8_NULL_CPPHEAP_POINTER_HANDLE_DEFINED
#define V8_NULL_CPPHEAP_POINTER_HANDLE_DEFINED
constexpr CppHeapPointerHandle kNullCppHeapPointerHandle = 0;
#endif

#ifndef V8_CPPHEAP_POINTER_MARKBIT_DEFINED
#define V8_CPPHEAP_POINTER_MARKBIT_DEFINED
constexpr Address kCppHeapPointerMarkBit = static_cast<Address>(0);
#endif
}  // namespace internal
}  // namespace v8

#endif  // V8_INCLUDE_WASI_NUCLEAR_FIX_H_
