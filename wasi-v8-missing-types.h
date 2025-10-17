#ifndef WASI_V8_MISSING_TYPES_H_
#define WASI_V8_MISSING_TYPES_H_

#ifdef __wasi__

// This header imports types from v8::base into v8::internal::base namespace
// for WASI builds where they are otherwise missing

#include <cstdint>
#include <functional>
#include <iterator>
#include <utility>
#include <memory>
#include <cstdlib>

// Include the actual V8 base headers
#include "src/base/hashmap.h"
#include "src/base/pointer-with-payload.h"
#include "src/base/threaded-list.h"
#include "src/base/bit-field.h"

namespace v8 {
namespace internal {
namespace base {

// ============================================================================
// DefaultAllocationPolicy - Simple allocator policy using malloc/free
// ============================================================================

struct DefaultAllocationPolicy {
  static void* Allocate(size_t size) { return std::malloc(size); }
  static void Free(void* ptr) { std::free(ptr); }
};

// ============================================================================
// HashMap and related types - Import from v8::base
// ============================================================================

template <typename Key, typename Value, typename MatchFun, typename AllocationPolicy>
using HashMap = ::v8::base::HashMap<Key, Value, MatchFun, AllocationPolicy>;

template <typename Key, typename Value, typename AllocationPolicy>
using CustomMatcherHashMap = ::v8::base::CustomMatcherHashMap<Key, Value, AllocationPolicy>;

template <typename Key, typename Value, typename AllocationPolicy>
using PointerTemplateHashMapImpl = ::v8::base::PointerTemplateHashMapImpl<Key, Value, AllocationPolicy>;

template <typename Key, typename Value, typename MatchFun, typename AllocationPolicy>
using CustomMatcherTemplateHashMapImpl = ::v8::base::CustomMatcherTemplateHashMapImpl<Key, Value, MatchFun, AllocationPolicy>;

template <typename Key, typename Value, typename AllocationPolicy>
using TemplateHashMapImpl = ::v8::base::TemplateHashMapImpl<Key, Value, AllocationPolicy>;

// ============================================================================
// PointerWithPayload - Import from v8::base
// ============================================================================

template <typename PointerType, typename PayloadType, int NumPayloadBits>
using PointerWithPayload = ::v8::base::PointerWithPayload<PointerType, PayloadType, NumPayloadBits>;

// ============================================================================
// ThreadedList and ThreadedListTraits - Import from v8::base
// ============================================================================

template <typename T>
using ThreadedListTraits = ::v8::base::ThreadedListTraits<T>;

template <typename T, typename BaseClass = ThreadedListTraits<T>>
using ThreadedList = ::v8::base::ThreadedList<T, BaseClass>;

// ============================================================================
// BitSetComputer - Import from v8::base
// ============================================================================

template <typename T, int kFieldSize, int kShift, typename U>
using BitSetComputer = ::v8::base::BitSetComputer<T, kFieldSize, kShift, U>;

// ============================================================================
// Owned smart pointer - Import from v8::base if it exists
// ============================================================================

#ifdef V8_BASE_OWNED_H_
template <typename T>
using Owned = ::v8::base::Owned<T>;
#else
// Simple owned pointer implementation if v8::base::Owned doesn't exist
template <typename T>
using Owned = std::unique_ptr<T>;
#endif

// ============================================================================
// Additional utility types
// ============================================================================

// KeyEqualityMatcher for HashMap
template <typename Key>
struct KeyEqualityMatcher {
  bool operator()(const Key& a, const Key& b) const { return a == b; }
};

// BitFieldUnion for bit manipulation
template <typename T, int shift, int size>
using BitFieldUnion = ::v8::base::BitField<T, shift, size>;

// DiscriminatedUnion helper (stub)
template <typename... Types>
struct DiscriminatedUnion {
  // Stub implementation - may need expansion
};

// LeakyObject for objects that are never freed
template <typename T>
class LeakyObject {
 public:
  template <typename... Args>
  static T* Get(Args&&... args) {
    static T* instance = new T(std::forward<Args>(args)...);
    return instance;
  }
};

// AllocationResult for allocation APIs
struct AllocationResult {
  void* ptr;
  size_t size;

  bool IsSuccess() const { return ptr != nullptr; }
  void* ToAddress() const { return ptr; }
  operator bool() const { return IsSuccess(); }
};

// AllocateAtLeast for allocation with size feedback
template <typename Allocator>
struct AllocateAtLeast {
  static AllocationResult Allocate(size_t n) {
    void* ptr = Allocator::Allocate(n);
    return AllocationResult{ptr, n};
  }
};

// ============================================================================
// Iterator type aliases for template compatibility
// ============================================================================

// Provide iterator base for backward compatibility (std::iterator is deprecated in C++17, removed in C++20)
template <typename Category, typename T, typename Distance = std::ptrdiff_t,
          typename Pointer = T*, typename Reference = T&>
struct iterator {
  using iterator_category = Category;
  using value_type = T;
  using difference_type = Distance;
  using pointer = Pointer;
  using reference = Reference;
};

}  // namespace base
}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // WASI_V8_MISSING_TYPES_H_
