#ifndef WASI_V8_REGLIST_FIX_H_
#define WASI_V8_REGLIST_FIX_H_

// Fixes for register list types in WASI build
// This file provides compatibility when building with IA32 target arch

#include <cstdint>
#include <initializer_list>

#include "../../../../../wasi-v8-bits-fixes.h"

namespace v8 {
namespace internal {

// Don't redefine RegListBase if it's already defined in src/codegen/reglist.h
// The main reglist.h provides the template definition we need

}  // namespace internal
}  // namespace v8

#endif  // WASI_V8_REGLIST_FIX_H_
