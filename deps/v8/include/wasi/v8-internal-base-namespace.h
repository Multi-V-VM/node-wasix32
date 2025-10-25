#ifndef V8_INCLUDE_WASI_V8_INTERNAL_BASE_NAMESPACE_H_
#define V8_INCLUDE_WASI_V8_INTERNAL_BASE_NAMESPACE_H_

#ifdef __wasi__
// No-op: avoid introducing aliases that may be seen before definitions and
// cause cascading name-resolution errors. Where needed, use fully qualified
// ::v8::base:: names, or include src/base/hashing.h in the relevant TU.
#endif  // __wasi__
#endif  // V8_INCLUDE_WASI_V8_INTERNAL_BASE_NAMESPACE_H_
