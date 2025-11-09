#ifndef V8_WASI_PROFILER_STUBS_H_
#define V8_WASI_PROFILER_STUBS_H_

#ifdef __wasi__

// WASI stubs for V8 profiler types that are not fully implemented
// These provide minimal definitions to allow compilation

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace v8 {

// Forward declarations
class Isolate;
class CpuProfiler;
class HeapProfiler;

namespace internal {

// ProfilerId - Unique identifier for profilers
using ProfilerId = int;

// Discarded samples delegate - callback for discarded profiling samples
class DiscardedSamplesDelegate {
 public:
  virtual ~DiscardedSamplesDelegate() = default;
  virtual void NotifyDiscardedSamples(int count) = 0;
};

// CPU Profiling result type
enum class CpuProfilingResult {
  kSuccess,
  kAlreadyStarted,
  kErrorTooManySamples
};

// Code entry storage - manages lifetime of profiler code entries
class CodeEntryStorage {
 public:
  CodeEntryStorage() = default;
  ~CodeEntryStorage() = default;

  CodeEntryStorage(const CodeEntryStorage&) = delete;
  CodeEntryStorage& operator=(const CodeEntryStorage&) = delete;
};

// Weak code registry - tracks code objects for profiling
class WeakCodeRegistry {
 public:
  WeakCodeRegistry() = default;
  ~WeakCodeRegistry() = default;

  WeakCodeRegistry(const WeakCodeRegistry&) = delete;
  WeakCodeRegistry& operator=(const WeakCodeRegistry&) = delete;
};

}  // namespace internal

// CPU Profiler naming modes
enum class CpuProfilingNamingMode {
  kStandardNaming,
  kDebugNaming
};

// CPU Profiler logging modes
enum class CpuProfilingLoggingMode {
  kLazyLogging,
  kEagerLogging
};

}  // namespace v8

#endif  // __wasi__

#endif  // V8_WASI_PROFILER_STUBS_H_
