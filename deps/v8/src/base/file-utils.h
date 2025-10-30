// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_FILE_UTILS_H_
#define V8_BASE_FILE_UTILS_H_

#ifdef __wasi__

#include <memory>

#include "src/base/base-export.h"

namespace v8 {
namespace base {

// Helper functions to manipulate file paths.

V8_BASE_EXPORT
std::unique_ptr<char[]> RelativePath(const char* exec_path, const char* name);

}  // namespace base
}  // namespace v8

#endif  // __wasi__
#endif  // V8_BASE_FILE_UTILS_H_