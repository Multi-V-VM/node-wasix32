#ifndef WASI_V8_FLAGS_FIX_H_
#define WASI_V8_FLAGS_FIX_H_

#include <cstdint>

// Fix for missing Flags template types and operators
namespace v8 {
namespace base {

// Note: Flags template is already defined in wasi-v8-template-utils.h with one parameter
// Don't redeclare it here

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
#if !defined(V8_WASI_SMI_TAG_MASK_DEFINED)
#define V8_WASI_SMI_TAG_MASK_DEFINED
constexpr intptr_t kSmiTagMask = (1 << 1) - 1;  // match nuclear-fix.h type
#endif
} // namespace internal
} // namespace v8

// Also define in global namespace for compatibility
namespace i = ::v8::internal;

#endif // WASI_V8_FLAGS_FIX_H_
