#ifndef V8_UTILS_VECTOR_ALIAS_H_
#define V8_UTILS_VECTOR_ALIAS_H_

#include "src/base/vector.h"

namespace v8 {
namespace internal {

// Make Vector available without base:: prefix
template<typename T>
using Vector = ::v8::base::Vector<T>;

} // namespace internal
} // namespace v8

#endif // V8_UTILS_VECTOR_ALIAS_H_
