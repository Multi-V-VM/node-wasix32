#ifndef INCLUDE_V8_LOCAL_HANDLE_H_
#define INCLUDE_V8_LOCAL_HANDLE_H_

#ifdef __wasi__
// Include Local template definition first
#include "wasi/wasi-v8-missing-types.h"
#endif

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

#ifndef __wasi__
// Original Local template definition for non-WASI builds
template <typename T>
class Local {
  // ... original implementation
};
#endif

}  // namespace v8

#endif  // INCLUDE_V8_LOCAL_HANDLE_H_
