// Copyright 2019 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_NUMBERS_HASH_SEED_INL_H_
#define V8_NUMBERS_HASH_SEED_INL_H_

#include <stdint.h>

// The #includes below currently lead to cyclic transitive includes, so
// HashSeed() ends up being required before it is defined, so we have to
// declare it here. This is a workaround; if we needed this permanently then
// we should put that line into a "hash-seed.h" header; but we won't need
// it for long.
// TODO(jkummerow): Get rid of this by breaking circular include dependencies.
namespace v8 {
namespace internal {

class Isolate;
class LocalIsolate;
class ReadOnlyRoots;

inline uint64_t HashSeed(Isolate* isolate);
inline uint64_t HashSeed(LocalIsolate* isolate);
inline uint64_t HashSeed(ReadOnlyRoots roots);

}  // namespace internal
}  // namespace v8

// See comment above for why this isn't at the top of the file.
#include "src/objects/fixed-array-inl.h"
#include "src/roots/roots-inl.h"

namespace v8 {
namespace internal {

inline uint64_t HashSeed(Isolate* isolate) {
  return HashSeed(ReadOnlyRoots(isolate));
}

inline uint64_t HashSeed(LocalIsolate* isolate) {
  return HashSeed(ReadOnlyRoots(isolate));
}

inline uint64_t HashSeed(ReadOnlyRoots roots) {
  uint64_t seed;
#ifdef __wasi__
  // Debug output for WASI
  size_t hash_seed_index = static_cast<size_t>(RootIndex::kHashSeed);
  fprintf(stderr, "HashSeed: hash_seed_index=%zu\n", hash_seed_index);

  // Use public accessor to check if hash_seed is initialized
  bool is_init = roots.is_initialized(RootIndex::kHashSeed);
  fprintf(stderr, "HashSeed: is_initialized=%d\n", is_init);

  Tagged<ByteArray> hash_seed_array = roots.hash_seed();
  fprintf(stderr, "HashSeed: hash_seed_array ptr=%p\n", (void*)hash_seed_array.ptr());

  if (hash_seed_array.ptr() == 0) {
    fprintf(stderr, "HashSeed: ERROR - hash_seed is null! Returning 0\n");
    fflush(stderr);
    return 0;
  }

  uint8_t* begin_ptr = hash_seed_array->begin();
  fprintf(stderr, "HashSeed: begin_ptr=%p\n", (void*)begin_ptr);
  fflush(stderr);
  MemCopy(&seed, begin_ptr, sizeof(seed));
#else
  MemCopy(&seed, roots.hash_seed()->begin(), sizeof(seed));
#endif
  return seed;
}

}  // namespace internal
}  // namespace v8

#endif  // V8_NUMBERS_HASH_SEED_INL_H_
