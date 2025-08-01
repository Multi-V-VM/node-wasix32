#ifndef WASI_CPPGC_STUBS_H_
#define WASI_CPPGC_STUBS_H_

#ifdef __wasi__

namespace cppgc {

// Forward declarations
class AllocationHandle;

// CustomSpaceBase is already defined in v8-cppgc.h
// We only need to extend it for WASI if necessary

// Define a template for custom spaces that matches the v8 internal usage
// In cppgc/custom-space.h, internal::CustomSpace is defined as size_t
// So we need to provide a compatible definition for template usage
template <typename T>
struct CustomSpaceForType {
  static constexpr size_t kSpaceIndex = 0;
  static constexpr bool kSupportsCompaction = false;
};

// Default CustomSpace using size_t as per v8's internal definition
using DefaultCustomSpace = size_t;

namespace internal {
// Define kNone constant
constexpr size_t kNone = 0;
} // namespace internal

// SpaceTrait template for WASI
template <typename T>
struct SpaceTrait {
  // Default to no custom space - this will use the default allocation
  using Space = void;
};

// Note: TraceTrait is already defined in cppgc/trace-trait.h
// No need to redefine it here for WASI builds

} // namespace cppgc

#endif // __wasi__

#endif // WASI_CPPGC_STUBS_H_