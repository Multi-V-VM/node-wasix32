#ifndef V8_SRC_BASE_BITS_H_
#define V8_SRC_BASE_BITS_H_

#ifdef __wasi__
// Include v8-internal.h which has the bits namespace definitions
#include "../../include/v8-internal.h"
// Include our additional bits fixes
#include "../../../../wasi-v8-bits-fixes.h"
#else
// Original bits.h content would go here for non-WASI builds
#include <cstdint>
namespace v8 {
namespace base {
namespace bits {
// Non-WASI implementations would be here
}  // namespace bits
}  // namespace base
}  // namespace v8
#endif

#endif  // V8_SRC_BASE_BITS_H_
