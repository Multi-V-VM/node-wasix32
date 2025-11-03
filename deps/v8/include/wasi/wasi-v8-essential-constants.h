#ifndef WASI_V8_ESSENTIAL_CONSTANTS_H_
#define WASI_V8_ESSENTIAL_CONSTANTS_H_

#ifdef __wasi__

#include <cstdint>
#include <limits>

#include "src/base/bounds.h"
#include "src/base/hashing.h"
#include "src/base/numerics/safe_conversions.h"
#include "src/base/address-region.h"
#include "src/base/platform/time.h"
#include "src/base/platform/platform.h"
#include "src/base/atomicops.h"
#include "src/base/lazy-instance.h"
#include "src/base/memory.h"
#include "src/base/vlq-base64.h"

// Include missing types implementation
#include "wasi-v8-missing-types.h"

namespace v8 {
namespace internal {

#ifndef kSmiTag
inline constexpr int kSmiTag = 0;
#endif

#ifndef kHeapObjectTag
inline constexpr int kHeapObjectTag = 1;
#endif

#ifndef kHeapObjectTagMask
inline constexpr intptr_t kHeapObjectTagMask = 3;
#endif

#ifndef kWeakHeapObjectTag
inline constexpr int kWeakHeapObjectTag = 3;
#endif

}  // namespace internal
}  // namespace v8

namespace v8 {
namespace internal {
namespace base {

// Import commonly used v8::base utilities into v8::internal::base namespace
using ::v8::base::IsInRange;
using ::v8::base::IsInBounds;
// Note: hash_combine is defined in wasi-v8-missing-types.h with wrapper functions
using ::v8::base::hash_value;
using ::v8::base::saturated_cast;

// Synchronization primitives
using ::v8::base::Mutex;
using ::v8::base::MutexGuard;
using ::v8::base::RecursiveMutex;
using ::v8::base::Semaphore;

// Bit field templates
using ::v8::base::BitField;
using ::v8::base::BitField8;
using ::v8::base::BitField64;

// Utility templates
using ::v8::base::bit_cast;
using ::v8::base::AtomicValue;

// Type utilities
using ::v8::base::uc16;
using ::v8::base::uc32;
using ::v8::base::Hasher;
using ::v8::base::double_to_uint64;

// Atomic primitives
using ::v8::base::Atomic32;
// Note: Atomic64 doesn't exist on wasm32
using ::v8::base::AtomicWord;

// Timer utilities
using ::v8::base::ElapsedTimer;
using ::v8::base::TimeDelta;
using ::v8::base::TimeTicks;

// Memory utilities
using ::v8::base::AddressRegion;
using ::v8::base::AddressRegionOf;

// Random number generation
using ::v8::base::RandomNumberGenerator;

// OS utilities
using ::v8::base::OS;

// Memory access utilities
using ::v8::base::ReadUnalignedValue;
using ::v8::base::WriteUnalignedValue;
using ::v8::base::ReadLittleEndianValue;
using ::v8::base::WriteLittleEndianValue;

// Atomic utilities
using ::v8::base::AsAtomic32;
using ::v8::base::Atomic16;
using ::v8::base::Relaxed_Store;
using ::v8::base::Acquire_Load;
using ::v8::base::Release_Store;
using ::v8::base::Relaxed_Load;
using ::v8::base::SeqCst_MemoryFence;
using ::v8::base::AcquireRelease_CompareAndSwap;
using ::v8::base::RecursiveMutexGuard;

// Atomic template utilities
template <typename T>
using AsAtomicImpl = ::v8::base::AsAtomicImpl<T>;

// Note: AtomicTypeFromByteWidth doesn't exist in v8::base

// Note: AsAtomic16 might be AsAtomicWord or similar
// Note: AlignedAlloc doesn't exist in v8::base

// Iterator utilities
using ::v8::base::iterator_range;
using ::v8::base::Reversed;
using ::v8::base::make_iterator_range;

// Platform utilities
using ::v8::base::LazyInstance;
using ::v8::base::LazyDynamicInstance;
using ::v8::base::OnceType;

// Thread utilities
using ::v8::base::Thread;

// Time utilities
using ::v8::base::Time;

// String utilities
using ::v8::base::CStrVector;
using ::v8::base::VSNPrintF;
using ::v8::base::SNPrintF;
using ::v8::base::ArrayVector;
using ::v8::base::StrNCpy;
using ::v8::base::StaticOneByteVector;
using ::v8::base::OwnedCopyOf;
using ::v8::base::VLQBase64Decode;

// Memory utilities
using ::v8::base::Memory;
using ::v8::base::FatalOOM;
using ::v8::base::Stack;
// In modern V8, these types are exposed in the public ::v8 namespace
using ::v8::VirtualAddressSpace;
using ::v8::PageAllocator;
using ::v8::base::Address;

// Platform utilities
using ::v8::base::TimezoneCache;

// Note: The following types don't exist in v8::base - they are in v8::internal or don't exist:
// DefaultAllocationPolicy, BoundedPageAllocator, PageInitializationMode, PageFreeingMode
// AlignedFree, Free, Fclose, AlignedAlloc
// HashMap, CustomMatcherHashMap, vector_append, double_to_uint64, OOMType
// PointerWithPayload, Owned, ThreadedList, ThreadedListTraits
// PointerTemplateHashMapImpl, CustomMatcherTemplateHashMapImpl, TemplateHashMapImpl
// KeyEqualityMatcher, BitSetComputer, BitFieldUnion, DiscriminatedUnion
// LeakyObject, AllocationResult, AllocateAtLeast, AtomicTypeFromByteWidth, AsAtomic16

// Type constants
using ::v8::base::kUC16Size;

// Numeric utilities
using ::v8::base::Double;

// Nested namespace for bits utilities
namespace bits {
using ::v8::base::bits::CountTrailingZeros;
using ::v8::base::bits::CountTrailingZerosNonZero;
using ::v8::base::bits::CountLeadingZeros;
using ::v8::base::bits::CountLeadingZeros32;
// Note: CountLeadingZeros64 doesn't exist on wasm32
using ::v8::base::bits::CountPopulation;
using ::v8::base::bits::IsPowerOfTwo;
// Note: WhichPowerOfTwo doesn't exist
using ::v8::base::bits::RoundUpToPowerOfTwo32;
using ::v8::base::bits::RoundUpToPowerOfTwo64;
// RoundDownToPowerOfTwo32 implementation
inline constexpr uint32_t RoundDownToPowerOfTwo32(uint32_t value) {
  if (value == 0) return 0;
  return uint32_t{1} << (31 - CountLeadingZeros32(value));
}
using ::v8::base::bits::RotateRight32;
using ::v8::base::bits::RotateRight64;
// Note: Signed*Overflow* functions don't exist - use templates instead
// Note: Unsigned*Overflow* functions don't exist - use templates instead
}  // namespace bits

// ieee754 namespace for math functions
namespace ieee754 {
using ::v8::base::ieee754::acos;
using ::v8::base::ieee754::acosh;
using ::v8::base::ieee754::asin;
using ::v8::base::ieee754::asinh;
using ::v8::base::ieee754::atan;
using ::v8::base::ieee754::atan2;
using ::v8::base::ieee754::atanh;
using ::v8::base::ieee754::cbrt;
using ::v8::base::ieee754::cos;
using ::v8::base::ieee754::cosh;
using ::v8::base::ieee754::exp;
using ::v8::base::ieee754::expm1;
using ::v8::base::ieee754::log;
using ::v8::base::ieee754::log1p;
using ::v8::base::ieee754::log2;
using ::v8::base::ieee754::log10;
using ::v8::base::ieee754::pow;
using ::v8::base::ieee754::sin;
using ::v8::base::ieee754::sinh;
using ::v8::base::ieee754::tan;
using ::v8::base::ieee754::tanh;
}  // namespace ieee754

// Expose template meta-programming utilities under v8::internal::base::tmp
// to satisfy code that references this namespace.
namespace tmp = ::v8::base::tmp;

}  // namespace base
}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // WASI_V8_ESSENTIAL_CONSTANTS_H_
