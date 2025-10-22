// Copyright 2022 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_TURBOSHAFT_FAST_HASH_H_
#define V8_COMPILER_TURBOSHAFT_FAST_HASH_H_

#include <tuple>
#include "src/base/hashing.h"

namespace v8::internal::compiler::turboshaft {

// Minimal helpers to combine hashes quickly. Type-specific hashing is defined
// alongside the types themselves (e.g., in index.h) to avoid ODR/ordering issues.
V8_INLINE size_t fast_hash_combine() { return 0u; }
V8_INLINE size_t fast_hash_combine(size_t acc) { return acc; }
V8_INLINE size_t fast_hash_combine(size_t acc, size_t value) {
  return 17 * acc + value;
}

template <typename T, typename... Ts>
V8_INLINE size_t fast_hash_combine(T const& v, Ts const&... vs) {
  return fast_hash_combine(fast_hash_combine(vs...), ::v8::base::hash<T>()(v));
}

template <typename Iterator>
V8_INLINE size_t fast_hash_range(Iterator first, Iterator last) {
  size_t acc = 0;
  for (; first != last; ++first) {
    acc = fast_hash_combine(acc, *first);
  }
  return acc;
}

}  // namespace v8::internal::compiler::turboshaft

#endif  // V8_COMPILER_TURBOSHAFT_FAST_HASH_H_

