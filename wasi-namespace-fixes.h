#ifndef WASI_NAMESPACE_FIXES_H_
#define WASI_NAMESPACE_FIXES_H_

#ifdef __wasi__

// This header resolves namespace conflicts between v8::internal::base and ::v8::base
// that occur during WASI compilation.

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <mutex>
#include <functional>
#include <atomic>
#include <iterator>

// Forward declarations
namespace v8 {
namespace base {
    // Forward declare base types
    template<typename T> struct hash;
    template<typename T> struct bit_hash;
    template<typename T> struct bit_equal_to;
    template<typename T, typename U = int, typename V = U> class Flags;
    template<typename T, typename U> class EnumSet;
    class AddressRegion;
    template<typename T> using is_trivially_copyable = std::is_trivially_copyable<T>;
    using OnceType = std::atomic<int>;
    using AtomicWord = intptr_t;
    
    // Memory allocation types
    enum class PageInitializationMode { kUninitialized, kZeroInitialized };
    enum class PageFreeingMode { kMakeInaccessible, kDiscard };
    
    struct AllocationResult {
        void* ptr;
        size_t size;
        AllocationResult() : ptr(nullptr), size(0) {}
        AllocationResult(void* p, size_t s) : ptr(p), size(s) {}
    };
    
    // Function declarations  
    void* Malloc(size_t size);
    void Free(void* ptr);
    AllocationResult AllocatePages(size_t size);
    void FreePages(void* address, size_t size);
    
    size_t hash_combine(size_t seed, size_t hash);
    
    namespace bits {
        unsigned CountLeadingZeros(uint32_t value);
        unsigned CountPopulation(uint32_t value); 
        unsigned CountPopulation(uint16_t value);
    }
}
}

// Create alias mappings in v8::internal::base to resolve namespace conflicts
namespace v8 {
namespace internal {
namespace base {

    // Type aliases to resolve namespace conflicts
    template<typename T>
    using hash = ::v8::base::hash<T>;
    
    template<typename T>
    using bit_hash = ::v8::base::bit_hash<T>;
    
    template<typename T>
    using bit_equal_to = ::v8::base::bit_equal_to<T>;
    
    template<typename T, typename U = int, typename V = U>
    using Flags = ::v8::base::Flags<T, U, V>;
    
    template<typename T, typename U>
    using EnumSet = ::v8::base::EnumSet<T, U>;
    
    using AddressRegion = ::v8::base::AddressRegion;
    
    template<typename T>
    using is_trivially_copyable = ::v8::base::is_trivially_copyable<T>;
    
    using PageInitializationMode = ::v8::base::PageInitializationMode;
    using PageFreeingMode = ::v8::base::PageFreeingMode;
    
    using AllocationResult = ::v8::base::AllocationResult;
    
    using OnceType = ::v8::base::OnceType;
    using AtomicWord = ::v8::base::AtomicWord;
    
    // Function aliases
    inline void* Malloc(size_t size) { return ::v8::base::Malloc(size); }
    inline void Free(void* ptr) { ::v8::base::Free(ptr); }
    inline AllocationResult AllocatePages(size_t size) { return ::v8::base::AllocatePages(size); }
    inline void FreePages(void* address, size_t size) { ::v8::base::FreePages(address, size); }
    
    inline size_t hash_combine(size_t seed, size_t hash) {
        return ::v8::base::hash_combine(seed, hash);
    }
    
    namespace bits {
        inline unsigned CountLeadingZeros(uint32_t value) {
            return ::v8::base::bits::CountLeadingZeros(value);
        }
        inline unsigned CountPopulation(uint32_t value) {
            return ::v8::base::bits::CountPopulation(value);
        }
        inline unsigned CountPopulation(uint16_t value) {
            return ::v8::base::bits::CountPopulation(value);
        }
    }
}
}
}

// Fix for missing iterator template in base namespace
namespace v8 {
namespace internal {  
namespace base {
    template<typename Category, typename T, typename Distance = std::ptrdiff_t, 
             typename Pointer = T*, typename Reference = T&>
    using iterator = std::iterator<Category, T, Distance, Pointer, Reference>;
}
}
}

// Fix for SegmentedTable FreelistHead missing type
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

// Fix mutex method names for WASI
#ifdef __wasi__
namespace std {
    class mutex {
        std::mutex impl_;
    public:
        void lock() { impl_.lock(); }
        void unlock() { impl_.unlock(); }
        
        // Add V8-style methods
        void Lock() { lock(); }
        void Unlock() { unlock(); }
        
        bool try_lock() { return impl_.try_lock(); }
    };
}
#endif

#endif // __wasi__

#endif // WASI_NAMESPACE_FIXES_H_