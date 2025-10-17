#ifndef V8_WASI_NAMESPACE_FIX_H_
#define V8_WASI_NAMESPACE_FIX_H_

#ifdef __wasi__
// Intentionally empty: avoid creating nested namespace aliases like v8::v8 or
// v8::std which break qualified lookups within V8 public headers.
// If specific bridges are required, implement them locally in translation
// units to prevent global side effects during header inclusion.
#endif  // __wasi__

#endif  // V8_WASI_NAMESPACE_FIX_H_
