#ifndef V8_WASI_NAMESPACE_FIX_H_
#define V8_WASI_NAMESPACE_FIX_H_

#ifdef __wasi__

// Provide lightweight namespace bridges so accidental nested `v8::v8::…`
// references resolve to the canonical definitions. Avoid touching std:: or
// third‑party namespaces here.

// Ensure the global cppgc namespace exists so we can bridge to it later even
// if cppgc headers have not been included yet.
namespace cppgc {}

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

// Forward common nested lookups for v8::base and v8::base::bits
namespace v8 {
namespace base {
using namespace ::v8::base;
namespace bits {
using namespace ::v8::base::bits;
}  // namespace bits
// Bridge cppgc lookups appearing inside the v8 namespace.
namespace cppgc = ::cppgc;
}  // namespace base
}  // namespace v8

namespace v8 {
namespace v8 {
namespace base {
using namespace ::v8::base;
namespace bits {
using namespace ::v8::base::bits;
}  // namespace bits
namespace cppgc = ::cppgc;
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
namespace cppgc = ::cppgc;
}  // namespace base
}  // namespace v8
}  // namespace v8
}  // namespace v8

// Also provide a direct bridge for v8::cppgc (outside of v8::base)
namespace v8 {
namespace cppgc = ::cppgc;
}  // namespace v8

namespace v8 {
namespace v8 {
namespace cppgc = ::cppgc;
}  // namespace v8
}  // namespace v8

namespace v8 {
namespace v8 {
namespace v8 {
namespace cppgc = ::cppgc;
}  // namespace v8
}  // namespace v8
}  // namespace v8

// Bridge v8::internal (and nested variants) back to ::v8::internal
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

#endif  // V8_WASI_NAMESPACE_FIX_H_
