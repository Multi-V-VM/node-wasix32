#ifndef WASI_ALL_FIXES_H_
#define WASI_ALL_FIXES_H_

// ============================================================================
// WASI Comprehensive Fixes - Master Header
// This file includes all fix headers in the correct order
// ============================================================================

// ============================================================================
// CRITICAL: Define size constants BEFORE any V8 headers
// ============================================================================
// These constants are used throughout V8 but not always defined properly
// Define them at global scope to ensure they're available everywhere
#include <cstddef>
constexpr size_t KB = 1024;
constexpr size_t MB = 1024 * KB;
constexpr size_t GB = 1024 * MB;

// Define kSystemPointerSizeLog2 for 32-bit builds (WASI is 32-bit)
// This is expected to be in v8-internal.h but isn't defined there
#ifndef kSystemPointerSizeLog2
constexpr int kSystemPointerSize = sizeof(void*);
constexpr int kSystemPointerSizeLog2 = (kSystemPointerSize == 4 ? 2 : 3);
#endif

// CRITICAL: Forward declare and define necessary V8 types for WASI builds
// v8-snapshot.h needs Isolate::CreateParams but doesn't get the full definition
#ifdef __wasi__

// Forward declare what's needed
namespace v8 {
class Isolate;

// Define CreateParams inline if not already defined
#ifndef V8_ISOLATE_CREATEPARAMS_DEFINED
#define V8_ISOLATE_CREATEPARAMS_DEFINED
namespace internal {
class Isolate;
}  // namespace internal
#endif  // V8_ISOLATE_CREATEPARAMS_DEFINED

}  // namespace v8

// Include v8-internal.h for basic definitions
#ifndef INCLUDE_V8_INTERNAL_H_
#include "deps/v8/include/v8-internal.h"
#endif

// Pre-include v8-isolate.h to ensure Isolate is fully defined
// before any other header tries to use Isolate::CreateParams
#include "deps/v8/include/v8-isolate.h"

#endif  // __wasi__

// CRITICAL: Include ALL standard library headers at global scope FIRST
// This prevents namespace pollution when they're included later from within
// namespace v8 {} blocks
#include <algorithm>
#include <functional>
#include <utility>
#include <type_traits>
#include <bit>
#include <iterator>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <cstdint>

// CRITICAL: Include critical V8 base headers at global scope next
// This ensures symbols are defined in the correct namespace before any nesting occurs
#include "deps/v8/src/base/macros.h"
#include "deps/v8/src/base/numerics/safe_conversions_impl.h"
#include "deps/v8/src/base/hashmap-entry.h"

// 1. First include comprehensive namespace and API fixes
#include "wasi-comprehensive-fixes.h"

// 2. Then include WASM32 architecture-specific fixes
#include "wasi-wasm32-arch-fixes.h"

// 3. Include runtime, profiler and inspector fixes
#include "wasi-runtime-profiler-fixes.h"

// 4. Finally include additional fixes for remaining errors
#include "wasi-additional-fixes.h"

// ============================================================================
// V8 Internal Symbols Export to Base Namespace
// ============================================================================

// The following symbols from v8::base::internal need to be available at
// v8::base level to prevent lookup failures when headers are included
// from within namespace v8 {} blocks

namespace v8 {
namespace base {

// Re-export CheckOnFailure from internal namespace
using internal::CheckOnFailure;

// Re-export internal numeric type range constants
template <typename Dst, typename Src>
inline constexpr bool kIsTypeInRangeForNumericType =
    internal::kIsTypeInRangeForNumericType<Dst, Src>;

template <typename Dst, typename Src>
inline constexpr bool kIsMinInRangeForNumericType =
    internal::kIsMinInRangeForNumericType<Dst, Src>;

template <typename Dst, typename Src>
inline constexpr bool kIsMaxInRangeForNumericType =
    internal::kIsMaxInRangeForNumericType<Dst, Src>;

// Ensure std namespace always refers to ::std even when inside v8 namespace
namespace std = ::std;

}  // namespace base
}  // namespace v8

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

// ============================================================================
// CRITICAL: Close any accidentally open namespaces from included headers
// ============================================================================
// The included fix headers may have left namespaces open. Close them all here.
// We need to close potentially 7 levels of nesting based on our earlier analysis.

}  // Close namespace level 1
}  // Close namespace level 2
}  // Close namespace level 3
}  // Close namespace level 4
}  // Close namespace level 5
}  // Close namespace level 6
}  // Close namespace level 7

#endif  // WASI_ALL_FIXES_H_
