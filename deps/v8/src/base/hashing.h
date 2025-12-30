#ifdef __wasi__
#include "wasi/macro-fixes.h"
#endif

// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_HASHING_H_
#define V8_BASE_HASHING_H_

// hashing.h extends functional.h with additional utilities like Hasher class.
// Include functional.h for the core hash functions.
#include "src/base/functional.h"

#include <vector>

#ifdef __wasi__
// Also include v8-internal.h to ensure bits namespace is available
#include "../../include/v8-internal.h"
#endif

namespace v8 {
namespace base {

// Hashable trait for WASI compatibility
template <typename T, typename = void>
struct is_hashable : ::std::false_type {};

template <typename T>
struct is_hashable<T, ::std::void_t<decltype(hash_value(::std::declval<const T&>()))>>
    : ::std::true_type {};

// Detect whether std::hash<T> is invocable for T without requiring C++20 concepts.
template <typename U, typename = void>
struct has_std_hash : ::std::false_type {};
template <typename U>
struct has_std_hash<U, ::std::void_t<decltype(::std::declval<::std::hash<U>>()(::std::declval<const U&>()))>>
    : ::std::true_type {};

// Internal hash computation to avoid name conflicts.
template <typename T>
V8_INLINE constexpr size_t ComputeHash(const T& v) {
  if constexpr (is_hashable<T>::value) {
    return hash_value(v);
  } else if constexpr (has_std_hash<T>::value) {
    return ::std::hash<T>{}(v);
  } else {
    return hash_combine(static_cast<size_t>(0),
                        static_cast<size_t>(reinterpret_cast<uintptr_t>(&v)));
  }
}

// base::Hasher makes it easier to combine multiple fields into one hash and
// avoids the ambiguity of the different {hash_combine} methods.
class Hasher {
 public:
  constexpr Hasher() = default;
  constexpr explicit Hasher(size_t seed) : hash_(seed) {}

  // Retrieve the current hash.
  constexpr size_t hash() const { return hash_; }

  // Combine an existing hash value into this hasher's hash.
  Hasher& AddHash(size_t other_hash) {
    hash_ = hash_combine(hash_, other_hash);
    return *this;
  }

  // Hash a value {t} and combine its hash into this hasher's hash.
  template <typename T>
  Hasher& Add(const T& t) { return AddHash(ComputeHash(t)); }

  // Hash a range of values and combine the hashes into this hasher's hash.
  template <typename Iterator>
  Hasher& AddRange(Iterator first, Iterator last) {
    for (; first != last; ++first) Add(*first);
    return *this;
  }

  // Hash a collection of values and combine the hashes into this hasher's hash.
  template <typename C>
  auto AddRange(C collection)
      -> decltype(AddRange(::std::begin(collection), ::std::end(collection))) {
    return AddRange(::std::begin(collection), ::std::end(collection));
  }

  // Hash multiple values and combine their hashes.
  template <typename... T>
  constexpr static size_t Combine(const T&... ts) {
    Hasher hasher;
    (..., hasher.Add(ts));
    return hasher.hash();
  }

 private:
  size_t hash_ = 0;
};

// Provide a hash_value function for each T with a hash_value member function.
template <typename T>
auto hash_value(const T& v) -> decltype(v.hash_value()) {
  return v.hash_value();
}

// Provide hash_value for std::vector
template <typename T>
V8_INLINE size_t hash_value(const ::std::vector<T>& v) {
  return Hasher{}.AddRange(v).hash();
}

}  // namespace base
}  // namespace v8

#ifdef __wasi__
// Include namespace aliases AFTER v8::base is defined
#include "wasi/v8-internal-base-namespace.h"
#endif

#endif  // V8_BASE_HASHING_H_
