#ifndef V8_INCLUDE_WASI_STD_NAMESPACE_FIX_H_
#define V8_INCLUDE_WASI_STD_NAMESPACE_FIX_H_

// Include ranges compatibility layer
#ifdef __wasi__
// Avoid pulling in ranges polyfill here to prevent injecting std:: symbols
// when this header is included under a namespace scope.
// #include "wasi/ranges-compat.h"
// Keep crdtp namespace fixes if needed; it does not modify std::
#include "wasi/crdtp-namespace-fix.h"
#endif

// Avoid forward-declaring V8 types here. When this header is included from
// within a `namespace v8 {}` block (which some internal files do), local
// forward declarations would become `v8::v8::...`, breaking qualified lookups
// such as `v8::String`. Rely on actual V8 public headers to provide forward
// declarations where needed.

// Intentionally do not include any <...> standard headers here. Including
// them from within a namespace v8 context in some translation units would
// create nested v8::std symbols and break libc++ lookups.

// Do not inject std::hash specializations here; libc++/libstdc++ already provide
// them, and redefining is undefined. If an environment lacks them, handle it in
// a targeted layer rather than globally.

// Avoid injecting std symbols here; libc++/libstdc++ provide these features
// (endian, ranges) in recent WASI SDKs. When unavailable, prefer targeted
// local shims instead of global namespace edits to prevent v8::std leakage.

// Do NOT create nested aliases like v8::std or v8::v8::std.
// They introduce a nested `v8::v8` namespace and break qualified lookups
// (e.g. resolving `std::chrono` as `v8::std::chrono`). Standard library
// symbols must be referenced as `::std::...` or `std::...` from any namespace.

// Intentionally avoid injecting any `std` aliases. If callers require
// compatibility shims, they should use fully qualified `::std::` names.

#endif  // V8_INCLUDE_WASI_STD_NAMESPACE_FIX_H_
