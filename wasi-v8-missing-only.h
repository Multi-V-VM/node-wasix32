#ifndef WASI_V8_MISSING_ONLY_H_
#define WASI_V8_MISSING_ONLY_H_

// Only define types that are truly missing in WASI builds
// This file should be included AFTER V8 headers to fill gaps

#include <memory>
#include <functional>
#include <cstdint>
#include <vector>

// Include v8-forward.h to get proper forward declarations
#include "v8-forward.h"

// Forward declare cppgc namespace types
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
}

namespace v8 {

// Forward declarations
class Platform;
class Isolate;
class Context;
class Value;
class Array;
class String;
class PromiseRejectMessage;
class Message;
class ArrayBuffer;
template<typename T> class Local;
template<typename T> class MaybeLocal;

// Only define internal namespace functions
namespace internal {
inline v8::Isolate* TryGetCurrent() { return nullptr; }
inline void IncrementLongTasksStatsCounter(v8::Isolate* isolate) {}

// Missing internal types
using Address = uintptr_t;
class PtrComprCageBase {};

template <bool check_statically_enabled>
inline Address ReadExternalPointerField(Address field_address,
                                       const PtrComprCageBase& cage_base,
                                       uint64_t tag) {
  return 0;
}
} // namespace internal

// CppHeap related types - these are missing in WASI builds
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
      v8::Platform* platform,
      const CppHeapCreateParams& params) { return nullptr; }
};
#endif // V8_CPPHEAP_DEFINED

// HandleScope types - only if truly missing
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

// Forward declare StartupData
struct StartupData {
  const char* data;
  int raw_size;
};

// SnapshotCreator - missing in WASI
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

// Callback typedefs - only those truly missing
typedef void (*FatalErrorCallback)(const char* location, const char* message);
typedef void (*OOMErrorCallback)(const char* location, bool is_heap_oom);
typedef void (*PromiseRejectCallback)(PromiseRejectMessage message);
typedef bool (*AllowWasmCodeGenerationCallback)(Local<Context> context,
                                               Local<String> source);
typedef bool (*ModifyCodeGenerationFromStringsCallback2)(
    Local<Context> context, Local<Value> source, bool is_code_like);

// MicrotasksPolicy if missing
enum class MicrotasksPolicy { kExplicit, kScoped, kAuto };

// ArrayBuffer::Allocator stub for WASI
class ArrayBuffer {
 public:
  class Allocator {
   public:
    virtual ~Allocator() = default;
    virtual void* Allocate(size_t length) = 0;
    virtual void* AllocateUninitialized(size_t length) = 0;
    virtual void Free(void* data, size_t length) = 0;
    virtual void* Reallocate(void* data, size_t old_length, size_t new_length) {
      return nullptr;
    }
  };
};

// Missing types needed by Node.js
typedef void (*PrepareStackTraceCallback)(Local<Context> context,
                                         Local<Value> error,
                                         Local<Array> sites);

class Date;
class ObjectTemplate;
class FunctionTemplate;
typedef void (*FunctionCallback)(const v8::FunctionCallbackInfo<Value>& info);
template<typename T> class FunctionCallbackInfo;

// V8 namespace extensions for missing functions
namespace internal {
// Isolate static methods
inline Isolate* Isolate_GetCurrent() { return nullptr; }
} // namespace internal

} // namespace v8

#endif // WASI_V8_MISSING_ONLY_H_