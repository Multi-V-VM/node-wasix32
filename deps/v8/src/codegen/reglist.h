// Copyright 2017 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_REGLIST_H_
#define V8_CODEGEN_REGLIST_H_

#include <initializer_list>

#include "src/base/bits.h"
// Avoid circular include with register.h; rely on forward declarations.

namespace v8 {
namespace internal {

// Forward declarations
class Register;
// Note: DoubleRegister is defined by arch-specific register headers.

// RegList implemented as a compact bit-mask.
template <class RegisterType>
class RegListBase {
 public:
  // Default: empty set
  constexpr RegListBase() : bits_(0) {}

  // Construct from explicit mask
  constexpr explicit RegListBase(uint32_t bits) : bits_(bits) {}

  // Construct from initializer list of registers
  constexpr RegListBase(std::initializer_list<RegisterType> init) : bits_(0) {
    for (auto r : init) {
      bits_ |= (uint32_t{1} << r.code());
    }
  }

  // Bit-mask of registers
  constexpr uint32_t bits() const { return bits_; }

  // Count of set bits
  constexpr int Count() const {
    return ::v8::base::bits::CountPopulation(bits_);
  }

  // Membership
  constexpr bool contains(RegisterType r) const {
    return (bits_ & (uint32_t{1} << r.code())) != 0;
  }

  // Set operations
  constexpr RegListBase operator|(const RegListBase& other) const {
    return RegListBase(bits_ | other.bits_);
  }
  constexpr RegListBase operator&(const RegListBase& other) const {
    return RegListBase(bits_ & other.bits_);
  }

 private:
  uint32_t bits_;
};

// Type definitions
using RegList = RegListBase<Register>;
using DoubleRegList = RegListBase<DoubleRegister>;

// Constants
static constexpr RegListBase<Register> kEmptyRegList{};

// Note: Architecture-specific allocatable sets are defined elsewhere.

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_REGLIST_H_
