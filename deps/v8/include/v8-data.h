#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_DATA_H_
#define INCLUDE_V8_DATA_H_

#include "v8config.h"

namespace v8 {

// Forward declaration for Local template
template <typename T> class Local;

/**
 * The superclass of objects that can be embedded in JavaScript values
 * or passed to most functions defined in V8.
 */
class V8_EXPORT Data {
 public:
  // Add base class methods if needed
 private:
  Data() = delete;
};

} // namespace v8

#endif // INCLUDE_V8_DATA_H_
