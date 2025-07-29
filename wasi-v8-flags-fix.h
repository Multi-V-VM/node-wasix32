#ifndef WASI_V8_FLAGS_FIX_H_
#define WASI_V8_FLAGS_FIX_H_

// Fix for missing Flags template types and operators
namespace v8 {
namespace base {

// Forward declare the template class
template <typename EnumT, typename BitfieldT, typename BitfieldStorageT> class Flags;

} // namespace base
} // namespace v8

// Forward declare CodeKindFlag to help with template resolution
namespace v8 {
namespace internal {
enum class CodeKindFlag;
}
}

// Fix for missing kSmiTagMask constant
namespace v8 {
namespace internal {
constexpr int kSmiTagMask = 1;
} // namespace internal
} // namespace v8

// Also define in global namespace for compatibility
namespace i = ::v8::internal;

#endif // WASI_V8_FLAGS_FIX_H_