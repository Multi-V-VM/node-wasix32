#ifndef WASI_V8_MISSING_TYPES_H_
#define WASI_V8_MISSING_TYPES_H_

#ifdef __wasi__

// This header imports types from v8::base into v8::internal::base namespace
// for WASI builds where they are otherwise missing

#include <cstdint>
#include <cstdio>
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
#include "src/base/bits.h"
#include "src/base/atomicops.h"
#include "src/base/atomic-utils.h"
#include "src/base/platform/mutex.h"
#include "src/base/vector.h"
#include "src/base/address-region.h"
#include "src/base/virtual-address-space.h"
#include "src/base/discriminated-union.h"
#include "src/base/strings.h"
#include "src/base/hashing.h"
#include "src/base/container-utils.h"
#include "src/base/template-utils.h"
#include "src/base/platform/memory.h"
#include "src/base/platform/wrappers.h"

// On some include paths these headers may be parsed while already inside a
// `namespace v8 {}` block. In that case, qualified declarations inside the
// base headers end up under `::v8::v8::base::...`. Use a helper macro to refer
// to the effective base namespace consistently.
#ifndef V8_BASE_NS
// Always reference the canonical base namespace explicitly to avoid
// accidental nesting into `v8::v8::base` when this header is included
// from within a `namespace v8 {}` block.
#define V8_BASE_NS ::v8::base
#endif

namespace v8 {
namespace internal {
namespace base {

// ============================================================================
// DefaultAllocationPolicy - Simple allocator policy compatible with V8 usage
// ============================================================================

struct DefaultAllocationPolicy {
  static void* Allocate(size_t size) { return ::malloc(size); }
  static void Free(void* ptr) { ::free(ptr); }

  template <typename T, typename TypeTag = T[]>
  V8_INLINE T* AllocateArray(size_t length) {
    return static_cast<T*>(Allocate(length * sizeof(T)));
  }

  template <typename T, typename TypeTag = T[]>
  V8_INLINE void DeleteArray(T* p, size_t /*length*/) {
    Free(static_cast<void*>(p));
  }
};

// ============================================================================
// HashMap and related types - Import from v8::base
// ============================================================================

// TemplateHashMapImpl takes 4 params: Key, Value, MatchFun, AllocationPolicy
template <typename Key, typename Value, typename MatchFun, typename AllocationPolicy>
using TemplateHashMapImpl = V8_BASE_NS::TemplateHashMapImpl<Key, Value, MatchFun, AllocationPolicy>;

// TemplateHashMap also takes 4 params
template <typename Key, typename Value, typename MatchFun, typename AllocationPolicy>
using TemplateHashMap = V8_BASE_NS::TemplateHashMap<Key, Value, MatchFun, AllocationPolicy>;

// These template classes only take AllocationPolicy as template parameter
template <typename AllocationPolicy>
using PointerTemplateHashMapImpl = V8_BASE_NS::PointerTemplateHashMapImpl<AllocationPolicy>;

template <typename AllocationPolicy>
using CustomMatcherTemplateHashMapImpl = V8_BASE_NS::CustomMatcherTemplateHashMapImpl<AllocationPolicy>;

// HashMap and CustomMatcherHashMap are shortcuts with DefaultAllocationPolicy
using HashMap = V8_BASE_NS::HashMap;
using CustomMatcherHashMap = V8_BASE_NS::CustomMatcherHashMap;

// KeyEqualityMatcher for HashMap
template <typename Key>
using KeyEqualityMatcher = V8_BASE_NS::KeyEqualityMatcher<Key>;

// ============================================================================
// PointerWithPayload - Import from v8::base
// ============================================================================

template <typename PointerType, typename PayloadType, int NumPayloadBits>
using PointerWithPayload = V8_BASE_NS::PointerWithPayload<PointerType, PayloadType, NumPayloadBits>;

// ============================================================================
// ThreadedList and ThreadedListTraits - Import from v8::base
// ============================================================================

template <typename T>
using ThreadedListTraits = V8_BASE_NS::ThreadedListTraits<T>;

template <typename T, typename BaseClass = ThreadedListTraits<T>>
using ThreadedList = V8_BASE_NS::ThreadedList<T, BaseClass>;

// ============================================================================
// BitSetComputer - Import from v8::base
// ============================================================================

template <typename T, int kFieldSize, int kShift, typename U>
using BitSetComputer = V8_BASE_NS::BitSetComputer<T, kFieldSize, kShift, U>;

// 64-bit BitField alias used in some internal headers
template <typename T, int shift, int size>
using BitField64 = V8_BASE_NS::BitField64<T, shift, size>;

// ============================================================================
// Vector types - Import from the canonical ::v8::base
// ============================================================================
// Note: Vector is imported by globals.h; only add OwnedVector here
using ::v8::base::VectorOf;
template <typename T>
using OwnedVector = ::v8::base::OwnedVector<T>;
using ::v8::base::make_array;

// ============================================================================
// Additional utility types
// ============================================================================

// BitFieldUnion for bit manipulation
template <typename T, int shift, int size>
using BitFieldUnion = V8_BASE_NS::BitField<T, shift, size>;

// DiscriminatedUnion - Import from v8::base
template <typename TagEnum, typename... Ts>
using DiscriminatedUnion = V8_BASE_NS::DiscriminatedUnion<TagEnum, Ts...>;

// Atomic operations can be referenced directly via ::v8::base::* where needed

// Hashing helpers are bridged by src/common/globals.h for WASI builds.

// bits::* helpers bridged into v8::internal::base::bits
namespace bits {
using ::v8::base::bits::CountTrailingZeros;
using ::v8::base::bits::CountTrailingZerosNonZero;
using ::v8::base::bits::CountLeadingZeros;
using ::v8::base::bits::CountLeadingZeros32;
using ::v8::base::bits::CountPopulation;
using ::v8::base::bits::IsPowerOfTwo;
using ::v8::base::bits::ReverseBytes;
using ::v8::base::bits::RoundUpToPowerOfTwo;
// Some internal callers reference the explicit 32/64-bit variants.
using ::v8::base::bits::RoundUpToPowerOfTwo32;
using ::v8::base::bits::RoundUpToPowerOfTwo64;
}  // namespace bits

// ============================================================================
// Mutex and locking - Provide minimal WASI stubs
// ============================================================================
// On WASI, threading may be unavailable or limited. To avoid tight coupling
// with V8's internal mutex implementations and include-order pitfalls, define
// lightweight stand-ins within v8::internal::base. These satisfy type and
// API expectations for single-threaded builds.

using ::v8::base::Mutex;
using ::v8::base::RecursiveMutex;
using ::v8::base::MutexGuard;
using ::v8::base::RecursiveMutexGuard;
// ============================================================================
// Memory and allocation helpers
// ============================================================================

// AddressRegion aliases (commonly referenced under v8::internal::base)
using ::v8::base::AddressRegion;
using ::v8::base::AddressRegionOf;
// Address space allocator/virtual address space types are provided by
// platform headers and/or src/common/globals.h for WASI builds. Avoid
// introducing aliases here to prevent conflicts.

// ============================================================================
// Utility functions - Import from v8::base
// ============================================================================

// Vector and string helpers commonly referenced from internal::base
// (Most of these are also bridged by src/common/globals.h; keep the set
// here minimal to avoid duplicate using-declarations.)
using ::v8::base::SNPrintF;
using ::v8::base::VSNPrintF;

// Aligned/memory helpers and file wrappers
using ::v8::base::AlignedAlloc;
using ::v8::base::AlignedFree;

// Avoid defining LeakyObject, AllocationResult, or AllocateAtLeast here to
// prevent ODR/alias conflicts with upstream base implementations. These are
// available under ::v8::base and imported by src/common/globals.h.

// ============================================================================
// Iterator type aliases for template compatibility
// ============================================================================

// Provide iterator alias that forwards to std::iterator so
// internal code using v8::internal::base::iterator resolves correctly.
// Note: std::iterator is deprecated in C++17 but still available.
template <typename Category, typename T, typename Distance = ::std::ptrdiff_t,
          typename Pointer = T*, typename Reference = T&>
struct iterator {
  using iterator_category = Category;
  using value_type = T;
  using difference_type = Distance;
  using pointer = Pointer;
  using reference = Reference;
};

// File operations wrapper is provided by ::v8::base::Fclose in wrappers.h

// Atomic type selector template - forward alias to base implementation
template <int ByteWidth>
using AtomicTypeFromByteWidth = ::v8::base::AtomicTypeFromByteWidth<ByteWidth>;

// Direct aliases for atomic types referenced by name
using ::v8::base::Atomic8;
using ::v8::base::Atomic16;
using ::v8::base::Atomic32;
#if defined(V8_HOST_ARCH_64_BIT)
using ::v8::base::Atomic64;
#endif

}  // namespace base
}  // namespace internal
}  // namespace v8

// Note: Local<> implementation has been moved to v8-data.h
// Note: Global Address alias has been moved to v8-data.h

#endif  // __wasi__

#endif  // WASI_V8_MISSING_TYPES_H_
