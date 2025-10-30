#ifndef V8_WASI_NAMESPACE_FIX_H_
#define V8_WASI_NAMESPACE_FIX_H_

#ifdef __wasi__
// Intentionally kept minimal. Avoid adding namespace aliases that can conflict
// with real declarations (e.g., defining v8::v8). This header mainly exists as
// a hook to include from sites that require an early, no-op bridge.
#endif  // __wasi__

#endif  // V8_WASI_NAMESPACE_FIX_H_
