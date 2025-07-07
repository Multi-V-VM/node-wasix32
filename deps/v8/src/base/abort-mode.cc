// Copyright 2023 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Minimal implementation for WASI builds
#ifdef __wasi__
// Empty - symbols provided by abort-mode-wasi-minimal.cc
#else
#include "src/base/abort-mode.h"
namespace v8 {
namespace base {
AbortMode g_abort_mode = AbortMode::kDefault;
}  // namespace base
}  // namespace v8
#endif