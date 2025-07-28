#ifndef WASI_V8_API_STUBS_H_
#define WASI_V8_API_STUBS_H_

#include <memory>
#include <cstdint>
#include <cstddef>

// Comprehensive V8 API stubs for WASI build

// Additional cppgc namespace stubs (must be before v8 namespace)
namespace cppgc {

class CustomSpaceBase {
 public:
  virtual ~CustomSpaceBase() = default;
};

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

// Note: Local and MaybeLocal are template classes that should be defined
// by V8 headers. We only provide stub implementations if they're missing.

// HandleScope stub
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

// CppHeap stub
class CppHeap {
 public:
  static std::unique_ptr<CppHeap> Create(
      v8::Platform* platform,
      const CppHeapCreateParams& params) { return nullptr; }
};

struct CppHeapCreateParams {
  std::vector<std::unique_ptr<cppgc::CustomSpaceBase>> custom_spaces;
  cppgc::Heap::MarkingType marking_support = 
      cppgc::Heap::MarkingType::kIncrementalAndConcurrent;
  cppgc::Heap::SweepingType sweeping_support =
      cppgc::Heap::SweepingType::kIncrementalAndConcurrent;
};

// Context methods are defined inline in V8 headers

// ExternalPointerTag enum
enum ExternalPointerTag : uint64_t {
  kExternalPointerNullTag = 0,
  kExternalPointerFreeEntryTag = 1,
  kAnyExternalPointerTag = ~0ULL,
};

// Internal namespace additions
namespace internal {

// Forward declarations for internal types
class Isolate;
// PtrComprCageBase is defined in V8 globals.h

// Basic types
#ifndef V8_HAS_ADDRESS_TYPE
using Address = uintptr_t;
#endif

// Missing internal functions
inline bool ShouldThrowOnError(v8::internal::Isolate* isolate) { return false; }
inline bool PointerCompressionIsEnabled() { return false; }
inline bool SandboxIsEnabled() { return false; }

// Missing constants
constexpr ExternalPointerTag kWasmWasmStreamingTag = static_cast<ExternalPointerTag>(0x1000);
constexpr ExternalPointerTag kWasmStringViewIterTag = static_cast<ExternalPointerTag>(0x1001);
constexpr ExternalPointerTag kApiAccessorPairGetterTag = static_cast<ExternalPointerTag>(0x1002);
constexpr ExternalPointerTag kApiAccessorPairSetterTag = static_cast<ExternalPointerTag>(0x1003);

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

#ifndef kSmiTagMask
constexpr intptr_t kSmiTagMask = (1 << kSmiTagSize) - 1;
#endif

inline int SmiValue(Address value) {
  return static_cast<int32_t>(static_cast<intptr_t>(value)) >> kSmiTagSize;
}

// SmiValuesAre31Bits and SmiValuesAre32Bits are already defined in v8-wasi-compat.h

// Embedder slot operations  
inline void IncrementLongTasksStatsCounter(v8::Isolate* isolate) {}
inline v8::Isolate* TryGetCurrent() { return nullptr; }

// ReadExternalPointerField - stub implementation
template <bool check_statically_enabled>
inline Address ReadExternalPointerField(Address field_address,
                                       const void* cage_base,
                                       ExternalPointerTag tag) {
  return 0;
}

// Isolate methods are defined in V8 headers

} // namespace internal

} // namespace v8

#endif // WASI_V8_API_STUBS_H_