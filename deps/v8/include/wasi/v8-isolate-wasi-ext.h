// WASI extensions for V8 Isolate
#ifndef V8_INCLUDE_WASI_V8_ISOLATE_WASI_EXT_H_
#define V8_INCLUDE_WASI_V8_ISOLATE_WASI_EXT_H_

#ifdef __wasi__

#include "v8.h"

namespace v8 {

// Extension traits for Isolate to add WASI-compatible methods
class IsolateWASIExt {
 public:
  // Static methods that act on an Isolate instance
  static void AddNearHeapLimitCallback(Isolate* isolate, 
                                       NearHeapLimitCallback callback, 
                                       void* data) {
    // No-op for WASI - heap limits are managed by the WASI runtime
  }
  
  static void RemoveNearHeapLimitCallback(Isolate* isolate,
                                         NearHeapLimitCallback callback,
                                         void* data) {
    // No-op for WASI
  }
  
  static void SetCaptureStackTraceForUncaughtExceptions(
      Isolate* isolate,
      bool capture,
      int frame_limit = 10,
      StackTrace::StackTraceOptions options = StackTrace::kOverview) {
    // No-op for WASI - stack traces are handled differently
  }
  
  static Isolate* Allocate() {
    // WASI doesn't support custom allocation - use New() instead
    return nullptr;
  }
};

// Extend Isolate::CreateParams for WASI
namespace internal {

class WASICreateParams : public Isolate::CreateParams {
 public:
  WASICreateParams() {
    // Initialize with WASI defaults
  }
  
  ArrayBuffer::Allocator* array_buffer_allocator = nullptr;
  const intptr_t* external_references = nullptr;
  CppHeap* cpp_heap = nullptr;
};

} // namespace internal

// JobHandle concrete implementation for WASI
class WASIJobHandle : public JobHandle {
 public:
  void Join() override {}
  void Cancel() override {}
  bool IsActive() override { return false; }
};

} // namespace v8

// Macro to inject methods into Isolate instances
#define WASI_ISOLATE_METHOD_INJECTION \
  void AddNearHeapLimitCallback(NearHeapLimitCallback callback, void* data) { \
    v8::IsolateWASIExt::AddNearHeapLimitCallback(this, callback, data); \
  } \
  void RemoveNearHeapLimitCallback(NearHeapLimitCallback callback, void* data) { \
    v8::IsolateWASIExt::RemoveNearHeapLimitCallback(this, callback, data); \
  } \
  void SetCaptureStackTraceForUncaughtExceptions( \
      bool capture, \
      int frame_limit = 10, \
      StackTrace::StackTraceOptions options = StackTrace::kOverview) { \
    v8::IsolateWASIExt::SetCaptureStackTraceForUncaughtExceptions( \
        this, capture, frame_limit, options); \
  } \
  static Isolate* Allocate() { \
    return v8::IsolateWASIExt::Allocate(); \
  }

#endif // __wasi__

#endif // V8_INCLUDE_WASI_V8_ISOLATE_WASI_EXT_H_