#ifndef V8_V8_ISOLATE_H_
#define V8_V8_ISOLATE_H_

#ifdef __wasi__
// For WASI builds, use our stub implementation
#include "wasi/v8-isolate-wasi-stub.h"
#else

// For non-WASI builds, provide minimal definitions needed
#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <utility>

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
#include "v8config.h"              // NOLINT(build/include_directory)

namespace v8 {

class CppHeap;
class HeapProfiler;
class MicrotaskQueue;
class StartupData;
class ScriptOrModule;
class SharedArrayBuffer;

namespace internal {
class MicrotaskQueue;
class ThreadLocalTop;
}  // namespace internal

namespace metrics {
class Recorder;
}  // namespace metrics

// Minimal forward declaration for Isolate
class V8_EXPORT Isolate {
 public:
  struct CreateParams {
    CreateParams() = default;
    
    // Add fields needed by v8-snapshot.h and Node.js
    struct {
      size_t max_old_generation_size_in_bytes() const { return 0; }
      void ConfigureDefaults(size_t physical_memory, size_t virtual_memory_limit) {}
    } constraints;
    
    int embedder_wrapper_object_index = -1;
    int embedder_wrapper_type_index = -1;
    
    // Additional fields for Node.js
    const intptr_t* external_references = nullptr;
    void* cpp_heap = nullptr;
    void* array_buffer_allocator = nullptr;
    std::shared_ptr<void> array_buffer_allocator_shared;
  };
  
  // Add other minimal methods as needed
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
  
  // DisallowJavascriptExecutionScope
  class DisallowJavascriptExecutionScope {
  public:
    enum OnFailure {
      CRASH_ON_FAILURE,
      THROW_ON_FAILURE
    };
    
    DisallowJavascriptExecutionScope(Isolate* isolate, OnFailure on_failure);
    ~DisallowJavascriptExecutionScope();
  };
  
  // Message error levels
  enum MessageErrorLevel {
    kMessageLog,
    kMessageDebug,
    kMessageInfo,
    kMessageError,
    kMessageWarning
  };
  
  // Add callback methods
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
  
  // Additional methods for Node.js
  static void Initialize(Isolate* isolate, const CreateParams& params) {}
  void DumpAndResetStats() {}
  
  // GetDataFromSnapshotOnce method
  template<typename T>
  MaybeLocal<T> GetDataFromSnapshotOnce(size_t index) {
    return MaybeLocal<T>();
  }
  
  // Priority enum
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

#endif // __wasi__

#endif // V8_V8_ISOLATE_H_