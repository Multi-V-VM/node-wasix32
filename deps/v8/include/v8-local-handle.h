#ifndef INCLUDE_V8_LOCAL_HANDLE_H_
#define INCLUDE_V8_LOCAL_HANDLE_H_

// Always include the lightweight Local<T> implementation to avoid
// dependency cycles and to provide a usable Local in host builds too.
// Avoid including std-namespace-fix from here to prevent accidental std
// polyfills being processed inside a namespace v8 block in some TUs.
// Note: Do NOT include wasi-v8-missing-types.h here - it creates circular dependencies
// through src/base/hashing.h -> v8-internal.h -> platform.h chain
// Local<> for WASI is already defined in v8-data.h

#include <stddef.h>
#include <stdint.h>

#include <limits>
#include <type_traits>
#include <utility>

#include "v8-data.h"  // Provides the lightweight Local<T> for both host and WASI
#include "v8config.h"

#ifdef __wasi__
#include "wasi/v8-wasi-compat.h"
#include "v8-forward.h"
// Provide aliases expected by internal code paths
namespace v8 {
template <class T>
using LocalBase = Local<T>;
namespace internal {
template <class T>
using LocalUnchecked = ::v8::Local<T>;
}  // namespace internal
}  // namespace v8
#endif  // __wasi__

namespace v8 {
// Local<T> is defined in v8-data.h for WASI builds
// Non-WASI builds use the implementation from v8-internal.h
}  // namespace v8


#endif  // INCLUDE_V8_LOCAL_HANDLE_H_
