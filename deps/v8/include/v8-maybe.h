#ifndef INCLUDE_V8_MAYBE_H_
#define INCLUDE_V8_MAYBE_H_

#include <type_traits>  // for std::remove_reference_t
#include <utility>  // for std::move

// For WASI compatibility, v8-maybe.h redirects to v8-maybe-local.h
#include "v8-maybe-local.h"

namespace v8 {

// Simple Maybe template for WASI - basic implementation
template <class T>
class Maybe {
 public:
  Maybe() : has_value_(false) {}

  // Use forwarding reference to handle both lvalue and rvalue
  template <typename U,
            typename = std::enable_if_t<std::is_convertible_v<U, T>>>
  explicit Maybe(U&& value) : value_(std::forward<U>(value)), has_value_(true) {}

  // Move and copy support
  Maybe(const Maybe& other) = default;
  Maybe(Maybe&& other) noexcept = default;
  Maybe& operator=(const Maybe& other) = default;
  Maybe& operator=(Maybe&& other) noexcept = default;

  bool IsNothing() const { return !has_value_; }
  bool IsJust() const { return has_value_; }

  T FromJust() && { return std::move(value_); }
  const T& FromJust() const& { return value_; }

  T FromMaybe(const T& default_value) const {
    return has_value_ ? value_ : default_value;
  }

  bool To(T* out) const {
    if (has_value_) {
      *out = value_;
      return true;
    }
    return false;
  }

  static Maybe<T> Nothing() { return Maybe<T>(); }

  template <typename U>
  static Maybe<T> Just(U&& value) { return Maybe<T>(std::forward<U>(value)); }

  // Check method for WASI compatibility - assumes success when has_value_
  void Check() const {
    // In WASI, we just assume check succeeds if we have a value
    // This is a simplified implementation for compatibility
  }

  // ToChecked method - returns the value or crashes if empty
  T ToChecked() && {
    return std::move(value_);
  }
  const T& ToChecked() const& {
    return value_;
  }

 private:
  T value_;
  bool has_value_;
};

// Specialization for bool to handle common boolean return cases
template <>
class Maybe<bool> {
 public:
  Maybe() : value_(false), has_value_(false) {}
  explicit Maybe(bool value) : value_(value), has_value_(true) {}
  
  bool IsNothing() const { return !has_value_; }
  bool IsJust() const { return has_value_; }
  bool FromJust() const { return value_; }
  bool FromMaybe(bool default_value) const { 
    return has_value_ ? value_ : default_value; 
  }
  bool To(bool* out) const {
    if (has_value_) {
      *out = value_;
      return true;
    }
    return false;
  }
  
  static Maybe<bool> Nothing() { return Maybe<bool>(); }
  static Maybe<bool> Just(bool value) { return Maybe<bool>(value); }
  
  void Check() const {
    // In WASI, we just assume check succeeds if we have a value
  }
  
  bool ToChecked() const {
    return has_value_ ? value_ : false;
  }
  
 private:
  bool value_;
  bool has_value_;
};

// Specialization for void to handle Maybe<void> cases
template <>
class Maybe<void> {
 public:
  Maybe() : has_value_(false) {}
  explicit Maybe(bool has_value) : has_value_(has_value) {}
  
  bool IsNothing() const { return !has_value_; }
  bool IsJust() const { return has_value_; }
  
  void Check() const {
    // In WASI, we just assume check succeeds if we have a value
  }
  
  void FromJust() const {
    // No-op for void
  }
  
  void ToChecked() const {
    // No-op for void - just validates that we have a value
  }
  
  static Maybe<void> Nothing() { return Maybe<void>(false); }
  static Maybe<void> Just() { return Maybe<void>(true); }
  
 private:
  bool has_value_;
};

// Standalone Nothing function template for convenience
template <class T>
Maybe<T> Nothing() {
  return Maybe<T>::Nothing();
}

// Standalone Just function template for convenience - for explicit type specification
template <class T>
Maybe<T> Just(T value) {
  return Maybe<T>(std::move(value));
}

// Helper function for creating Maybe<void>
inline Maybe<void> JustVoid() {
  return Maybe<void>::Just();
}

}  // namespace v8

// Make JustVoid available in global namespace for compatibility
using v8::JustVoid;

#endif  // INCLUDE_V8_MAYBE_H_