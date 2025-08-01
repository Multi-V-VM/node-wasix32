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
// Removed v8-source-location-fix.h as it causes namespace conflicts

// Include WASM32 architecture definitions
#include "wasm32-arch-fix.h"

// Forward declare v8::Isolate early
namespace v8 {
class Isolate;
}  // namespace v8

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

// Forward declaration - Isolate is defined elsewhere
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
  static constexpr int kJSObjectType = 1057;  // Must match JS_OBJECT_TYPE from instance-type.h
  static constexpr int kFirstNonstringType = 128;  // Updated to match actual value (was 64)
  static constexpr int kForeignType = 204;  // Updated to match actual value (was 69)
  static constexpr int kFirstJSApiObjectType = 1058;
  static constexpr int kLastJSApiObjectType = 2058;  // Add missing constant
  static constexpr int kJSSpecialApiObjectType = 1040;  // Updated to match error
  static constexpr int kOddballType = 131;  // Updated to match actual value (was 70)
  
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
  static constexpr uint64_t kExternalPointerMarkBit = 0x0001000000000000ULL;
  
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
  static ::v8::Isolate* GetIsolateForSandbox(Address obj) { 
    // In V8, internal::Isolate and v8::Isolate are different types
    // This is a stub implementation, so we just return nullptr
    return static_cast<::v8::Isolate*>(nullptr); 
  }
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
  
  // Missing buffer and WASM constants
  static constexpr size_t kMaxSafeBufferSizeForSandbox = 2147483647; // 2GB - 1
  
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
  
  // Additional overload for the call pattern in v8-context.h
  template<uint64_t tag>
  static Address ReadExternalPointerField(void* isolate, Address obj, int offset) { 
    return 0; 
  }
  
  // Additional missing methods for Smi handling
  static constexpr bool IsValidSmi(int32_t value) { return true; }
  static Address IntegralToSmi(int32_t value) { return static_cast<Address>(value << 1); }
  static Address* TryIntegralToSmi(int32_t value) { 
    static Address result = static_cast<Address>(value << 1);
    return &result;
  }
  
  // Missing method for LongTaskStats
  static void IncrementLongTasksStatsCounter(void* isolate) {
    // No-op implementation for WASI
  }
  
  // SmiValue method for extracting value from Smi-tagged address
  static int32_t SmiValue(Address value) {
    // Smi values are stored in the upper 32 bits on 64-bit systems
    // or use 31 bits on 32-bit systems
    if constexpr (kSystemPointerSize == 8) {
      // 64-bit: shift right by 32 to get the value
      return static_cast<int32_t>(value >> 32);
    } else {
      // 32-bit: shift right by 1 (tagged value)
      return static_cast<int32_t>(value >> 1);
    }
  }
};

// Add namespace functions
inline void* IsolateFromNeverReadOnlySpaceObject(Address obj) { return nullptr; }

// ShouldThrowOnError function for v8::internal namespace
inline bool ShouldThrowOnError(Address args, int mode) { return false; }
inline bool ShouldThrowOnError(Isolate* isolate) { return false; }

// Missing compression and sandbox functions
inline bool PointerCompressionIsEnabled() { return false; }
inline bool SandboxIsEnabled() { return false; }

// Missing cast check function
inline void PerformCastCheck(Address value) { /* no-op for WASI */ }
template<typename T>
inline void PerformCastCheck(T* value) { /* no-op for WASI */ }

template<typename T>
inline T* ReadCppHeapPointerField(void* isolate, Address obj, int offset, uint64_t tag_range) {
  return nullptr;
}

// Additional WASM constants
static constexpr uint64_t kWasmWasmStreamingTag = 0x2;

// Make internals constants available directly in v8::internal namespace
static constexpr size_t kMaxSafeBufferSizeForSandbox = Internals::kMaxSafeBufferSizeForSandbox;

// Add missing size constants
static constexpr int kInt64Size = sizeof(int64_t);

// Add ExternalPointerTag type
using ExternalPointerTag = uint64_t;
using ExternalPointerHandle = uint32_t;

// kExternalPointerTagShift is already defined in wasi-v8-sandbox-stubs.h
static constexpr uint64_t kExternalPointerTagMask = Internals::kExternalPointerTagMask;
static constexpr uint64_t kExternalPointerMarkBit = Internals::kExternalPointerMarkBit;
static constexpr uint64_t kExternalPointerPayloadMask = ~kExternalPointerTagMask;

// Add special external pointer tags
static constexpr ExternalPointerTag kExternalPointerFreeEntryTag = 0x7FFF000000000000ULL;
static constexpr ExternalPointerTag kExternalPointerEvacuationEntryTag = 0x7FFE000000000000ULL;

// Add ExternalPointerTagRange class
class ExternalPointerTagRange {
 public:
  ExternalPointerTagRange(ExternalPointerTag lower, ExternalPointerTag upper)
      : lower_(lower), upper_(upper) {}
  
  explicit ExternalPointerTagRange(ExternalPointerTag tag)
      : lower_(tag), upper_(tag) {}
  
  bool Contains(ExternalPointerTag tag) const {
    return tag >= lower_ && tag <= upper_;
  }
  
  bool IsEmpty() const { return lower_ > upper_; }
  
  ExternalPointerTag lower() const { return lower_; }
  ExternalPointerTag upper() const { return upper_; }
  
 private:
  ExternalPointerTag lower_;
  ExternalPointerTag upper_;
};

// Helper function for ExternalPointerCanBeEmpty
inline bool ExternalPointerCanBeEmpty(ExternalPointerTagRange tag_range) {
  // For now, allow all pointers to be empty
  return true;
}

// Make SmiValuesAre31Bits available directly
using ::SmiValuesAre31Bits;

// CppHeapPointerTagRange for WASI compatibility
#ifndef V8_CPPHEAP_POINTER_TAG_RANGE_DEFINED
#define V8_CPPHEAP_POINTER_TAG_RANGE_DEFINED
struct CppHeapPointerTagRange {
  constexpr CppHeapPointerTagRange(uint16_t lower, uint16_t upper)
      : lower_bound(lower), upper_bound(upper) {}
  
  uint16_t lower_bound;
  uint16_t upper_bound;
  
  // Check whether the tag of the given CppHeapPointerTable entry is within
  // this range. This method encodes implementation details of the
  // CppHeapPointerTable.
  bool CheckTagOf(uint64_t entry) const {
    // Extract the tag from the entry (assuming tag is in lower 16 bits)
    uint32_t actual_tag = static_cast<uint16_t>(entry);
    uint32_t lower = static_cast<uint32_t>(lower_bound);
    uint32_t upper = static_cast<uint32_t>(upper_bound);
    return actual_tag >= lower && actual_tag <= upper;
  }
};
#endif

// CppHeap pointer constants
constexpr int kCppHeapPointerPayloadShift = 16;  // Shift for payload
constexpr int kCppHeapPointerTagShift = 0;       // Tag is in lower bits
constexpr uint32_t kCppHeapPointerMarkBit = 0x80000000;  // Mark bit for GC

// External pointer constants
constexpr uint32_t kExternalPointerIndexShift = 8;  // Common shift value

#ifndef V8_EXTERNAL_POINTER_TAG_SHIFT
#define V8_EXTERNAL_POINTER_TAG_SHIFT
constexpr int kExternalPointerTagShift = 48;  // Tag is in upper 16 bits
#endif

#ifndef V8_EXTERNAL_POINTER_TABLE_SIZE
#define V8_EXTERNAL_POINTER_TABLE_SIZE
constexpr size_t kExternalPointerTableReservationSize = 1024 * 1024;  // 1MB reservation
constexpr size_t kCppHeapPointerTableReservationSize = 1024 * 1024;   // 1MB reservation
#endif

#ifndef V8_NULL_EXTERNAL_POINTER_HANDLE
#define V8_NULL_EXTERNAL_POINTER_HANDLE
using ExternalPointerHandle = uint32_t;
using CodePointerHandle = uint32_t;
using CppHeapPointerHandle = uint32_t;
constexpr ExternalPointerHandle kNullExternalPointerHandle = 0;
constexpr CodePointerHandle kNullCodePointerHandle = 0;
constexpr CppHeapPointerHandle kNullCppHeapPointerHandle = 0;
#endif

// Code pointer constants
#ifndef V8_CODE_POINTER_TABLE_ENTRY_SIZE
#define V8_CODE_POINTER_TABLE_ENTRY_SIZE
// Check if already defined elsewhere with correct type
#if !defined(kCodePointerTableEntrySize)
constexpr size_t kCodePointerTableEntrySize = 8;  // 64-bit entries
#endif
#endif

#ifndef V8_MAX_EXTERNAL_POINTERS
#define V8_MAX_EXTERNAL_POINTERS
// Check if already defined elsewhere, use uint32_t to match existing definitions
#if !defined(kMaxExternalPointers)
constexpr uint32_t kMaxExternalPointers = 65536;  // Match existing type
#endif
#endif

#ifndef V8_MAX_CODE_POINTERS
#define V8_MAX_CODE_POINTERS
// Check if already defined elsewhere, use uint32_t to match existing definitions  
#if !defined(kMaxCodePointers)
constexpr uint32_t kMaxCodePointers = 65536;  // Match existing type
#endif
#endif

#ifndef V8_MAX_CAPACITY
#define V8_MAX_CAPACITY
constexpr size_t kMaxCapacity = kMaxExternalPointers;  // For table capacity assertions
#endif

}  // namespace internal

// ResourceConstraints stub
class ResourceConstraints {
 public:
  ResourceConstraints() = default;
  void ConfigureDefaultsFromHeapSize(size_t initial_heap_size_in_bytes, 
                                   size_t maximum_heap_size_in_bytes) {}
  void ConfigureDefaults(uint64_t physical_memory, uint64_t virtual_memory_limit) {}
  size_t max_heap_size() const { return max_heap_size_; }
  void set_max_heap_size(size_t value) { max_heap_size_ = value; }
  size_t initial_heap_size() const { return initial_heap_size_; }
  void set_initial_heap_size(size_t value) { initial_heap_size_ = value; }
  size_t code_range_size() const { return code_range_size_; }
  void set_code_range_size(size_t value) { code_range_size_ = value; }
  size_t max_zone_pool_size() const { return max_zone_pool_size_; }
  void set_max_zone_pool_size(size_t value) { max_zone_pool_size_ = value; }
  
  // Worker constraints methods
  void set_stack_limit(uint32_t* value) { stack_limit_ = value; }
  uint32_t* stack_limit() const { return stack_limit_; }
  
  void set_max_young_generation_size_in_bytes(size_t value) { max_young_generation_size_in_bytes_ = value; }
  size_t max_young_generation_size_in_bytes() const { return max_young_generation_size_in_bytes_; }
  
  void set_max_old_generation_size_in_bytes(size_t value) { max_old_generation_size_in_bytes_ = value; }
  size_t max_old_generation_size_in_bytes() const { return max_old_generation_size_in_bytes_; }
  
  void set_code_range_size_in_bytes(size_t value) { code_range_size_in_bytes_ = value; }
  size_t code_range_size_in_bytes() const { return code_range_size_in_bytes_; }
  
 private:
  size_t max_heap_size_ = 0;
  size_t initial_heap_size_ = 0;
  size_t code_range_size_ = 0;
  size_t max_zone_pool_size_ = 0;
  uint32_t* stack_limit_ = nullptr;
  size_t max_young_generation_size_in_bytes_ = 0;
  size_t max_old_generation_size_in_bytes_ = 0;
  size_t code_range_size_in_bytes_ = 0;
};

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

// SourceLocation stub for V8 API - commented out as it's now defined in v8-source-location.h
// class SourceLocation {
//  public:
//   SourceLocation() : function_(""), file_(""), line_(0) {}
//   SourceLocation(const char* function, const char* file, int line)
//       : function_(function), file_(file), line_(line) {}
//   
//   const char* function() const { return function_; }
//   const char* file() const { return file_; }
//   int line() const { return line_; }
//   
//  private:
//   const char* function_;
//   const char* file_;
//   int line_;
// };

namespace base {

// Add missing base types
using uc32 = uint32_t;

// Vector template is now defined in v8/src/base/vector.h
// Remove duplicate definition to avoid conflicts

// AllocationResult template - removed to avoid conflicts with wasi-v8-base-fixes.h

// AddressRegion class - disabled, V8 now provides this
// #if !defined(V8_BASE_ADDRESS_REGION_H_) && !defined(V8_SRC_BASE_ADDRESS_REGION_H_)
// class AddressRegion {
//  public:
//   AddressRegion() : address_(0), size_(0) {}
//   AddressRegion(Address address, size_t size) : address_(address), size_(size) {}
//   
//   Address begin() const { return address_; }
//   size_t size() const { return size_; }
//   
//  private:
//   Address address_;
//   size_t size_;
// };
// #endif

// EnumSet template is now defined in v8/src/base/enum-set.h
// Remove duplicate definition to avoid conflicts

// RecursiveMutex class - disabled, V8 now provides this
// #if !defined(V8_BASE_PLATFORM_MUTEX_H_) && !defined(V8_SRC_BASE_PLATFORM_MUTEX_H_)
// class RecursiveMutex {
//  public:
//   void Lock() {}
//   void Unlock() {}
// };
// #endif

// AsAtomicPointerImpl template
template <typename T>
class AsAtomicPointerImpl {
 public:
  using type = ::std::atomic<T>;
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
  static ::std::mutex mtx;
  static ::std::set<void*> initialized_funcs;
  
  ::std::lock_guard<::std::mutex> lock(mtx);
  if (initialized_funcs.find((void*)init_func) == initialized_funcs.end()) {
    init_func(arg);
    initialized_funcs.insert((void*)init_func);
  }
}

}  // namespace base

// StackState enum - add to v8 namespace
enum class StackState {
  kNoHeapPointers,
  kMayContainHeapPointers
};

// Maybe template is now properly defined in v8-maybe.h
// Remove duplicate definition to avoid conflicts
// Just function will be available from v8-maybe.h

}  // namespace v8

#endif  // V8_INCLUDE_WASI_NUCLEAR_FIX_H_

