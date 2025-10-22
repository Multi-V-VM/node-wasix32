#ifndef WASI_RUNTIME_PROFILER_FIXES_H_
#define WASI_RUNTIME_PROFILER_FIXES_H_

// ============================================================================
// Profiler/Inspector/Runtime API Fixes
// ============================================================================

namespace v8 {

// ============================================================================
// Isolate Use Counter Features (Additional)
// ============================================================================

enum class IsolateUseCounterFeature {
  kFunctionPrototypeArguments,
  kFunctionPrototypeCaller,
  kArrayBufferTransfer,
  kCallSiteAPIGetThisSloppyCall,
  kCallSiteAPIGetFunctionSloppyCall,
  kErrorCaptureStackTrace,
  kErrorIsError,
  kFunctionConstructorReturnedUndefined,
  kDefineGetterOrSetterWouldThrow,
  kRegExpPrototypeToString,
  kRegExpEscape,
  kAtomicsWaitAsync,
  kTemporalObject,
  kAsyncStackTaggingCreateTaskCall,
  kErrorStackTraceLimit,
  kMessageError,
  kMessageInfo,
  kMessageWarning,
  kMessageDebug,
  kMessageLog,
  kIndexAccessor,
  kAttemptOverrideReadOnlyOnPrototypeStrict,
  kAttemptOverrideReadOnlyOnPrototypeSloppy,
  kForcedGC,
  kInvalidatedArrayBufferDetachingProtector,
  kInvalidatedArrayConstructorProtector,
  kInvalidatedArrayIteratorLookupChainProtector,
  kInvalidatedArraySpeciesLookupChainProtector,
  kInvalidatedIsConcatSpreadableLookupChainProtector,
  kInvalidatedMapIteratorLookupChainProtector,
  kInvalidatedMegaDOMProtector,
  kInvalidatedNoElementsProtector,
  kInvalidatedNoProfilingProtector,
  kInvalidatedNoUndetectableObjectsProtector,
  kInvalidatedNumberStringNotRegexpLikeProtector,
  kInvalidatedPromiseHookProtector,
  kInvalidatedPromiseResolveLookupChainProtector,
  kInvalidatedPromiseSpeciesLookupChainProtector,
  kInvalidatedPromiseThenLookupChainProtector,
  kInvalidatedRegExpSpeciesLookupChainProtector,
  kInvalidatedSetIteratorLookupChainProtector,
  kInvalidatedStringIteratorLookupChainProtector,
  kInvalidatedStringLengthOverflowLookupChainProtector,
  kSloppyModeBlockScopedFunctionRedefinition,
  kForInInitializer,
  kHtmlComment,
  kHtmlCommentInExternalScript,
  kCompileHintsMagicAll,
  kSourceMappingUrlMagicCommentAtSign,
  kSloppyMode,
  kStrictMode,
  kUseAsm,
  kVarRedeclaredCatchBinding
};

// Isolate additions for use counters
class Isolate {
public:
  // Use counter features
  static constexpr int kFunctionPrototypeArguments = 0;
  static constexpr int kFunctionPrototypeCaller = 1;
  static constexpr int kArrayBufferTransfer = 2;
  static constexpr int kCallSiteAPIGetThisSloppyCall = 3;
  static constexpr int kCallSiteAPIGetFunctionSloppyCall = 4;
  static constexpr int kErrorCaptureStackTrace = 5;
  static constexpr int kErrorIsError = 6;
  static constexpr int kFunctionConstructorReturnedUndefined = 7;
  static constexpr int kDefineGetterOrSetterWouldThrow = 8;
  static constexpr int kRegExpPrototypeToString = 9;
  static constexpr int kRegExpEscape = 10;
  static constexpr int kAtomicsWaitAsync = 11;
  static constexpr int kTemporalObject = 12;
  static constexpr int kAsyncStackTaggingCreateTaskCall = 13;
  static constexpr int kErrorStackTraceLimit = 14;
  static constexpr int kMessageError = 15;
  static constexpr int kMessageInfo = 16;
  static constexpr int kMessageWarning = 17;
  static constexpr int kMessageDebug = 18;
  static constexpr int kMessageLog = 19;
  static constexpr int kIndexAccessor = 20;
  static constexpr int kAttemptOverrideReadOnlyOnPrototypeStrict = 21;
  static constexpr int kAttemptOverrideReadOnlyOnPrototypeSloppy = 22;
  static constexpr int kForcedGC = 23;
  static constexpr int kInvalidatedArrayBufferDetachingProtector = 24;
  static constexpr int kInvalidatedArrayConstructorProtector = 25;
  static constexpr int kInvalidatedArrayIteratorLookupChainProtector = 26;
  static constexpr int kInvalidatedArraySpeciesLookupChainProtector = 27;
  static constexpr int kInvalidatedIsConcatSpreadableLookupChainProtector = 28;
  static constexpr int kInvalidatedMapIteratorLookupChainProtector = 29;
  static constexpr int kInvalidatedMegaDOMProtector = 30;
  static constexpr int kInvalidatedNoElementsProtector = 31;
  static constexpr int kInvalidatedNoProfilingProtector = 32;
  static constexpr int kInvalidatedNoUndetectableObjectsProtector = 33;
  static constexpr int kInvalidatedNumberStringNotRegexpLikeProtector = 34;
  static constexpr int kInvalidatedPromiseHookProtector = 35;
  static constexpr int kInvalidatedPromiseResolveLookupChainProtector = 36;
  static constexpr int kInvalidatedPromiseSpeciesLookupChainProtector = 37;
  static constexpr int kInvalidatedPromiseThenLookupChainProtector = 38;
  static constexpr int kInvalidatedRegExpSpeciesLookupChainProtector = 39;
  static constexpr int kInvalidatedSetIteratorLookupChainProtector = 40;
  static constexpr int kInvalidatedStringIteratorLookupChainProtector = 41;
  static constexpr int kInvalidatedStringLengthOverflowLookupChainProtector = 42;
  static constexpr int kSloppyModeBlockScopedFunctionRedefinition = 43;
  static constexpr int kForInInitializer = 44;
  static constexpr int kHtmlComment = 45;
  static constexpr int kHtmlCommentInExternalScript = 46;
  static constexpr int kCompileHintsMagicAll = 47;
  static constexpr int kSourceMappingUrlMagicCommentAtSign = 48;
  static constexpr int kSloppyMode = 49;
  static constexpr int kStrictMode = 50;
  static constexpr int kUseAsm = 51;

  // Additional Isolate methods
  inline void RemoveCallCompletedCallback(void* callback) {}
  inline void AddCallCompletedCallback(void* callback) {}
  inline void RemoveNearHeapLimitCallback(void* callback, size_t limit) {}
  inline void SetCaptureStackTraceForUncaughtExceptions(bool capture, int frame_limit = 10, v8::StackTrace::StackTraceOptions options = static_cast<v8::StackTrace::StackTraceOptions>(0)) {}
  inline bool InContext() const { return true; }
  inline Local<Context> GetEnteredOrMicrotaskContext() { return Local<Context>(); }
  inline void RequestGarbageCollectionForTesting(GarbageCollectionType type) {}

  // Heap space statistics
  struct HeapSpaceStatistics {
    const char* space_name_;
    size_t space_size_;
    size_t space_used_size_;
    size_t space_available_size_;
    size_t physical_space_size_;
  };

  // SuppressMicrotaskExecutionScope
  class SuppressMicrotaskExecutionScope {
  public:
    explicit SuppressMicrotaskExecutionScope(Isolate* isolate) {}
  };
};

// ============================================================================
// CpuProfiler API Additions
// ============================================================================

class CpuProfiler {
public:
  // ProfilerId type
  using ProfilerId = int;

  // Create new profiler
  static CpuProfiler* New(Isolate* isolate, CpuProfilingNamingMode = kStandardNaming, CpuProfilingLoggingMode = kLazyLogging) {
    return nullptr;
  }

  // Start profiling
  void StartProfiling(Local<String> title, bool record_samples = false) {}
  void StartProfiling(Local<String> title, CpuProfilingOptions options) {}
};

class CpuProfile {
public:
  // Get sample timestamp
  int64_t GetSampleTimestamp(int index) const { return 0; }
};

class CpuProfileNode {
public:
  // Additional node methods
  const char* GetBailoutReason() const { return ""; }
  unsigned GetHitLineCount() const { return 0; }
};

// ============================================================================
// HeapProfiler API Additions
// ============================================================================

class HeapProfiler {
public:
  // Persistent handle class ID
  static constexpr uint16_t kPersistentHandleNoClassId = 0;

  // Additional heap profiler methods
  inline Local<Value> FindObjectById(SnapshotObjectId id) { return Local<Value>(); }
  inline void StopSamplingHeapProfiler() {}
  inline void ClearObjectIds() {}

  // Sampling flags (complete)
  enum SamplingFlags {
    kSamplingNoFlags = 0,
    kSamplingIncludeObjectsCollectedByMajorGC = 1 << 0,
    kSamplingIncludeObjectsCollectedByMinorGC = 1 << 1,
    kSamplingForceGC = 1 << 2
  };

  // Heap snapshot options
  struct HeapSnapshotOptions {
    enum NumericsMode {
      kExposeNumericValues = 0,
      kHideNumericValues = 1
    };

    enum ControlOption {
      kContinue = 0,
      kAbort = 1
    };

    NumericsMode numerics_mode = kHideNumericValues;
    ControlOption control = kContinue;
    StackState stack_state = StackState::kMayContainHeapPointers;
    void* snapshot_resolver = nullptr;
    void* global_object_name_resolver = nullptr;
  };

  // Object name resolver
  class ObjectNameResolver {
  public:
    virtual ~ObjectNameResolver() {}
    virtual const char* GetName(Local<Object> object) = 0;
  };
};

// Heap space statistics forward declaration
class HeapSpaceStatistics {
public:
  const char* space_name() const { return ""; }
  size_t space_size() const { return 0; }
  size_t space_used_size() const { return 0; }
  size_t space_available_size() const { return 0; }
  size_t physical_space_size() const { return 0; }
};

// ============================================================================
// OutputStream for Heap Snapshots
// ============================================================================

class OutputStream {
public:
  enum WriteResult {
    kContinue = 0,
    kAbort = 1
  };

  virtual ~OutputStream() {}
  virtual void EndOfStream() = 0;
  virtual WriteResult WriteAsciiChunk(char* data, int size) = 0;
  virtual int GetChunkSize() { return 1024; }
};

// ============================================================================
// AllocationProfile Additions
// ============================================================================

namespace AllocationProfile {
  static constexpr int kNoLineNumberInfo = 0;
  static constexpr int kNoColumnNumberInfo = 0;

  struct Allocation {
    size_t size;
    int count;

    Allocation() : size(0), count(0) {}
    Allocation(size_t s, int c) : size(s), count(c) {}
  };
}

// ============================================================================
// CodeEvent Types
// ============================================================================

namespace CodeEvent {
  enum Type {
    kBuiltinType,
    kCallbackType,
    kEvalType,
    kFunctionType,
    kHandlerType,
    kBytecodeHandlerType,
    kRegExpType,
    kScriptType,
    kStubType,
    kRelocationType
  };
}

// Message level constants
enum MessageLevel {
  kMessageLog = 0,
  kMessageDebug = 1,
  kMessageInfo = 2,
  kMessageError = 3,
  kMessageWarning = 4
};

// ============================================================================
// Value API Additions
// ============================================================================

class Value {
public:
  inline Local<Boolean> ToBoolean(Isolate* isolate) const {
    return Local<Boolean>();
  }
};

// ============================================================================
// Tracing API
// ============================================================================

namespace tracing {

enum CategoryGroupEnabledFlags {
  kEnabledForRecording_CategoryGroupEnabledFlags = 1 << 0,
  kEnabledForEventCallback_CategoryGroupEnabledFlags = 1 << 1
};

constexpr uint64_t kNoId = 0;
constexpr int kGlobalScope = 0;

class ScopedTracer {
public:
  ScopedTracer(char phase, const uint8_t* category_group_enabled, const char* name) {}
};

inline uint64_t AddTraceEvent(char phase, const uint8_t* category_enabled_flag,
                               const char* name, const char* scope, uint64_t id,
                               uint64_t bind_id, int num_args, const char** arg_names,
                               const uint8_t* arg_types, const uint64_t* arg_values,
                               unsigned int flags) {
  return 0;
}

} // namespace tracing

// ============================================================================
// EmbedderStackStateScope
// ============================================================================

class EmbedderStackStateScope {
public:
  EmbedderStackStateScope(Isolate* isolate, StackState stack_state) {}
  EmbedderStackStateScope(Isolate* isolate, Local<Context> context, StackState stack_state) {}
};

// ============================================================================
// Resource Constraints Additions
// ============================================================================

class ResourceConstraints {
public:
  size_t initial_young_generation_size_in_bytes() const { return 0; }
  size_t initial_old_generation_size_in_bytes() const { return 0; }
};

} // namespace v8

// ============================================================================
// Internal Namespace Additions
// ============================================================================

namespace v8 {
namespace internal {

// Base functions
namespace base {

// ScopedTimer stub
class ScopedTimer {
public:
  explicit ScopedTimer(void* timer) {}
};

// VLQ encoding/decoding
using ::v8::base::VLQEncode;
using ::v8::base::VLQDecode;
using ::v8::base::VLQDecodeUnsigned;
using ::v8::base::VLQBase64Decode;

// DTOA constants and functions
enum DtoaMode {
  DTOA_SHORTEST = 0,
  DTOA_FIXED = 1,
  DTOA_PRECISION = 2
};

inline void DoubleToAscii(double value, DtoaMode mode, int requested_digits,
                          char* buffer, int buffer_length,
                          bool* sign, int* length, int* point) {
  // Stub implementation
  *sign = false;
  *length = 0;
  *point = 0;
}

constexpr int kBase10MaximalLength = 17;

// FPU control
using ::v8::base::FPU;

// RingBuffer
template<typename T, int N>
using RingBuffer = ::v8::base::RingBuffer<T, N>;

// OS functions
using ::v8::base::OS;

// LockGuard
template<typename Mutex>
using LockGuard = ::v8::base::LockGuard<Mutex>;

// TemplateHashMapEntry
template<typename Key, typename Value>
using TemplateHashMapEntry = ::v8::base::TemplateHashMapEntry<Key, Value>;

// CallOnce
using ::v8::base::CallOnce;
using ::v8::base::OnceType;

// IterateWithoutFirst
using ::v8::base::IterateWithoutFirst;

// NoHashMapValue
struct NoHashMapValue {};

// overloaded helper
using ::v8::base::overloaded;

} // namespace base

// Foreign address tags
constexpr uint64_t kWaiterQueueForeignTag = 0;
constexpr uint64_t kMicrotaskCallbackDataTag = 1;
constexpr uint64_t kMessageListenerTag = 2;
constexpr uint64_t kCFunctionTag = 3;
constexpr uint64_t kCFunctionInfoTag = 4;
constexpr uint64_t kSyntheticModuleTag = 5;
constexpr uint64_t kWaiterQueueNodeTag = 6;
constexpr uint64_t kDisplayNamesInternalTag = 7;

// ToCData helper
template<typename T>
inline T ToCData(void* ptr) {
  return reinterpret_cast<T>(ptr);
}

// Internals additions
struct Internals {
  static constexpr int kNodeStateMask = 0x7;
  static constexpr int kNodeStateIsWeakValue = 2;
  static constexpr int kDisallowGarbageCollectionAlign = 1;
  static constexpr int kDisallowGarbageCollectionSize = 4;
};

// GRACEFUL_FATAL macro
#ifndef GRACEFUL_FATAL
#define GRACEFUL_FATAL(msg) FATAL(msg)
#endif

// V8_DECLARE_ONCE macro
#ifndef V8_DECLARE_ONCE
#define V8_DECLARE_ONCE(name) ::v8::base::OnceType name = V8_ONCE_INIT
#endif

// Garbage collection reason max value
constexpr int kGarbageCollectionReasonMaxValue = 100;

// SNPrintF wrapper
template<size_t N>
inline int SNPrintF(char (&buffer)[N], const char* format, ...) {
  va_list args;
  va_start(args, format);
  int result = vsnprintf(buffer, N, format, args);
  va_end(args);
  return result;
}

// Strtod wrapper
inline double Strtod(const char* str, char** endptr) {
  return strtod(str, endptr);
}

// IntToSmi helper
inline void* IntToSmi(int value) {
  return reinterpret_cast<void*>(static_cast<intptr_t>(value) << 1);
}

} // namespace internal
} // namespace v8

#endif  // WASI_RUNTIME_PROFILER_FIXES_H_
