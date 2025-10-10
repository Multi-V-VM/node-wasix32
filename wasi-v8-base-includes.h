#ifndef WASI_V8_BASE_INCLUDES_H_
#define WASI_V8_BASE_INCLUDES_H_

#ifdef __wasi__

// This header MUST be included at global scope (outside any namespace blocks)
// to prevent creating nested v8::v8 namespaces

// Include V8 base headers - they define types in the ::v8::base namespace
#include "src/base/macros.h"
#include "src/base/atomicops.h"
#include "src/base/atomic-utils.h"
#include "src/base/bit-field.h"
#include "src/base/bits.h"
#include "src/base/once.h"
#include "src/base/lazy-instance.h"
#include "src/base/iterator.h"
#include "src/base/numbers/double.h"
#include "src/base/vector.h"                    // Vector needed by strings.h
#include "src/base/platform/semaphore.h"        // Must precede platform.h
#include "src/base/platform/elapsed-timer.h"
// NOTE: mutex.h is NOT included here to avoid circular dependency
// (lazy-instance.h -> once.h -> v8-internal.h -> wasi-v8-base-includes.h -> mutex.h)
// mutex.h will be included separately where needed
#include "src/base/platform/platform.h"
#include "src/base/platform/time.h"
#include "src/base/strings.h"
#include "src/base/utils/random-number-generator.h"
#include "wasi-v8-bits-fixes.h"
#include "wasi-v8-lazy-instance-fix.h"

#endif  // __wasi__

#endif  // WASI_V8_BASE_INCLUDES_H_
