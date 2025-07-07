#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
// Copyright 2021 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_V8_MAYBE_H_
#define INCLUDE_V8_MAYBE_H_

#include <type_traits>

#include "v8config.h"  // NOLINT(build/include_directory)

namespace v8 {

namespace api_internal {
// Called when Maybe<> is passed to ToChecked.
V8_EXPORT void JustEmptyError(const char* location);
}  // namespace api_internal

/**
 * A simple Maybe type, representing an object which may or may not have a
 * value, see https://hackage.haskell.org/package/base/docs/Data-Maybe.html.
 *
 * If an API method returns a Maybe<>, the API method can potentially fail
 * either because an exception is thrown, or because an exception is pending,
 * e.g. because a previous API call threw an exception that hasn't been caught
 * yet, or because a TerminateExecution exception was thrown. In that case, a
 * "Nothing" value is returned.
 */
template <class T>
class Maybe {
 public:
  V8_INLINE bool IsNothing() const { return !has_value_; }
  V8_INLINE bool IsJust() const { return has_value_; }

  /**
   * An alias for |FromJust|. Will crash if the Maybe<> is nothing.
   */
  V8_INLINE T ToChecked() const { return FromJust(); }

  /**
   * Short-hand for ToChecked(), which doesn't return a value. To be used, where
   * the actual value of the Maybe is not needed like Object::Set.
   */
  V8_INLINE void Check() const {
    if (V8_UNLIKELY(!IsJust())) api_internal::JustEmptyError(nullptr);
  }

  /**
   * Converts this Maybe<> to a value of type T. If this Maybe<> is nothing
   * (empty), then the supplied default value is returned.
   */
  V8_INLINE T FromMaybe(const T& default_value) const {
    return has_value_ ? value_ : default_value;
  }

  /**
   * Converts this Maybe<> to a value of type T. This method should only be used
   * if it has been verified that this Maybe<> is not empty.
   */
  V8_INLINE T FromJust() const& {
    if (V8_UNLIKELY(!IsJust())) api_internal::JustEmptyError(nullptr);
    return value_;
  }

  /**
   * Converts this Maybe<> to a value of type T. This method should only be used
   * if it has been verified that this Maybe<> is not empty.
   */
  V8_INLINE T FromJust() && {
    if (V8_UNLIKELY(!IsJust())) api_internal::JustEmptyError(nullptr);
    return std::move(value_);
  }

  V8_INLINE T ToLocalChecked() const {
    return FromJust();
  }

  V8_INLINE bool To(T* out) const {
    if (V8_LIKELY(IsJust())) *out = value_;
    return IsJust();
  }

  // Move semantics
  V8_INLINE Maybe(Maybe&& other) noexcept
      : has_value_(other.has_value_), value_(std::move(other.value_)) {
    other.has_value_ = false;
  }

  V8_INLINE Maybe& operator=(Maybe&& other) noexcept {
    if (this != &other) {
      has_value_ = other.has_value_;
      value_ = std::move(other.value_);
      other.has_value_ = false;
    }
    return *this;
  }

  // Copy semantics
  V8_INLINE Maybe(const Maybe& other) = default;
  V8_INLINE Maybe& operator=(const Maybe& other) = default;

  V8_INLINE bool operator==(const Maybe& other) const {
    return (IsJust() == other.IsJust()) &&
           (!IsJust() || FromJust() == other.FromJust());
  }

  V8_INLINE bool operator!=(const Maybe& other) const {
    return !operator==(other);
  }

 private:
  Maybe() : has_value_(false) {}
  explicit Maybe(const T& t) : has_value_(true), value_(t) {}
  explicit Maybe(T&& t) : has_value_(true), value_(std::move(t)) {}

  bool has_value_;
  T value_;

  template <class U>
  friend Maybe<U> Nothing();
  template <class U>
  friend Maybe<U> Just(const U& u);
  template <class U, class... Args>
  friend Maybe<U> Just(Args&&... args);
};

template <class T>
inline Maybe<T> Nothing() {
  return Maybe<T>();
}

template <class T>
inline Maybe<T> Just(const T& t) {
  return Maybe<T>(t);
}

// Don't use forwarding references here but instead use Args... to avoid
// clashing with the copy constructor.
template <class T, class... Args>
inline Maybe<T> Just(Args&&... args) {
  return Maybe<T>(T(std::forward<Args>(args)...));
}

}  // namespace v8

#endif  // INCLUDE_V8_MAYBE_H_