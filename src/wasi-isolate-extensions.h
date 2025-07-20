#ifndef SRC_WASI_ISOLATE_EXTENSIONS_H_
#define SRC_WASI_ISOLATE_EXTENSIONS_H_

#ifdef __wasi__

// WASI compatibility extensions for Isolate methods

namespace v8 {
class Isolate;
}

// Extension functions to handle missing Isolate methods in WASI
namespace node {
namespace wasi_compat {

// Check if isolate is in context (WASI always returns true)
inline bool IsolateInContext(v8::Isolate* isolate) {
  return true;
}

} // namespace wasi_compat
} // namespace node

#endif // __wasi__

#endif // SRC_WASI_ISOLATE_EXTENSIONS_H_