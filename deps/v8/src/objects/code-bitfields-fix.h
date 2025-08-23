#ifndef V8_OBJECTS_CODE_BITFIELDS_FIX_H_
#define V8_OBJECTS_CODE_BITFIELDS_FIX_H_

namespace v8 {
namespace internal {

// Stub definitions for missing bitfields in WASI build
struct FLAGS_BIT_FIELDS_Ranges {
  static constexpr int kBitsCount = 32;
};

using KindField = ::v8::base::BitField<int, 0, 5>;
using IsTurbofannedField = ::v8::base::BitField<bool, 5, 1>;
using MarkedForDeoptimizationField = ::v8::base::BitField<bool, 6, 1>;

}  // namespace internal
}  // namespace v8

#endif  // V8_OBJECTS_CODE_BITFIELDS_FIX_H_
