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
#include "src/base/atomicops.h"
#include "src/base/atomic-utils.h"
#include "src/base/platform/mutex.h"
#include "src/base/vector.h"
#include "src/base/address-region.h"
#include "src/base/discriminated-union.h"
#include "src/base/strings.h"
#include "src/base/hashing.h"
#include "src/base/container-utils.h"

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

// TemplateHashMapImpl takes 4 params: Key, Value, MatchFun, AllocationPolicy
template <typename Key, typename Value, typename MatchFun, typename AllocationPolicy>
using TemplateHashMapImpl = ::v8::base::TemplateHashMapImpl<Key, Value, MatchFun, AllocationPolicy>;

// TemplateHashMap also takes 4 params
template <typename Key, typename Value, typename MatchFun, typename AllocationPolicy>
using TemplateHashMap = ::v8::base::TemplateHashMap<Key, Value, MatchFun, AllocationPolicy>;

// These template classes only take AllocationPolicy as template parameter
template <typename AllocationPolicy>
using PointerTemplateHashMapImpl = ::v8::base::PointerTemplateHashMapImpl<AllocationPolicy>;

template <typename AllocationPolicy>
using CustomMatcherTemplateHashMapImpl = ::v8::base::CustomMatcherTemplateHashMapImpl<AllocationPolicy>;

// HashMap and CustomMatcherHashMap are shortcuts with DefaultAllocationPolicy
using HashMap = ::v8::base::HashMap;
using CustomMatcherHashMap = ::v8::base::CustomMatcherHashMap;

// KeyEqualityMatcher for HashMap
template <typename Key>
using KeyEqualityMatcher = ::v8::base::KeyEqualityMatcher<Key>;

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
// Vector types - Import from v8::base
// ============================================================================
// Note: Vector is imported by globals.h, so we only add OwnedVector here

template <typename T>
using OwnedVector = ::v8::base::OwnedVector<T>;

// Owned smart pointer with explicit template instantiation
template <typename T>
class Owned : public ::v8::base::OwnedVector<T> {
 public:
  using ::v8::base::OwnedVector<T>::OwnedVector;
};

// ============================================================================
// Additional utility types
// ============================================================================

// BitFieldUnion for bit manipulation
template <typename T, int shift, int size>
using BitFieldUnion = ::v8::base::BitField<T, shift, size>;

// DiscriminatedUnion - Import from v8::base
template <typename TagEnum, typename... Ts>
using DiscriminatedUnion = ::v8::base::DiscriminatedUnion<TagEnum, Ts...>;

// ============================================================================
// Atomic operations - Import from v8::base
// ============================================================================
// Note: AtomicWord, AsAtomicWord, AsAtomicPointer are imported by globals.h

using Atomic8 = ::v8::base::Atomic8;
using Atomic16 = ::v8::base::Atomic16;
using Atomic32 = ::v8::base::Atomic32;

#if defined(V8_HOST_ARCH_64_BIT)
using Atomic64 = ::v8::base::Atomic64;
#endif

using AsAtomic8 = ::v8::base::AsAtomic8;
using AsAtomic16 = ::v8::base::AsAtomic16;
using AsAtomic32 = ::v8::base::AsAtomic32;

// Atomic operations functions are available through ADL

// ============================================================================
// Mutex and locking - Import from v8::base
// ============================================================================

using Mutex = ::v8::base::Mutex;
using RecursiveMutex = ::v8::base::RecursiveMutex;
using MutexGuard = ::v8::base::MutexGuard;
using MutexGuardIf = ::v8::base::MutexGuardIf;
using LazyMutex = ::v8::base::LazyMutex;

// ============================================================================
// Memory and allocation helpers
// ============================================================================

using AddressRegion = ::v8::base::AddressRegion;

// ============================================================================
// Utility functions - Import from v8::base
// ============================================================================

using ::v8::base::VectorOf;
using ::v8::base::SNPrintF;
using ::v8::base::VSNPrintF;
using ::v8::base::vector_append;

// hash_combine implementations - inline the logic to avoid namespace issues
inline size_t hash_combine(size_t seed, size_t hash) {
#if V8_HOST_ARCH_32_BIT
  const uint32_t c1 = 0xCC9E2D51;
  const uint32_t c2 = 0x1B873593;

  hash *= c1;
  hash = ::v8::base::bits::RotateRight32(hash, 15);
  hash *= c2;

  seed ^= hash;
  seed = ::v8::base::bits::RotateRight32(seed, 13);
  seed = seed * 5 + 0xE6546B64;
#else
  const uint64_t m = uint64_t{0xC6A4A7935BD1E995};
  const uint32_t r = 47;

  hash *= m;
  hash ^= hash >> r;
  hash *= m;

  seed ^= hash;
  seed *= m;
#endif  // V8_HOST_ARCH_32_BIT
  return seed;
}

// Helper to hash a single value using std::hash
template <typename T>
inline size_t hash_single_value(const T& v) {
  return std::hash<T>{}(v);
}

// Variadic hash_combine using fold expression
template <typename... Ts>
inline size_t hash_combine(Ts const&... vs) {
  size_t hash_val = 0;
  (..., (hash_val = hash_combine(hash_val, hash_single_value(vs))));
  return hash_val;
}

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

// Note: Local<> implementation has been moved to v8-data.h
// Note: Global Address alias has been moved to v8-data.h

#endif  // __wasi__

#endif  // WASI_V8_MISSING_TYPES_H_
