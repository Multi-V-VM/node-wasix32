#ifndef V8_WASI_EMBEDDER_GRAPH_STUB_H_
#define V8_WASI_EMBEDDER_GRAPH_STUB_H_

#include <memory>
#include <string>

#include "v8-local-handle.h"
#include "v8-value.h"
#include "v8-primitive.h"

// Only define these stubs if V8_PROFILER_H_ hasn't been included yet
#ifndef V8_PROFILER_H_
#define V8_WASI_EMBEDDER_GRAPH_STUB_ACTIVE 1

namespace v8 {

// Forward declarations
class HeapGraphNode;
class HeapGraphEdge;
class OutputStream;
class HeapSnapshot;
using SnapshotObjectId = uint32_t;

struct HeapStatsUpdate {
  uint32_t index;
  uint32_t count;
  uint32_t size;
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
    virtual bool IsRootNode() { return false; }
    virtual Detachedness GetDetachedness() { return Detachedness::kUnknown; }
  };
  
  virtual ~EmbedderGraph() = default;
  virtual Node* V8Node(Local<Value> value) = 0;
  virtual Node* AddNode(::std::unique_ptr<Node> node) = 0;
  virtual void AddEdge(Node* from, Node* to, const char* name = nullptr) = 0;
};
#endif // V8_EMBEDDER_GRAPH_DEFINED

// Stub for HeapProfiler
#ifndef V8_HEAP_PROFILER_DEFINED
#define V8_HEAP_PROFILER_DEFINED
class HeapProfiler {
 public:
  struct HeapSnapshotOptions {
    enum class ControlOption {
      kDefault = 0,
    };
    
    enum class NumericsMode {
      kHideNumericValues = 0,
      kExposeNumericValues = 1,
    };
    
    ControlOption control = ControlOption::kDefault;
    NumericsMode numerics_mode = NumericsMode::kHideNumericValues;
    bool capture_numeric_value = false;
  };
  
  ~HeapProfiler() = default;
};
#endif // V8_HEAP_PROFILER_DEFINED

// Stub for HeapGraphEdge - defined before HeapGraphNode due to dependency
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

// Stub for HeapGraphNode
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

// Stub for OutputStream
class OutputStream {
 public:
  enum WriteResult {
    kContinue = 0,
    kAbort = 1,
  };
  
  virtual ~OutputStream() = default;
  virtual void EndOfStream() = 0;
  virtual int GetChunkSize() = 0;
  virtual WriteResult WriteAsciiChunk(char* data, int size) = 0;
  virtual WriteResult WriteHeapStatsChunk(HeapStatsUpdate* data, int count) = 0;
};

// Stub for HeapSnapshot
#ifndef V8_HEAP_SNAPSHOT_DEFINED
#define V8_HEAP_SNAPSHOT_DEFINED
class HeapSnapshot {
 public:
  enum class SerializationFormat {
    kJSON = 0,
  };
  
  virtual ~HeapSnapshot() = default;
  virtual const HeapGraphNode* GetRoot() const = 0;
  virtual const HeapGraphNode* GetNodeById(SnapshotObjectId id) const = 0;
  virtual int GetNodesCount() const = 0;
  virtual const HeapGraphNode* GetNode(int index) const = 0;
  virtual SnapshotObjectId GetMaxSnapshotJSObjectId() const = 0;
  virtual void Delete() = 0;
  virtual void Serialize(OutputStream* stream,
                        SerializationFormat format) const = 0;
};
#endif // V8_HEAP_SNAPSHOT_DEFINED

}  // namespace v8

#endif // V8_PROFILER_H_

#endif  // V8_WASI_EMBEDDER_GRAPH_STUB_H_