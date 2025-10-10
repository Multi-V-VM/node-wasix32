#ifndef INCLUDE_V8_LOCAL_HANDLE_H_
#define INCLUDE_V8_LOCAL_HANDLE_H_

// Always include the lightweight Local<T> implementation to avoid
// dependency cycles and to provide a usable Local in host builds too.
#include "wasi/std-namespace-fix.h"
#include "wasi/wasi-v8-missing-types.h"

#include <stddef.h>
#include <stdint.h>

#include <limits>
#include <type_traits>
#include <utility>

#include "v8-internal.h"
#include "v8config.h"

#ifdef __wasi__
#include "wasi/v8-wasi-compat.h"
#endif

namespace v8 {
// Local<T> is defined in wasi/wasi-v8-missing-types.h for both WASI and
// non-WASI builds to keep public headers self-contained in this tree.
}  // namespace v8


#endif  // INCLUDE_V8_LOCAL_HANDLE_H_
