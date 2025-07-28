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
  
 private:
  T value_;
  bool has_value_;
};

}  // namespace v8

#endif  // INCLUDE_V8_MAYBE_H_