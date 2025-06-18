// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_SOURCE_LOCATION_H_
#define INCLUDE_SOURCE_LOCATION_H_

#include <cstddef>
#include "v8config.h"  // NOLINT(build/include_directory)

#ifndef __wasi__
#include <string>
#endif

namespace v8 {

// 确保 V8_NODISCARD 被定义
#define V8_NODISCARD

/**
 * Simplified source location class for WASI
 */
class SourceLocation final {
 public:
  static constexpr SourceLocation Current() { return SourceLocation(); }
  constexpr SourceLocation() = default;
  constexpr const char* Function() const { return nullptr; }
  constexpr const char* FileName() const { return nullptr; }
  constexpr size_t Line() const { return 0; }
  
#ifdef __wasi__
  // Simplified for WASI
  const char* ToString() const { return ""; }
#else
  std::string ToString() const { return {}; }
#endif

 private:
  constexpr SourceLocation(const char* function, const char* file, size_t line)
      : function_(function), file_(file), line_(line) {}

  const char* function_ = nullptr;
  const char* file_ = nullptr;
  size_t line_ = 0u;
};

}  // namespace v8

#endif  // INCLUDE_SOURCE_LOCATION_H_
