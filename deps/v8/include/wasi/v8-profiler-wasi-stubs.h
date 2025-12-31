#ifndef V8_WASI_PROFILER_STUBS_H_
#define V8_WASI_PROFILER_STUBS_H_

#ifdef __wasi__

// WASI stubs for V8 profiler types that are not fully implemented
// These provide minimal definitions to allow compilation
// This file is included from profile-generator.h for internal use
// NOTE: v8-profiler-stubs.h provides the main API types for v8-profiler.h

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace v8 {

// Forward declarations
class Isolate;
class CpuProfiler;
class HeapProfiler;

// Only define these types if v8-profiler-stubs.h hasn't already defined them
#ifndef WASI_V8_PROFILER_STUBS_H_

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

// CpuProfilingMode enum for profiling configuration
enum class CpuProfilingMode {
  kLeafNodeLineNumbers,
  kCallerLineNumbers
};

#endif  // WASI_V8_PROFILER_STUBS_H_

// Unqualified constants for profiler modes - needed for default arguments
constexpr CpuProfilingNamingMode kStandardNaming = CpuProfilingNamingMode::kStandardNaming;
constexpr CpuProfilingNamingMode kDebugNaming = CpuProfilingNamingMode::kDebugNaming;
constexpr CpuProfilingLoggingMode kLazyLogging = CpuProfilingLoggingMode::kLazyLogging;
constexpr CpuProfilingLoggingMode kEagerLogging = CpuProfilingLoggingMode::kEagerLogging;
constexpr CpuProfilingMode kLeafNodeLineNumbers = CpuProfilingMode::kLeafNodeLineNumbers;
constexpr CpuProfilingMode kCallerLineNumbers = CpuProfilingMode::kCallerLineNumbers;

namespace internal {

// Forward declarations
class CodeEventObserver;
class LogEventListener;

// Import types from v8 namespace for internal use
// These use the types from either this file or v8-profiler-stubs.h
using ProfilerId = uint32_t;

// Import profiler types and constants into internal namespace
using CpuProfilingNamingMode = ::v8::CpuProfilingNamingMode;
using CpuProfilingLoggingMode = ::v8::CpuProfilingLoggingMode;
using CpuProfilingMode = ::v8::CpuProfilingMode;
using CpuProfilingStatus = ::v8::CpuProfilingStatus;
using CpuProfilingResult = ::v8::CpuProfilingResult;

// Unqualified constants in internal namespace
constexpr CpuProfilingNamingMode kStandardNaming = ::v8::kStandardNaming;
constexpr CpuProfilingNamingMode kDebugNaming = ::v8::kDebugNaming;
constexpr CpuProfilingLoggingMode kLazyLogging = ::v8::kLazyLogging;
constexpr CpuProfilingLoggingMode kEagerLogging = ::v8::kEagerLogging;
constexpr CpuProfilingMode kLeafNodeLineNumbers = ::v8::kLeafNodeLineNumbers;
constexpr CpuProfilingMode kCallerLineNumbers = ::v8::kCallerLineNumbers;

// Discarded samples delegate - callback for discarded profiling samples
class DiscardedSamplesDelegate {
 public:
  virtual ~DiscardedSamplesDelegate() = default;
  virtual void Notify() = 0;
  void SetId(unsigned id) { id_ = id; }
  unsigned GetId() const { return id_; }
 private:
  unsigned id_ = 0;
};

}  // namespace internal

}  // namespace v8

#endif  // __wasi__

#endif  // V8_WASI_PROFILER_STUBS_H_
