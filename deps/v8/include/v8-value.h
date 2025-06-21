#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_VALUE_H_
#define INCLUDE_V8_VALUE_H_
#ifdef __wasi__
#include "v8-local-handle.h"
namespace v8 {
  class Value {};
}
#endif
#endif
