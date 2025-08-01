#ifndef V8_WASI_COMPAT_SIMPLIFIED_H_
#define V8_WASI_COMPAT_SIMPLIFIED_H_

#ifdef __wasi__

// Simplified WASI compatibility header for V8
// This header provides minimal stubs and definitions for WASI builds

#include <cstdint>
#include <cstddef>

namespace v8 {

// Forward declarations to avoid circular dependencies
class Isolate;
template<typename T> class Local;
template<typename T> class MaybeLocal;

} // namespace v8

#endif // __wasi__

#endif // V8_WASI_COMPAT_SIMPLIFIED_H_