#ifndef V8_V8_PROFILER_H_
#define V8_V8_PROFILER_H_

#ifdef __wasi__
// WASI stubs for profiler

namespace v8 {

class EmbedderGraph {
 public:
  class Node {
   public:
    enum class Detachedness { kUnknown, kAttached, kDetached };
    
    virtual ~Node() = default;
    virtual const char* Name() = 0;
    virtual const char* NamePrefix() { return nullptr; }
    virtual size_t SizeInBytes() = 0;
    virtual Node* WrapperNode() { return nullptr; }
    virtual bool IsRootNode() { return false; }
    virtual bool IsEmbedderNode() { return true; }
    virtual Detachedness GetDetachedness() { return Detachedness::kUnknown; }
  };
  
  class Edge {
   public:
    virtual ~Edge() = default;
    virtual Node* GetFrom() = 0;
    virtual Node* GetTo() = 0;
    virtual const char* GetName() = 0;
  };
  
  virtual ~EmbedderGraph() = default;
  virtual Node* V8Node(const v8::Local<v8::Object>& value) = 0;
  virtual Node* AddNode(std::unique_ptr<Node> node) = 0;
  virtual void AddEdge(Node* from, Node* to, const char* name = nullptr) = 0;
};

class HeapSnapshot {
 public:
  ~HeapSnapshot() {}
};

class HeapProfiler {
 public:
  struct HeapSnapshotOptions {
    bool capture_numeric_value = false;
    bool is_brief = false;
  };
  
  void DeleteAllHeapSnapshots() {}
  const HeapSnapshot* TakeHeapSnapshot(
      const HeapSnapshotOptions& options = HeapSnapshotOptions()) {
    return nullptr;
  }
};

} // namespace v8

#endif // __wasi__

#endif // V8_V8_PROFILER_H_