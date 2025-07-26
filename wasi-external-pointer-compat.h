#ifndef WASI_EXTERNAL_POINTER_COMPAT_H_
#define WASI_EXTERNAL_POINTER_COMPAT_H_

namespace v8 {
namespace internal {

// Compatibility typedef for ExternalPointer_t - only if not already defined
#ifndef V8_EXTERNAL_POINTER_T_DEFINED
using ExternalPointer_t = Address;
#endif

} // namespace internal
} // namespace v8

#endif // WASI_EXTERNAL_POINTER_COMPAT_H_
