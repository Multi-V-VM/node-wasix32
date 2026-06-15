#ifndef WASI_V8_ESSENTIAL_CONSTANTS_H_
#define WASI_V8_ESSENTIAL_CONSTANTS_H_

#if defined(__wasi__) || defined(V8_USING_WASI_SHIMS)

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
#include "src/base/vlq.h"

// Include missing types implementation
#include "wasi-v8-missing-types.h"
#include "v8-profiler-wasi-stubs.h"
#include "v8-api-constants-wasi.h"
// NOTE: cppgc-wasi-compat.h removed - causes conflicts with cppgc/heap.h
// The real cppgc headers should be used instead.

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

// CppHeap pointer tag moved to v8-sandbox.h where CppHeapPointerTagRange is defined

}  // namespace internal
}  // namespace v8

#endif  // defined(__wasi__) || defined(V8_USING_WASI_SHIMS)

#endif  // WASI_V8_ESSENTIAL_CONSTANTS_H_
