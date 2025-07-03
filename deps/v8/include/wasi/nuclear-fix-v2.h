#ifndef V8_WASI_NUCLEAR_FIX_V2_H_
#define V8_WASI_NUCLEAR_FIX_V2_H_

// Include the original fix
#include "nuclear-fix.h"

// Don't redefine V8_BASE_EXPORT if already defined
#ifndef V8_BASE_EXPORT
#define V8_BASE_EXPORT __attribute__((visibility("default")))
#endif

#endif
