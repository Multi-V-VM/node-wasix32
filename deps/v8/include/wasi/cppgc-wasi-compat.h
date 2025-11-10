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

// GC collection type (in main cppgc namespace)
enum class CollectionType {
  kMinor,
  kMajor
};

// Stack state for GC (in main cppgc namespace)
enum class StackState {
  kMayContainHeapPointers,
  kNoHeapPointers
};

// Marking type (in main cppgc namespace)
enum class MarkingType {
  kIncremental,
  kIncrementalAndConcurrent,
  kAtomic
};

// Sweeping type (in main cppgc namespace)
enum class SweepingType {
  kIncremental,
  kIncrementalAndConcurrent,
  kAtomic
};

namespace internal {

// Also expose types in internal namespace for compatibility
using CollectionType = cppgc::CollectionType;
using StackState = cppgc::StackState;
using MarkingType = cppgc::MarkingType;
using SweepingType = cppgc::SweepingType;

// MarkingConfig for internal use
struct MarkingConfig {
  using MarkingType = cppgc::MarkingType;
  using StackState = cppgc::StackState;

  enum class IsForcedGC : uint8_t {
    kNotForced,
    kForced,
  };

  static constexpr MarkingConfig Default() {
    return MarkingConfig(CollectionType::kMajor, StackState::kMayContainHeapPointers,
                        MarkingType::kIncrementalAndConcurrent, IsForcedGC::kNotForced);
  }

  // Explicit constructor to prevent aggregate initialization
  constexpr MarkingConfig(CollectionType ct = CollectionType::kMajor,
                          StackState ss = StackState::kMayContainHeapPointers,
                          MarkingType mt = MarkingType::kIncrementalAndConcurrent,
                          IsForcedGC fg = IsForcedGC::kNotForced)
      : collection_type(ct), stack_state(ss), marking_type(mt), is_forced_gc(fg) {}

  CollectionType collection_type;
  StackState stack_state;
  MarkingType marking_type;
  IsForcedGC is_forced_gc;
};

// SweepingConfig for internal use
struct SweepingConfig {
  using SweepingType = cppgc::SweepingType;

  enum class CompactableSpaceHandling { kSweep, kIgnore };
  enum class FreeMemoryHandling { kDoNotDiscard, kDiscardWherePossible };

  // Explicit constructor to prevent aggregate initialization
  constexpr SweepingConfig(SweepingType st = SweepingType::kIncrementalAndConcurrent,
                           CompactableSpaceHandling csh = CompactableSpaceHandling::kSweep,
                           FreeMemoryHandling fmh = FreeMemoryHandling::kDoNotDiscard)
      : sweeping_type(st), compactable_space_handling(csh), free_memory_handling(fmh) {}

  SweepingType sweeping_type;
  CompactableSpaceHandling compactable_space_handling;
  FreeMemoryHandling free_memory_handling;
};

// GC Configuration - flat structure matching V8's usage patterns
struct GCConfig {
  using MarkingType = MarkingConfig::MarkingType;
  using SweepingType = SweepingConfig::SweepingType;
  using StackState = MarkingConfig::StackState;
  using IsForcedGC = MarkingConfig::IsForcedGC;
  using FreeMemoryHandling = SweepingConfig::FreeMemoryHandling;

  static constexpr GCConfig Default() {
    return GCConfig();
  }

  // Constructor for flat initialization pattern used in V8 code
  // {CollectionType, StackState, MarkingType, SweepingType, FreeMemoryHandling, IsForcedGC}
  constexpr GCConfig(CollectionType ct = CollectionType::kMajor,
                     StackState ss = StackState::kMayContainHeapPointers,
                     MarkingType mt = MarkingType::kIncrementalAndConcurrent,
                     SweepingType st = SweepingType::kIncrementalAndConcurrent,
                     FreeMemoryHandling fmh = FreeMemoryHandling::kDoNotDiscard,
                     IsForcedGC fg = IsForcedGC::kNotForced)
      : collection_type(ct),
        stack_state(ss),
        marking_type(mt),
        sweeping_type(st),
        free_memory_handling(fmh),
        is_forced_gc(fg),
        marking_config{ct, ss, mt, fg},
        sweeping_config{st, SweepingConfig::CompactableSpaceHandling::kSweep, fmh} {}

  // Constructor with nested configs
  constexpr GCConfig(StackState ss, MarkingConfig mc, SweepingConfig sc)
      : collection_type(mc.collection_type),
        stack_state(ss),
        marking_type(mc.marking_type),
        sweeping_type(sc.sweeping_type),
        free_memory_handling(sc.free_memory_handling),
        is_forced_gc(mc.is_forced_gc),
        marking_config(mc),
        sweeping_config(sc) {}

  // Public fields for direct access (matching V8's usage)
  CollectionType collection_type = CollectionType::kMajor;
  StackState stack_state = StackState::kMayContainHeapPointers;
  MarkingType marking_type = MarkingType::kIncrementalAndConcurrent;
  SweepingType sweeping_type = SweepingType::kIncrementalAndConcurrent;
  FreeMemoryHandling free_memory_handling = FreeMemoryHandling::kDoNotDiscard;
  IsForcedGC is_forced_gc = IsForcedGC::kNotForced;

  // Nested configs for APIs that expect them
  MarkingConfig marking_config;
  SweepingConfig sweeping_config;
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
  enum class DetailLevel {
    kBrief,
    kDetailed,
    kDiagnostic
  };

  size_t used_size_bytes = 0;
  size_t allocated_size_bytes = 0;
  size_t pooled_memory_size_bytes = 0;
  size_t resident_size_bytes = 0;
  DetailLevel detail_level = DetailLevel::kBrief;

  // Free list statistics (for detailed level)
  struct FreeListStatistics {
    size_t bucket_size = 0;
    size_t free_count = 0;
    size_t free_size = 0;
  };

  // Make detail_level and pooled_memory_size_bytes accessible
  void set_detail_level(DetailLevel level) { detail_level = level; }
  DetailLevel get_detail_level() const { return detail_level; }
};

// Forward declaration for custom spaces
class CustomSpaceBase;

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
  std::vector<std::unique_ptr<CustomSpaceBase>> custom_spaces;
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

// Forward declarations for visitor types
class Visitor;

// TraceDescriptor for visitor pattern
struct TraceDescriptor {
  const void* base_object_payload = nullptr;
  void (*trace_callback)(Visitor*, const void*) = nullptr;
};

using TraceDescriptorCallback = void (*)(const void*, TraceDescriptor);
using WeakCallback = void (*)(const void*);
using TraceConservativelyCallback = void (*)(Visitor*, const void*);

// Forward declarations for heap types
class HeapObjectHeader;

// Visitor for tracing heap objects
class Visitor {
 public:
  virtual ~Visitor() = default;

  // Trace a member
  template <typename T>
  void Trace(const T& member) {
    // Minimal stub - actual implementation would dispatch to TraceImpl
  }

  // Virtual methods that can be overridden
  virtual void Visit(const void*, TraceDescriptor) {}
  virtual void VisitMultipleUncompressedMember(const void*, size_t,
                                                TraceDescriptorCallback) {}
  virtual void VisitMultipleCompressedMember(const void*, size_t,
                                              TraceDescriptorCallback) {}
  virtual void VisitWeak(const void*, TraceDescriptor, WeakCallback, const void*) {}
  virtual void VisitEphemeron(const void*, const void*, TraceDescriptor) {}
  virtual void VisitWeakContainer(const void*, TraceDescriptor, TraceDescriptor,
                                  WeakCallback, const void*) {}
  virtual void RegisterWeakCallback(WeakCallback, const void*) {}
  virtual void HandleMovableReference(const void**) {}
  virtual void VisitInConstructionConservatively(HeapObjectHeader&,
                                                  TraceConservativelyCallback) {}
  virtual void VisitPointer(const void*) {}

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

// Custom space base class for custom allocation spaces
class CustomSpaceBase {
 public:
  virtual ~CustomSpaceBase() = default;

  // Custom space configuration
  virtual size_t GetCustomSpaceIndex() const = 0;
  virtual bool IsCompactable() const { return false; }

 protected:
  CustomSpaceBase() = default;
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
