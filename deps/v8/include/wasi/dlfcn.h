#ifndef WASI_DLFCN_H_
#define WASI_DLFCN_H_

#ifdef __wasi__

// WASI stub for dlfcn.h - dynamic loading is not supported in WASI
// This provides minimal definitions to allow compilation

#ifdef __cplusplus
extern "C" {
#endif

// Error modes for dlopen
#define RTLD_LAZY     0x00001  // Lazy function call binding
#define RTLD_NOW      0x00002  // Immediate function call binding
#define RTLD_GLOBAL   0x00100  // Symbols available for subsequently loaded objects
#define RTLD_LOCAL    0x00000  // Symbols not available for subsequently loaded objects

// Special handle values for dlsym
#define RTLD_DEFAULT  ((void *) 0)     // Use default search algorithm
#define RTLD_NEXT     ((void *) -1)    // Use next occurrence of symbol

// Stub functions - all return failure
static inline void* dlopen(const char* filename, int flags) {
    return NULL;  // WASI doesn't support dynamic loading
}

static inline void* dlsym(void* handle, const char* symbol) {
    return NULL;  // WASI doesn't support symbol lookup
}

static inline int dlclose(void* handle) {
    return -1;  // WASI doesn't support dynamic libraries
}

static inline char* dlerror(void) {
    return (char*)"Dynamic loading not supported in WASI";
}

// dladdr structure for symbol information
typedef struct {
    const char *dli_fname;  // Pathname of shared object
    void       *dli_fbase;  // Base address of shared object
    const char *dli_sname;  // Name of symbol
    void       *dli_saddr;  // Address of symbol
} Dl_info;

static inline int dladdr(const void* addr, Dl_info* info) {
    return 0;  // WASI doesn't support address lookup
}

#ifdef __cplusplus
}
#endif

#endif // __wasi__

#endif // WASI_DLFCN_H_