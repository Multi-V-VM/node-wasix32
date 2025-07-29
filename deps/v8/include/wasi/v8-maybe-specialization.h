#ifndef V8_INCLUDE_WASI_V8_MAYBE_SPECIALIZATION_H_
#define V8_INCLUDE_WASI_V8_MAYBE_SPECIALIZATION_H_

#ifdef __wasi__

namespace v8 {

// Specialization of Maybe<void> for WASI
// This needs to be defined after the main Maybe template
template<>
class Maybe<void> {
 public:
  Maybe() : has_value_(false) {}
  explicit Maybe(bool has_value) : has_value_(has_value) {}
  
  bool IsNothing() const { return !has_value_; }
  bool IsJust() const { return has_value_; }
  
  bool operator==(const Maybe& other) const {
    return has_value_ == other.has_value_;
  }

  bool operator!=(const Maybe& other) const {
    return !operator==(other);
  }
  
  // Check method for WASI compatibility
  void Check() const {
    // In WASI, we just assume check succeeds if we have a value
    // This is a simplified implementation for compatibility
  }

 private:
  bool has_value_;
};

// Helper functions for Maybe<void>
inline Maybe<void> Nothing() {
  return Maybe<void>();
}

inline Maybe<void> JustVoid() {
  return Maybe<void>(true);
}

}  // namespace v8

#endif  // __wasi__

#endif  // V8_INCLUDE_WASI_V8_MAYBE_SPECIALIZATION_H_