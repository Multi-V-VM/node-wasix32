#ifndef WASI_V8_INTERNALS_MINIMAL_H_
#define WASI_V8_INTERNALS_MINIMAL_H_

// Minimal V8 Internals class for WASI builds
// This avoids conflicts with macros defined in nuclear-fix.h

namespace v8 {

// Forward declare v8::Isolate
class Isolate;

namespace internal {

// Forward declare internal::Isolate
class Isolate;

// Basic type already defined in v8-wasi-compat.h
// using Address = uintptr_t;

#ifndef V8_INTERNALS_CLASS_DEFINED
#define V8_INTERNALS_CLASS_DEFINED

class Internals {
 public:
  // Use enum instead of static const to avoid macro conflicts
  enum {
    kNodeStateIsWeakValue = 1,
    kNodeClassIdOffset = 4,
    kEmptyStringRootIndex = 0,
    kUndefinedValueRootIndex = 1,
    kNullValueRootIndex = 2,
    kTrueValueRootIndex = 3,
    kFalseValueRootIndex = 4,
    kStringResourceOffset = 8,
    kStringRepresentationAndEncodingMask = 0x0f,
    kStringEncodingMask = 0x08,
    kExternalOneByteRepresentationTag = 0x04,
    kExternalTwoByteRepresentationTag = 0x02,
    kExternalStringResourceTag = 2,
    kJSAPIObjectWithEmbedderSlotsHeaderSize = 16,
    kEmbedderDataSlotSize = 8,
    kEmbedderDataSlotExternalPointerOffset = 0,
    kJSObjectHeaderSize = 12,
    kNativeContextEmbedderDataOffset = 32,
    kEmbedderDataArrayHeaderSize = 8,
    kInferShouldThrowMode = -1,
    kDontThrow = 0,
    kThrowOnError = 1
  };
  
  static const uintptr_t kEmbedderDataSlotPayloadTag = 1;
  // kExternalAllocationSoftLimit is already defined as a macro in nuclear-fix.h
  
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
  
  static inline Address IntegralToSmi(int value) {
    return (static_cast<Address>(value) << 1) | 1;
  }
  
  // Helper functions
  static constexpr bool IsValidSmi(intptr_t value) {
    return value >= (-static_cast<intptr_t>(1) << 30) &&
           value <= (static_cast<intptr_t>(1) << 30) - 1;
  }
  
  static inline Address* TryIntegralToSmi(intptr_t value) {
    if (IsValidSmi(value)) {
      static thread_local Address smi = IntegralToSmi(value);
      return &smi;
    }
    return nullptr;
  }
  
  // Stub functions for compatibility
  static inline int GetNodeState(void* slot) { return 0; }
  static inline void CheckInitialized(void* isolate) {}
  
  template<typename T>
  static inline void CheckInitialized(T* isolate) {}
  
  static inline Address* GetRootSlot(void* isolate, int index) {
    static Address dummy = 0;
    return &dummy;
  }
  
  static inline bool IsExternalTwoByteString(int instance_type) { return false; }
  static inline int GetInstanceType(Address obj) { return 0; }
  static inline v8::Isolate* GetIsolateForSandbox(Address obj) { return nullptr; }
  static inline bool CanHaveInternalField(int instance_type) { return true; }
  
  static inline Address ReadTaggedPointerField(Address obj, int offset) {
    return *reinterpret_cast<Address*>(obj + offset);
  }
  
  static inline Address GetRoot(v8::Isolate* isolate, int index) {
    switch (index) {
      case kUndefinedValueRootIndex: return 0x1000;
      case kNullValueRootIndex: return 0x1004;
      case kTrueValueRootIndex: return 0x1008;
      case kFalseValueRootIndex: return 0x100C;
      case kEmptyStringRootIndex: return 0x1010;
      default: return 0;
    }
  }
  
  static inline bool HasHeapObjectTag(Address value) {
    return (value & 1) == 1;
  }
  
  template<typename T>
  static inline T ReadRawField(Address obj, int offset) {
    return *reinterpret_cast<T*>(obj + offset);
  }
  
  static inline Address DecompressTaggedField(Address obj, uint32_t value) {
    return static_cast<Address>(value);
  }
  
  static inline void* IsolateFromNeverReadOnlySpaceObject(Address obj) {
    return nullptr;
  }
  
  static inline v8::internal::Isolate* IsolateFromNeverReadOnlySpaceObject_internal(Address obj) {
    return nullptr;
  }

  template <int tag>
  static inline Address ReadExternalPointerField(void* isolate, Address obj,
                                                int offset) {
    return 0;
  }
  
  // Add ReadCppHeapPointerField implementation
  template <typename T>
  static inline T* ReadCppHeapPointerField(void* isolate, Address obj,
                                          int offset, int tag_range) {
    // For WASI, simply read the pointer from the object
    return reinterpret_cast<T*>(*reinterpret_cast<Address*>(obj + offset));
  }
};

#endif // V8_INTERNALS_CLASS_DEFINED

// Global namespace function that delegates to Internals
static inline void* IsolateFromNeverReadOnlySpaceObject(Address obj) {
  return Internals::IsolateFromNeverReadOnlySpaceObject(obj);
}

// Add a global namespace template function for ReadCppHeapPointerField
template <typename T>
static inline T* ReadCppHeapPointerField(void* isolate, Address obj,
                                        int offset, int tag_range) {
  return Internals::ReadCppHeapPointerField<T>(isolate, obj, offset, tag_range);
}

}  // namespace internal
}  // namespace v8

#endif  // WASI_V8_INTERNALS_MINIMAL_H_