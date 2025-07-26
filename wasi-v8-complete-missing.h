#ifndef WASI_V8_COMPLETE_MISSING_H_
#define WASI_V8_COMPLETE_MISSING_H_

// Complete set of missing V8 types for WASI build
// This should be included BEFORE any V8 headers in util.h

#include <memory>
#include <vector>
#include <cstdint>
#include <functional>

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
} // namespace cppgc

namespace v8 {

// Forward declarations
class Platform;
class Isolate;
class Context;
class Value;
class Array;
class String;
class Message;
class Date;
class ObjectTemplate;
class FunctionTemplate;
class Template;
class Object;
class ArrayBuffer;
class Function;
class Data;
class Signature;
template<typename T> class FunctionCallbackInfo;

// Extend Isolate with MessageErrorLevel before v8-inspector.h needs it
namespace internal {
enum IsolateMessageErrorLevel {
  kMessageLog = (1 << 0),
  kMessageDebug = (1 << 1),
  kMessageInfo = (1 << 2),
  kMessageError = (1 << 3),
  kMessageWarning = (1 << 4),
  kMessageAll = kMessageLog | kMessageDebug | kMessageInfo | kMessageError | kMessageWarning
};
} // namespace internal

// Missing callback typedefs
typedef void (*FatalErrorCallback)(const char* location, const char* message);
typedef void (*OOMErrorCallback)(const char* location, bool is_heap_oom);
class PromiseRejectMessage;
typedef void (*PromiseRejectCallback)(PromiseRejectMessage message);
typedef bool (*AllowWasmCodeGenerationCallback)(Local<Context> context,
                                               Local<String> source);
typedef bool (*ModifyCodeGenerationFromStringsCallback2)(
    Local<Context> context, Local<Value> source, bool is_code_like);
typedef void (*PrepareStackTraceCallback)(Local<Context> context,
                                         Local<Value> error,
                                         Local<Array> sites);
typedef void (*FunctionCallback)(const FunctionCallbackInfo<Value>& info);

// Missing enums
enum class MicrotasksPolicy { kExplicit, kScoped, kAuto };

// StartupData struct
struct StartupData {
  const char* data;
  int raw_size;
};

// CppHeap types
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

// ArrayBuffer::Allocator 
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

// Date stub
class Date : public Object {
 public:
  static Local<Value> New(Local<Context> context, double value) { return Local<Value>(); }
  double ValueOf() const { return 0.0; }
};

// Name forward declaration
class Name;

// Template classes
class Template : public Data {
 public:
  void Set(Local<Name> name, Local<Data> value) {}
};

class ObjectTemplate : public Template {
 public:
  static Local<ObjectTemplate> New(Isolate* isolate) { return Local<ObjectTemplate>(); }
  Local<Object> NewInstance(Local<Context> context) { return Local<Object>(); }
};

class FunctionTemplate : public Template {
 public:
  static Local<FunctionTemplate> New(
      Isolate* isolate,
      FunctionCallback callback = nullptr,
      Local<Value> data = Local<Value>(),
      Local<Signature> signature = Local<Signature>(),
      int length = 0) { return Local<FunctionTemplate>(); }
  void SetClassName(Local<String> name) {}
  Local<Function> GetFunction(Local<Context> context) { return Local<Function>(); }
  Local<ObjectTemplate> PrototypeTemplate() { return Local<ObjectTemplate>(); }
};

// Signature stub
class Signature : public Data {
 public:
  static Local<Signature> New(Isolate* isolate, Local<FunctionTemplate> receiver = Local<FunctionTemplate>()) {
    return Local<Signature>();
  }
};

namespace internal {
// Internal functions
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

// HandleScope types
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

// SnapshotCreator
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

// Missing types that are referenced but not defined
class ArrayBufferView;
class TryCatch;
class CFunction;
enum class ConstructorBehavior { kThrow, kAllow };

// AbortOnUncaughtExceptionCallback typedef in v8 namespace
typedef void (*AbortOnUncaughtExceptionCallback)(Isolate* isolate);

// Add missing static method as a free function
inline Isolate* Isolate_GetCurrent() { return nullptr; }

// Define GetCurrent as a macro since we can't extend the Isolate class
#define V8_ISOLATE_GET_CURRENT_DEFINED
namespace internal {
inline Isolate* GetCurrentIsolate() { return nullptr; }
} // namespace internal

// Function stub
class Function : public Object {
 public:
  void SetName(Local<String> name) {}
};


// TryCatch stub
class TryCatch {
 public:
  explicit TryCatch(Isolate* isolate) {}
  ~TryCatch() {}
  bool HasCaught() const { return false; }
  Local<Value> Exception() const { return Local<Value>(); }
};

// Isolate extension methods as free functions  
inline Local<Context> Isolate_GetCurrentContext(Isolate* isolate) { return Local<Context>(); }
inline void Isolate_SetOOMErrorHandler(Isolate* isolate, OOMErrorCallback callback) {}
inline void Isolate_SetFatalErrorHandler(Isolate* isolate, FatalErrorCallback callback) {}
inline void Isolate_SetPromiseRejectCallback(Isolate* isolate, PromiseRejectCallback callback) {}
inline void Isolate_SetAllowWasmCodeGenerationCallback(Isolate* isolate, AllowWasmCodeGenerationCallback callback) {}
inline void Isolate_SetModifyCodeGenerationFromStringsCallback(Isolate* isolate, ModifyCodeGenerationFromStringsCallback2 callback) {}
inline void Isolate_SetMicrotasksPolicy(Isolate* isolate, MicrotasksPolicy policy) {}
inline void Isolate_SetCppHeap(Isolate* isolate, CppHeap* cpp_heap) {}

} // namespace v8

#endif // WASI_V8_COMPLETE_MISSING_H_