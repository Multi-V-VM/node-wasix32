#ifndef V8_BASE_ONCE_H_
#define V8_BASE_ONCE_H_

#include "src/base/base-export.h"
#ifdef __wasi__
#include "include/v8-internal.h"
#endif

namespace v8 {
namespace base {

using OnceType = int;

#define V8_ONCE_INIT 0
#define ONCE_STATE_UNINITIALIZED 0
#define ONCE_STATE_DONE 1

#ifndef __wasi__
template <typename T>
void CallOnce(v8::Once::OnceType* once, T init_func) {
  if (*once == ONCE_STATE_UNINITIALIZED) {
    init_func();
    *once = ONCE_STATE_DONE;
  }
}
#endif

}  // namespace base
}  // namespace v8

#endif  // V8_BASE_ONCE_H_
