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

#endif  // V8CONFIG_H_
#ifndef V8_TRIVIAL_ABI
#define V8_TRIVIAL_ABI
#endif  // V8_TRIVIAL_ABI

// WASI 编译所需的额外宏定义
#ifndef V8_TRIVIAL_ABI
#define V8_TRIVIAL_ABI
#endif

#ifndef V8_NODISCARD
#define V8_NODISCARD
#endif

// 禁用某些 WASI 环境不支持的功能
#define V8_DISABLE_WRITE_BARRIERS 1
#define V8_ENABLE_SANDBOX 0
#define V8_ENABLE_WEBASSEMBLY 1
