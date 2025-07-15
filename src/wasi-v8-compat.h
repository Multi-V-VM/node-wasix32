// WASI compatibility layer for V8 API
#ifndef SRC_WASI_V8_COMPAT_H_
#define SRC_WASI_V8_COMPAT_H_

#ifdef __wasi__

#include "v8.h"

namespace v8 {

// SealHandleScope stub for WASI
// This is used to ensure no handles are created in certain scopes
class SealHandleScope {
 public:
  explicit SealHandleScope(Isolate* isolate) : isolate_(isolate) {}
  ~SealHandleScope() {}
  
 private:
  Isolate* isolate_;
  SealHandleScope(const SealHandleScope&) = delete;
  SealHandleScope& operator=(const SealHandleScope&) = delete;
};

// JobHandle stub implementation for WASI
class WASIJobHandle : public JobHandle {
 public:
  void Join() override {}
  void Cancel() override {}
  bool IsActive() override { return false; }
};

// Extension methods for Isolate
inline void IsolateAddNearHeapLimitCallback(Isolate* isolate, 
                                           v8::NearHeapLimitCallback callback, 
                                           void* data) {
  // No-op for WASI
}

inline void IsolateRemoveNearHeapLimitCallback(Isolate* isolate,
                                               v8::NearHeapLimitCallback callback,
                                               void* data) {
  // No-op for WASI
}

inline void IsolateSetCaptureStackTraceForUncaughtExceptions(
    Isolate* isolate,
    bool capture,
    int frame_limit = 10,
    v8::StackTrace::StackTraceOptions options = v8::StackTrace::kOverview) {
  // No-op for WASI
}

inline Isolate* IsolateAllocate() {
  // For WASI, we don't support custom allocation
  return nullptr;
}

// Patch for CreateParams
struct WASICreateParams : public Isolate::CreateParams {
  ArrayBuffer::Allocator* array_buffer_allocator = nullptr;
  const intptr_t* external_references = nullptr;
  CppHeap* cpp_heap = nullptr;
};

} // namespace v8

// Macros to redirect calls
#define AddNearHeapLimitCallback(callback, data) \
  v8::IsolateAddNearHeapLimitCallback(this, callback, data)
  
#define RemoveNearHeapLimitCallback(callback, data) \
  v8::IsolateRemoveNearHeapLimitCallback(this, callback, data)

#define SetCaptureStackTraceForUncaughtExceptions(capture, frame_limit, options) \
  v8::IsolateSetCaptureStackTraceForUncaughtExceptions(this, capture, frame_limit, options)

#endif // __wasi__

#endif // SRC_WASI_V8_COMPAT_H_