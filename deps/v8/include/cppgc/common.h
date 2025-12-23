#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef CPPGC_COMMON_H_
#define CPPGC_COMMON_H_

#ifdef __wasi__
// WASI stub for cppgc common
// StackState is defined in cppgc-wasi-compat.h (included via other paths)
// EmbedderStackState is defined as an alias in the cppgc namespace
#include <cstdint>

namespace cppgc {

// Forward declare StackState - it's defined in cppgc-wasi-compat.h
#ifndef CPPGC_STACKSTATE_DEFINED
#define CPPGC_STACKSTATE_DEFINED
enum class StackState : uint8_t {
  kNoHeapPointers,
  kMayContainHeapPointers
};
#endif

// EmbedderStackState is the public API name, same as StackState
using EmbedderStackState = StackState;

}  // namespace cppgc
#else
// Include real cppgc/common.h for non-WASI
#endif

#endif // CPPGC_COMMON_H_
