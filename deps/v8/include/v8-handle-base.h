#ifndef INCLUDE_V8_HANDLE_BASE_H_
#define INCLUDE_V8_HANDLE_BASE_H_

#ifdef __wasi__
// WASI stub for v8-handle-base
#ifndef __wasi__
#include "v8-internal.h"  // Ensure internal::ValueHelper is available
#endif
#include "wasi/v8-wasi-compat.h"

namespace v8 {

// Tag type for constructors that skip checks
namespace internal {
struct no_checking_tag {};
}  // namespace internal

template <bool B>
class StackAllocated {
 public:
  StackAllocated() = default;
  // Constructor that accepts no_checking_tag for compatibility
  explicit StackAllocated(internal::no_checking_tag) {}

  // Provide a tag instance accessible from derived classes to bypass checks,
  // mirroring upstream V8's pattern (used as `Derived::do_not_check`).
  static constexpr internal::no_checking_tag do_not_check{};
};

// Simplified handle base for WASI
class HandleBase {
 public:
  HandleBase() = default;
};

namespace api_internal {
// Forward declare Address type for api_internal
using Address = uintptr_t;

// Simplified IndirectHandleBase for WASI
class IndirectHandleBase {
 protected:
  IndirectHandleBase() : slot_(nullptr) {}
  explicit IndirectHandleBase(Address* location) : slot_(location) {}

  Address*& slot() { return slot_; }
  Address* slot() const { return slot_; }
  Address* const* slot_address() const { return &slot_; }

 private:
  Address* slot_;
};
}  // namespace api_internal

namespace internal {
// Define Address type for WASI if not already defined
#ifndef V8_INTERNAL_ADDRESS_DEFINED
#define V8_INTERNAL_ADDRESS_DEFINED
using Address = uintptr_t;
#endif

// ValueHelper is defined centrally in v8-internal.h for WASI builds.
// Do not forward-declare to avoid incomplete-type issues in nested name use.

class HandleHelper {
 public:
  template <typename T1, typename T2>
  static bool EqualHandles(const T1& a, const T2& b) {
    return false;  // Simplified implementation
  }
};
}  // namespace internal

}  // namespace v8

#else
// Include real v8-handle-base.h for non-WASI
#endif

#endif  // INCLUDE_V8_HANDLE_BASE_H_
