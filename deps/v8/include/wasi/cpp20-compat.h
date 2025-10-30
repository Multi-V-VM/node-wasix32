#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_CPP20_COMPAT_H_
#define V8_WASI_CPP20_COMPAT_H_

#ifdef __wasi__

#include <type_traits>
#include <utility>
#include <functional>
#include <bit>

// IMPORTANT:
// This header is included very early and sometimes from within other
// namespaces in various translation units. To avoid accidentally creating
// nested namespaces like v8::base::std, do not open `namespace std` here.
// The WASI toolchain used by this project targets C++20; the standard library
// provides the necessary facilities (type traits, variable templates, etc.).
// If a specific translation unit truly lacks a feature, provide a narrowly
// scoped shim in that TU instead of injecting global polyfills.

#endif  // __wasi__

#endif  // V8_WASI_CPP20_COMPAT_H_
