// Copyright 2024 the V8 project authors. All rights reserved.
// WASI build base type includes
#ifndef V8_INCLUDE_WASI_V8_BASE_INCLUDES_H_
#define V8_INCLUDE_WASI_V8_BASE_INCLUDES_H_

#ifdef __wasi__

#include "wasi/abseil-namespace-fix.h"

// Ensure we rely on the official V8 base headers rather than local stubs.
#include "src/base/atomic-utils.h"
#include "src/base/bit-field.h"
#include "src/base/bits.h"
#include "src/base/iterator.h"
#include "src/base/numbers/double.h"
#include "src/base/platform/elapsed-timer.h"
#include "src/base/platform/mutex.h"
#include "src/base/platform/platform.h"
#include "src/base/platform/semaphore.h"
#include "src/base/platform/time.h"
#include "src/base/strings.h"
#include "src/base/utils/random-number-generator.h"
#include "src/base/vector.h"

// Include V8 profiler stubs for WASI
#include "wasi/v8-profiler-stubs.h"

#endif  // __wasi__

#endif  // V8_INCLUDE_WASI_V8_BASE_INCLUDES_H_
