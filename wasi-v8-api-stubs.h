#ifndef WASI_V8_API_STUBS_H_
#define WASI_V8_API_STUBS_H_

#include <memory>
#include <cstdint>
#include <cstddef>

// Comprehensive V8 API stubs for WASI build

// Additional cppgc namespace stubs (must be before v8 namespace)
// Only define if V8 headers haven't been included
#if !defined(CPPGC_STUBS_DEFINED) && !defined(INCLUDE_V8_CPPGC_H_)
#define CPPGC_STUBS_DEFINED

namespace cppgc {

#ifndef CPPGC_CUSTOM_SPACE_BASE_DEFINED
#define CPPGC_CUSTOM_SPACE_BASE_DEFINED
class CustomSpaceBase {
 public:
  virtual ~CustomSpaceBase() = default;
};
#endif

#ifndef CPPGC_HEAP_DEFINED  
#define CPPGC_HEAP_DEFINED
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
#endif

} // namespace cppgc

#endif // CPPGC_STUBS_DEFINED

namespace v8 {

// Template forward declarations
template<typename T> class Local;
template<typename T> class MaybeLocal;

// Forward declarations - only declare if not already defined
class Isolate;
class Context;
class Value;
class Object;
class String;
class Function;
class Data;
class Template;
class FunctionTemplate;
class ObjectTemplate;
class EscapableHandleScope;
class TryCatch;
class Message;
class StackTrace;
class StackFrame;
class ScriptOrigin;
class Script;
class Name;
class Module;
class Promise;
class Proxy;
class WasmMemoryObject;
class WasmModuleObject;
class SharedArrayBuffer;
class ArrayBuffer;
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
class Platform;

// SourceLocation is now defined in v8-source-location.h

// Missing QueryObjectPredicate for internal_only_v8.cc
#ifndef V8_QUERY_OBJECT_PREDICATE_DEFINED
#define V8_QUERY_OBJECT_PREDICATE_DEFINED
class QueryObjectPredicate {
 public:
  virtual ~QueryObjectPredicate() = default;
  virtual bool Filter(Local<Object> object) = 0;
};
#endif // V8_QUERY_OBJECT_PREDICATE_DEFINED

// Note: Local and MaybeLocal are template classes that should be defined
// by V8 headers. We only provide stub implementations if they're missing.

// HandleScope stub - prevent redefinition, only define if not already defined
#if !defined(HANDLESCOPE_STUBS_DEFINED) && !defined(V8_HANDLESCOPE_DEFINED)
#define HANDLESCOPE_STUBS_DEFINED
#define V8_HANDLESCOPE_DEFINED

class HandleScope {
 public:
  explicit HandleScope(Isolate* isolate) {}
  ~HandleScope() = default;
  HandleScope(const HandleScope&) = delete;
  void operator=(const HandleScope&) = delete;
  
  static int NumberOfHandles(Isolate* isolate) { return 0; }
  Isolate* GetIsolate() const { return nullptr; }
};

// EscapableHandleScope stub
class EscapableHandleScope : public HandleScope {
 public:
  explicit EscapableHandleScope(Isolate* isolate) : HandleScope(isolate) {}
  ~EscapableHandleScope() = default;
  
  template <typename T>
  Local<T> Escape(Local<T> value) { return value; }
  template <typename T>
  MaybeLocal<T> EscapeMaybe(MaybeLocal<T> value) { return value; }
};

#endif // HANDLESCOPE_STUBS_DEFINED

// Note: CppHeap and CppHeapCreateParams are provided by V8 headers

// Context methods are defined inline in V8 headers

// ExternalPointerTag enum
enum ExternalPointerTag : uint64_t {
  kExternalPointerNullTag = 0,
  kExternalPointerFreeEntryTag = 1,
  kAnyExternalPointerTag = ~0ULL,
};

// ExternalPointerHandle type  
using ExternalPointerHandle = uint32_t;

// Internal namespace additions
namespace internal {

// Forward declarations for internal types
class Isolate;
// PtrComprCageBase is defined in V8 globals.h

// Basic types
#ifndef V8_HAS_ADDRESS_TYPE
using Address = uintptr_t;
#endif

// Note: Internal functions are defined in other WASI compatibility files
// - ShouldThrowOnError is defined in nuclear-fix.h
// - PointerCompressionIsEnabled and SandboxIsEnabled are defined in wasi-v8-initialization-functions.h

// Note: HeapProfiler class is provided by V8 headers

// Note: External pointer tags are defined in nuclear-fix.h to avoid conflicts

// External pointer table constants for WASI
#ifndef V8_EXTERNAL_POINTER_TABLE_SIZE
#define V8_EXTERNAL_POINTER_TABLE_SIZE
constexpr size_t kExternalPointerTableReservationSize = 1024 * 1024; // 1MB
#endif

#ifndef V8_MAX_EXTERNAL_POINTERS
#define V8_MAX_EXTERNAL_POINTERS
constexpr size_t kMaxExternalPointers = kExternalPointerTableReservationSize / 8;
#endif

#ifndef V8_MAX_CAPACITY
#define V8_MAX_CAPACITY
constexpr size_t kMaxCapacity = kMaxExternalPointers;
#endif

// V8 slot and smi value operations
#ifndef kApiSystemPointerSize
constexpr int kApiSystemPointerSize = sizeof(void*);
#endif

#ifndef kApiDoubleSize
constexpr int kApiDoubleSize = sizeof(double);
#endif

// kApiInt32Size and kApiInt64Size are already defined in v8-wasi-compat.h
// constexpr int kApiInt32Size = sizeof(int32_t);
// constexpr int kApiInt64Size = sizeof(int64_t);

#ifndef kApiSizetSize
constexpr int kApiSizetSize = sizeof(size_t);
#endif

// V8 object slots - only define if not already defined
#ifndef kApiTaggedSize
constexpr int kApiTaggedSize = kApiSystemPointerSize;
#endif

// kSmiTagSize needs to be defined for the code below
constexpr int kSmiTagSize = 1;

// Note: kSmiTagMask is defined in wasi-v8-flags-fix.h

inline int SmiValue(Address value) {
  return static_cast<int32_t>(static_cast<intptr_t>(value)) >> kSmiTagSize;
}

// SmiValuesAre31Bits and SmiValuesAre32Bits are already defined in v8-wasi-compat.h

// Embedder slot operations  
inline void IncrementLongTasksStatsCounter(::v8::Isolate* isolate) {}
inline ::v8::Isolate* TryGetCurrent() { return nullptr; }

// ReadExternalPointerField - stub implementation
template <bool check_statically_enabled>
inline Address ReadExternalPointerField(Address field_address,
                                       const void* cage_base,
                                       ExternalPointerTag tag) {
  return 0;
}

// Isolate methods are defined in V8 headers

} // namespace internal

// Note: Isolate stub methods like New() and SetIdle() are defined in v8-isolate-wasi-ext.h

} // namespace v8

#endif // WASI_V8_API_STUBS_H_