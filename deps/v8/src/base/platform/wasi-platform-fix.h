#ifndef V8_BASE_PLATFORM_WASI_FIX_H_
#define V8_BASE_PLATFORM_WASI_FIX_H_

#ifdef __wasi__
#include <stddef.h>  // for size_t

// WASI doesn't have these
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 16384
#endif

#ifndef _SC_PAGESIZE
#define _SC_PAGESIZE 30
#endif

#ifndef _SC_NPROCESSORS_ONLN
#define _SC_NPROCESSORS_ONLN 84
#endif

// Forward declare to avoid conflicts
#ifdef __cplusplus
extern "C" {
#endif
long sysconf(int);
#ifdef __cplusplus
}
#endif

// Stub functions - only define if not already declared
#ifndef WASI_SYSCONF_DEFINED
#define WASI_SYSCONF_DEFINED
#ifdef __cplusplus
extern "C" {
#endif
// Will be defined in implementation
#ifdef __cplusplus
}
#endif
#endif

inline int pthread_attr_setstacksize(void* attr, size_t size) { return 0; }
inline int pthread_attr_init(void* attr) { return 0; }
inline int pthread_attr_destroy(void* attr) { return 0; }

#endif // __wasi__

#endif // V8_BASE_PLATFORM_WASI_FIX_H_
