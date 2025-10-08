#ifndef WASI_V8_NAMESPACE_FIX_H_
#define WASI_V8_NAMESPACE_FIX_H_

#ifdef __wasi__

// Note: The actual V8 header includes should happen BEFORE this file
// via wasi-v8-base-includes.h to avoid namespace nesting issues

namespace v8::metrics {
class Recorder;
}  // namespace v8::metrics

// Note: We do NOT forward std namespace here because it would interfere
// with system headers that are included from within namespace v8 blocks.
// Instead, all std:: references in V8 code should use ::std:: qualification.

// Create forwarding namespaces to handle nested namespace issues
// Level 1: v8::v8::base -> ::v8::base
namespace v8 {
namespace v8 {

namespace base {
using namespace ::v8::base;
namespace bits {
using namespace ::v8::base::bits;
}  // namespace bits
}  // namespace base

}  // namespace v8
}  // namespace v8

// Level 2: v8::v8::v8::base -> ::v8::base (for triple-nested cases)
namespace v8 {
namespace v8 {
namespace v8 {

namespace base {
using namespace ::v8::base;
namespace bits {
using namespace ::v8::base::bits;
}  // namespace bits
}  // namespace base

}  // namespace v8
}  // namespace v8
}  // namespace v8

// Create v8::internal::base namespace that forwards to ::v8::base
namespace v8 {
namespace internal {
namespace base {
using namespace ::v8::base;

namespace bits {
using namespace ::v8::base::bits;
}  // namespace bits

}  // namespace base
}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // WASI_V8_NAMESPACE_FIX_H_
