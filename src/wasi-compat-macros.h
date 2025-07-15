// WASI compatibility macros for Node.js
#ifndef SRC_WASI_COMPAT_MACROS_H_
#define SRC_WASI_COMPAT_MACROS_H_

#ifdef __wasi__

// Stub out V8 features not available in WASI

// SealHandleScope - just use regular HandleScope
#define SealHandleScope HandleScope

// Isolate methods that don't exist in WASI
#define WASI_STUB_ISOLATE_METHOD(isolate, method, ...) \
  do { /* No-op */ } while(0)

// Redefine problematic method calls
#define AddNearHeapLimitCallback(...) WASI_STUB_ISOLATE_METHOD(this, AddNearHeapLimitCallback, __VA_ARGS__)
#define RemoveNearHeapLimitCallback(...) WASI_STUB_ISOLATE_METHOD(this, RemoveNearHeapLimitCallback, __VA_ARGS__)
#define SetCaptureStackTraceForUncaughtExceptions(...) WASI_STUB_ISOLATE_METHOD(this, SetCaptureStackTraceForUncaughtExceptions, __VA_ARGS__)

// For static methods
#define WASI_ISOLATE_ALLOCATE() (Isolate::New(Isolate::CreateParams()))

// For Isolate::CreateParams - use a wrapper
#define WASI_CREATE_PARAMS(params) \
  do { \
    /* WASI doesn't support these fields */ \
    (void)(params); \
  } while(0)

#endif // __wasi__

#endif // SRC_WASI_COMPAT_MACROS_H_