#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_TRACED_HANDLE_H_
#define INCLUDE_V8_TRACED_HANDLE_H_
#ifdef __wasi__
namespace v8 {
  template<typename T> class TracedReference {
  public:
    bool IsEmptyThreadSafe() const { return true; }
  };
  class TracedReferenceBase {
  public:
    bool IsEmptyThreadSafe() const { return true; }
  };
}
#endif
#endif
