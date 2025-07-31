#ifndef WASI_NODE_COMPAT_H_
#define WASI_NODE_COMPAT_H_

#ifdef __wasi__

// Compatibility helpers for WASI build
namespace v8 {
class Isolate;

// Static method extensions for Isolate
namespace IsolateCompat {
inline Isolate* GetCurrent() { return nullptr; }
} // namespace IsolateCompat

// Redefine static calls to use our compat functions
#define V8_ISOLATE_GET_CURRENT() v8::IsolateCompat::GetCurrent()
#define V8_ISOLATE_GET_CURRENT_CONTEXT(isolate) v8::Isolate_GetCurrentContext(isolate)

} // namespace v8

// Compatibility for node.h
#define NODE_V8_UNIXTIME(date) 0.0

// Include filesystem stubs header for WASI instead of duplicating
#include "../wasi-filesystem-stubs.h"

// All filesystem functions are now defined in wasi-filesystem-stubs.h

#else // !__wasi__

#define V8_ISOLATE_GET_CURRENT() v8::Isolate::GetCurrent()

#endif // __wasi__

#endif // WASI_NODE_COMPAT_H_