#ifndef INCLUDE_V8_HANDLE_BASE_H_
#define INCLUDE_V8_HANDLE_BASE_H_

// Minimal stubs for handle base used by both WASI and host tool builds
// when upstream header is not available in this tree.

#include <cstdint>

namespace v8 {
namespace api_internal {
using Address = uintptr_t;

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
class HandleHelper {
 public:
  template <typename T1, typename T2>
  static bool EqualHandles(const T1& /*a*/, const T2& /*b*/) {
    return false;  // Minimal stub for host/WASI builds
  }
};
}  // namespace internal
}  // namespace v8

#endif  // INCLUDE_V8_HANDLE_BASE_H_
