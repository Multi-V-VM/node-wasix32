#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef CPPGC_COMMON_H_
#define CPPGC_COMMON_H_

#ifdef __wasi__
// WASI stub for cppgc common
namespace cppgc {
enum class EmbedderStackState { kEmpty, kNonEmpty };
}
#else
// Include real cppgc/common.h for non-WASI
#endif

#endif  // CPPGC_COMMON_H_
