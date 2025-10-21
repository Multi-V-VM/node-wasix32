#ifndef WASI_V8_REGLIST_FIX_H_
#define WASI_V8_REGLIST_FIX_H_

// Fixes for register list types in WASI build
// This file provides RegListBase implementation for WASM32 when building with IA32 target

#include <cstdint>
#include <initializer_list>

#include "../../../../../wasi-v8-bits-fixes.h"

namespace v8 {
namespace internal {

// Forward declarations
class Register;
class DoubleRegister;

// RegListBase template implementation for WASM32
// This is used when V8_TARGET_ARCH_IA32 is defined but we're actually building for WASM
template <typename RegisterT>
class RegListBase {
 public:
  using storage_t = uint32_t;

  // Default constructor: empty set
  constexpr RegListBase() : bits_(0) {}

  // Constructor from initializer list
  constexpr RegListBase(std::initializer_list<RegisterT> regs) : bits_(0) {
    for (RegisterT reg : regs) {
      bits_ |= (1u << reg.code());
    }
  }

  // Constructor from raw bits
  constexpr explicit RegListBase(storage_t bits) : bits_(bits) {}

  // Set a register bit
  constexpr void set(RegisterT reg) {
    bits_ |= (1u << reg.code());
  }

  // Clear a register bit
  constexpr void clear(RegisterT reg) {
    bits_ &= ~(1u << reg.code());
  }

  // Check if a register is in the list
  constexpr bool has(RegisterT reg) const {
    return (bits_ & (1u << reg.code())) != 0;
  }

  // Check if the list is empty
  constexpr bool is_empty() const {
    return bits_ == 0;
  }

  // Count the number of registers in the list
  constexpr int Count() const {
    return __builtin_popcount(bits_);
  }

  // Get the raw bits
  constexpr storage_t bits() const {
    return bits_;
  }

  // Get first register in the list
  constexpr RegisterT first() const {
    if (bits_ == 0) {
      return RegisterT::no_reg();
    }
    int first_code = __builtin_ctz(bits_);
    return RegisterT::from_code(first_code);
  }

  // Get last register in the list
  constexpr RegisterT last() const {
    if (bits_ == 0) {
      return RegisterT::no_reg();
    }
    int last_code = 31 - __builtin_clz(bits_);
    return RegisterT::from_code(last_code);
  }

  // Static factory method to create from bits
  static constexpr RegListBase FromBits(storage_t bits) {
    return RegListBase(bits);
  }

  // Bitwise OR
  constexpr RegListBase operator|(const RegListBase& other) const {
    return RegListBase(bits_ | other.bits_);
  }

  // Bitwise AND
  constexpr RegListBase operator&(const RegListBase& other) const {
    return RegListBase(bits_ & other.bits_);
  }

  // Difference (this & ~other)
  constexpr RegListBase operator-(const RegListBase& other) const {
    return RegListBase(bits_ & ~other.bits_);
  }

  // Equality
  constexpr bool operator==(const RegListBase& other) const {
    return bits_ == other.bits_;
  }

  constexpr bool operator!=(const RegListBase& other) const {
    return bits_ != other.bits_;
  }

  // Iterator support for range-based for loops
  class Iterator {
   public:
    constexpr explicit Iterator(storage_t bits, int index = -1)
        : bits_(bits), index_(index) {
      if (index_ == -1) {
        // Find first set bit
        AdvanceToNext();
      }
    }

    constexpr RegisterT operator*() const {
      return RegisterT::from_code(index_);
    }

    constexpr Iterator& operator++() {
      // Clear the current bit and find next
      bits_ &= ~(1u << index_);
      AdvanceToNext();
      return *this;
    }

    constexpr bool operator!=(const Iterator& other) const {
      return bits_ != other.bits_ || index_ != other.index_;
    }

   private:
    constexpr void AdvanceToNext() {
      if (bits_ == 0) {
        index_ = -1;
        return;
      }
      // Find the lowest set bit
      index_ = __builtin_ctz(bits_);
    }

    storage_t bits_;
    int index_;
  };

  constexpr Iterator begin() const {
    return Iterator(bits_);
  }

  constexpr Iterator end() const {
    return Iterator(0, -1);
  }

 private:
  storage_t bits_;
};

// Type aliases for register lists
using RegList = RegListBase<Register>;
using DoubleRegList = RegListBase<DoubleRegister>;

// Empty list constants
static constexpr RegList kEmptyRegList{};
static constexpr DoubleRegList kEmptyDoubleRegList{};

}  // namespace internal
}  // namespace v8

#endif  // WASI_V8_REGLIST_FIX_H_
