#ifndef WASI_V8_ESSENTIAL_CONSTANTS_H_
#define WASI_V8_ESSENTIAL_CONSTANTS_H_

#ifdef __wasi__

#include <cstdint>
#include <limits>

#include "src/base/bounds.h"
#include "src/base/hashing.h"
#include "src/base/numerics/safe_conversions.h"

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
using ::v8::base::hash_combine;
using ::v8::base::hash_value;
using ::v8::base::saturated_cast;

// Synchronization primitives
using ::v8::base::Mutex;
using ::v8::base::MutexGuard;
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
using ::v8::base::Hasher;
using ::v8::base::AddressRegion;

// Atomic primitives
using ::v8::base::Atomic32;

// Hash map utilities
using ::v8::base::CustomMatcherHashMap;

// Memory utilities
using ::v8::base::WriteLittleEndianValue;
using ::v8::base::ReadUnalignedValue;
using ::v8::base::WriteUnalignedValue;

// Timer utilities
using ::v8::base::ElapsedTimer;

}  // namespace base
}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // WASI_V8_ESSENTIAL_CONSTANTS_H_
