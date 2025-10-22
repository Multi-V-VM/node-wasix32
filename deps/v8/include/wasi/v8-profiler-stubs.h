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
  int GetScriptId() const { return 0; }
  const char* GetScriptResourceName() const { return ""; }
  int GetLineNumber() const { return 0; }
  int GetColumnNumber() const { return 0; }
  unsigned int GetHitCount() const { return 0; }
  unsigned int GetNodeId() const { return 0; }
  int GetChildrenCount() const { return 0; }
  const CpuProfileNode* GetChild(int index) const { return nullptr; }
  SourceType GetSourceType() const { return kScript; }
  const LineTick* GetLineTicks() const { return nullptr; }
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
  const char* GetTitle() const { return ""; }
  int64_t GetStartTime() const { return 0; }
  int64_t GetEndTime() const { return 0; }
  void Delete() {}
};

// CpuProfiler stub
class CpuProfiler {
 public:
  enum CpuProfilingMode {
    kLeafNodeLineNumbers,
    kCallerLineNumbers
  };

  enum CpuProfilingNamingMode {
    kStandardNaming,
    kDebugNaming
  };

  enum CpuProfilingLoggingMode {
    kLazyLogging,
    kEagerLogging
  };

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
  CpuProfile* StopProfiling(const char* title) { return nullptr; }
  void Dispose() {}

  static void CollectSample(Isolate* isolate) {}
};

// Provide a top-level alias for compatibility with src callers that expect
// v8::CpuProfilingMode.
using CpuProfilingMode = CpuProfiler::CpuProfilingMode;

// Provide a top-level CpuProfilingOptions compatible with src/* expectations.
struct CpuProfilingOptions {
  using CpuProfilingMode = CpuProfiler::CpuProfilingMode;
  static constexpr unsigned kNoSampleLimit = 0;
  CpuProfilingMode mode_;
  unsigned max_samples_limit_;
  int sampling_interval_us_;
  CpuProfilingOptions()
      : mode_(CpuProfiler::kLeafNodeLineNumbers),
        max_samples_limit_(kNoSampleLimit),
        sampling_interval_us_(0) {}
  explicit CpuProfilingOptions(CpuProfilingMode mode,
                               unsigned max_samples_arg = kNoSampleLimit,
                               int sampling_interval_us_arg = 0)
      : mode_(mode),
        max_samples_limit_(max_samples_arg),
        sampling_interval_us_(sampling_interval_us_arg) {}
  unsigned max_samples() const { return max_samples_limit_; }
  int sampling_interval_us() const { return sampling_interval_us_; }
};

// CodeEvent types for logging
struct CodeEvent {
  enum Type {
    kUnknown = 0
  };
};

using CodeEventType = CodeEvent::Type;

// JitCodeEventHandler (formerly CodeEventHandler)
using JitCodeEventHandler = void (*)(const struct JitCodeEvent* event);
using CodeEventHandler = JitCodeEventHandler;  // Alias for compatibility

// AllocationProfile forward declarations
class AllocationProfile {
 public:
  class Node;
  class Sample;

  class Node {
   public:
    using Code = void*;
    const char* name() const { return ""; }
    const char* script_name() const { return ""; }
    int script_id() const { return 0; }
    int line_number() const { return 0; }
    int column_number() const { return 0; }
    size_t byte_size() const { return 0; }
    std::vector<Node*> children() const { return {}; }
   std::vector<Sample*> allocations() const { return {}; }
  };

  class Sample {
   public:
    size_t size() const { return 0; }
    unsigned int node_id() const { return 0; }
    int script_id() const { return 0; }
  };

  virtual Node* GetRootNode() const { return nullptr; }
  virtual const std::vector<Sample>& GetSamples() const {
    static std::vector<Sample> empty;
    return empty;
  }
};

// Note: HeapGraphNode, HeapSnapshot, and HeapProfiler are defined in embedder-graph-stub.h

}  // namespace v8

// (HeapProfiler is defined in embedder-graph-stub.h for WASI builds)

#endif  // __wasi__

#endif  // WASI_V8_PROFILER_STUBS_H_
