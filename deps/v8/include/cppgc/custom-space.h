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

}  // namespace cppgc

#endif  // INCLUDE_CPPGC_CUSTOM_SPACE_H_
