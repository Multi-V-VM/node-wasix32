#ifndef SRC_NODE_WASI_FIXES_H_
#define SRC_NODE_WASI_FIXES_H_

#ifdef __wasi__

// Include all WASI compatibility fixes
#include "wasi-isolate-extensions.h"
#include "wasi-v8-extensions.h"
#include "wasi-v8-api-fixes.h"

// Additional fixes for common issues

// Fix for Local<T>::FromRepr
namespace v8 {
template<typename T>
class Local {
public:
  static Local<T> FromRepr(internal::Address repr) {
    return Local<T>(repr);
  }
};
}

#endif // __wasi__

#endif // SRC_NODE_WASI_FIXES_H_
