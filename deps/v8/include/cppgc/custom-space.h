// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_CPPGC_CUSTOM_SPACE_H_
#define INCLUDE_CPPGC_CUSTOM_SPACE_H_

#include <cstddef>

namespace cppgc {

/**
 * Base class for custom spaces.
 */
class CustomSpace {
 public:
  virtual ~CustomSpace() = default;
  
  /**
   * Returns the index of the custom space.
   */
  virtual size_t GetIndex() const = 0;
  
  /**
   * Returns true if the custom space is compactable.
   */
  virtual bool IsCompactable() const = 0;
};

/**
 * Base class for custom space indices.
 */
class CustomSpaceIndex {
 public:
  constexpr CustomSpaceIndex(size_t value) : value_(value) {}
  constexpr size_t value() const { return value_; }

 private:
  size_t value_;
};

namespace internal {
// This needs to be here for compatibility
using CustomSpace = size_t;
}  // namespace internal

}  // namespace cppgc

#endif  // INCLUDE_CPPGC_CUSTOM_SPACE_H_