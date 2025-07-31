#ifndef WASI_V8_NODE_MISSING_H_
#define WASI_V8_NODE_MISSING_H_

// Types and functions missing in WASI build but required by Node.js
// This header should be included AFTER V8 headers

#include <memory>
#include <vector>
#include <cstdint>

namespace v8 {
// Forward declarations
class Platform;
class Isolate;
class Context;
class Value;
class Array;
class Message;
class String;
template<typename T> class Local;
template<typename T> class MaybeLocal;

// Missing callback typedefs
typedef void (*FatalErrorCallback)(const char* location, const char* message);
typedef void (*OOMErrorCallback)(const char* location, bool is_heap_oom);
class PromiseRejectMessage;
typedef void (*PromiseRejectCallback)(PromiseRejectMessage message);
typedef bool (*AllowWasmCodeGenerationCallback)(Local<Context> context,
                                               Local<String> source);
typedef bool (*ModifyCodeGenerationFromStringsCallback2)(
    Local<Context> context, Local<Value> source, bool is_code_like);

// Missing enums
enum class MicrotasksPolicy { kExplicit, kScoped, kAuto };

// StartupData struct
struct StartupData {
  const char* data;
  int raw_size;
};
} // namespace v8

namespace cppgc {
class CustomSpaceBase;
class Heap {
 public:
  enum class MarkingType {
    kAtomic,
    kIncremental,
    kIncrementalAndConcurrent
  };
  
  enum class SweepingType {
    kAtomic,
    kIncremental,
    kIncrementalAndConcurrent
  };
};
} // namespace cppgc

namespace v8 {

// CppHeap types required by Node.js
#ifndef V8_CPPHEAP_DEFINED
#define V8_CPPHEAP_DEFINED
struct CppHeapCreateParams {
  std::vector<std::unique_ptr<cppgc::CustomSpaceBase>> custom_spaces;
  cppgc::Heap::MarkingType marking_support = 
      cppgc::Heap::MarkingType::kIncrementalAndConcurrent;
  cppgc::Heap::SweepingType sweeping_support =
      cppgc::Heap::SweepingType::kIncrementalAndConcurrent;
};

class CppHeap {
 public:
  static std::unique_ptr<CppHeap> Create(
      Platform* platform,
      const CppHeapCreateParams& params) { return nullptr; }
};
#endif // V8_CPPHEAP_DEFINED

namespace internal {
// Internal namespace functions
inline v8::Isolate* TryGetCurrent() { return nullptr; }
inline void IncrementLongTasksStatsCounter(v8::Isolate* isolate) {}

// Internal types
using Address = uintptr_t;
class PtrComprCageBase {};

template <bool check_statically_enabled>
inline Address ReadExternalPointerField(Address field_address,
                                       const PtrComprCageBase& cage_base,
                                       uint64_t tag) {
  return 0;
}
} // namespace internal

// Extend Isolate with missing static method
inline Isolate* Isolate_GetCurrent() { return nullptr; }

// Missing callback type
typedef void (*PrepareStackTraceCallback)(Local<Context> context,
                                         Local<Value> error,
                                         Local<Array> sites);

// Add missing Isolate methods as free functions
inline void Isolate_SetOOMErrorHandler(Isolate* isolate, OOMErrorCallback callback) {}
inline void Isolate_SetFatalErrorHandler(Isolate* isolate, FatalErrorCallback callback) {}
inline void Isolate_SetPromiseRejectCallback(Isolate* isolate, PromiseRejectCallback callback) {}
inline void Isolate_SetAllowWasmCodeGenerationCallback(Isolate* isolate, AllowWasmCodeGenerationCallback callback) {}
inline void Isolate_SetModifyCodeGenerationFromStringsCallback(Isolate* isolate, ModifyCodeGenerationFromStringsCallback2 callback) {}
inline void Isolate_SetMicrotasksPolicy(Isolate* isolate, MicrotasksPolicy policy) {}
inline void Isolate_SetCppHeap(Isolate* isolate, CppHeap* cpp_heap) {}

// HandleScope stubs if missing
class HandleScope {
 public:
  explicit HandleScope(Isolate* isolate) {}
  ~HandleScope() = default;
  HandleScope(const HandleScope&) = delete;
  void operator=(const HandleScope&) = delete;
  
  static int NumberOfHandles(Isolate* isolate) { return 0; }
  Isolate* GetIsolate() const { return nullptr; }
};

class EscapableHandleScope : public HandleScope {
 public:
  explicit EscapableHandleScope(Isolate* isolate) : HandleScope(isolate) {}
  ~EscapableHandleScope() = default;
  
  template <typename T>
  Local<T> Escape(Local<T> value) { return value; }
  template <typename T>
  MaybeLocal<T> EscapeMaybe(MaybeLocal<T> value) { return value; }
};

// SnapshotCreator stub
class SnapshotCreator {
 public:
  enum class FunctionCodeHandling { kClear, kKeep };
  
  explicit SnapshotCreator(Isolate* isolate) {}
  ~SnapshotCreator() {}
  void SetDefaultContext(Local<Context> context) {}
  size_t AddContext(Local<Context> context) { return 0; }
  size_t AddData(Local<Value> value) { return 0; }
  StartupData CreateBlob(FunctionCodeHandling function_code_handling) {
    return {nullptr, 0};
  }
};

} // namespace v8

#endif // WASI_V8_NODE_MISSING_H_