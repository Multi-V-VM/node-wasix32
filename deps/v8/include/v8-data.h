#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_DATA_H_
#define INCLUDE_V8_DATA_H_

#include "v8config.h"

#ifdef __wasi__
// Close any open v8 namespace before we declare our own
}  // close any v8 namespace if open
#endif

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
#ifdef __wasi__
  // WASI: Add missing IsValue method
  bool IsValue() const {
    // In the V8 hierarchy, Value inherits from Data
    // For WASI stub, we assume all Data objects are Values
    return true;
  }
#endif
 private:
  Data() = delete;
};

} // namespace v8

#ifdef __wasi__
// Re-open the v8 namespace if it was closed at the beginning
namespace v8 {
#endif

#endif // INCLUDE_V8_DATA_H_
