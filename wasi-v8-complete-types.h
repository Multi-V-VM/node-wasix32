#ifndef WASI_V8_COMPLETE_TYPES_H_
#define WASI_V8_COMPLETE_TYPES_H_

// Complete V8 type definitions for WASI build

#include <memory>
#include <cstdint>
#include <vector>
#include <functional>

namespace v8 {

// Forward declarations of all missing types
class ArrayBuffer;
class Uint8Array;
class Int8Array;
class Uint16Array;
class Int16Array;
class Uint32Array;
class Int32Array;
class Float32Array;
class Float64Array;
class BigInt64Array;
class BigUint64Array;
class Message;
class StackTrace;
class StackFrame;
class ScriptOrigin;
class Script;
class Module;
class Promise;
class Proxy;
class WasmMemoryObject;
class WasmModuleObject;
class SharedArrayBuffer;
class BackingStore;
class Boolean;
class Integer;
class Number;
class BigInt;
class Date;
class RegExp;
class External;
class Signature;
class AccessorSignature;
class Extension;
class ResourceConstraints;
class Exception;
class HeapStatistics;
class HeapSpaceStatistics;
class HeapObjectStatistics;
class HeapCodeStatistics;
class Isolate;
class PromiseRejectMessage;
class MicrotaskQueue;
class FixedArray;
class PrimitiveArray;
template<typename T> class ReturnValue;
class EscapableHandleScope;
class SealHandleScope;
class Data;
class Template;
class FunctionTemplate;
class ObjectTemplate;
class Signature;
class Snapshot;
class SnapshotCreator;
class Platform;
class Task;
class JobHandle;
class JobDelegate;
class TaskRunner;
class ConvertableToTraceFormat;
class TracingController;
class PageAllocator;
class Allocator;

// Type aliases and enums - only define those missing in v8-object.h
enum class MicrotasksPolicy { kExplicit, kScoped, kAuto };
enum class MemoryPressureLevel { kNone, kModerate, kCritical };
enum class GCType { kGCTypeScavenge, kGCTypeMarkSweepCompact, kGCTypeAll };
enum class GCCallbackFlags { kNoGCCallbackFlags, kGCCallbackFlagForced };

// Forward declare for callbacks
template<typename T> class Local;
template<typename T> class MaybeLocal;
template<typename T> class FunctionCallbackInfo;
template<typename T> class PropertyCallbackInfo;
class Value;
class Context;
class String;
class Array;
class Function;
class Object;
class Boolean;

// Function callback typedef
typedef void (*FunctionCallback)(const FunctionCallbackInfo<Value>& info);

// Callback typedefs  
typedef void (*FatalErrorCallback)(const char* location, const char* message);
typedef void (*OOMErrorCallback)(const char* location, const char* message);
typedef void (*PromiseRejectCallback)(PromiseRejectMessage message);
typedef MaybeLocal<Value> (*PrepareStackTraceCallback)(Local<Context> context,
                                                       Local<Value> error,
                                                       Local<StackTrace> trace);
typedef bool (*AllowWasmCodeGenerationCallback)(Local<Context> context,
                                               Local<String> source);
typedef bool (*ModifyCodeGenerationFromStringsCallback2)(
    Local<Context> context, Local<Value> source, bool is_code_like);
typedef void (*InterruptCallback)(Isolate* isolate, void* data);

// Property callbacks
typedef void (*AccessorGetterCallback)(Local<String> property,
                                      const PropertyCallbackInfo<Value>& info);
typedef void (*AccessorSetterCallback)(Local<String> property,
                                      Local<Value> value,
                                      const PropertyCallbackInfo<void>& info);
typedef void (*NamedPropertyGetterCallback)(Local<String> property,
                                           const PropertyCallbackInfo<Value>& info);
typedef void (*NamedPropertySetterCallback)(Local<String> property,
                                           Local<Value> value,
                                           const PropertyCallbackInfo<Value>& info);
typedef void (*NamedPropertyDeleterCallback)(Local<String> property,
                                            const PropertyCallbackInfo<Boolean>& info);
typedef void (*NamedPropertyEnumeratorCallback)(const PropertyCallbackInfo<Array>& info);
typedef void (*NamedPropertyDefinerCallback)(Local<String> property,
                                           const PropertyDescriptor& desc,
                                           const PropertyCallbackInfo<Value>& info);


// ArrayBuffer and TypedArray stubs
class ArrayBuffer {
 public:
  class Allocator {
   public:
    virtual ~Allocator() = default;
    virtual void* Allocate(size_t length) = 0;
    virtual void* AllocateUninitialized(size_t length) = 0;
    virtual void Free(void* data, size_t length) = 0;
  };
  
  static Local<ArrayBuffer> New(Isolate* isolate, size_t byte_length);
  size_t ByteLength() const;
  void* Data() const;
};

// ArrayBufferView base class
class ArrayBufferView : public Object {
 public:
  Local<ArrayBuffer> Buffer();
  size_t ByteOffset() const;
  size_t ByteLength() const;
};

class TypedArray : public ArrayBufferView {
 public:
  size_t Length() const;
};

#define TYPED_ARRAY(Type, type, TYPE, ctype)                           \
  class Type##Array : public TypedArray {                              \
   public:                                                             \
    static Local<Type##Array> New(Local<ArrayBuffer> array_buffer,    \
                                  size_t byte_offset, size_t length);  \
  };

TYPED_ARRAY(Uint8, uint8, UINT8, uint8_t)
TYPED_ARRAY(Int8, int8, INT8, int8_t)
TYPED_ARRAY(Uint16, uint16, UINT16, uint16_t)
TYPED_ARRAY(Int16, int16, INT16, int16_t)
TYPED_ARRAY(Uint32, uint32, UINT32, uint32_t)
TYPED_ARRAY(Int32, int32, INT32, int32_t)
TYPED_ARRAY(Float32, float32, FLOAT32, float)
TYPED_ARRAY(Float64, float64, FLOAT64, double)
TYPED_ARRAY(BigInt64, bigint64, BIGINT64, int64_t)
TYPED_ARRAY(BigUint64, biguint64, BIGUINT64, uint64_t)

#undef TYPED_ARRAY

// Object and ObjectTemplate
class ObjectTemplate : public Template {
 public:
  static Local<ObjectTemplate> New(Isolate* isolate);
  MaybeLocal<Object> NewInstance(Local<Context> context);
  void SetInternalFieldCount(int value);
  void SetAccessor(Local<String> name,
                  AccessorGetterCallback getter,
                  AccessorSetterCallback setter = nullptr,
                  Local<Value> data = Local<Value>());
};

// Function and FunctionTemplate  
class FunctionTemplate : public Template {
 public:
  static Local<FunctionTemplate> New(
      Isolate* isolate,
      FunctionCallback callback = nullptr,
      Local<Value> data = Local<Value>(),
      Local<Signature> signature = Local<Signature>(),
      int length = 0);
  MaybeLocal<Function> GetFunction(Local<Context> context);
  Local<ObjectTemplate> InstanceTemplate();
  Local<ObjectTemplate> PrototypeTemplate();
  void SetClassName(Local<String> name);
};

// Promise reject event enum
enum PromiseRejectEvent {
  kPromiseRejectWithNoHandler = 0,
  kPromiseHandlerAddedAfterReject = 1,
  kPromiseRejectAfterResolved = 2,
  kPromiseResolveAfterResolved = 3
};

// PromiseRejectMessage
class PromiseRejectMessage {
 public:
  Local<Promise> GetPromise() const;
  PromiseRejectEvent GetEvent() const;
  Local<Value> GetValue() const;
};

// StartupData struct
struct StartupData {
  const char* data;
  int raw_size;
};


// SnapshotCreator
class SnapshotCreator {
 public:
  enum class FunctionCodeHandling { kClear, kKeep };
  
  explicit SnapshotCreator(Isolate* isolate);
  ~SnapshotCreator();
  void SetDefaultContext(Local<Context> context);
  size_t AddContext(Local<Context> context);
  size_t AddData(Local<Value> value);
  StartupData CreateBlob(FunctionCodeHandling function_code_handling);
};

// CppHeap (already defined in wasi-v8-minimal-missing.h)
// Just forward declare to avoid redefinition
class CppHeap;

// Isolate extensions
class Isolate {
 public:
  enum MessageErrorLevel {
    kMessageLog = (1 << 0),
    kMessageDebug = (1 << 1),
    kMessageInfo = (1 << 2),
    kMessageError = (1 << 3),
    kMessageWarning = (1 << 4),
    kMessageAll = kMessageLog | kMessageDebug | kMessageInfo | kMessageError | kMessageWarning
  };
  
  enum UseCounterFeature {
    kUseAsm,
    kUseCounterFeatureCount
  };
  
  typedef void (*UseCounterCallback)(Isolate* isolate, UseCounterFeature feature);
  
  // Methods that were missing
  void SetOOMErrorHandler(OOMErrorCallback callback);
  void SetFatalErrorHandler(FatalErrorCallback that);
  void SetPrepareStackTraceCallback(PrepareStackTraceCallback callback);
  void SetPromiseRejectCallback(PromiseRejectCallback callback);
  void SetAllowWasmCodeGenerationCallback(AllowWasmCodeGenerationCallback callback);
  void SetModifyCodeGenerationFromStringsCallback(ModifyCodeGenerationFromStringsCallback2 callback);
  
  // Other methods
  static Isolate* GetCurrent();
  void Enter();
  void Exit();
  Local<Context> GetCurrentContext();
  bool InContext();
  void ThrowException(Local<Value> exception);
  void TerminateExecution();
  bool IsExecutionTerminating();
  void CancelTerminateExecution();
  void RequestInterrupt(InterruptCallback callback, void* data);
  void SetMicrotasksPolicy(MicrotasksPolicy policy);
  MicrotasksPolicy GetMicrotasksPolicy() const;
  void PerformMicrotaskCheckpoint();
  void EnqueueMicrotask(Local<Function> microtask);
  void SetUseCounterCallback(UseCounterCallback callback);
  void SetCppHeap(CppHeap* cpp_heap);
};

} // namespace v8

#endif // WASI_V8_COMPLETE_TYPES_H_