#ifndef WASI_ALL_FIXES_H_
#define WASI_ALL_FIXES_H_

// ============================================================================
// WASI Comprehensive Fixes - Master Header
// This file includes all fix headers in the correct order
// ============================================================================

// ============================================================================
// CRITICAL: Define size constants BEFORE any V8 headers
// ============================================================================
// These constants are defined in v8-constants.h, so we don't need to define them here
// Just include cstddef for size_t
#include <cstddef>

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

// Skip v8-missing-types.h as it has C++20 dependencies
// The V8 WASI stubs provide the necessary type aliases
// #include "deps/v8/include/wasi/wasi-v8-missing-types.h"

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
// Skip safe_conversions_impl.h as it has complex include dependencies
// #include "deps/v8/src/base/numerics/safe_conversions_impl.h"
// Skip hashmap-entry.h as it has complex include dependencies (src/base/memory.h)
// #include "deps/v8/src/base/hashmap-entry.h"

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
// Skip if not available
// using internal::CheckOnFailure;

// Re-export internal numeric type range constants
// These don't exist in the internal namespace, skip them
// template <typename Dst, typename Src>
// inline constexpr bool kIsTypeInRangeForNumericType =
//     internal::kIsTypeInRangeForNumericType<Dst, Src>;
//
// template <typename Dst, typename Src>
// inline constexpr bool kIsMinInRangeForNumericType =
//     internal::kIsMinInRangeForNumericType<Dst, Src>;
//
// template <typename Dst, typename Src>
// inline constexpr bool kIsMaxInRangeForNumericType =
//     internal::kIsMaxInRangeForNumericType<Dst, Src>;

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

// These classes are now properly included from V8 cppgc headers
// Skip duplicate definitions to avoid redefinition errors
// class Visitor {
// public:
//   virtual ~Visitor() = default;
// };
//
// struct HeapOptions {
//   size_t initial_heap_size_bytes = 0;
// };
//
// enum class CollectionType {
//   kMajor,
//   kMinor
// };
//
// struct ResourceConstraints {
//   size_t initial_heap_size_bytes = 0;
//   size_t max_heap_size_bytes = 0;
// };
//
// class HeapStatistics {
// public:
//   enum class DetailLevel {
//     kBrief,
//     kDetailed,
//     kDiagnostic
//   };
//
//   size_t used_size_bytes = 0;
//   size_t committed_size_bytes = 0;
//   size_t pooled_memory_size_bytes = 0;
//   DetailLevel detail_level = DetailLevel::kBrief;
// };
//
// struct SpaceStatistics {
//   const char* name;
//   size_t used_size_bytes;
//   size_t committed_size_bytes;
// };
//
// class CustomSpaceBase {
// public:
//   virtual ~CustomSpaceBase() {}
//   virtual bool IsCompactable() const { return false; }
// };

namespace internal {

// These types are now properly included from V8 cppgc headers
// Skip definitions that reference the commented-out cppgc types
// using CollectionType = cppgc::CollectionType;
//
// class RootVisitorBase {
// public:
//   virtual ~RootVisitorBase() = default;
// };
//
// class VisitorFactory {
// public:
//   static cppgc::Visitor* CreateVisitor(void* heap, void* config) {
//     return nullptr;
//   }
// };
//
// struct GCConfig {
//   cppgc::CollectionType collection_type = cppgc::CollectionType::kMajor;
//
//   enum class FreeMemoryHandling {
//     kDiscardWherePossible,
//     kDoNotDiscard
//   };
// };
//
// struct MarkingConfig {
//   enum class IsForcedGC {
//     kNotForced,
//     kForced
//   };
// };
//
// struct SweepingConfig {
//   enum class FreeMemoryHandling {
//     kDiscardWherePossible,
//     kDoNotDiscard
//   };
// };

} // namespace internal

// JSVisitor is now properly included from V8 headers
// Skip duplicate definition
// class JSVisitor : public Visitor {
// public:
//   virtual ~JSVisitor() = default;
// };

} // namespace cppgc

// ============================================================================
// V8 EmbedderGraph Additions
// ============================================================================

// EmbedderGraph is already defined in embedder-graph-stub.h
// Skip redefinition here to avoid conflicts
// namespace v8 {
//
// class EmbedderGraph {
// ...
// };
//
// } // namespace v8

// ============================================================================
// Misc V8 Internal Fixes
// ============================================================================

namespace v8 {
namespace internal {

// Heap additions
class Heap {
public:
  void WeakenDescriptorArrays() {}
  void IncrementDeferredCounts(const std::vector<uint8_t>& indices) {}
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
// Assembler is already defined in architecture-specific code
// Skip redefinition
// class Assembler {
// public:
//   void db(uint8_t data) {}
//   void Align(int alignment) {}
//   uint32_t uint32_constant_at(int offset) { return 0; }
//   void set_uint32_constant_at(int offset, uint32_t value) {}
//
//   // WriteCodeComments
//   void WriteCodeComments() {}
// };

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
// Namespaces are properly closed by included headers

#endif  // WASI_ALL_FIXES_H_
