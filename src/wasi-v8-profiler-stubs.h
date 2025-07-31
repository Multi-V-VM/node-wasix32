#ifndef WASI_V8_PROFILER_STUBS_H_
#define WASI_V8_PROFILER_STUBS_H_

#ifdef __wasi__

namespace v8 {

// Stub for QueryObjectPredicate
class QueryObjectPredicate {
 public:
  virtual ~QueryObjectPredicate() = default;
  virtual bool Filter(Local<Object> object) = 0;
};

// Note: HeapProfiler class is provided by V8 headers

// Stub for OutputStream
class OutputStream {
 public:
  enum WriteResult {
    kContinue = 0,
    kAbort = 1
  };

  virtual ~OutputStream() = default;
  virtual int GetChunkSize() = 0;
  virtual void EndOfStream() = 0;
  virtual WriteResult WriteAsciiChunk(char* data, int size) = 0;
  virtual WriteResult WriteHeapStatsChunk(HeapStatsUpdate* data, int count) { 
    return kContinue; 
  }
};

// Forward declaration
struct HeapStatsUpdate;

// Additional V8 API methods
namespace internal {

// Value methods
template<>
struct ApiCheck<Value> {
  static bool IsValue(Data* data) { return true; }
};

} // namespace internal

} // namespace v8

#endif // __wasi__

#endif // WASI_V8_PROFILER_STUBS_H_