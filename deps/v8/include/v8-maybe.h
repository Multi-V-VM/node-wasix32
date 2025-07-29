#ifndef INCLUDE_V8_MAYBE_H_
#define INCLUDE_V8_MAYBE_H_

// For WASI compatibility, v8-maybe.h redirects to v8-maybe-local.h
#include "v8-maybe-local.h"

namespace v8 {

// Simple Maybe template for WASI - basic implementation
template <class T>
class Maybe {
 public:
  Maybe() : has_value_(false) {}
  explicit Maybe(const T& value) : value_(value), has_value_(true) {}
  
  bool IsNothing() const { return !has_value_; }
  bool IsJust() const { return has_value_; }
  T FromJust() const { return value_; }
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
  static Maybe<T> Just(const T& value) { return Maybe<T>(value); }
  
  // Check method for WASI compatibility - assumes success when has_value_
  void Check() const {
    // In WASI, we just assume check succeeds if we have a value
    // This is a simplified implementation for compatibility
  }
  
 private:
  T value_;
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

// Standalone Just function template for convenience  
template <class T>
Maybe<T> Just(const T& value) {
  return Maybe<T>::Just(value);
}

// Helper function for creating Maybe<void>
inline Maybe<void> JustVoid() {
  return Maybe<void>::Just();
}

}  // namespace v8

// Make JustVoid available in global namespace for compatibility
using v8::JustVoid;

#endif  // INCLUDE_V8_MAYBE_H_