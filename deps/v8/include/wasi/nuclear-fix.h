#ifndef V8_WASI_NUCLEAR_FIX_CLEAN_H_
#define V8_WASI_NUCLEAR_FIX_CLEAN_H_

#include <memory>

#include <sstream>
#include <limits>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <memory>
#include <atomic>
#include <cstdio>
#include <cstdlib>

// SourcePosition/SourceLocation definitions
struct SourcePosition {
  int line;
  int column;
  SourcePosition() : line(0), column(0) {}
  SourcePosition(int l, int c) : line(l), column(c) {}
  bool operator==(const SourcePosition& other) const {
    return line == other.line && column == other.column;
  }
  bool operator!=(const SourcePosition& other) const {
    return !(*this == other);
  }
};

class SourceLocation {
 public:
  int line;
  int column;
  SourceLocation() : line(0), column(0) {}
  SourceLocation(int l, int c) : line(l), column(c) {}
  SourceLocation(const SourcePosition& pos) : line(pos.line), column(pos.column) {}
};

// Macro definitions
#ifndef CHECK_GE
#define CHECK_GE(a, b) ((void)0)
#endif

#ifndef CHECK_LT
#define CHECK_LT(a, b) ((void)0)
#endif

#ifndef DCHECK
#define DCHECK(condition) ((void)0)
#endif

#ifndef DCHECK_EQ
#define DCHECK_EQ(a, b) ((void)0)
#endif

#ifndef DCHECK_GT
#define DCHECK_GT(a, b) ((void)0)
#endif

#ifndef DCHECK_GE
#define DCHECK_GE(a, b) ((void)0)
#endif

#ifndef DCHECK_LE
#define DCHECK_LE(a, b) ((void)0)
#endif

#ifndef DCHECK_LT
#define DCHECK_LT(a, b) ((void)0)
#endif

#ifndef DCHECK_NOT_NULL
#define DCHECK_NOT_NULL(val) (val)
#endif

#ifndef DCHECK_NE
#define DCHECK_NE(a, b) ((void)0)
#endif

#ifndef CHECK
#define CHECK(condition) ((void)0)
#endif

#ifndef CHECK_EQ
#define CHECK_EQ(a, b) CHECK((a) == (b))
#endif

#ifndef CHECK_GT
#define CHECK_GT(a, b) CHECK((a) > (b))
#endif

#ifndef CHECK_LE
#define CHECK_LE(a, b) CHECK((a) <= (b))
#endif

#ifndef UNREACHABLE
#define UNREACHABLE() __builtin_unreachable()
#endif

#ifndef UNIMPLEMENTED
#define UNIMPLEMENTED() UNREACHABLE()
#endif

#ifndef FATAL
#define FATAL(...) do { fprintf(stderr, __VA_ARGS__); abort(); } while(0)
#endif

#ifndef IMMEDIATE_CRASH
#define IMMEDIATE_CRASH() abort()
#endif

#ifndef V8_GLIBC_PREREQ
#define V8_GLIBC_PREREQ(maj, min) 0
#endif

namespace v8 {
namespace base {
#ifndef V8_BASE_BITS_DEFINED
namespace bits {
  inline uint32_t RotateRight32(uint32_t value, uint32_t shift) {
    return (value >> shift) | (value << (32 - shift));
  }
  
  inline uint32_t RotateLeft32(uint32_t value, uint32_t shift) {
    return (value << shift) | (value >> (32 - shift));
  }
  
  inline uint64_t RotateRight64(uint64_t value, uint64_t shift) {
    return (value >> shift) | (value << (64 - shift));
  }
  
  inline uint64_t RotateLeft64(uint64_t value, uint64_t shift) {
    return (value << shift) | (value >> (64 - shift));
  }
  
  inline int64_t SignedSaturatedAdd64(int64_t a, int64_t b) {
    int64_t result;
    if (__builtin_add_overflow(a, b, &result)) {
      return (a > 0) ? std::numeric_limits<int64_t>::max() 
                     : std::numeric_limits<int64_t>::min();
    }
    return result;
  }
  
  inline int64_t SignedSaturatedSub64(int64_t a, int64_t b) {
    int64_t result;
    if (__builtin_sub_overflow(a, b, &result)) {
      return (a > 0) ? std::numeric_limits<int64_t>::max() 
                     : std::numeric_limits<int64_t>::min();
    }
    return result;
  }
  
  inline unsigned CountTrailingZerosNonZero(uint32_t value) {
    return __builtin_ctz(value);
  }
  
  inline unsigned CountTrailingZeros32(uint32_t value) {
    return value ? __builtin_ctz(value) : 32;
  }
  
  inline unsigned CountPopulation(uint32_t value) {
    return __builtin_popcount(value);
  }
  
  template<typename T>
  inline bool IsPowerOfTwo(T value) {
    return value && !(value & (value - 1));
  }
}  // namespace bits
#endif // V8_BASE_BITS_DEFINED

// Helper functions
template <typename T>
inline T RoundDown(T value, T alignment) {
  return value & ~(alignment - 1);
}

using AtomicWord = intptr_t;

template<typename T>
class AsAtomicPointerImpl {
 public:
  using type = std::atomic<T>;
};

// CallOnce implementation
#ifndef V8_BASE_ONCE_TYPE_DEFINED
typedef std::atomic<int> OnceType;

template<typename Function, typename Arg>
inline void CallOnce(OnceType* once, Function function, Arg* arg) {
  int expected = 0;
  if (once->compare_exchange_strong(expected, 1, std::memory_order_acquire)) {
    function(arg);
    once->store(2, std::memory_order_release);
  } else {
    while (once->load(std::memory_order_acquire) != 2) {
      // Spin wait
    }
  }
}
#endif

}  // namespace base

namespace internal {

// Basic types
using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

// SmiTagging template for compatibility
template<int PointerSize>
struct SmiTagging {
  static constexpr int kSmiShiftSize = 1;
  static constexpr int kSmiTagSize = 1;
};

// Constants - only define once
#ifndef V8_INTERNAL_CONSTANTS_DEFINED
#define V8_INTERNAL_CONSTANTS_DEFINED
#define kSystemPointerSize sizeof(void*)
#define kApiSystemPointerSize kSystemPointerSize
#define kInt32Size sizeof(int32_t)
#define kInt64Size sizeof(int64_t)
#define kTaggedSize kInt32Size
#define kApiTaggedSize kTaggedSize

// Memory units
#define KB 1024
#define MB KB * KB
#define GB KB * KB * KB
#endif

// Pointer types
using ExternalPointer_t = uint32_t;
using CppHeapPointer_t = uint32_t;
using IndirectPointerHandle = uint32_t;
using TrustedPointerHandle = uint32_t;
using AtomicTagged_t = std::atomic<uint32_t>;

// JS Dispatch handle constants
constexpr int kJSDispatchHandleShift = 0;
#ifndef kJSDispatchHandleSize
#define kJSDispatchHandleSize 16
#endif

// External allocation limit
#ifndef kExternalAllocationSoftLimit
#define kExternalAllocationSoftLimit (64 * MB)
#endif

// Internals class is defined in v8-wasi-compat.h

namespace bits {
  using v8::base::bits::RotateRight32;
  using v8::base::bits::SignedSaturatedAdd64;
  using v8::base::bits::SignedSaturatedSub64;
  using v8::base::bits::CountTrailingZerosNonZero;
}  // namespace bits

namespace base {
  using v8::base::AtomicWord;
  using v8::base::AsAtomicPointerImpl;
}  // namespace base

}  // namespace internal
}  // namespace v8

namespace v8 {

// Platform definitions - matches OS::MemoryPermission values
enum class PagePermissions {
  kNoAccess = 0,
  kRead = 1,
  kReadWrite = 2,
  kReadWriteExecute = 3,
  kReadExecute = 4,
  kNoAccessWillJitLater = 5
};

// Forward declaration
using PlatformSharedMemoryHandle = int;  // WASI uses int for file descriptors
using Address = uintptr_t;

// Constants
constexpr Address kNoHint = 0;

class VirtualAddressSpace {
 public:
  virtual ~VirtualAddressSpace() = default;
  
  virtual void SetRandomSeed(int64_t seed) = 0;
  virtual Address RandomPageAddress() = 0;
  virtual Address AllocatePages(Address hint, size_t size, size_t alignment,
                               PagePermissions permissions) = 0;
  virtual void FreePages(Address address, size_t size) = 0;
  virtual bool SetPagePermissions(Address address, size_t size,
                                  PagePermissions permissions) = 0;
  virtual bool AllocateGuardRegion(Address address, size_t size) = 0;
  virtual void FreeGuardRegion(Address address, size_t size) = 0;
  virtual Address AllocateSharedPages(Address hint, size_t size,
                                     PagePermissions permissions,
                                     PlatformSharedMemoryHandle handle,
                                     uint64_t offset) = 0;
  virtual void FreeSharedPages(Address address, size_t size) = 0;
  virtual bool CanAllocateSubspaces() = 0;
  virtual std::unique_ptr<VirtualAddressSpace> AllocateSubspace(
      Address hint, size_t size, size_t alignment,
      PagePermissions max_page_permissions) = 0;
  virtual bool RecommitPages(Address address, size_t size,
                            PagePermissions permissions) = 0;
  virtual bool DiscardSystemPages(Address address, size_t size) = 0;
  virtual bool DecommitPages(Address address, size_t size) = 0;
  
  // Helper methods
  virtual Address base() const = 0;
  virtual size_t size() const = 0;
  virtual size_t page_size() const = 0;
  virtual size_t allocation_granularity() const = 0;
  virtual PagePermissions max_page_permissions() const = 0;
};

class SharedMemory {
 public:
  virtual ~SharedMemory() = default;
};

}  // namespace v8

// V8_BASE_EXPORT macro is defined in src/base/base-export.h

#endif  // V8_WASI_NUCLEAR_FIX_CLEAN_H_
