#ifndef WASI_ABSEIL_NAMESPACE_FIX_H_
#define WASI_ABSEIL_NAMESPACE_FIX_H_

#ifdef __wasi__

// Abseil sources are frequently included while wrapped inside namespace v8 or
// v8::internal. Provide a lightweight bridge so that unqualified lookups of
// "std" resolve to the global standard library instead of creating nested
// v8::std or v8::internal::std namespaces.

namespace v8 {
namespace std {
using namespace ::std;
}  // namespace std
}  // namespace v8

#endif  // __wasi__

#endif  // WASI_ABSEIL_NAMESPACE_FIX_H_
