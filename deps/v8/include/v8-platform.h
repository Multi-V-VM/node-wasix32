#ifndef V8_V8_PLATFORM_H_
#define V8_V8_PLATFORM_H_

#ifdef __wasi__
#include "v8-platform-full.h"
#else
// For non-WASI builds, include the original v8-platform.h content
#include "v8-platform-original.h"
#endif

#endif  // V8_V8_PLATFORM_H_
