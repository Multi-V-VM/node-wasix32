// WASI C++ runtime stubs
// These stubs provide minimal implementations for C++ runtime features
// that are not available or not fully supported in WASI

#ifdef __wasi__

#include <cstdlib>
#include <cstdio>

// C++ exception handling stubs for WASI
// WASI does not support C++ exceptions, so these stubs abort on use
extern "C" {

void* __cxa_allocate_exception(size_t size) {
  fprintf(stderr, "C++ exceptions not supported in WASI\n");
  abort();
}

// OpenSSL stubs for missing functions
// EVP_MD is an opaque type so we use void*
void EVP_MD_free(void* md) {
  // No-op stub - in OpenSSL 3.x this would free a fetched digest
}

[[noreturn]] void __cxa_throw(void* thrown_exception, void* tinfo, void (*dest)(void*)) {
  fprintf(stderr, "C++ exceptions not supported in WASI\n");
  abort();
}

void* __cxa_begin_catch(void* exception) {
  return nullptr;
}

void __cxa_end_catch() {
  // No-op
}

[[noreturn]] void __cxa_rethrow() {
  fprintf(stderr, "C++ rethrow not supported in WASI\n");
  abort();
}

// Type info pointers that may be needed
void* __cxa_current_exception_type() {
  return nullptr;
}

}  // extern "C"

#endif  // __wasi__
