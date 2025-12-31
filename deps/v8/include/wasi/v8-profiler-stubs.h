#ifndef WASI_V8_PROFILER_STUBS_H_
#define WASI_V8_PROFILER_STUBS_H_

#ifdef __wasi__

#include <vector>
#include <string>

namespace v8 {

// Forward declarations (Local, String, Value, Context already declared in v8-forward.h)
class Isolate;

// Forward declarations
class CpuProfile;
class CpuProfileNode;
class EmbedderGraph;

// CpuProfileDeoptFrame stub - defined before CpuProfileDeoptInfo
struct CpuProfileDeoptFrame {
  int script_id;
  size_t position;
};

// CpuProfileDeoptInfo stub - defined before CpuProfileNode uses it
struct CpuProfileDeoptInfo {
  const char* deopt_reason;
  std::vector<CpuProfileDeoptFrame> stack;
};

// CpuProfileNode stub
class CpuProfileNode {
 public:
  // Stubbed constants for API compatibility
  static constexpr int kNoLineNumberInfo = 0;
  static constexpr int kNoColumnNumberInfo = 0;

  struct LineTick {
    int line;
    unsigned hit_count;
  };
  enum SourceType {
    kScript = 0,
    kBuiltin = 1,
    kCallback = 2,
    kInternal = 3,
    kUnresolved = 4
  };

  const char* GetFunctionName() const { return ""; }
  const char* GetFunctionNameStr() const { return ""; }
  int GetScriptId() const { return 0; }
  const char* GetScriptResourceName() const { return ""; }
  const char* GetScriptResourceNameStr() const { return ""; }
  const char* GetBailoutReason() const { return ""; }
  int GetLineNumber() const { return 0; }
  int GetColumnNumber() const { return 0; }
  unsigned int GetHitCount() const { return 0; }
  unsigned int GetNodeId() const { return 0; }
  int GetChildrenCount() const { return 0; }
  const CpuProfileNode* GetChild(int index) const { return nullptr; }
  SourceType GetSourceType() const { return kScript; }
  // GetLineTicks - two overloads for compatibility
  const LineTick* GetLineTicks() const { return nullptr; }
  bool GetLineTicks(LineTick* entries, unsigned int length) const { return false; }
  int GetHitLineCount() const { return 0; }
  const std::vector<CpuProfileDeoptInfo>& GetDeoptInfos() const {
    static std::vector<CpuProfileDeoptInfo> empty;
    return empty;
  }
};

// CpuProfile stub
class CpuProfile {
 public:
  const CpuProfileNode* GetTopDownRoot() const { return nullptr; }
  const CpuProfileNode* GetSample(int index) const { return nullptr; }
  int GetSamplesCount() const { return 0; }
  uint64_t GetSampleTimestamp(int /*index*/) const { return 0; }
  const char* GetTitle() const { return ""; }
  int64_t GetStartTime() const { return 0; }
  int64_t GetEndTime() const { return 0; }
  void Delete() {}
};

// CpuProfiler stub
class CpuProfiler {
 public:
  // Use the v8:: namespace enum classes for type compatibility
  using CpuProfilingMode = v8::CpuProfilingMode;
  using CpuProfilingNamingMode = v8::CpuProfilingNamingMode;
  using CpuProfilingLoggingMode = v8::CpuProfilingLoggingMode;

  // Re-export the enum values for compatibility with code using CpuProfiler::kXxx syntax
  static constexpr CpuProfilingMode kLeafNodeLineNumbers = CpuProfilingMode::kLeafNodeLineNumbers;
  static constexpr CpuProfilingMode kCallerLineNumbers = CpuProfilingMode::kCallerLineNumbers;
  static constexpr CpuProfilingNamingMode kStandardNaming = CpuProfilingNamingMode::kStandardNaming;
  static constexpr CpuProfilingNamingMode kDebugNaming = CpuProfilingNamingMode::kDebugNaming;
  static constexpr CpuProfilingLoggingMode kLazyLogging = CpuProfilingLoggingMode::kLazyLogging;
  static constexpr CpuProfilingLoggingMode kEagerLogging = CpuProfilingLoggingMode::kEagerLogging;

  struct CpuProfilingOptions {
    CpuProfilingMode mode_;
    unsigned max_samples;
    int sampling_interval_us;
    CpuProfilingOptions()
        : mode_(kLeafNodeLineNumbers),
          max_samples(0),
          sampling_interval_us(0) {}
    CpuProfilingOptions(CpuProfilingMode mode, unsigned max_samples_arg = 0,
                        int sampling_interval_us_arg = 0)
        : mode_(mode),
          max_samples(max_samples_arg),
          sampling_interval_us(sampling_interval_us_arg) {}
  };

  void SetSamplingInterval(int us) {}
  void SetUsePreciseSampling(bool) {}
  void StartProfiling(const char* title, CpuProfilingOptions options = CpuProfilingOptions()) {}
  void StartProfiling(const char* title, CpuProfilingMode mode,
                     bool record_samples = false) {}
  void StartProfiling(const char* title, bool record_samples = false) {}
  void StartProfiling(Local<String> title, CpuProfilingOptions options = CpuProfilingOptions()) {}
  void StartProfiling(Local<String> title, CpuProfilingMode mode,
                     bool record_samples = false) {}
  void StartProfiling(Local<String> title, bool record_samples = false) {}
  CpuProfile* StopProfiling(const char* title) { return nullptr; }
  CpuProfile* StopProfiling(Local<String> title) { return nullptr; }
  void Dispose() {}

  static CpuProfiler* New(Isolate* /*isolate*/) { return new CpuProfiler(); }

  static void CollectSample(Isolate* isolate) {}
};

// Provide a top-level alias for compatibility with src callers that expect
// v8::CpuProfilingMode. Only define if not already defined by v8-profiler-wasi-stubs.h
#ifndef V8_WASI_PROFILER_STUBS_H_
using CpuProfilingMode = CpuProfiler::CpuProfilingMode;
#endif

// DiscardedSamplesDelegate stub
class DiscardedSamplesDelegate {
 public:
  virtual ~DiscardedSamplesDelegate() = default;
  virtual void Notify() {}
  void SetId(unsigned id) { id_ = id; }
  unsigned GetId() const { return id_; }
 private:
  unsigned id_ = 0;
};

// Provide a top-level CpuProfilingOptions compatible with src/* expectations.
struct CpuProfilingOptions {
  using CpuProfilingMode = CpuProfiler::CpuProfilingMode;
  static constexpr unsigned kNoSampleLimit = 0;
  CpuProfilingMode mode_;
  unsigned max_samples_limit_;
  int sampling_interval_us_;
  void* filter_context_ = nullptr;
  CpuProfilingOptions()
      : mode_(CpuProfiler::kLeafNodeLineNumbers),
        max_samples_limit_(kNoSampleLimit),
        sampling_interval_us_(0),
        filter_context_(nullptr) {}
  explicit CpuProfilingOptions(CpuProfilingMode mode,
                               unsigned max_samples_arg = kNoSampleLimit,
                               int sampling_interval_us_arg = 0,
                               void* filter_context = nullptr)
      : mode_(mode),
        max_samples_limit_(max_samples_arg),
        sampling_interval_us_(sampling_interval_us_arg),
        filter_context_(filter_context) {}
  unsigned max_samples() const { return max_samples_limit_; }
  int sampling_interval_us() const { return sampling_interval_us_; }
  CpuProfilingMode mode() const { return mode_; }
  bool has_filter_context() const { return filter_context_ != nullptr; }
  void* raw_filter_context() const { return filter_context_; }
};

// CodeEvent types for logging
struct CodeEvent {
  enum Type {
    kUnknown = 0,
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
};

using CodeEventType = CodeEvent::Type;

// JitCodeEventHandler (formerly CodeEventHandler)
using JitCodeEventHandler = void (*)(const struct JitCodeEvent* event);

// CodeEventHandler - class-based handler for code events
class CodeEventHandler {
 public:
  virtual ~CodeEventHandler() = default;
  virtual void Handle(CodeEvent* event) {}
};

// AllocationProfile forward declarations
class AllocationProfile {
 public:
  static constexpr int kNoLineNumberInfo = 0;
  static constexpr int kNoColumnNumberInfo = 0;
  class Node;
  class Sample;
  struct Allocation;

  struct Allocation {
    size_t size;
    unsigned int count;
  };

  // Node structure for heap allocation profiling
  // Fields match what sampling-heap-profiler.cc expects for aggregate initialization
  struct Node {
    using Code = void*;
    Local<String> name;           // Script/function name (Local<String>, not const char*)
    Local<String> script_name;    // Script resource name
    int script_id;
    int start_position;
    int line_number;
    int column_number;
    unsigned int node_id;         // Unique node identifier
    std::vector<Node*> children;
    std::vector<Allocation> allocations;
  };

  struct Sample {
    unsigned int node_id;
    size_t size;
    unsigned int count;
    uint64_t sample_id;
  };

  virtual ~AllocationProfile() = default;
  virtual Node* GetRootNode() { return nullptr; }
  virtual const std::vector<Sample>& GetSamples() {
    static std::vector<Sample> empty;
    return empty;
  }
};

// Note: HeapGraphNode, HeapSnapshot, and HeapProfiler are defined in embedder-graph-stub.h

}  // namespace v8

// (HeapProfiler is defined in embedder-graph-stub.h for WASI builds)

#endif  // __wasi__

#endif  // WASI_V8_PROFILER_STUBS_H_
