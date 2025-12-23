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

// CPU Profiler naming modes (in v8 namespace for public API)
enum class CpuProfilingNamingMode {
  kStandardNaming,
  kDebugNaming
};

// CPU Profiler logging modes (in v8 namespace for public API)
enum class CpuProfilingLoggingMode {
  kLazyLogging,
  kEagerLogging
};

// ProfilerId - Unique identifier for profilers (in v8 namespace like original)
using ProfilerId = uint32_t;

// CPU Profiling status enum (in v8 namespace like original)
enum class CpuProfilingStatus {
  kStarted,
  kAlreadyStarted,
  kErrorTooManyProfilers
};

// CPU Profiling result struct (in v8 namespace like original)
struct CpuProfilingResult {
  const ProfilerId id;
  const CpuProfilingStatus status;
};

namespace internal {

// Forward declarations
class CodeEventObserver;
class LogEventListener;

// Import ProfilerId from v8 namespace for internal use
using ProfilerId = v8::ProfilerId;

// Discarded samples delegate - callback for discarded profiling samples
class DiscardedSamplesDelegate {
 public:
  virtual ~DiscardedSamplesDelegate() = default;
  virtual void NotifyDiscardedSamples(int count) = 0;
};

// Code entry - represents a single code entry in the profiler
class CodeEntry {
 public:
  CodeEntry() = default;
  virtual ~CodeEntry() = default;

  CodeEntry(const CodeEntry&) = delete;
  CodeEntry& operator=(const CodeEntry&) = delete;
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
  class Listener {
   public:
    virtual ~Listener() = default;
  };

  WeakCodeRegistry() = default;
  ~WeakCodeRegistry() = default;

  WeakCodeRegistry(const WeakCodeRegistry&) = delete;
  WeakCodeRegistry& operator=(const WeakCodeRegistry&) = delete;
};

// ProfilerListener - listens to code events for profiling
class ProfilerListener {
 public:
  ProfilerListener(Isolate*, CodeEventObserver*,
                   CodeEntryStorage& code_entry_storage,
                   WeakCodeRegistry& weak_code_registry,
                   CpuProfilingNamingMode mode = CpuProfilingNamingMode::kDebugNaming) {}
  virtual ~ProfilerListener() = default;

  ProfilerListener(const ProfilerListener&) = delete;
  ProfilerListener& operator=(const ProfilerListener&) = delete;
};

// Also expose naming modes in internal namespace for internal code
using CpuProfilingNamingMode = v8::CpuProfilingNamingMode;
using CpuProfilingLoggingMode = v8::CpuProfilingLoggingMode;

// Use kDebugNaming constant
constexpr CpuProfilingNamingMode kDebugNaming = CpuProfilingNamingMode::kDebugNaming;
constexpr CpuProfilingNamingMode kStandardNaming = CpuProfilingNamingMode::kStandardNaming;

}  // namespace internal

}  // namespace v8

#endif  // __wasi__

#endif  // V8_WASI_PROFILER_STUBS_H_
