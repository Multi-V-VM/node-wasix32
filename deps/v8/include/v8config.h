// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8CONFIG_H_
#define V8CONFIG_H_

// For WASI/Emscripten, include custom WASI config file first
#if defined(__wasi__) || defined(__EMSCRIPTEN__)
#include "wasi/v8config-wasi.h"
#endif

// Basic V8 macros
#define V8_INLINE inline
#define V8_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define V8_EXPORT
#define V8_NOINLINE __attribute__((noinline))
#define V8_BASE_EXPORT
#define V8_EXPORT_PRIVATE
#define V8_NODISCARD [[nodiscard]]

// Branch prediction macros
#define V8_LIKELY(condition) __builtin_expect(!!(condition), 1)
#define V8_UNLIKELY(condition) __builtin_expect(!!(condition), 0)

// Platform-specific types for WASI
#ifdef __wasi__
typedef int PlatformSharedMemoryHandle;
#endif

// Additional macros for WASI compatibility
#define V8_ASSUME(condition) __builtin_assume(condition)
#define V8_TRIVIAL_ABI
#define V8_DEPRECATED_SOON(message)

// Add basic macro definitions to prevent redefinition conflicts
#ifndef EXPORT_CONTEXTUAL_VARIABLE
#define EXPORT_CONTEXTUAL_VARIABLE(TYPE, NAME) \
  using NAME = ContextualVariable<TYPE, TYPE>
#endif

#endif  // V8CONFIG_H_
