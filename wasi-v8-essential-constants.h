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
#include "src/base/memory.h"

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

// Bridge a few v8::base::bits helpers into v8::internal::base::bits for WASI
// so unqualified internal uses compile.
namespace v8 { namespace internal { namespace base { namespace bits {
using ::v8::base::bits::RoundUpToPowerOfTwo32;
using ::v8::base::bits::RoundUpToPowerOfTwo64;
} } } }

// WASI compatibility header: keep minimal constants only.

#endif  // __wasi__

#endif  // WASI_V8_ESSENTIAL_CONSTANTS_H_
