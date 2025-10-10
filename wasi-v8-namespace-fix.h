#ifndef WASI_V8_NAMESPACE_FIX_H_
#define WASI_V8_NAMESPACE_FIX_H_

#ifdef __wasi__

// Note: The actual V8 header includes should happen BEFORE this file
// via wasi-v8-base-includes.h to avoid namespace nesting issues. At this
// point we only need to forward declare the minimal surface that Node pulls in
// before the full V8 headers are available.

namespace v8::metrics {
class Recorder;
}  // namespace v8::metrics

// If some translation units included V8 headers from within `namespace v8 { … }`
// scopes we can end up with symbols instantiated as `v8::v8::…`. Provide light
// weight namespace bridges so those accidental nestings still resolve to the
// canonical definitions. This mirrors the original workaround, but without
// touching std:: or other C++ library namespaces.

namespace v8 {
namespace v8 {
using namespace ::v8;
}  // namespace v8
}  // namespace v8

namespace v8 {
namespace v8 {
namespace v8 {
using namespace ::v8;
}  // namespace v8
}  // namespace v8
}  // namespace v8

// Forward v8::base lookups for the common nested cases.
namespace v8 {
namespace base {
using namespace ::v8::base;
namespace bits {
using namespace ::v8::base::bits;
}  // namespace bits
}  // namespace base
}  // namespace v8

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

// Make sure v8::internal and its nested variants point to the global one.
namespace v8 {
namespace internal {
using namespace ::v8::internal;
namespace base {
using namespace ::v8::base;
}  // namespace base
}  // namespace internal
}  // namespace v8

namespace v8 {
namespace v8 {
namespace internal {
using namespace ::v8::internal;
}  // namespace internal
}  // namespace v8
}  // namespace v8

namespace v8 {
namespace v8 {
namespace v8 {
namespace internal {
using namespace ::v8::internal;
}  // namespace internal
}  // namespace v8
}  // namespace v8
}  // namespace v8

#endif  // __wasi__

#endif  // WASI_V8_NAMESPACE_FIX_H_
