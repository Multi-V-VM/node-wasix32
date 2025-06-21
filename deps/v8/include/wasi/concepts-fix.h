#ifndef WASI_CONCEPTS_FIX_H_
#define WASI_CONCEPTS_FIX_H_

#ifdef __wasi__
// Minimal WASI compatibility - only replace concepts
#undef concept
#undef requires
#define concept bool
#define requires(...) true
#define SIMDUTF_NO_CONCEPTS 1
#endif // __wasi__

#endif // WASI_CONCEPTS_FIX_H_
