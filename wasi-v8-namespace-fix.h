#ifndef WASI_V8_NAMESPACE_FIX_H_
#define WASI_V8_NAMESPACE_FIX_H_

#ifdef __wasi__

// Keep this shim minimal and avoid creating nested v8::v8 namespaces.
// Delegate namespace bridging to the in-tree V8 WASI header, and only
// provide the lightweight wrappers actually referenced by our patches.

#include <vector>
// Ensure ::v8::base::Vector is declared before bridging.
#include "deps/v8/src/base/vector.h"
// Use the safer in-tree namespace bridge for nested v8 lookups.
#include "deps/v8/include/wasi/v8-namespace-fix.h"

// Bridge so GlobalHandle::v8::base::Vector<T> resolves to ::v8::base::Vector<T>
namespace GlobalHandle {
namespace v8 {
namespace base {
template <typename T>
using Vector = ::v8::base::Vector<T>;
}  // namespace base
}  // namespace v8
}  // namespace GlobalHandle

// Minimal Detachable::v8::base::Vector wrapper for a small API surface.
namespace Detachable {
namespace v8 {
namespace base {
template <typename T>
class Vector {
 public:
  Vector() = default;
  // Container-like API
  void detach() { data_.clear(); data_.shrink_to_fit(); }
  void free() { data_.clear(); data_.shrink_to_fit(); }
  bool empty() const { return data_.empty(); }
  size_t size() const { return data_.size(); }
  void push_back(const T& v) { data_.push_back(v); }
  void pop_back() { data_.pop_back(); }
  T& back() { return data_.back(); }
  const T& back() const { return data_.back(); }
  // Indexing used in a few places
  T& operator[](size_t i) { return data_[i]; }
  const T& operator[](size_t i) const { return data_[i]; }

 private:
  std::vector<T> data_;
};
}  // namespace base
}  // namespace v8
}  // namespace Detachable

#endif  // __wasi__

#endif  // WASI_V8_NAMESPACE_FIX_H_
