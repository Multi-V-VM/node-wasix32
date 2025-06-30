#ifndef V8_WASI_NUCLEAR_FIX_CLEAN_H_
#define V8_WASI_NUCLEAR_FIX_CLEAN_H_

#include <sstream>
#include <limits>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <memory>
#include <atomic>

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

#ifndef UNREACHABLE
#define UNREACHABLE() __builtin_unreachable()
#endif

#ifndef UNIMPLEMENTED
#define UNIMPLEMENTED() UNREACHABLE()
#endif

#ifndef V8_GLIBC_PREREQ
#define V8_GLIBC_PREREQ(maj, min) 0
#endif

namespace v8 {
namespace base {
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
}  // namespace bits

using AtomicWord = intptr_t;

template<typename T>
class AsAtomicPointerImpl {
 public:
  using type = std::atomic<T>;
};

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

// Constants - only define once
#ifndef V8_INTERNAL_CONSTANTS_DEFINED
#define V8_INTERNAL_CONSTANTS_DEFINED
constexpr int kSystemPointerSize = sizeof(void*);
constexpr int kApiSystemPointerSize = kSystemPointerSize;
constexpr int kInt32Size = sizeof(int32_t);
constexpr int kInt64Size = sizeof(int64_t);
constexpr int kTaggedSize = kInt32Size;
constexpr int kApiTaggedSize = kTaggedSize;

// Memory units
constexpr int KB = 1024;
constexpr int MB = KB * KB;
constexpr int GB = KB * KB * KB;
#endif

// Pointer types
using ExternalPointer_t = uint32_t;
using CppHeapPointer_t = uint32_t;
using IndirectPointerHandle = uint32_t;
using TrustedPointerHandle = uint32_t;
using AtomicTagged_t = std::atomic<uint32_t>;

// JS Dispatch handle constants
constexpr int kJSDispatchHandleShift = 0;
constexpr int kJSDispatchHandleSize = 16;

// External allocation limit
constexpr size_t kExternalAllocationSoftLimit = 64 * MB;

// Internals class
class Internals {
 public:
  static constexpr size_t kExternalAllocationSoftLimit = 64 * MB;
};

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

// Platform definitions
enum class PagePermissions {
  kNoAccess = 0,
  kRead = 1,
  kReadWrite = 3,
  kReadExecute = 5
};

class VirtualAddressSpace {
 public:
  virtual ~VirtualAddressSpace() = default;
};

class SharedMemory {
 public:
  virtual ~SharedMemory() = default;
};

// V8_BASE_EXPORT macro
#ifndef V8_BASE_EXPORT
#define V8_BASE_EXPORT
#endif

#endif  // V8_WASI_NUCLEAR_FIX_CLEAN_H_
