#ifndef V8_INCLUDE_WASI_V8_INTERNAL_BASE_NAMESPACE_H_
#define V8_INCLUDE_WASI_V8_INTERNAL_BASE_NAMESPACE_H_

#ifdef __wasi__

// This header provides namespace aliases to map v8::base functions
// into v8::internal::base where the compiler code expects them.

// Ensure bits helpers are visible before aliasing
#include "src/base/bits.h"

namespace v8 {
namespace internal {
namespace base {

// Bring hash_combine from v8::base into v8::internal::base
using ::v8::base::hash_combine;
using ::v8::base::Hasher;

// Note: bits namespace functions are already aliased in src/base/bits.h
// Note: all_of, any_of, sort are aliased in container-utils.h, not here
// This file only handles hash_combine and Hasher

}  // namespace base
}  // namespace internal
}  // namespace v8

#endif  // __wasi__
#endif  // V8_INCLUDE_WASI_V8_INTERNAL_BASE_NAMESPACE_H_
