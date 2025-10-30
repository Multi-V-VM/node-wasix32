#ifndef V8_WASI_BASE_PREINCLUDE_H_
#define V8_WASI_BASE_PREINCLUDE_H_

// Always include core V8 base headers at global scope on WASI.
// This prevents accidental nesting (e.g. ::v8::v8::base) when these
// headers are pulled from within a `namespace v8 {}` block in some TUs.

#if defined(__wasi__)

// Minimal standard headers used by base/
#include <cstdint>
#include <cstddef>

// Enable safe installation of std:: polyfills in WASI only at TU global scope.
// Use a distinct macro to avoid accidental injection when these headers are
// included from nested namespace contexts later on.
#ifndef V8_WASI_STD_POLYFILLS_GLOBAL_SCOPE
#define V8_WASI_STD_POLYFILLS_GLOBAL_SCOPE 1
#endif
// Install a minimal set of C++20 polyfills where the SDK may be lacking.
#include "wasi/concepts-fix.h"
#include "wasi/cpp20-compat.h"
// Avoid ranges polyfill unless needed by specific TUs.

// Do not include any src/base/* headers here. This header is intentionally
// lightweight and safe to include from any context without risking namespace
// pollution (e.g., creating v8::v8::base).

#endif  // defined(__wasi__)

#if defined(__wasi__)
// Do not inject nested v8 aliasing; it can collide with legitimate
// namespace declarations in translation units. Keep global state simple.
#endif

#endif  // V8_WASI_BASE_PREINCLUDE_H_
