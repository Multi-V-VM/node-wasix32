#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_HANDLE_BASE_H_
#define INCLUDE_V8_HANDLE_BASE_H_

#ifdef __wasi__
// WASI stub for v8-handle-base

namespace v8 {
template<bool B> class StackAllocated {};

// Simplified handle base for WASI
class HandleBase {
public:
  HandleBase() = default;
};

} // namespace v8

#else
// Include real v8-handle-base.h for non-WASI
#endif

#endif // INCLUDE_V8_HANDLE_BASE_H_
