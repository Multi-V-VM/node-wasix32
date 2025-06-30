#ifndef V8_BASE_ONCE_H_
#define V8_BASE_ONCE_H_

#include "src/base/base-export.h"

namespace v8 {
namespace base {

#ifdef __wasi__
// Already defined in nuclear-fix.h
#else
using OnceType = int;
#endif

#define V8_ONCE_INIT 0
#define ONCE_STATE_UNINITIALIZED 0
#define ONCE_STATE_DONE 1

#ifndef __wasi__
template <typename T>
void CallOnce(OnceType* once, T init_func) {
  if (*once == ONCE_STATE_UNINITIALIZED) {
    init_func();
    *once = ONCE_STATE_DONE;
  }
}
#endif

}  // namespace base
}  // namespace v8

#endif  // V8_BASE_ONCE_H_
