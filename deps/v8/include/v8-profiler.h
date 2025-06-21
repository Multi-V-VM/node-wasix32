#ifndef INCLUDE_V8_PROFILER_H_
#define INCLUDE_V8_PROFILER_H_

#ifdef __wasi__
// WASI-specific v8-profiler implementation

#include <memory>
#include <vector>
#include <string>

namespace v8 {

// Forward declarations
class Isolate;
template<typename T> class Local;
class String;
class Object;
class Context;

// Basic profiling interfaces
class CpuProfiler {
public:
  virtual ~CpuProfiler() = default;
};

class HeapProfiler {
public:
  virtual ~HeapProfiler() = default;
};

// EmbedderGraph for memory tracking
class EmbedderGraph {
public:
  class Node {
  public:
    enum class Detachedness { kUnknown, kDetached, kAttached };
    virtual ~Node() = default;
  };
  virtual ~EmbedderGraph() = default;
};

} // namespace v8

#else
// Non-WASI: Include original v8-profiler
// Original content would go here
#endif // __wasi__

#endif // INCLUDE_V8_PROFILER_H_
