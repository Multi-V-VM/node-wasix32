// Copyright 2012 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/** \mainpage V8 API Reference Guide
 *
 * V8 is Google's open source JavaScript engine.
 *
 * This set of documents provides reference material generated from the
 * V8 header files in the include/ subdirectory.
 *
 * For other documentation see https://v8.dev/.
 */

#ifndef INCLUDE_V8_H_
#define INCLUDE_V8_H_

#ifdef __wasi__
#include "wasi/v8-namespace-fix.h"
#include "wasi/v8-handlescope-fix.h"
#include "wasi/std-namespace-fix.h"
#endif

// Include all the V8 API headers
#include "v8-array-buffer.h"
#include "v8-callbacks.h"
#include "v8-container.h"
#include "v8-context.h"
#include "v8-data.h"
#include "v8-date.h"
#include "v8-debug.h"
#include "v8-embedder-heap.h"
#include "v8-embedder-state-scope.h"
#include "v8-exception.h"
#include "v8-extension.h"
#include "v8-external.h"
#include "v8-function.h"
#ifdef __wasi__
// Include WASI initialization functions before v8-initialization.h needs them
#include "../../../wasi-v8-initialization-functions.h"
#endif
#include "v8-initialization.h"
#include "v8-internal.h"
#include "v8-isolate.h"
#include "v8-json.h"
#include "v8-local-handle.h"
#include "v8-locker.h"
#include "v8-maybe.h"
#include "v8-maybe-local.h"
#include "v8-memory-span.h"
#include "v8-message.h"
#include "v8-microtask-queue.h"
#include "v8-microtask.h"
#include "v8-object.h"
#include "v8-persistent-handle.h"
#include "v8-platform.h"
#include "v8-primitive-object.h"
#include "v8-primitive.h"
#include "v8-profiler.h"
#include "v8-promise.h"
#include "v8-proxy.h"
#include "v8-regexp.h"
#include "v8-script.h"
#include "v8-snapshot.h"
#include "v8-statistics.h"
#include "v8-template.h"
#include "v8-traced-handle.h"
#include "v8-typed-array.h"
#include "v8-unwinder.h"
#include "v8-value-serializer.h"
#include "v8-value.h"
#include "v8-version.h"
#include "v8-wasm.h"
#include "v8-weak-callback-info.h"
#include "v8config.h"

// WASI fixes - include after all V8 headers
#ifdef __wasi__
#include "wasi/v8-wasi-helpers.h"
#include "wasi/v8-wasi-compat.h"
#include "wasi/v8-fixed-array-stub.h"  // Include after v8-data.h
// wasi-v8-complete-missing.h removed - should be included before V8 headers, not after
#include "../../../wasi-v8-namespace-fix.h"
#include "../../../wasi-v8-flags-fix.h"
// wasi-v8-internals-constants.h removed - causes redefinition errors
#endif

#endif  // INCLUDE_V8_H_