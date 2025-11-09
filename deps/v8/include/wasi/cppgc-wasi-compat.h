#ifndef CPPGC_WASI_COMPAT_H_
#define CPPGC_WASI_COMPAT_H_

#ifdef __wasi__

#include <cstddef>
#include <cstdint>
#include <memory>

namespace cppgc {

// Forward declarations
class Platform;
class Heap;

namespace internal {

// GC collection type
enum class CollectionType {
  kMinor,
  kMajor
};

// Stack state for GC
enum class StackState {
  kMayContainHeapPointers,
  kNoHeapPointers
};

// Marking type
enum class MarkingType {
  kIncremental,
  kIncrementalAndConcurrent,
  kAtomic
};

// Sweeping type
enum class SweepingType {
  kIncremental,
  kIncrementalAndConcurrent,
  kAtomic
};

// GC Configuration
struct GCConfig {
  enum class FreeMemoryHandling {
    kDoNotDiscard,
    kDiscardWherePossible
  };

  CollectionType collection_type = CollectionType::kMajor;
  StackState stack_state = StackState::kMayContainHeapPointers;
  MarkingType marking_type = MarkingType::kAtomic;
  SweepingType sweeping_type = SweepingType::kAtomic;
  FreeMemoryHandling free_memory_handling = FreeMemoryHandling::kDoNotDiscard;
  bool is_forced_gc = false;
};

// Embedder stack state
struct EmbedderStackState {
  enum class State {
    kMayContainHeapPointers,
    kNoHeapPointers,
    kUnknown
  };

  State state = State::kMayContainHeapPointers;
};

// Write barrier parameters
struct WriteBarrierParams {
  uintptr_t start = 0;
  uintptr_t end = 0;
};

// Heap statistics
struct HeapStatistics {
  size_t used_size_bytes = 0;
  size_t allocated_size_bytes = 0;
  size_t pooled_memory_size_bytes = 0;
  size_t resident_size_bytes = 0;
};

// Heap options
struct HeapOptions {
  enum class MarkingSupport {
    kDisabled,
    kEnabled,
    kIncrementalAndConcurrent
  };

  enum class SweepingSupport {
    kDisabled,
    kEnabled,
    kIncrementalAndConcurrent
  };

  std::shared_ptr<Platform> platform;
  size_t custom_spaces_count = 0;
  MarkingSupport marking_support = MarkingSupport::kEnabled;
  SweepingSupport sweeping_support = SweepingSupport::kEnabled;
  size_t resource_constraints_max_heap_size_bytes = 0;
};

// Allocation handle
class AllocationHandle {
 public:
  AllocationHandle() = default;
  ~AllocationHandle() = default;

  AllocationHandle(const AllocationHandle&) = delete;
  AllocationHandle& operator=(const AllocationHandle&) = delete;
};

// Visitor for tracing heap objects
class Visitor {
 public:
  virtual ~Visitor() = default;

  // Trace a member
  template <typename T>
  void Trace(const T& member) {
    // Minimal stub - actual implementation would dispatch to TraceImpl
  }

 protected:
  virtual void TraceImpl(const void* object) = 0;
};

// Root visitor for tracing roots
class RootVisitor {
 public:
  virtual ~RootVisitor() = default;

  template <typename T>
  void Trace(const T& root) {
    // Minimal stub
  }

 protected:
  virtual void VisitRoot(const void* root, size_t size) = 0;
};

// Persistent location for persistent handles
struct PersistentNode {
  void* object = nullptr;
  PersistentNode* next = nullptr;
  PersistentNode* prev = nullptr;
};

// Cross-thread persistent location
struct CrossThreadPersistentNode : public PersistentNode {
  // Extends PersistentNode for cross-thread access
};

}  // namespace internal

// GarbageCollected - Base class for garbage-collected objects
template <typename T>
class GarbageCollected {
 public:
  using IsGarbageCollectedType = void;

  // Objects allocated on the cppgc heap must use cppgc allocators
  void* operator new(size_t size) = delete;
  void* operator new[](size_t size) = delete;

  void operator delete(void* ptr) = delete;
  void operator delete[](void* ptr) = delete;

 protected:
  GarbageCollected() = default;
};

// Heap API
class Heap {
 public:
  static std::unique_ptr<Heap> Create(const internal::HeapOptions& options);

  virtual ~Heap() = default;

  // Force garbage collection
  virtual void ForceGarbageCollectionSlow(
      const char* source,
      const char* reason,
      internal::GCConfig config = internal::GCConfig()) = 0;

  // Get heap statistics
  virtual internal::HeapStatistics GetHeapStatistics() const = 0;

  // Get allocation handle
  virtual internal::AllocationHandle& GetAllocationHandle() = 0;

 protected:
  Heap() = default;
};

// Member base - base class for traced members
template <typename T>
class MemberBase {
 protected:
  T* ptr_ = nullptr;

 public:
  MemberBase() = default;
  explicit MemberBase(T* ptr) : ptr_(ptr) {}

  T* Get() const { return ptr_; }
  T* operator->() const { return ptr_; }
  T& operator*() const { return *ptr_; }

  explicit operator bool() const { return ptr_ != nullptr; }
};

// Member - traced member for garbage-collected objects
template <typename T>
class Member : public MemberBase<T> {
 public:
  using MemberBase<T>::MemberBase;
};

// WeakMember - weak reference to garbage-collected objects
template <typename T>
class WeakMember : public MemberBase<T> {
 public:
  using MemberBase<T>::MemberBase;
};

// Persistent - off-heap reference to garbage-collected objects
template <typename T>
class Persistent {
 private:
  T* ptr_ = nullptr;
  internal::PersistentNode* node_ = nullptr;

 public:
  Persistent() = default;
  explicit Persistent(T* ptr) : ptr_(ptr) {}
  ~Persistent() { Clear(); }

  Persistent(const Persistent&) = delete;
  Persistent& operator=(const Persistent&) = delete;

  Persistent(Persistent&& other) : ptr_(other.ptr_), node_(other.node_) {
    other.ptr_ = nullptr;
    other.node_ = nullptr;
  }

  Persistent& operator=(Persistent&& other) {
    if (this != &other) {
      Clear();
      ptr_ = other.ptr_;
      node_ = other.node_;
      other.ptr_ = nullptr;
      other.node_ = nullptr;
    }
    return *this;
  }

  T* Get() const { return ptr_; }
  T* operator->() const { return ptr_; }
  T& operator*() const { return *ptr_; }

  void Clear() {
    ptr_ = nullptr;
    node_ = nullptr;
  }

  explicit operator bool() const { return ptr_ != nullptr; }
};

// CrossThreadPersistent - thread-safe persistent handle
template <typename T>
class CrossThreadPersistent {
 private:
  T* ptr_ = nullptr;
  internal::CrossThreadPersistentNode* node_ = nullptr;

 public:
  CrossThreadPersistent() = default;
  explicit CrossThreadPersistent(T* ptr) : ptr_(ptr) {}
  ~CrossThreadPersistent() { Clear(); }

  CrossThreadPersistent(const CrossThreadPersistent&) = delete;
  CrossThreadPersistent& operator=(const CrossThreadPersistent&) = delete;

  CrossThreadPersistent(CrossThreadPersistent&& other)
      : ptr_(other.ptr_), node_(other.node_) {
    other.ptr_ = nullptr;
    other.node_ = nullptr;
  }

  CrossThreadPersistent& operator=(CrossThreadPersistent&& other) {
    if (this != &other) {
      Clear();
      ptr_ = other.ptr_;
      node_ = other.node_;
      other.ptr_ = nullptr;
      other.node_ = nullptr;
    }
    return *this;
  }

  T* Get() const { return ptr_; }
  T* operator->() const { return ptr_; }
  T& operator*() const { return *ptr_; }

  void Clear() {
    ptr_ = nullptr;
    node_ = nullptr;
  }

  explicit operator bool() const { return ptr_ != nullptr; }
};

// Subtle namespace for low-level operations
namespace subtle {

// Write barrier for incremental/concurrent marking
inline void WriteBarrier(const void* slot) {
  // Minimal stub - actual implementation would notify GC
}

// Generational write barrier
inline void GenerationalBarrier(const void* slot) {
  // Minimal stub
}

}  // namespace subtle

}  // namespace cppgc

#endif  // __wasi__

#endif  // CPPGC_WASI_COMPAT_H_
