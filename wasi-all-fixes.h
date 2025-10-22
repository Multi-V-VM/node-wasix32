#ifndef WASI_ALL_FIXES_H_
#define WASI_ALL_FIXES_H_

// ============================================================================
// WASI Comprehensive Fixes - Master Header
// This file includes all fix headers in the correct order
// ============================================================================

// 1. First include comprehensive namespace and API fixes
#include "wasi-comprehensive-fixes.h"

// 2. Then include WASM32 architecture-specific fixes
#include "wasi-wasm32-arch-fixes.h"

// 3. Include runtime, profiler and inspector fixes
#include "wasi-runtime-profiler-fixes.h"

// 4. Finally include additional fixes for remaining errors
#include "wasi-additional-fixes.h"

// ============================================================================
// Additional Global Fixes
// ============================================================================

// cppgc namespace fixes
namespace cppgc {

// Forward declarations
class Heap;
class Visitor;

// HeapOptions
struct HeapOptions {
  size_t initial_heap_size_bytes = 0;
};

// CollectionType
enum class CollectionType {
  kMajor,
  kMinor
};

// ResourceConstraints
struct ResourceConstraints {
  size_t initial_heap_size_bytes = 0;
  size_t max_heap_size_bytes = 0;
};

// HeapStatistics
class HeapStatistics {
public:
  enum class DetailLevel {
    kBrief,
    kDetailed,
    kDiagnostic
  };

  size_t used_size_bytes = 0;
  size_t committed_size_bytes = 0;
  size_t pooled_memory_size_bytes = 0;
  DetailLevel detail_level = DetailLevel::kBrief;
};

// SpaceStatistics
struct SpaceStatistics {
  const char* name;
  size_t used_size_bytes;
  size_t committed_size_bytes;
};

// CustomSpaceBase
class CustomSpaceBase {
public:
  virtual ~CustomSpaceBase() {}
  virtual bool IsCompactable() const { return false; }
};

namespace internal {

// CollectionType alias
using CollectionType = cppgc::CollectionType;

// RootVisitorBase
class RootVisitorBase {
public:
  virtual ~RootVisitorBase() = default;
};

// VisitorFactory
class VisitorFactory {
public:
  static cppgc::Visitor* CreateVisitor(void* heap, void* config) {
    return nullptr;
  }
};

// GCConfig
struct GCConfig {
  cppgc::CollectionType collection_type = cppgc::CollectionType::kMajor;

  enum class FreeMemoryHandling {
    kDiscardWherePossible,
    kDoNotDiscard
  };
};

// MarkingConfig
struct MarkingConfig {
  enum class IsForcedGC {
    kNotForced,
    kForced
  };
};

// SweepingConfig
struct SweepingConfig {
  enum class FreeMemoryHandling {
    kDiscardWherePossible,
    kDoNotDiscard
  };
};

} // namespace internal

// JSVisitor (base class for JS heap visitors)
class JSVisitor : public Visitor {
public:
  virtual ~JSVisitor() = default;
};

} // namespace cppgc

// ============================================================================
// V8 EmbedderGraph Additions
// ============================================================================

namespace v8 {

class EmbedderGraph {
public:
  class Node {
  public:
    virtual ~Node() = default;
    virtual const char* Name() = 0;
    virtual size_t SizeInBytes() = 0;
  };

  virtual ~EmbedderGraph() = default;
  virtual Node* V8Node(const Local<Value>& value) = 0;
  virtual Node* AddNode(std::unique_ptr<Node> node) = 0;
  virtual void AddEdge(Node* from, Node* to, const char* name = nullptr) = 0;
  virtual void AddNativeSize(void* native_object, size_t size) {}
};

} // namespace v8

// ============================================================================
// Misc V8 Internal Fixes
// ============================================================================

namespace v8 {
namespace internal {

// Heap additions
class Heap {
public:
  void WeakenDescriptorArrays() {}
  void IncrementDeferredCounts(Vector<const uint8_t> indices) {}
};

// SandboxHardwareSupport
enum class SandboxHardwareSupport {
  kNotSupported,
  kSupported
};

// ThreadIsolatedAllocator
class ThreadIsolatedAllocator {
public:
  static void SetDefaultPermissionsForSignalHandler() {}
};

// Platform additions
class Platform {
public:
  ThreadIsolatedAllocator* GetThreadIsolatedAllocator() { return nullptr; }
  void DumpWithoutCrashing() {}
};

// Assembler additions for data directives
class Assembler {
public:
  void db(uint8_t data) {}
  void Align(int alignment) {}
  uint32_t uint32_constant_at(int offset) { return 0; }
  void set_uint32_constant_at(int offset, uint32_t value) {}

  // WriteCodeComments
  void WriteCodeComments() {}
};

// DetachableVector
template<typename T>
using DetachableVector = std::vector<T>;

// CachedZoneVector
template<typename T>
using CachedZoneVector = ZoneVector<T>;

// ToCharacterZoneVector
template<typename Char>
inline ZoneVector<const Char> ToCharacterZoneVector(const void* data, int length, Zone* zone) {
  return ZoneVector<const Char>(zone);
}

// RegisterName function
inline const char* RegisterName(int code) {
  return "unknown";
}

} // namespace internal
} // namespace v8

// ============================================================================
// Disassembler Namespace
// ============================================================================

namespace disasm {

class NameConverter {
public:
  virtual ~NameConverter() {}
};

class Disassembler {
public:
  Disassembler(const NameConverter& converter) {}
};

} // namespace disasm

// ============================================================================
// Standard Library Additions
// ============================================================================

// streamsize for ostream fixes
using std::streamsize;

#endif  // WASI_ALL_FIXES_H_
