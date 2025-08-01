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
} // namespace api_internal

namespace internal {
// Define Address type for WASI if not already defined
#ifndef V8_INTERNAL_ADDRESS_DEFINED
#define V8_INTERNAL_ADDRESS_DEFINED
using Address = uintptr_t;
#endif

// Stub classes needed by v8-persistent-handle.h
class ValueHelper {
 public:
  using InternalRepresentationType = Address;
  static constexpr InternalRepresentationType kEmpty = 0;
  
  static void* Wrap(void* value) { return value; }
  
  template<typename T>
  static bool IsEmpty(T* value) { 
    return value == nullptr; 
  }
  
  // Overload for pointer types (when called with *Local<T>)
  template<typename T>
  static InternalRepresentationType ValueAsAddress(T* value) {
    return reinterpret_cast<InternalRepresentationType>(value);
  }
  
  // Overload for const pointer types
  template<typename T>
  static InternalRepresentationType ValueAsAddress(const T* value) {
    return reinterpret_cast<InternalRepresentationType>(const_cast<T*>(value));
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
