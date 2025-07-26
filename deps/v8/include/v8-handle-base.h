#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_HANDLE_BASE_H_
#define INCLUDE_V8_HANDLE_BASE_H_

#ifdef __wasi__
// WASI stub for v8-handle-base
#include "wasi/v8-wasi-compat.h"

namespace v8 {

// Tag type for constructors that skip checks
namespace internal {
struct no_checking_tag {};
}

template<bool B> class StackAllocated {
public:
  StackAllocated() = default;
  // Constructor that accepts no_checking_tag for compatibility
  explicit StackAllocated(internal::no_checking_tag) {}
};

// Simplified handle base for WASI
class HandleBase {
public:
  HandleBase() = default;
};

namespace api_internal {
// Simplified IndirectHandleBase for WASI
class IndirectHandleBase {
 protected:
  IndirectHandleBase() : slot_(nullptr) {}
  explicit IndirectHandleBase(v8::internal::Address* location) : slot_(location) {}
  
  v8::internal::Address*& slot() { return slot_; }
  v8::internal::Address* slot() const { return slot_; }
  v8::internal::Address* const* slot_address() const { return &slot_; }
  
 private:
  v8::internal::Address* slot_;
};
} // namespace api_internal

namespace internal {
// Stub classes needed by v8-persistent-handle.h
class ValueHelper {
 public:
  using InternalRepresentationType = v8::internal::Address;
  static constexpr InternalRepresentationType kEmpty = 0;
  
  static void* Wrap(void* value) { return value; }
  
  template<typename T>
  static bool IsEmpty(T* value) { 
    return value == nullptr; 
  }
  
  template<typename T>
  static Address ValueAsAddress(const T* value) {
    return reinterpret_cast<Address>(const_cast<T*>(value));
  }
  
  // Overload for references
  template<typename T>
  static Address ValueAsAddress(const T& value) {
    return reinterpret_cast<Address>(const_cast<T*>(&value));
  }
  
  template<typename T>
  static T* ReprAsValue(InternalRepresentationType repr) {
    return reinterpret_cast<T*>(repr);
  }
  
  template<typename T>
  static T* SlotAsValue(Address* slot) {
    return reinterpret_cast<T*>(*slot);
  }
};

class HandleHelper {
 public:
  template<typename T1, typename T2>
  static bool EqualHandles(const T1& a, const T2& b) {
    return false; // Simplified implementation
  }
};
} // namespace internal

} // namespace v8

#else
// Include real v8-handle-base.h for non-WASI
#endif

#endif // INCLUDE_V8_HANDLE_BASE_H_
