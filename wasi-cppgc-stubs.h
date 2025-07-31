#ifndef WASI_CPPGC_STUBS_H_
#define WASI_CPPGC_STUBS_H_

#ifdef __wasi__

namespace cppgc {

// Forward declarations
class AllocationHandle;

// Check if CustomSpace is already defined
#ifndef CPPGC_CUSTOM_SPACE_H_
namespace internal {
// CustomSpace type for memory allocation
using CustomSpace = size_t;
constexpr CustomSpace kNone = 0;
} // namespace internal
#endif

// SpaceTrait template for WASI
template <typename T>
struct SpaceTrait {
  using Space = size_t;
  static constexpr Space value = 0;
};

// TraceTrait template for WASI - only if not already defined
#ifndef CPPGC_TRACE_TRAIT_H_
template <typename T>
struct TraceTrait {
  using Type = T;
  static constexpr bool kIsGarbageCollectedMixinType = false;
  
  // The Space typedef that was missing
  using Space = internal::CustomSpace;
  
  static void Trace(Visitor* visitor, const void* object) {
    // WASI stub - no-op
  }
};
#endif // CPPGC_TRACE_TRAIT_H_

} // namespace cppgc

#endif // __wasi__

#endif // WASI_CPPGC_STUBS_H_