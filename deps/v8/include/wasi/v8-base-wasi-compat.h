#ifndef V8_INCLUDE_WASI_V8_BASE_WASI_COMPAT_H_
#define V8_INCLUDE_WASI_V8_BASE_WASI_COMPAT_H_

#ifdef __wasi__

#include "../../src/base/atomic-utils.h"
#include "../../src/base/platform/mutex.h"
#include "../../src/base/platform/semaphore.h"
#include "../../src/base/utils/random-number-generator.h"
#include "../../src/base/vector.h"

#endif  // __wasi__

#endif  // V8_INCLUDE_WASI_V8_BASE_WASI_COMPAT_H_
