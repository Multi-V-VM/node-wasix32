#ifndef V8_WASI_EMBEDDER_GRAPH_STUB_H_
#define V8_WASI_EMBEDDER_GRAPH_STUB_H_

#include <memory>
#include <string>

#include "v8-local-handle.h"
#include "v8-value.h"

// Define WASI stubs for profiler
#define V8_WASI_EMBEDDER_GRAPH_STUB_ACTIVE 1

namespace v8 {

// Forward declarations
class HeapGraphNode;
class HeapGraphEdge;
using SnapshotObjectId = uint32_t;

// HeapStatsUpdate struct definition
struct HeapStatsUpdate {
  uint32_t index;
  uint32_t count;
  uint32_t size;
};

// OutputStream class definition
class OutputStream {
 public:
  enum WriteResult {
    kAbort = 0,
    kContinue = 1
  };
  
  virtual ~OutputStream() = default;
  virtual void EndOfStream() = 0;
  virtual int GetChunkSize() = 0;
  virtual WriteResult WriteAsciiChunk(char* data, int size) = 0;
  virtual void WriteHeapStatsChunk(const HeapStatsUpdate* data, int count) = 0;
};

// SerializationFormat enum
enum class SerializationFormat {
  kJSON = 0  // Only JSON format for WASI
};

// HeapSnapshot base class definition
class HeapSnapshot {
 public:
  static const SerializationFormat kJSON = SerializationFormat::kJSON;
  
  virtual ~HeapSnapshot() = default;
  virtual void Delete() = 0;
  virtual void Serialize(OutputStream* stream, SerializationFormat format) const = 0;
  virtual const HeapGraphNode* GetRoot() const = 0;
  virtual const HeapGraphNode* GetNodeById(SnapshotObjectId id) const = 0;
  virtual int GetNodesCount() const = 0;
  virtual const HeapGraphNode* GetNode(int index) const = 0;
  virtual SnapshotObjectId GetMaxSnapshotJSObjectId() const = 0;
};


// Stub for EmbedderGraph - used for heap snapshots
#ifndef V8_EMBEDDER_GRAPH_DEFINED
#define V8_EMBEDDER_GRAPH_DEFINED
class EmbedderGraph {
 public:
  class Node {
   public:
    enum class Detachedness {
      kUnknown = 0,
      kAttached = 1,
      kDetached = 2,
    };
    
    virtual ~Node() = default;
    virtual const char* Name() = 0;
    virtual const char* NamePrefix() = 0;
    virtual size_t SizeInBytes() = 0;
    virtual Node* JSWrapperNode() { return nullptr; }
    virtual Node* WrapperNode() { return JSWrapperNode(); }  // Alias for compatibility
    virtual bool IsRootNode() { return false; }
    virtual bool IsEmbedderNode() { return true; }
    virtual Detachedness GetDetachedness() { return Detachedness::kUnknown; }
  };
  
  virtual ~EmbedderGraph() = default;
  virtual Node* V8Node(Local<Value> value) = 0;
  virtual Node* AddNode(::std::unique_ptr<Node> node) = 0;
  virtual void AddEdge(Node* from, Node* to, const char* name = nullptr) = 0;
};
#endif // V8_EMBEDDER_GRAPH_DEFINED

// QueryObjectPredicate class for WASI
#ifndef V8_QUERY_OBJECT_PREDICATE_DEFINED
#define V8_QUERY_OBJECT_PREDICATE_DEFINED
class QueryObjectPredicate {
 public:
  virtual ~QueryObjectPredicate() = default;
  virtual bool Filter(Local<Object> object) = 0;
};
#endif // V8_QUERY_OBJECT_PREDICATE_DEFINED

// Stub for HeapProfiler
#ifndef V8_HEAP_PROFILER_DEFINED
#define V8_HEAP_PROFILER_DEFINED
class HeapProfiler {
 public:
  // HeapSnapshotMode enum for WASI
  enum class HeapSnapshotMode {
    kRegular = 0,
    kExposeInternals = 1,
  };
  
  struct HeapSnapshotOptions {
    enum class ControlOption {
      kDefault = 0,
    };
    
    enum class NumericsMode {
      kHideNumericValues = 0,
      kExposeNumericValues = 1,
    };
    
    ControlOption control;
    NumericsMode numerics_mode;
    bool capture_numeric_value;
    HeapSnapshotMode snapshot_mode;
    
    // Constructor with default values
    HeapSnapshotOptions() 
      : control(ControlOption::kDefault),
        numerics_mode(NumericsMode::kHideNumericValues),
        capture_numeric_value(false),
        snapshot_mode(HeapSnapshotMode::kRegular) {}
  };
  
  // Callback types
  using BuildEmbedderGraphCallback = void (*)(v8::Isolate* isolate, EmbedderGraph* graph, void* data);
  
  // WASI: Add missing methods
  void RemoveBuildEmbedderGraphCallback(BuildEmbedderGraphCallback callback, void* data) {
    // WASI stub - no-op
  }
  
  void QueryObjects(Local<Context> context,
                    QueryObjectPredicate* predicate,
                    std::vector<Global<Object>>* objects) {
    // WASI stub - no-op
    // In a real implementation, this would search the heap for objects
    // matching the predicate
  }
  
  const HeapSnapshot* TakeHeapSnapshot(
      const HeapSnapshotOptions& options = HeapSnapshotOptions()) {
    // WASI stub - return a minimal heap snapshot
    static struct MinimalHeapSnapshot : public HeapSnapshot {
      void Delete() override {}
      void Serialize(OutputStream* stream, SerializationFormat format) const override {
        // Write minimal JSON output
        const char* json = "{}";
        stream->WriteAsciiChunk(const_cast<char*>(json), 2);
        stream->EndOfStream();
      }
      const HeapGraphNode* GetRoot() const override { return nullptr; }
      const HeapGraphNode* GetNodeById(SnapshotObjectId id) const override { return nullptr; }
      int GetNodesCount() const override { return 0; }
      const HeapGraphNode* GetNode(int index) const override { return nullptr; }
      SnapshotObjectId GetMaxSnapshotJSObjectId() const override { return 0; }
    } snapshot;
    return &snapshot;
  }
  
  void DeleteAllHeapSnapshots() {
    // WASI stub - no-op
  }
  
  void StartTrackingHeapObjects(bool track_allocations = false) {
    // WASI stub - no-op
  }
  
  void StopTrackingHeapObjects() {
    // WASI stub - no-op
  }
  
  void AddBuildEmbedderGraphCallback(BuildEmbedderGraphCallback callback, void* data) {
    // WASI stub - no-op
  }
  
  ~HeapProfiler() = default;
};
#endif // V8_HEAP_PROFILER_DEFINED

// Stub for HeapGraphEdge - defined before HeapGraphNode due to dependency
#ifndef V8_HEAP_GRAPH_EDGE_DEFINED
#define V8_HEAP_GRAPH_EDGE_DEFINED
class HeapGraphEdge {
 public:
  enum Type {
    kContextVariable = 0,
    kElement = 1,
    kProperty = 2,
    kInternal = 3,
    kHidden = 4,
    kShortcut = 5,
    kWeak = 6,
  };
  
  virtual ~HeapGraphEdge() = default;
  virtual Type GetType() const = 0;
  virtual Local<Value> GetName() const = 0;
  virtual const HeapGraphNode* GetFromNode() const = 0;
  virtual const HeapGraphNode* GetToNode() const = 0;
};
#endif // V8_HEAP_GRAPH_EDGE_DEFINED

// Stub for HeapGraphNode
#ifndef V8_HEAP_GRAPH_NODE_DEFINED
#define V8_HEAP_GRAPH_NODE_DEFINED
class HeapGraphNode {
 public:
  enum Type {
    kHidden = 0,
    kArray = 1,
    kString = 2,
    kObject = 3,
    kCode = 4,
    kClosure = 5,
    kRegExp = 6,
    kHeapNumber = 7,
    kNative = 8,
    kSynthetic = 9,
    kConsString = 10,
    kSlicedString = 11,
    kSymbol = 12,
    kSimdValue = 13,
    kBigInt = 14,
  };
  
  virtual ~HeapGraphNode() = default;
  virtual Type GetType() const = 0;
  virtual Local<String> GetName() const = 0;
  virtual SnapshotObjectId GetId() const = 0;
  virtual size_t GetShallowSize() const = 0;
  virtual int GetChildrenCount() const = 0;
  virtual const HeapGraphEdge* GetChild(int index) const = 0;
};
#endif // V8_HEAP_GRAPH_NODE_DEFINED

// Stub for OutputStream
// OutputStream is defined above

// HeapSnapshot is defined above

}  // namespace v8

#endif  // V8_WASI_EMBEDDER_GRAPH_STUB_H_