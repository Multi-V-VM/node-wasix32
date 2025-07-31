#ifndef WASI_V8_REGLIST_FIX_H_
#define WASI_V8_REGLIST_FIX_H_

// Fixes for register list types in WASI build

#include <cstdint>
#include <initializer_list>

#include "../../../../../wasi-v8-bits-fixes.h"

namespace v8 {
namespace internal {

// Forward declare register types
class Register;
class DoubleRegister;

// Basic RegListBase template definition for WASI
template <typename RegisterT>
class RegListBase {
 public:
  using storage_t = uint64_t;  // Use 64-bit storage for simplicity

  constexpr RegListBase() : bits_(0) {}

  constexpr RegListBase(std::initializer_list<RegisterT> regs) : bits_(0) {
    for (RegisterT reg : regs) {
      set(reg);
    }
  }

  constexpr explicit RegListBase(storage_t bits) : bits_(bits) {}

  constexpr void set(RegisterT reg) {
    if (reg.is_valid()) {
      bits_ |= (storage_t{1} << reg.code());
    }
  }

  constexpr void clear(RegisterT reg) {
    if (reg.is_valid()) {
      bits_ &= ~(storage_t{1} << reg.code());
    }
  }

  constexpr bool has(RegisterT reg) const {
    return reg.is_valid() && (bits_ & (storage_t{1} << reg.code())) != 0;
  }

  constexpr bool is_empty() const { return bits_ == 0; }

  constexpr int Count() const { return v8::base::bits::CountPopulation(bits_); }

  constexpr storage_t bits() const { return bits_; }

  constexpr RegListBase operator|(const RegListBase& other) const {
    return RegListBase(bits_ | other.bits_);
  }

  constexpr RegListBase operator&(const RegListBase& other) const {
    return RegListBase(bits_ & other.bits_);
  }

  constexpr RegListBase operator-(const RegListBase& other) const {
    return RegListBase(bits_ & ~other.bits_);
  }

  constexpr bool operator==(const RegListBase& other) const {
    return bits_ == other.bits_;
  }

  constexpr bool operator!=(const RegListBase& other) const {
    return bits_ != other.bits_;
  }

  // Iterator support (simplified)
  class Iterator {
   public:
    Iterator(storage_t bits) : bits_(bits) {
      if (bits_ != 0) {
        int code = ::v8::base::bits::CountTrailingZeros(bits_);
        current_ = RegisterT::from_code(code);
      }
    }

    RegisterT operator*() const { return current_; }

    Iterator& operator++() {
      bits_ &= bits_ - 1;  // Clear lowest set bit
      if (bits_ != 0) {
        int code = ::v8::base::bits::CountTrailingZeros(bits_);
        current_ = RegisterT::from_code(code);
      }
      return *this;
    }

    bool operator!=(const Iterator& other) const {
      return bits_ != other.bits_;
    }

   private:
    storage_t bits_;
    RegisterT current_;
  };

  Iterator begin() const { return Iterator(bits_); }
  Iterator end() const { return Iterator(0); }

 private:
  storage_t bits_;
};

// Type aliases
using RegList = RegListBase<Register>;
using DoubleRegList = RegListBase<DoubleRegister>;

}  // namespace internal
}  // namespace v8

#endif  // WASI_V8_REGLIST_FIX_H_