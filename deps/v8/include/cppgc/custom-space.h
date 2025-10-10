// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_CPPGC_CUSTOM_SPACE_H_
#define INCLUDE_CPPGC_CUSTOM_SPACE_H_

#include <stddef.h>
#include <vector>

#include "v8config.h"

namespace cppgc {

// Type alias for custom space index
using CustomSpaceIndex = size_t;

class V8_EXPORT CustomSpaceBase {
 public:
  virtual ~CustomSpaceBase() = default;
  virtual CustomSpaceIndex GetCustomSpaceIndex() const = 0;

 protected:
  CustomSpaceBase() = default;
};

template <size_t kSpaceIndex>
class CustomSpace : public CustomSpaceBase {
 public:
  static constexpr CustomSpaceIndex kSpaceIndex_ = kSpaceIndex;

  CustomSpaceIndex GetCustomSpaceIndex() const final {
    return kSpaceIndex_;
  }

 protected:
  CustomSpace() = default;
};

#ifdef __wasi__
// WASI-specific: DefaultCustomSpace for default allocation
#ifndef CPPGC_WASI_DEFAULT_CUSTOM_SPACE_ALIAS
class DefaultCustomSpace : public CustomSpaceBase {
 public:
  static constexpr size_t kSpaceIndex = 0;
  CustomSpaceIndex GetCustomSpaceIndex() const override {
    return kSpaceIndex;
  }
};
#endif

// SpaceTrait template for WASI - use DefaultCustomSpace as default
// Only define SpaceTrait here if no external WASI stub provided it.
#ifndef CPPGC_WASI_SPACETRAIT_DEFINED
template <typename T>
struct SpaceTrait {
  using Space = DefaultCustomSpace;
};
#endif
#endif  // __wasi__

}  // namespace cppgc

#endif  // INCLUDE_CPPGC_CUSTOM_SPACE_H_
