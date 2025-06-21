#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_MAYBE_H_
#define INCLUDE_V8_MAYBE_H_
#ifdef __wasi__
namespace v8 {
  template<typename T> class Maybe {
    T value_;
  public:
    Maybe(T val) : value_(val) {}
    bool IsNothing() const { return false; }
    T FromJust() const { return value_; }
  };
}
#endif
#endif
