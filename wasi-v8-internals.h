#ifndef WASI_V8_INTERNALS_H_
#define WASI_V8_INTERNALS_H_

// V8 Internals class for WASI builds
#include <cstdint>
#include <cstddef>

namespace v8 {
namespace internal {

// Forward declarations
using Address = uintptr_t;

}  // namespace internal

// Forward declare v8::Isolate in v8 namespace
class Isolate;

namespace internal {

#ifndef V8_INTERNALS_CLASS_DEFINED
#define V8_INTERNALS_CLASS_DEFINED

class Internals {
 public:
  // Basic constants
  static const int kApiTaggedSize = 4;
  static const size_t kExternalAllocationSoftLimit = 64 * 1024 * 1024;
  static constexpr int kDontThrow = 0;
  static constexpr int kThrowOnError = 1;
  
  // Root indices
  static constexpr int kNodeStateIsWeakValue = 1;
  static constexpr int kNodeClassIdOffset = 4;
  static constexpr int kEmptyStringRootIndex = 0;
  static constexpr int kUndefinedValueRootIndex = 1;
  static constexpr int kNullValueRootIndex = 2;
  static constexpr int kTrueValueRootIndex = 3;
  static constexpr int kFalseValueRootIndex = 4;
  
  // String constants
  static constexpr int kStringResourceOffset = 8;
  static constexpr int kStringRepresentationAndEncodingMask = 0x0f;
  static constexpr int kStringEncodingMask = 0x08;
  static constexpr int kExternalOneByteRepresentationTag = 0x04;
  static constexpr int kExternalTwoByteRepresentationTag = 0x02;
  static constexpr int kExternalStringResourceTag = 2;
  
  // Object layout constants
  static constexpr int kJSAPIObjectWithEmbedderSlotsHeaderSize = 16;
  static constexpr int kEmbedderDataSlotSize = 8;
  static constexpr int kEmbedderDataSlotExternalPointerOffset = 0;
  static constexpr int kJSObjectHeaderSize = 12;
  static constexpr uintptr_t kEmbedderDataSlotPayloadTag = 1;
  static constexpr int kNativeContextEmbedderDataOffset = 32;
  static constexpr int kEmbedderDataArrayHeaderSize = 8;
  static constexpr int kInferShouldThrowMode = -1;
  
  // SMI conversion functions
  static inline int IntToSmi(int value) {
    return (value << 1) | 1;
  }
  
  static inline int32_t SmiValue(int smi) {
    return smi >> 1;
  }
  
  static inline int32_t SmiValue(Address ptr) {
    return static_cast<int32_t>(ptr >> 1);
  }
  
  static constexpr Address IntegralToSmi(int value) {
    return (static_cast<Address>(value) << 1) | 1;
  }
  
  // Helper functions
  static constexpr bool IsValidSmi(intptr_t value) {
    // For 32-bit systems, SMI values are 31 bits
    return value >= (-static_cast<intptr_t>(1) << 30) &&
           value <= (static_cast<intptr_t>(1) << 30) - 1;
  }
  
  static Address* TryIntegralToSmi(intptr_t value) {
    if (IsValidSmi(value)) {
      static thread_local Address smi = IntegralToSmi(value);
      return &smi;
    }
    return nullptr;
  }
  
  // Stub functions for compatibility
  static int GetNodeState(void* slot) { return 0; }
  static void CheckInitialized(void* isolate) {}
  
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
  
  static Address ReadTaggedPointerField(Address obj, int offset) {
    return *reinterpret_cast<Address*>(obj + offset);
  }
  
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
  
  static bool HasHeapObjectTag(Address value) {
    return (value & 1) == 1;
  }
  
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
};

#endif // V8_INTERNALS_CLASS_DEFINED

}  // namespace internal
}  // namespace v8

#endif  // WASI_V8_INTERNALS_H_