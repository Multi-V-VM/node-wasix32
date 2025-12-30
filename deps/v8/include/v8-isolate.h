#ifndef V8_V8_ISOLATE_H_
#define V8_V8_ISOLATE_H_

#ifdef __wasi__
// For WASI builds, use our stub implementation
#include "wasi/v8-isolate-wasi-stub.h"
#else

// For non-WASI (HOST) builds, include the full V8 headers
#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <utility>

#include "v8config.h"              // NOLINT(build/include_directory)
#include "v8-array-buffer.h"       // NOLINT(build/include_directory)
#include "v8-callbacks.h"          // NOLINT(build/include_directory)
#include "v8-data.h"               // NOLINT(build/include_directory)
#include "v8-debug.h"              // NOLINT(build/include_directory)
#include "v8-embedder-heap.h"      // NOLINT(build/include_directory)
#include "v8-exception.h"          // NOLINT(build/include_directory)
#include "v8-function-callback.h"  // NOLINT(build/include_directory)
#include "v8-internal.h"           // NOLINT(build/include_directory)
#include "v8-local-handle.h"       // NOLINT(build/include_directory)
#include "v8-microtask.h"          // NOLINT(build/include_directory)
#include "v8-persistent-handle.h"  // NOLINT(build/include_directory)
#include "v8-primitive.h"          // NOLINT(build/include_directory)
#include "v8-statistics.h"         // NOLINT(build/include_directory)
#include "v8-unwinder.h"           // NOLINT(build/include_directory)

namespace v8 {

class CppHeap;
class HeapProfiler;
class MicrotaskQueue;
class ScriptOrModule;
class SharedArrayBuffer;

namespace internal {
class MicrotaskQueue;
class ThreadLocalTop;
}  // namespace internal

namespace metrics {
class Recorder;
}  // namespace metrics

// ResourceConstraints for HOST builds
class V8_EXPORT ResourceConstraints {
 public:
  ResourceConstraints() = default;

  size_t max_old_generation_size_in_bytes() const {
    return max_old_generation_size_;
  }
  void set_max_old_generation_size_in_bytes(size_t size) {
    max_old_generation_size_ = size;
  }

  size_t max_young_generation_size_in_bytes() const {
    return max_young_generation_size_;
  }
  void set_max_young_generation_size_in_bytes(size_t size) {
    max_young_generation_size_ = size;
  }

  size_t initial_old_generation_size_in_bytes() const {
    return initial_old_generation_size_;
  }
  void set_initial_old_generation_size_in_bytes(size_t size) {
    initial_old_generation_size_ = size;
  }

  size_t initial_young_generation_size_in_bytes() const {
    return initial_young_generation_size_;
  }
  void set_initial_young_generation_size_in_bytes(size_t size) {
    initial_young_generation_size_ = size;
  }

  size_t code_range_size_in_bytes() const { return code_range_size_; }
  void set_code_range_size_in_bytes(size_t size) { code_range_size_ = size; }

  size_t max_code_range_size_in_bytes() const { return max_code_range_size_; }
  void set_max_code_range_size_in_bytes(size_t size) { max_code_range_size_ = size; }

  size_t stack_limit() const { return stack_limit_; }
  void set_stack_limit(size_t limit) { stack_limit_ = limit; }

  void ConfigureDefaults(uint64_t physical_memory, uint64_t virtual_memory_limit) {
    // Default configuration based on available memory
    if (physical_memory > 0) {
      max_old_generation_size_ = physical_memory / 4;
    }
  }

  void ConfigureDefaultsFromHeapSize(size_t initial_heap_size_in_bytes,
                                     size_t maximum_heap_size_in_bytes) {
    max_old_generation_size_ = maximum_heap_size_in_bytes;
    initial_old_generation_size_ = initial_heap_size_in_bytes;
  }

 private:
  size_t max_old_generation_size_ = 0;
  size_t max_young_generation_size_ = 0;
  size_t initial_old_generation_size_ = 0;
  size_t initial_young_generation_size_ = 0;
  size_t code_range_size_ = 0;
  size_t max_code_range_size_ = 0;
  size_t stack_limit_ = 0;
};

// Isolate class for HOST builds - uses real V8 types
class V8_EXPORT Isolate {
 public:
  struct CreateParams {
    CreateParams() = default;

    ResourceConstraints constraints;

    int embedder_wrapper_object_index = -1;
    int embedder_wrapper_type_index = -1;

    const intptr_t* external_references = nullptr;
    const StartupData* snapshot_blob = nullptr;
    CppHeap* cpp_heap = nullptr;
    ArrayBuffer::Allocator* array_buffer_allocator = nullptr;
    std::shared_ptr<ArrayBuffer::Allocator> array_buffer_allocator_shared;
  };

  using AbortOnUncaughtExceptionCallback = bool (*)(Isolate*);
  static Isolate* GetCurrent();
  static Isolate* TryGetCurrent();
  static Isolate* New(const CreateParams& params);
  static Isolate* Allocate();
  static void Free(Isolate* isolate);

  void Enter();
  void Exit();
  void Dispose();
  void Deinitialize();

  class Scope {
   public:
    explicit Scope(Isolate* isolate);
    ~Scope();
  };

  class DisallowJavascriptExecutionScope {
   public:
    enum OnFailure {
      CRASH_ON_FAILURE,
      THROW_ON_FAILURE
    };

    DisallowJavascriptExecutionScope(Isolate* isolate, OnFailure on_failure);
    ~DisallowJavascriptExecutionScope();
  };

  enum MessageErrorLevel {
    kMessageLog,
    kMessageDebug,
    kMessageInfo,
    kMessageError,
    kMessageWarning
  };

  using GCCallbackWithData = void (*)(Isolate* isolate, GCType, GCCallbackFlags, void* data);
  using GCCallback = void (*)(Isolate* isolate, GCType, GCCallbackFlags);
  void AddMessageListenerWithErrorLevel(MessageCallback callback,
                                        int message_levels,
                                        Local<Value> data = Local<Value>());
  void SetAbortOnUncaughtExceptionCallback(AbortOnUncaughtExceptionCallback callback);
  void SetFatalErrorHandler(FatalErrorCallback callback);
  void SetOOMErrorHandler(OOMErrorCallback callback);
  void SetPrepareStackTraceCallback(PrepareStackTraceCallback callback);
  void SetMicrotasksPolicy(MicrotasksPolicy policy);
  void SetAllowWasmCodeGenerationCallback(AllowWasmCodeGenerationCallback callback);
  void SetModifyCodeGenerationFromStringsCallback(ModifyCodeGenerationFromStringsCallback2 callback);
  void SetWasmStreamingCallback(WasmStreamingCallback callback);
  void SetHostCreateShadowRealmContextCallback(HostCreateShadowRealmContextCallback callback);
  void SetPromiseRejectCallback(PromiseRejectCallback callback);

  void* GetData(uint32_t slot);
  void SetData(uint32_t slot, void* data);
  Local<Context> GetCurrentContext();
  Local<Value> GetContinuationPreservedEmbedderData();
  void SetContinuationPreservedEmbedderData(Local<Value> data);
  void SetIdle(bool idle);
  Local<Value> ThrowException(Local<Value> exception);

  static void Initialize(Isolate* isolate, const CreateParams& params) {}
  void DumpAndResetStats() {}

  template<typename T>
  MaybeLocal<T> GetDataFromSnapshotOnce(size_t index) {
    return MaybeLocal<T>();
  }

  enum class Priority {
    kBestEffort = 0,
    kUserVisible = 1,
    kUserBlocking = 2,
  };
};

// Type definitions
using AbortOnUncaughtExceptionCallback = bool (*)(Isolate*);
using StackTracePrinter = void (*)();

}  // namespace v8

// Include v8-snapshot.h AFTER Isolate is fully defined
#define V8_ISOLATE_FULLY_DEFINED
#include "v8-snapshot.h"           // NOLINT(build/include_directory)

#endif // __wasi__

#endif // V8_V8_ISOLATE_H_
