#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef INCLUDE_V8_DATA_H_
#define INCLUDE_V8_DATA_H_

#ifdef __wasi__
#include "v8-local-handle.h"

namespace v8 {
class Data {
public:
  Local<Data> Get(Local<Context> context, int i) const { return Local<Data>(); }
};
} // namespace v8

#else
// Include real v8-data.h for non-WASI
#endif

#endif // INCLUDE_V8_DATA_H_
