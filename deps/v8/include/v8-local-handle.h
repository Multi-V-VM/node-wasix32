#ifndef INCLUDE_V8_LOCAL_HANDLE_H_
#define INCLUDE_V8_LOCAL_HANDLE_H_

// Always include the lightweight Local<T> implementation to avoid
// dependency cycles and to provide a usable Local in host builds too.
#include "wasi/std-namespace-fix.h"
// Note: Do NOT include wasi-v8-missing-types.h here - it creates circular dependencies
// through src/base/hashing.h -> v8-internal.h -> platform.h chain
// Local<> for WASI is already defined in v8-data.h

#include <stddef.h>
#include <stdint.h>

#include <limits>
#include <type_traits>
#include <utility>

#ifndef __wasi__
// For non-WASI builds, include v8-internal.h for the real Local<> implementation
#include "v8-internal.h"
#endif

#include "v8config.h"

#ifdef __wasi__
#include "wasi/v8-wasi-compat.h"
#endif

namespace v8 {
// Local<T> is defined in v8-data.h for WASI builds
// Non-WASI builds use the implementation from v8-internal.h
}  // namespace v8


#endif  // INCLUDE_V8_LOCAL_HANDLE_H_
