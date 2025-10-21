#ifndef V8_INCLUDE_WASI_V8_INTERNAL_BASE_NAMESPACE_H_
#define V8_INCLUDE_WASI_V8_INTERNAL_BASE_NAMESPACE_H_

#ifdef __wasi__

// This header provides namespace aliases to map v8::base functions
// into v8::internal::base where the compiler code expects them.

namespace v8 {
namespace internal {
namespace base {

// Bring hash_combine from v8::base into v8::internal::base
using ::v8::base::hash_combine;
using ::v8::base::Hasher;

// Bring bits namespace functions from v8::base::bits
namespace bits {
  using ::v8::base::bits::SignedAddOverflow32;
  using ::v8::base::bits::SignedSubOverflow32;
  using ::v8::base::bits::SignedMulOverflow32;
  using ::v8::base::bits::SignedDiv32;
  using ::v8::base::bits::SignedDiv64;
  using ::v8::base::bits::SignedMod32;
  using ::v8::base::bits::SignedMod64;
  using ::v8::base::bits::UnsignedDiv32;
  using ::v8::base::bits::UnsignedDiv64;
  using ::v8::base::bits::UnsignedMod32;
  using ::v8::base::bits::UnsignedMod64;
  using ::v8::base::bits::RotateRight32;
  using ::v8::base::bits::RotateRight64;
  using ::v8::base::bits::RotateLeft32;
  using ::v8::base::bits::RotateLeft64;
  using ::v8::base::bits::CountPopulation;
  using ::v8::base::bits::CountLeadingZeros;
  using ::v8::base::bits::CountLeadingZeros32;
  using ::v8::base::bits::CountLeadingZeros64;
  using ::v8::base::bits::CountTrailingZeros;
  using ::v8::base::bits::CountTrailingZeros32;
  using ::v8::base::bits::CountTrailingZeros64;
  using ::v8::base::bits::IsPowerOfTwo;
  using ::v8::base::bits::RoundUpToPowerOfTwo32;
  using ::v8::base::bits::RoundUpToPowerOfTwo64;
  using ::v8::base::bits::RoundDownToPowerOfTwo32;
  using ::v8::base::bits::SignedSaturatedAdd64;
  using ::v8::base::bits::SignedSaturatedSub64;
}  // namespace bits

// Note: all_of, any_of, sort are aliased in container-utils.h, not here
// This file only handles hash_combine and bits namespace functions

}  // namespace base
}  // namespace internal
}  // namespace v8

#endif  // __wasi__
#endif  // V8_INCLUDE_WASI_V8_INTERNAL_BASE_NAMESPACE_H_
