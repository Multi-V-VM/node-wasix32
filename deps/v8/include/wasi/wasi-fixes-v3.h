#ifndef V8_WASI_FIXES_V3_H_
#define V8_WASI_FIXES_V3_H_

// Include base fixes
#include "nuclear-fix.h"

// Fix for std namespace issues in sstream
#ifdef __wasi__
namespace v8 {
namespace internal {
// Re-export selected standard library utilities used throughout v8::internal.
using ::std::move;
using ::std::swap;
using ::std::addressof;
using ::std::max;
}  // namespace internal
}  // namespace v8
#endif

#endif  // V8_WASI_FIXES_V3_H_
