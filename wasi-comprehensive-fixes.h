#ifndef WASI_COMPREHENSIVE_FIXES_H_
#define WASI_COMPREHENSIVE_FIXES_H_

// Only apply these fixes for WASI builds
#ifdef __wasi__

// Forward declarations
namespace v8 {
class StartupData;  // Changed from struct to class to match v8-snapshot.h
class Isolate;
class JobHandle;
class Platform;
class PageAllocator;
class TracingController;
template<typename T> class Local;
template<typename T> class Handle;

namespace base {
template<typename T> class OwnedVector;
}

namespace internal {
template<typename T> class ZoneVector;
template<typename T> class Vector;
class HeapObject;
class DisallowGarbageCollection;
class Script;
class AccessorInfo;
class FunctionTemplateInfo;
class DescriptorArray;
class String;
template<typename T> class GlobalHandleVector;

namespace wasm {
class ModuleWireBytes;
}
}
}

// ============================================================================
// Namespace Alias Fixes - v8::internal::base -> ::v8::base
// ============================================================================

namespace v8 {
namespace internal {
namespace base {

// Re-export ::v8::base types into v8::internal::base
// Note: Many types have been removed as they don't exist in current V8 versions
// and don't appear to be used by the build

// OOMType
using OOMType = ::v8::base::OOMType;

// PageInitializationMode and PageFreeingMode
enum PageInitializationMode {
  kZeroInitialized = 0,
  kUninitialized = 1
};

enum PageFreeingMode {
  kImmediateFreeing = 0,
  kDeferred = 1
};

// Free function
using Free = void(*)(void*);

// FlushDenormalsScope stub
class FlushDenormalsScope {
public:
  explicit FlushDenormalsScope(bool disable_denormals) {}
};

// SetPrintStackTrace stub
inline void SetPrintStackTrace(void (*print_stack_trace)(void)) {}

} // namespace base
} // namespace internal
} // namespace v8

// ============================================================================
// ZoneVector <-> Vector Conversion Helpers
// ============================================================================

namespace v8 {
namespace internal {

// Helper to convert ZoneVector to Vector
template<typename T>
inline Vector<T> ZoneVectorToVector(ZoneVector<T>& zv) {
  return Vector<T>(zv.data(), static_cast<int>(zv.size()));
}

template<typename T>
inline Vector<const T> ZoneVectorToVector(const ZoneVector<T>& zv) {
  return Vector<const T>(zv.data(), static_cast<int>(zv.size()));
}

// OwnedZoneVector alias
namespace base {
template<typename T>
using OwnedZoneVector = ::v8::base::OwnedVector<T>;
}

} // namespace internal
} // namespace v8

// ============================================================================
// Missing V8 API Stubs
// ============================================================================

namespace v8 {

// Isolate, JobHandle, Platform, and PageAllocator are already defined in WASI stubs
// Skip redefinitions here
// Only use forward declarations from above

// HeapProfiler is already defined in WASI stubs
// Skip redefinition here
// namespace HeapProfiler {
//   using SamplingFlags = int;
//
//   class HeapSnapshotOptions {
//   public:
//     enum NumericsMode {
//       kExposeNumericValues,
//       kHideNumericValues
//     };
//   };
//
//   class ObjectNameResolver {
//   public:
//     virtual ~ObjectNameResolver() {}
//   };
// }

// ActivityControl is already defined in embedder-graph-stub.h
// Skip redefinition here
// class ActivityControl {
// public:
//   enum ControlOption {
//     kContinue = 0,
//     kAbort = 1
//   };
//   virtual ~ActivityControl() {}
//   virtual ControlOption ReportProgressValue(uint32_t done, uint32_t total) = 0;
// };

// AllocationProfile is already defined as a class in v8-profiler-stubs.h
// Skip redefinition here
// namespace AllocationProfile {
//   struct Allocation {
//     size_t size;
//     int count;
//   };
// }

// CpuProfile additions
using ProfilerId = int;

class DiscardedSamplesDelegate {
public:
  virtual ~DiscardedSamplesDelegate() {}
};

enum CpuProfilingResult {
  kSuccess = 0,
  kErrorTooManyProfilers = 1
};

// TracedReferenceBase is already defined in v8-traced-handle.h
// Skip redefinition here
// class TracedReferenceBase {
// public:
//   void* val_;
// };

// HandleScope is now properly included from v8-handlescope-fix.h
// Skip duplicate definition here
// class HandleScope {
// public:
//   explicit HandleScope(Isolate* isolate) {}
// };

// SnapshotCreator is already defined in v8-snapshot.h
// Skip redefinition here
// class SnapshotCreator {
// public:
//   SnapshotCreator(Isolate* isolate, const intptr_t* external_references = nullptr,
//                   StartupData* existing_blob = nullptr) {}
//   SnapshotCreator(const intptr_t* external_references = nullptr,
//                   StartupData* existing_blob = nullptr) {}
// };

// SnapshotBlobRef (if needed)
using SnapshotBlobRef = StartupData;

// Local<T> is already defined in v8-data.h
// Skip redefinition here
// template<typename T>
// class Local {
// public:
//   inline void* slot() const { return nullptr; }
//
//   // Cast method
//   template<typename S>
//   static inline Local<T> Cast(Local<S> that) {
//     return Local<T>();
//   }
// };

// UnboundScript forward declaration
class UnboundScript;

// TracingController is now properly included from v8-tracing-base.h
// Skip duplicate definition here
// class TracingController {
// public:
//   virtual ~TracingController() = default;
//   virtual const uint8_t* GetCategoryGroupEnabled(const char* name) { return nullptr; }
//   virtual uint64_t AddTraceEvent(char phase, const uint8_t* category_enabled_flag,
//                                   const char* name, const char* scope, uint64_t id,
//                                   uint64_t bind_id, int32_t num_args,
//                                   const char** arg_names, const uint8_t* arg_types,
//                                   const uint64_t* arg_values, unsigned int flags) { return 0; }
//   virtual void UpdateTraceEventDuration(const uint8_t* category_enabled_flag,
//                                        const char* name, uint64_t handle) {}
// };

} // namespace v8

// ============================================================================
// Missing Constants and Types
// ============================================================================

namespace v8 {
namespace internal {

// CppHeapPointer constants - already defined in nuclear-fix.h
// Skip redefinition here
// constexpr uint32_t kAnyCppHeapPointer = 0;
// constexpr uint32_t kNullCppHeapPointer = 0;
// constexpr size_t kMaxCppHeapPointers = 1024;

// Interpreter register (stub)
constexpr int kInterpreterBytecodeOffsetRegister = 0;
constexpr int kInterpreterBytecodeArrayRegister = 1;

// Tagged index constants
constexpr uintptr_t kUintptrAllBitsSet = ~static_cast<uintptr_t>(0);

// Zone page size constants
constexpr size_t kZonePageSize = 256 * 1024;
constexpr size_t kMinZonePageSize = 8 * 1024;
constexpr size_t kMinimumOSPageSize = 4 * 1024;

// TracedReferenceStoreMode
enum TracedReferenceStoreMode {
  kInitializingStore,
  kAssigningStore
};

// TracedReferenceHandling
enum TracedReferenceHandling {
  kDefault,
  kDroppable
};

// AssertCondition type
using AssertCondition = int;

// Condition type for comparisons (architecture-specific)
using Condition = int;
constexpr Condition kEqual = 0;
constexpr Condition kNotEqual = 1;
constexpr Condition kLessThan = 2;
constexpr Condition kLessThanOrEqual = 3;
constexpr Condition kGreaterThan = 4;
constexpr Condition kGreaterThanOrEqual = 5;
constexpr Condition kUnsignedLessThan = 6;
constexpr Condition kUnsignedLessThanEqual = 7;
constexpr Condition kUnsignedGreaterThan = 8;
constexpr Condition kUnsignedGreaterThanEqual = 9;

// Aliases for legacy names
constexpr Condition kGreaterThanEqual = kGreaterThanOrEqual;
constexpr Condition kLessThanEqual = kLessThanOrEqual;

// DeoptimizeReason type
using DeoptimizeReason = int;

// WasmDebugBreakFrameConstants stubs
struct WasmDebugBreakFrameConstants {
  static constexpr int GetPushedGpRegisterOffset(int index) { return index * 8; }
  static constexpr int GetPushedFpRegisterOffset(int index) { return index * 8; }
};

// DCHECK_BOUNDS macro (stub)
#ifndef DCHECK_BOUNDS
#define DCHECK_BOUNDS(index, limit) DCHECK_LT(index, limit)
#endif

// CHECK_IMPLIES macro (stub)
#ifndef CHECK_IMPLIES
#define CHECK_IMPLIES(a, b) CHECK(!(a) || (b))
#endif

// Heap graph node types
namespace HeapGraphNode {
  constexpr int kObjectShape = 0;
}

// CppHeapPointerTableEntry methods
class CppHeapPointerTableEntry {
public:
  inline void SetRawPayload(uint64_t payload) {}
  inline uint64_t GetRawPayload() const { return 0; }
};

} // namespace internal
} // namespace v8

// ============================================================================
// WASM-specific Fixes
// ============================================================================

namespace v8 {
namespace internal {
namespace wasm {

// ModuleWireBytes constructor fix
class ModuleWireBytes {
public:
  ModuleWireBytes() : start_(nullptr), length_(0) {}

  // Constructors disabled due to forward-declared template types
  // Cannot instantiate Vector<T> or ZoneVector<T> without full definitions
  // Only default constructor available

  const uint8_t* start() const { return start_; }
  size_t length() const { return length_; }

private:
  const uint8_t* start_;
  size_t length_;
};

// SmallVector for WASM - commented out as ::v8::base::SmallVector doesn't exist
// template<typename T, size_t N>
// using SmallVector = ::v8::base::SmallVector<T, N>;

} // namespace wasm
} // namespace internal
} // namespace v8

// ============================================================================
// Snapshot/Deserializer Fixes
// ============================================================================

namespace v8 {
namespace internal {

// Deserializer base class members (stubs for missing methods)
class DeserializerStubs {
protected:
  void AddAttachedObject(void* obj) {}  // Changed from Handle<HeapObject> to void*
  void* ReadObject() { return nullptr; }  // Changed from Handle<HeapObject> to void*
  void DeserializeDeferredObjects() {}
  void LogNewMapEvents() {}
  void WeakenDescriptorArrays() {}
  void Rehash() {}
  void LogScriptEvents(const DisallowGarbageCollection& no_gc) {}
  void FlushInstructionCache(void* start, size_t size) {}

  bool should_rehash() const { return false; }
  Isolate* isolate() { return nullptr; }
  const void* source() { return nullptr; }
  // Removed template instantiations to avoid incomplete type errors
  // std::vector<Handle<Script>> new_scripts;
  // std::vector<Handle<AccessorInfo>> accessor_infos;
  // std::vector<Handle<FunctionTemplateInfo>> function_template_infos;
  // GlobalHandleVector<DescriptorArray> descriptor_arrays;
};

// SerializedCodeData::Payload return type fix
// Skip using Vector template as it's incomplete
// namespace SerializedCodeData {
//   using Payload = Vector<const uint8_t>;
// }
//
// // SnapshotData::Payload return type fix
// namespace SnapshotData {
//   using Payload = Vector<const uint8_t>;
// }

} // namespace internal
} // namespace v8

// ============================================================================
// String/URI Fixes
// ============================================================================

namespace v8 {
namespace internal {

class String {
public:
  // GetCharVector - disabled due to incomplete template types
  // template<typename Char>
  // static Vector<Char> GetCharVector(Handle<String> string, const DisallowGarbageCollection& no_gc) {
  //   return Vector<Char>(nullptr, 0);
  // }
};

namespace unibrow {
class Wtf8 {
public:
  static int ScanForSurrogates(const uint8_t* start, size_t length) { return 0; }
};
}

} // namespace internal
} // namespace v8

// ============================================================================
// Platform/Allocator Fixes
// ============================================================================

namespace v8 {

// VirtualAddressSpace pointer return fix - removed as LeakyObject doesn't exist
// in current V8 version
// inline VirtualAddressSpace* GetVirtualAddressSpaceForProcess() {
//   static ::v8::base::LeakyObject<::v8::base::VirtualAddressSpace> vas;
//   return vas.Get();
// }

} // namespace v8

#endif  // __wasi__
#endif  // WASI_COMPREHENSIVE_FIXES_H_
