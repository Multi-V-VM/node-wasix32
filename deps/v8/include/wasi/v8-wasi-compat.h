#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_INCLUDE_WASI_V8_WASI_COMPAT_H_
#define V8_INCLUDE_WASI_V8_WASI_COMPAT_H_

#ifdef __wasi__

// 防止重复定义的保护宏
#ifndef V8_WASI_CONSTANTS_DEFINED
#define V8_WASI_CONSTANTS_DEFINED

#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>
#include <cstring>  // for memcpy
#include <type_traits>  // for std::false_type, std::true_type, std::void_t

// V8 attribute macros for WASI
#ifndef V8_DEPRECATE_SOON
#define V8_DEPRECATE_SOON(message) [[deprecated(message)]]
#endif

// Ensure V8_INLINE is defined
#ifndef V8_INLINE
#define V8_INLINE inline
#endif

// Ensure V8_NOINLINE is defined
#ifndef V8_NOINLINE
#define V8_NOINLINE __attribute__((noinline))
#endif

// Ensure V8_WARN_UNUSED_RESULT is defined
#ifndef V8_WARN_UNUSED_RESULT
#define V8_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#endif

// Undefine any macros that might conflict from nuclear-fix.h
#ifdef kTaggedSize
#undef kTaggedSize
#endif
#ifdef kSystemPointerSize
#undef kSystemPointerSize
#endif
#ifdef kApiTaggedSize
#undef kApiTaggedSize
#endif
#ifdef kApiSystemPointerSize
#undef kApiSystemPointerSize
#endif
#ifdef kExternalAllocationSoftLimit
#undef kExternalAllocationSoftLimit
#endif
#ifdef kInt32Size
#undef kInt32Size
#endif

// WASI 平台基础常量
constexpr int kCppHeapPointerTagShift = 1;
constexpr int kJSAPIObjectWithEmbedderSlotsHeaderSize = 16;
constexpr int kEmbedderDataSlotSize = 8;

// Smi constants are defined in v8-internal.h
// Only define additional constants needed for WASI
constexpr int kSmiTag = 0;
constexpr int kSmiTagMask = 1;  // (1 << kSmiTagSize) - 1

// Heap object tag constants
constexpr int kHeapObjectTagSize = 2;
constexpr int kHeapObjectTagMask = (1 << kHeapObjectTagSize) - 1;  // = 3
constexpr int kWeakHeapObjectTag = 3;

// 指针和标记大小常量
constexpr int kTaggedSize = 4;  // 32位指针
constexpr int kSystemPointerSize = 4;
constexpr int kSystemPointerSizeLog2 = 2;
constexpr int kTaggedSizeLog2 = 2;

// API 常量
constexpr int kApiTaggedSize = 4;  // Same as kTaggedSize for WASI
constexpr int kApiSystemPointerSize = 4;

// 堆对象常量
constexpr int kHeapObjectTag = 1;
constexpr int kObjectAlignmentBits = 2;
constexpr intptr_t kObjectAlignment = 4;
constexpr intptr_t kObjectAlignmentMask = 3;

// 外部分配限制
constexpr size_t kExternalAllocationSoftLimit = 64 * 1024 * 1024;

// Size constants
constexpr int kInt32Size = sizeof(int32_t);

// V8 内部类型定义
namespace v8 {
// Forward declaration
class Isolate;
template<typename T> class Local;
class FixedArray;
class CppHeap;
class ArrayBuffer;

// SealHandleScope stub for WASI
class SealHandleScope {
 public:
  explicit SealHandleScope(Isolate* isolate) : isolate_(isolate) {}
  ~SealHandleScope() {}
  
 private:
  Isolate* isolate_;
  SealHandleScope(const SealHandleScope&) = delete;
  SealHandleScope& operator=(const SealHandleScope&) = delete;
};

// EscapableHandleScope stub
class EscapableHandleScope {
 public:
  explicit EscapableHandleScope(Isolate* isolate) {}
  ~EscapableHandleScope() {}
  
  template<typename T>
  Local<T> Escape(Local<T> value) {
    return value;
  }
};

// LocalVector template
template<typename T>
using LocalVector = std::vector<Local<T>>;
// HandleScope is already defined by V8

// NearHeapLimitCallback type
typedef size_t (*NearHeapLimitCallback)(void* data, size_t current_heap_limit,
                                         size_t initial_heap_limit);

// Forward declare StackTrace class to avoid conflicts
class StackTrace;

// StackTraceOptions enum - placed in v8 namespace to avoid conflicts
enum StackTraceOptions {
  kSTLineNumber = 1,
  kSTColumnOffset = 1 << 1,
  kSTScriptName = 1 << 2,
  kSTFunctionName = 1 << 3,
  kSTIsEval = 1 << 4,
  kSTIsConstructor = 1 << 5,
  kSTScriptNameOrSourceURL = 1 << 6,
  kSTScriptId = 1 << 7,
  kSTExposeFramesAcrossSecurityOrigins = 1 << 8,
  kSTOverview = kSTLineNumber | kSTColumnOffset | kSTScriptName | kSTFunctionName,
  kSTDetailed = kSTOverview | kSTIsEval | kSTIsConstructor | kSTScriptNameOrSourceURL
};

namespace internal {

// Forward declaration for internal::Isolate
class Isolate;

// Don't redefine Address if it's already defined by v8-internal.h
#ifndef V8_INTERNAL_ADDRESS_DEFINED
#define V8_INTERNAL_ADDRESS_DEFINED
// Use uintptr_t to match v8-internal.h definition
using Address = uintptr_t;
#endif

// Define kNullAddress constant
constexpr Address kNullAddress = 0;

#ifndef V8_INTERNAL_POINTER_TYPES_DEFINED
#define V8_INTERNAL_POINTER_TYPES_DEFINED
using ExternalPointer_t = uint32_t;
using CppHeapPointer_t = uint32_t;
using IndirectPointerHandle = uint32_t;
#endif

// Smi 工具函数 - commented out to avoid redefinition
// These functions are already defined in v8-internal.h
// #ifndef V8_SMI_VALUE_FUNCTIONS_DEFINED
// #define V8_SMI_VALUE_FUNCTIONS_DEFINED
// constexpr bool SmiValuesAre31Bits() { return kSmiValueSize == 31; }
// constexpr bool SmiValuesAre32Bits() { return kSmiValueSize == 32; }
// #endif

// 内部常量类（仅声明，避免重复定义）
#ifndef V8_INTERNALS_CLASS_DEFINED
#define V8_INTERNALS_CLASS_DEFINED
class Internals {
 public:
  static constexpr int kNodeStateIsWeakValue = 1;
  static constexpr int kNodeClassIdOffset = 4;
  static constexpr int kEmptyStringRootIndex = 0;
  static constexpr int kUndefinedValueRootIndex = 1;
  static constexpr int kNullValueRootIndex = 2;
  static constexpr int kTrueValueRootIndex = 3;
  static constexpr int kFalseValueRootIndex = 4;
  static constexpr int kStringResourceOffset = 8;
  static constexpr int kStringRepresentationAndEncodingMask = 0x0f;
  static constexpr int kStringEncodingMask = 0x08;
  static constexpr int kExternalOneByteRepresentationTag = 0x04;
  static constexpr int kExternalTwoByteRepresentationTag = 0x02;
  static constexpr int kExternalStringResourceTag = 2;  // External string tag
  static constexpr uintptr_t kEmbedderDataSlotPayloadTag = 1;  // Embedder data slot tag
  static constexpr int kDontThrow = 0;
  static constexpr int kThrowOnError = 1;

  // SMI 转换
  static constexpr Address IntegralToSmi(int value) {
    // kSmiTagSize = 1, kSmiShiftSize = 0, kSmiTag = 0
    return (static_cast<Address>(value) << 1) | 0;
  }

  // 虚拟函数声明
  static int GetNodeState(void* slot) { return 0; }
  static void CheckInitialized(void* isolate) {}
  
  // Template version of CheckInitialized
  template<typename T>
  static void CheckInitialized(T* isolate) {}
  static Address* GetRootSlot(void* isolate, int index) {
    static Address dummy = 0;
    return &dummy;
  }
  static bool IsExternalTwoByteString(int instance_type) { return false; }
  static int GetInstanceType(Address obj) { return 0; }
  static v8::Isolate* GetIsolateForSandbox(Address obj) { return nullptr; }
  static bool CanHaveInternalField(int instance_type) { return true; }
  
  // Constants as static members
  static constexpr int kJSAPIObjectWithEmbedderSlotsHeaderSize = ::kJSAPIObjectWithEmbedderSlotsHeaderSize;
  static constexpr int kEmbedderDataSlotSize = ::kEmbedderDataSlotSize;
  static constexpr int kEmbedderDataSlotExternalPointerOffset = 0;
  static constexpr int kJSObjectHeaderSize = 12;
  // kEmbedderDataSlotPayloadTag already defined above at line 135
  static constexpr int kNativeContextEmbedderDataOffset = 32;
  static constexpr int kEmbedderDataArrayHeaderSize = 8;
  
  // Additional methods needed by v8-context.h
  static Address ReadTaggedPointerField(Address obj, int offset) {
    return *reinterpret_cast<Address*>(obj + offset);
  }
  
  // IsValidSmi function
  static constexpr bool IsValidSmi(intptr_t value) {
    // kSmiValueSize = 31
    return value >= (-static_cast<intptr_t>(1) << 30) &&
           value <= (static_cast<intptr_t>(1) << 30) - 1;
  }
  
  // TryIntegralToSmi - returns pointer to Smi or nullptr
  static Address* TryIntegralToSmi(intptr_t value) {
    if (IsValidSmi(value)) {
      static thread_local Address smi = IntegralToSmi(value);
      return &smi;
    }
    return nullptr;
  }
  
  // GetRoot method
  static Address GetRoot(v8::Isolate* isolate, int index) {
    // Return dummy addresses for root values
    switch (index) {
      case kUndefinedValueRootIndex: return 0x1000;
      case kNullValueRootIndex: return 0x1004;
      case kTrueValueRootIndex: return 0x1008;
      case kFalseValueRootIndex: return 0x100C;
      case kEmptyStringRootIndex: return 0x1010;
      default: return 0;
    }
  }
  
  // HasHeapObjectTag
  static bool HasHeapObjectTag(Address value) {
    return (value & kHeapObjectTag) == kHeapObjectTag;
  }
  
  // kInferShouldThrowMode constant
  static constexpr int kInferShouldThrowMode = -1;
  
  // Missing methods
  template<typename T>
  static T ReadRawField(Address obj, int offset) {
    return *reinterpret_cast<T*>(obj + offset);
  }
  
  static Address DecompressTaggedField(Address obj, uint32_t value) {
    return static_cast<Address>(value);
  }
  
  static void* IsolateFromNeverReadOnlySpaceObject(Address obj) {
    return nullptr;
  }

  template <int tag>
  static Address ReadExternalPointerField(void* isolate, Address obj,
                                          int offset) {
    return 0;
  }
  
  static void IncrementLongTasksStatsCounter(void* isolate) {
    // No-op for WASI
  }
};
#endif // V8_INTERNALS_CLASS_DEFINED

// CppHeapPointerTagRange forward declaration
// Skip if already defined in v8-sandbox.h
#ifndef V8_CPPHEAP_POINTER_TAG_RANGE_DEFINED
#define V8_CPPHEAP_POINTER_TAG_RANGE_DEFINED
struct CppHeapPointerTagRange {
  int start;
  int end;
  CppHeapPointerTagRange(int s, int e) : start(s), end(e) {}
};
#endif

// Re-export constants to internal namespace
constexpr int kApiSystemPointerSize = ::kApiSystemPointerSize;
// These constants are already members of the Internals class above
// Just use them directly when needed instead of re-exporting

// Helper function for reading CppHeapPointer fields
template<typename T>
T* ReadCppHeapPointerField(void* isolate, Address obj, int offset, 
                          const CppHeapPointerTagRange& tag_range) {
  // Simplified implementation for WASI
  return reinterpret_cast<T*>(*reinterpret_cast<Address*>(obj + offset));
}

// IsolateFromNeverReadOnlySpaceObject function
inline void* IsolateFromNeverReadOnlySpaceObject(Address obj) {
  return nullptr;
}

// PerformCastCheck stub
template<typename T>
inline void PerformCastCheck(T* obj) {
  // No-op for WASI
}

// CustomArguments stub
template<typename T>
class CustomArguments {
  // Stub implementation
};

// Friend function stubs
inline void PrintFunctionCallbackInfo(void*) {}
inline void PrintPropertyCallbackInfo(void*) {}

// ShouldThrowOnError stub - accept internal::Isolate*
inline bool ShouldThrowOnError(internal::Isolate* isolate) {
  return false;
}

// BackingStoreBase stub for WASI
class BackingStoreBase {
 public:
  virtual ~BackingStoreBase() = default;
};

// Additional functions needed by v8-initialization.h
inline bool PointerCompressionIsEnabled() { return false; }
inline bool SandboxIsEnabled() { return false; }

// ExternalPointerTag enum
enum ExternalPointerTag : uint64_t {
  kWasmWasmStreamingTag = 0x1000
};

// Re-export smi constants to internal namespace
// Only export constants not already in v8-internal.h
using ::kSmiTag;
using ::kSmiTagMask;
using ::kHeapObjectTagSize;
using ::kHeapObjectTagMask;
using ::kWeakHeapObjectTag;
using ::kHeapObjectTag;

}  // namespace internal

}  // namespace v8

#endif  // V8_WASI_CONSTANTS_DEFINED

#endif  // __wasi__

#endif  // V8_INCLUDE_WASI_V8_WASI_COMPAT_H_
