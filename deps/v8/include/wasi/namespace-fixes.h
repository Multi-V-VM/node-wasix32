#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef WASI_NAMESPACE_FIXES_H
#define WASI_NAMESPACE_FIXES_H

#ifdef __wasi__

// Remove all namespace aliases to avoid conflicts with WASI libc++
// Instead, we'll use direct type definitions and includes

#include <cstddef>
#include <cstdint>
#include <type_traits>

// Include WASI-specific compatibility headers
#include "type-traits-compat.h"

#endif  // __wasi__
#endif  // WASI_NAMESPACE_FIXES_H