#ifndef V8_BASE_ONCE_H_
#define V8_BASE_ONCE_H_

#include "../../include/v8config.h"
#include "base-export.h"
// Avoid including heavy V8 internals here to prevent include cycles with
// lazy-instance.h. The Once wrapper only needs a trivial integral state type.

// Define macros before any namespaces so they're globally visible
#define V8_ONCE_INIT 0
#define ONCE_STATE_UNINITIALIZED 0
#define ONCE_STATE_DONE 1

// Declare a process-wide once control variable with static initialization.
#ifndef V8_DECLARE_ONCE
#define V8_DECLARE_ONCE(name) ::v8::Once::OnceType name = V8_ONCE_INIT
#endif

namespace v8 {

// Provide v8::Once wrapper expected by headers using v8::Once::OnceType.
struct Once {
  using OnceType = int;
};

namespace base {

using OnceType = int;

// Simple CallOnce implementation used by LazyInstance on WASI and generic builds.
template <typename Function, typename Storage>
inline void CallOnce(Once::OnceType* once, Function function, Storage storage) {
  if (*once == ONCE_STATE_UNINITIALIZED) {
    function(storage);
    *once = ONCE_STATE_DONE;
  }
}

// Backwards-compatible overload taking only a function with no storage param.
template <typename Function>
inline void CallOnce(Once::OnceType* once, Function function) {
  if (*once == ONCE_STATE_UNINITIALIZED) {
    function();
    *once = ONCE_STATE_DONE;
  }
}

}  // namespace base
}  // namespace v8

#endif  // V8_BASE_ONCE_H_
