#ifndef INCLUDE_V8_HANDLE_BASE_H_
#define INCLUDE_V8_HANDLE_BASE_H_

// Minimal stand-in for V8's handle infrastructure headers. This keeps host and
// WASI cross builds compiling without pulling in the full upstream dependency.

#include <cstdint>

namespace v8 {

template <typename T>
class Local;
template <typename T>
class PersistentBase;

namespace internal {

// Tag type used to skip expensive validation paths when constructing Handles.
struct no_checking_tag {};

}  // namespace internal

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

// Minimal stack-allocation enforcement shim used by various handle types.
template <bool allow_heap_allocation>
class StackAllocated {
 public:
  StackAllocated() = default;
  explicit StackAllocated(::v8::internal::no_checking_tag) {}
  void VerifyOnStack() const {}
};

}  // namespace api_internal

namespace internal {

using api_internal::StackAllocated;

class HandleHelper {
 public:
#ifdef __wasi__
  template <typename T>
  static T* HandleValue(const Local<T>& handle) {
    return *handle;
  }

  template <typename T>
  static T* HandleValue(const PersistentBase<T>& handle) {
    return &*handle;
  }

  template <typename T1, typename T2>
  static bool EqualHandles(const T1& lhs, const T2& rhs) {
    if (lhs.IsEmpty() || rhs.IsEmpty()) {
      return lhs.IsEmpty() == rhs.IsEmpty();
    }
    return HandleValue(lhs) == HandleValue(rhs);
  }
#else
  template <typename T1, typename T2>
  static bool EqualHandles(const T1&, const T2&) {
    return false;
  }
#endif
};

}  // namespace internal

}  // namespace v8

#endif  // INCLUDE_V8_HANDLE_BASE_H_
