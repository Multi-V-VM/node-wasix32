#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef CPPGC_COMMON_H_
#define CPPGC_COMMON_H_

#ifdef __wasi__
// WASI stub for cppgc common
// Note: StackState is defined in cppgc/heap.h
// EmbedderStackState is defined here as a forward declaration
// and will be aliased after StackState is defined
namespace cppgc {
  // Forward declare - will be defined as alias to StackState in cppgc/heap.h
  enum class StackState : uint8_t;
  // EmbedderStackState is the public-facing type, same as StackState
  using EmbedderStackState = StackState;
}
#else
// Include real cppgc/common.h for non-WASI
#endif

#endif // CPPGC_COMMON_H_
