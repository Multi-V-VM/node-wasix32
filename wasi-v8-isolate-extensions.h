#ifndef WASI_V8_ISOLATE_EXTENSIONS_H_
#define WASI_V8_ISOLATE_EXTENSIONS_H_

// This file provides extensions to v8::Isolate for WASI builds
// Include this after v8.h when you need these methods

#ifdef __wasi__

#include "deps/v8/include/v8.h"

namespace v8 {

// Extension functions for Isolate methods that are missing in WASI builds
// These are standalone functions that take an Isolate* as the first parameter

inline void GetHeapStatistics_WASI(Isolate* isolate, HeapStatistics* heap_statistics) {
  // Initialize all fields to zero for WASI stub
  *heap_statistics = HeapStatistics();
}

inline void GetHeapCodeAndMetadataStatistics_WASI(Isolate* isolate, HeapCodeStatistics* code_statistics) {
  // Initialize all fields to zero for WASI stub
  *code_statistics = HeapCodeStatistics();
}

inline bool InContext_WASI(Isolate* isolate) {
  // WASI stub - assume we're always in a context
  return true;
}

inline uint64_t GetHashSeed_WASI(Isolate* isolate) {
  // WASI stub - return a fixed seed
  return 0x12345678;
}

inline void SetCaptureStackTraceForUncaughtExceptions_WASI(Isolate* isolate, bool capture) {
  // WASI stub - no-op
}

inline void SetPromiseHook_WASI(Isolate* isolate, PromiseHook hook) {
  // WASI stub - no-op
}

inline Local<Value> ThrowError_WASI(Isolate* isolate, Local<String> message) {
  // WASI stub - return empty
  return Local<Value>();
}

inline Local<Value> ThrowError_WASI(Isolate* isolate, const char* message) {
  // WASI stub - return empty
  return Local<Value>();
}

// Helper macros to redirect calls
#define GetHeapStatistics(isolate, stats) GetHeapStatistics_WASI(isolate, stats)
#define GetHeapCodeAndMetadataStatistics(isolate, stats) GetHeapCodeAndMetadataStatistics_WASI(isolate, stats)
#define InContext() InContext_WASI(this)
#define GetHashSeed() GetHashSeed_WASI(this)

} // namespace v8

#endif // __wasi__

#endif // WASI_V8_ISOLATE_EXTENSIONS_H_