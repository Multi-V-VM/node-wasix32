#ifndef WASI_V8_FLAGS_FIX_H_
#define WASI_V8_FLAGS_FIX_H_

// Fix for missing Flags template types and operators
namespace v8 {
namespace internal {
namespace base {

// Extend the Flags template to have the missing type definitions
template<typename E, typename T>
struct FlagsTypeHelper {
  using flag_type = E;
  using mask_type = T;
};

} // namespace base
} // namespace internal
} // namespace v8

// Fix for missing kSmiTagMask constant
namespace v8 {
namespace internal {
constexpr int kSmiTagMask = 1;
} // namespace internal
} // namespace v8

// Also define in global namespace for compatibility
namespace i = ::v8::internal;

#endif // WASI_V8_FLAGS_FIX_H_