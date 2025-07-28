#ifndef V8_INCLUDE_WASI_NUCLEAR_FIX_H_
#define V8_INCLUDE_WASI_NUCLEAR_FIX_H_

#include <stdint.h>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <memory>
#include <limits>
#include <set>

// Include deprecation fixes
#include "v8-deprecation-fix.h"

// Constants needed for VirtualAddressSpace
constexpr uintptr_t kNoHint = 0;

// Define Address in global namespace first
using Address = uintptr_t;

// Define pointer types for WASI
using ExternalPointer_t = uint32_t;
using CppHeapPointer_t = uint32_t;
using IndirectPointerHandle = uint32_t;
using TrustedPointerHandle = uint32_t;
using JSDispatchHandle = uint32_t;
using PlatformSharedMemoryHandle = int;

// Smi tagging constants
constexpr int kSmiTagSize = 1;
constexpr int kSmiValueSize = 31;
constexpr int kSmiShiftSize = 0;
constexpr int kSmiTag = 0;
constexpr int kSmiTagMask = (1 << kSmiTagSize) - 1;

// JS Dispatch handle constants
constexpr int kJSDispatchHandleShift = 0;

// Define constants
static constexpr int kApiSystemPointerSize = sizeof(void*);
static constexpr int kApiDoubleSize = sizeof(double);
static constexpr int kApiInt32Size = sizeof(int32_t);
static constexpr int kApiInt64Size = sizeof(int64_t);
static constexpr int kApiSizetSize = sizeof(size_t);

// Size constants
constexpr size_t KB = 1024;
constexpr size_t MB = KB * 1024;
constexpr size_t GB = MB * 1024;

// Crash macro
#ifndef IMMEDIATE_CRASH
#define IMMEDIATE_CRASH() __builtin_trap()
#endif

// System pointer constants
constexpr int kSystemPointerSize = sizeof(void*);
constexpr int kSystemPointerSizeLog2 = (kSystemPointerSize == 8) ? 3 : 2;
constexpr int kTaggedSize = kSystemPointerSize;
constexpr int kTaggedSizeLog2 = kSystemPointerSizeLog2;
constexpr int kApiTaggedSize = kTaggedSize;

// Smi value functions
constexpr bool SmiValuesAre31Bits() { return kSmiValueSize == 31; }
constexpr bool SmiValuesAre32Bits() { return kSmiValueSize == 32; }
constexpr bool kIsSmiValueInUpper32Bits = false;
constexpr bool kIsSmiValueInLower32Bits = true;

namespace v8 {

// Import Address into v8 namespace
using ::Address;

// Forward declare v8::Isolate for compatibility
class Isolate;

// Define PagePermissions enum
enum class PagePermissions {
  kNoAccess,
  kRead,
  kReadWrite,
  kReadExecute,
  kReadWriteExecute,
  kNoAccessWillJitLater,
};

namespace internal {
// Import Address into v8::internal namespace
using ::Address;

// Forward declaration
class Isolate;

// Forward declarations and stubs for missing base classes
class BackingStoreBase {
public:
  virtual ~BackingStoreBase() = default;
};

// Null address constant
constexpr Address kNullAddress = 0;

// Define complete Internals class
class Internals {
 public:
  static constexpr int kJSObjectType = 0;
  static constexpr int kFirstNonstringType = 0x40;
  static constexpr int kForeignType = 0x45;
  
  static constexpr int kUndefinedValueRootIndex = 5;
  static constexpr int kNullValueRootIndex = 6;
  static constexpr int kTrueValueRootIndex = 7;
  static constexpr int kFalseValueRootIndex = 8;
  static constexpr int kEmptyStringRootIndex = 9;
  
  static constexpr int kNodeClassIdOffset = 1;
  static constexpr int kNodeStateIsWeakValue = 2;
  static constexpr int kNodeStateIsPendingValue = 3;
  
  static constexpr int kNativeContextEmbedderDataOffset = 7;
  
  static constexpr int kExternalPointerTableSizeLog2 = 22;
  static constexpr int kExternalPointerTableSize = 1 << kExternalPointerTableSizeLog2;
  
  static constexpr uintptr_t kExternalPointerTagShift = 48;
  static constexpr uint64_t kExternalPointerTagMask = 0xFFFF000000000000ULL;
  
  static constexpr int kIsolateRootsOffset = 0;
  static constexpr int kExternalPointerTableOffset = 16;
  static constexpr int kBuiltinTier0TableOffset = 32;
  
  static constexpr int kStringResourceOffset = 16;
  static constexpr int kOddballKindOffset = 16;
  static constexpr int kJSObjectHeaderSize = 24;
  static constexpr int kFixedArrayHeaderSize = 16;
  static constexpr int kEmbedderDataArrayHeaderSize = 16;
  static constexpr int kEmbedderDataSlotSize = 8;
  static constexpr int kContextHeaderSize = 16;
  static constexpr int kContextEmbedderDataIndex = 5;
  
  static int GetInstanceType(Address obj) { return 0; }
  static void* GetRoot(void* isolate, int index) { return nullptr; }
  static uint8_t GetNodeState(void* obj) { return 0; }
  static void UpdateNodeState(void* obj, uint8_t state) {}
  static void CheckInitialized(void* isolate) {}
  static Address* GetRootSlot(void* isolate, int index) { return nullptr; }
  static bool IsExternalTwoByteString(int instance_type) { return false; }
  static int ReadRawField(Address heap_object_ptr, int offset) { return 0; }
  static Address ReadRawValue(Address heap_object_ptr, int offset) { return 0; }
  static void WriteRawField(Address heap_object_ptr, int offset, Address value) {}
  static uint8_t GetNodeFlag(void* obj, int shift) { return 0; }
  static void UpdateNodeFlag(void* obj, bool value, int shift) {}
  static uint16_t GetNodeClassId(void* obj) { return 0; }
  static void SetNodeClassId(void* obj, uint16_t id) {}
  
  // Additional missing methods  
  static v8::Isolate* GetIsolateForSandbox(Address obj) { return nullptr; }
  static Address ReadExternalPointerField(void* isolate, Address heap_object_ptr, 
                                         int offset, uint64_t tag) { return 0; }
  
  // Overloaded version for different parameter order
  static Address ReadExternalPointerField(Address heap_object_ptr, int offset, void* isolate, uint64_t tag) { return 0; }
  
  template<typename T>
  static T* GetExternalStringResource(void* isolate, Address obj) { return nullptr; }
  
  // Missing constants
  static constexpr int kExternalStringResourceTag = 0x1;
  static constexpr int kStringRepresentationAndEncodingMask = 0x0F;
  static constexpr int kStringEncodingMask = 0x08;
  static constexpr int kExternalOneByteRepresentationTag = 0x0A;
  static constexpr int kExternalTwoByteRepresentationTag = 0x02;
  
  // Additional missing constants
  static constexpr int kJSAPIObjectWithEmbedderSlotsHeaderSize = 24;
  static constexpr int kEmbedderDataSlotExternalPointerOffset = 8;
  static constexpr uint64_t kEmbedderDataSlotPayloadTag = 0x1;
  
  // Additional methods
  static bool CanHaveInternalField(int instance_type) { return true; }
  
  // Missing heap and error constants
  static bool HasHeapObjectTag(Address obj) { return true; }
  static constexpr int kInferShouldThrowMode = 0;
  static constexpr int kDontThrow = 1;
  
  template<typename T>
  static T ReadRawField(Address heap_object_ptr, int offset) { return T{}; }
  
  static Address DecompressTaggedField(Address obj, uint32_t value) { return value; }
  
  static Address ReadTaggedPointerField(Address obj, int offset) { return 0; }
  
  template<uint64_t tag>
  static Address ReadExternalPointerField(Address obj, int offset, void* isolate) { 
    return 0; 
  }
  
  // Additional missing methods for Smi handling
  static constexpr bool IsValidSmi(int32_t value) { return true; }
  static Address IntegralToSmi(int32_t value) { return static_cast<Address>(value << 1); }
  static Address* TryIntegralToSmi(int32_t value) { 
    static Address result = static_cast<Address>(value << 1);
    return &result;
  }
};

// Add namespace functions
inline void* IsolateFromNeverReadOnlySpaceObject(Address obj) { return nullptr; }

// ShouldThrowOnError function for v8::internal namespace
inline bool ShouldThrowOnError(Address args, int mode) { return false; }

// Missing cast check function
inline void PerformCastCheck(Address value) { /* no-op for WASI */ }

template<typename T>
inline T* ReadCppHeapPointerField(void* isolate, Address obj, int offset, uint64_t tag_range) {
  return nullptr;
}

}  // namespace internal

// VirtualAddressSpace stub
class VirtualAddressSpace {
 public:
  VirtualAddressSpace() : page_size_(0), allocation_granularity_(0), base_(0), size_(0) {}
  VirtualAddressSpace(size_t page_size, size_t allocation_granularity,
                     Address base, size_t size) : page_size_(page_size), allocation_granularity_(allocation_granularity), base_(base), size_(size) {}
  virtual ~VirtualAddressSpace() = default;
  
  virtual void SetRandomSeed(int64_t seed) {}
  virtual Address RandomPageAddress() { 
    // Simple implementation - just return base address
    return base_; 
  }
  virtual Address AllocatePages(Address hint, size_t size, size_t alignment,
                              PagePermissions permissions) { return 0; }
  virtual Address AllocateSharedPages(Address hint, size_t size, PagePermissions permissions,
                                    PlatformSharedMemoryHandle fd, uint64_t offset) { return 0; }
  virtual void FreeSharedPages(Address address, size_t size) {}
  virtual void FreePages(Address address, size_t size) {}
  virtual bool SetPagePermissions(Address address, size_t size,
                                 PagePermissions permissions) { return false; }
  virtual bool AllocateGuardRegion(Address address, size_t size) { return false; }
  virtual void FreeGuardRegion(Address address, size_t size) {}
  virtual bool CanAllocateSubspaces() { return false; }
  virtual ::std::unique_ptr<VirtualAddressSpace> AllocateSubspace(
      Address hint, size_t size, size_t alignment,
      PagePermissions max_permissions) { return nullptr; }
      
  virtual bool RecommitPages(Address address, size_t size,
                            PagePermissions permissions) { return false; }
  virtual bool DiscardSystemPages(Address address, size_t size) { return false; }
  virtual bool DecommitPages(Address address, size_t size) { return false; }
  
  virtual PagePermissions max_page_permissions() const { return max_permissions_; }
  
  virtual size_t page_size() const { return page_size_; }
  virtual size_t allocation_granularity() const { return allocation_granularity_; }
  virtual Address base() const { return base_; }
  virtual size_t size() const { return size_; }
  
 private:
  size_t page_size_;
  size_t allocation_granularity_;
  Address base_;
  size_t size_;
  PagePermissions max_permissions_ = PagePermissions::kReadWriteExecute;
};

// Additional stubs for missing types
using SmiTagging = internal::Internals;
using AtomicTagged_t = ::std::atomic<uint32_t>;

// SmiTagging template - moved outside v8 namespace for global access
} // namespace v8

// SmiTagging template in global namespace 
template<int kPointerSize>
class SmiTagging {
 public:
  static constexpr int kSmiShiftSize = kPointerSize == 8 ? 31 : 0;
  static constexpr int kSmiValueSize = kPointerSize == 8 ? 32 : 31;
  static constexpr int kSmiMinValue = -(1 << (kSmiValueSize - 1));
  static constexpr int kSmiMaxValue = (1 << (kSmiValueSize - 1)) - 1;
};

namespace v8 {

// Once implementation
class Once {
 public:
  using OnceType = ::std::atomic<int>;
  
  Once() : state_(0) {}
  
  bool load(::std::memory_order mo = ::std::memory_order_acquire) const {
    return state_.load(mo) == 2;
  }
  
  void store(int value, ::std::memory_order mo = ::std::memory_order_release) {
    state_.store(value, mo);
  }
  
  void CallOnce(void (*init_func)()) {
    int expected = 0;
    if (state_.compare_exchange_strong(expected, 1, ::std::memory_order_acquire)) {
      init_func();
      state_.store(2, ::std::memory_order_release);
    } else {
      while (state_.load(::std::memory_order_acquire) != 2) {
        // Spin wait
      }
    }
  }
  
 private:
  ::std::atomic<int> state_;
};

// Additional forward declarations and stubs
class Platform;
class PageAllocator;
class ThreadIsolatedAllocator;

// SourceLocation stub for V8 API
class SourceLocation {
 public:
  SourceLocation() : function_(""), file_(""), line_(0) {}
  SourceLocation(const char* function, const char* file, int line)
      : function_(function), file_(file), line_(line) {}
  
  const char* function() const { return function_; }
  const char* file() const { return file_; }
  int line() const { return line_; }
  
 private:
  const char* function_;
  const char* file_;
  int line_;
};

namespace base {

// AsAtomicPointerImpl template
template <typename T>
class AsAtomicPointerImpl {
 public:
  using type = std::atomic<T>;
};

namespace bits {
  inline int64_t SignedSaturatedAdd64(int64_t a, int64_t b) {
    // Simple implementation without overflow detection for WASI
    return a + b;
  }
  
  inline int64_t SignedSaturatedSub64(int64_t a, int64_t b) {
    // Simple implementation without overflow detection for WASI
    return a - b;
  }
  
  inline int CountTrailingZerosNonZero(uint32_t value) {
    return __builtin_ctz(value);
  }
  
  inline uint32_t RotateRight32(uint32_t value, int shift) {
    return (value >> shift) | (value << (32 - shift));
  }
  
  template<typename T>
  inline bool IsPowerOfTwo(T value) {
    return value && !(value & (value - 1));
  }
}  // namespace bits

// CallOnce implementation for lazy initialization
inline void CallOnce(Once* once, void (*init_func)()) {
  once->CallOnce(init_func);
}

// CallOnce implementation for atomic<int> types (used by v8::base)
inline void CallOnce(::std::atomic<int>* once, void (*init_func)(void*), void* arg) {
  // Use a simple flag-based implementation
  int expected = 0;
  if (once->compare_exchange_strong(expected, 1, ::std::memory_order_acquire)) {
    init_func(arg);
    once->store(2, ::std::memory_order_release);
  } else {
    while (once->load(::std::memory_order_acquire) != 2) {
      // Spin wait
    }
  }
}

// Simple CallOnce implementation for cases with arguments
template<typename Arg>
inline void CallOnce(Once* once, void (*init_func)(Arg*), Arg* arg) {
  // Use a global flag since we can't access private state_
  static std::mutex mtx;
  static std::set<void*> initialized_funcs;
  
  std::lock_guard<std::mutex> lock(mtx);
  if (initialized_funcs.find((void*)init_func) == initialized_funcs.end()) {
    init_func(arg);
    initialized_funcs.insert((void*)init_func);
  }
}

}  // namespace base

}  // namespace v8

#endif  // V8_INCLUDE_WASI_NUCLEAR_FIX_H_

