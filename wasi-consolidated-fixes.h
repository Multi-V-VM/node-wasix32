#ifndef WASI_CONSOLIDATED_FIXES_H_
#define WASI_CONSOLIDATED_FIXES_H_

#ifdef __wasi__

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <mutex>
#include <functional>
#include <atomic>
#include <iterator>

// =============================================================================
// NAMESPACE RESOLUTION FIXES
// =============================================================================

// First, define the base namespace types that are missing
namespace v8 {
namespace base {

// Template types for hashing and comparison
template<typename T>
struct hash : public std::hash<T> {};

template<typename T>
struct bit_hash : public std::hash<T> {};

template<typename T>
struct bit_equal_to : public std::equal_to<T> {};

template<typename T, typename U>
class Flags {
 public:
  Flags() = default;
};

template<typename T, typename U>
class EnumSet {
 public:
  EnumSet() = default;
};

// Address and memory types
using Address = uintptr_t;

class AddressRegion {
 public:
  AddressRegion() = default;
  AddressRegion(Address addr, size_t sz) : address_(addr), size_(sz) {}
  
  Address address() const { return address_; }
  size_t size() const { return size_; }
  
 private:
  Address address_ = 0;
  size_t size_ = 0;
};

// Memory allocation types
enum class PageInitializationMode { kUninitialized, kZeroInitialized };
enum class PageFreeingMode { kMakeInaccessible, kDiscard };

struct AllocationResult {
  void* ptr;
  size_t size;
  AllocationResult() : ptr(nullptr), size(0) {}
  AllocationResult(void* p, size_t s) : ptr(p), size(s) {}
};

// Type traits
template<typename T>
using is_trivially_copyable = std::is_trivially_copyable<T>;

// Mutex types
using Mutex = std::mutex;
using MutexGuard = std::lock_guard<std::mutex>;

// Atomic types
using OnceType = std::atomic<int>;
using AtomicWord = intptr_t;

// Function declarations
inline void* Malloc(size_t size) { return malloc(size); }
inline void Free(void* ptr) { free(ptr); }
inline AllocationResult AllocatePages(size_t size) {
  return AllocationResult(malloc(size), size);
}
inline void FreePages(void* address, size_t size) { free(address); }

inline size_t hash_combine(size_t seed, size_t hash) {
  return seed ^ (hash + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

// Bit manipulation functions
// Note: Most are defined in nuclear-fix.h, but we need some additional ones
namespace bits {
  // Functions already defined in nuclear-fix.h
  
  // Additional functions needed by V8
  inline unsigned CountTrailingZeros(uint32_t value) {
    return value ? __builtin_ctz(value) : 32;
  }
  // CountTrailingZeros32 is deprecated, use CountTrailingZerosNonZero instead
}

// Iterator type for std::iterator usage
template<typename Category, typename T, typename Distance = std::ptrdiff_t, 
         typename Pointer = T*, typename Reference = T&>
using iterator = std::iterator<Category, T, Distance, Pointer, Reference>;

}  // namespace base
}  // namespace v8

// Now create alias mappings in v8::internal::base to resolve namespace conflicts
namespace v8 {
namespace internal {

// Address type
using Address = uintptr_t;

namespace base {

    // Type aliases to resolve namespace conflicts
    template<typename T>
    using hash = ::v8::base::hash<T>;
    
    template<typename T>
    using bit_hash = ::v8::base::bit_hash<T>;
    
    template<typename T>
    using bit_equal_to = ::v8::base::bit_equal_to<T>;
    
    template<typename T, typename U>
    using Flags = ::v8::base::Flags<T, U>;
    
    template<typename T, typename U>
    using EnumSet = ::v8::base::EnumSet<T, U>;
    
    using AddressRegion = ::v8::base::AddressRegion;
    using Address = ::v8::base::Address;
    
    template<typename T>
    using is_trivially_copyable = ::v8::base::is_trivially_copyable<T>;
    
    using PageInitializationMode = ::v8::base::PageInitializationMode;
    using PageFreeingMode = ::v8::base::PageFreeingMode;
    
    using AllocationResult = ::v8::base::AllocationResult;
    
    using OnceType = ::v8::base::OnceType;
    using AtomicWord = ::v8::base::AtomicWord;
    
    using Mutex = ::v8::base::Mutex;
    using MutexGuard = ::v8::base::MutexGuard;
    // RecursiveMutex types are defined elsewhere
    
    // Function aliases
    inline void* Malloc(size_t size) { return ::v8::base::Malloc(size); }
    inline void Free(void* ptr) { ::v8::base::Free(ptr); }
    inline AllocationResult AllocatePages(size_t size) { return ::v8::base::AllocatePages(size); }
    inline void FreePages(void* address, size_t size) { ::v8::base::FreePages(address, size); }
    
    inline size_t hash_combine(size_t seed, size_t hash) {
        return ::v8::base::hash_combine(seed, hash);
    }
    
    namespace bits {
        // Forward to the implementations in the outer namespace
        using ::v8::base::bits::CountLeadingZeros;
        using ::v8::base::bits::CountPopulation;
        using ::v8::base::bits::CountTrailingZerosNonZero;
        using ::v8::base::bits::CountTrailingZeros;
        // CountTrailingZeros32 is deprecated, use CountTrailingZerosNonZero instead
    }

    // Iterator type for std::iterator usage
    template<typename Category, typename T, typename Distance = std::ptrdiff_t, 
             typename Pointer = T*, typename Reference = T&>
    using iterator = ::v8::base::iterator<Category, T, Distance, Pointer, Reference>;
}

}  // namespace internal
}  // namespace v8

// =============================================================================
// SEGMENTED TABLE FIXES
// =============================================================================

namespace v8 {
namespace internal {
    template<typename Entry, size_t size>
    class SegmentedTable {
    public:
        struct FreelistHead {
            uint32_t next;
            uint32_t length;
            FreelistHead() : next(0), length(0) {}
            FreelistHead(uint32_t n, uint32_t l) : next(n), length(l) {}
        };
        
        // Constants to fix kEntriesPerSegment errors
        static constexpr size_t kEntriesPerSegment = 512;
        
        void SweepSegments(size_t threshold = 2 * kEntriesPerSegment) {
            // Stub implementation
        }
    };
}
}

// =============================================================================
// V8 INTERNALS FIXES
// =============================================================================

namespace v8 {
namespace internal {

// Fix for SmiValue function conflict
class Internals {
 public:
  static int SmiValue(intptr_t ptr) {
    return static_cast<int>(ptr >> 1);
  }
};

// Constants
constexpr int kSystemPointerSize = sizeof(void*);
constexpr int kSimd128Size = 16;

}  // namespace internal
}  // namespace v8

// =============================================================================
// MACRO DEFINITIONS
// =============================================================================

// Basic assertion macros for WASI
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

#ifndef UNREACHABLE
#define UNREACHABLE() __builtin_unreachable()
#endif

// =============================================================================
// ADDITIONAL FIXES
// =============================================================================

// Fix for LocalVector constructor issues - moved to wasi-v8-api-additions.h
// Include v8.h first, then wasi-v8-api-additions.h after it in files that need these fixes

// Fix for aliased buffer index conflict
#ifdef index
#undef index
#endif

// Node namespace forward declarations
namespace node {
    class Environment;
    class MemoryTracker;
}

// Fix for crypto namespace issues
namespace node {
namespace crypto {
    using Environment = ::node::Environment;
    using MemoryTracker = ::node::MemoryTracker;
}
}

#endif // __wasi__

#endif // WASI_CONSOLIDATED_FIXES_H_