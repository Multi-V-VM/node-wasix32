#ifndef V8_WASI_NAMESPACE_FIX_H_
#define V8_WASI_NAMESPACE_FIX_H_

#ifdef __wasi__

// Don't create namespace aliases here since types might not be defined yet
// Instead, let the code that needs v8::internal::v8 handle the missing namespace

#endif // __wasi__

#endif // V8_WASI_NAMESPACE_FIX_H_