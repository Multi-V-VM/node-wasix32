#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_EXPORT_FIX_H_
#define V8_WASI_EXPORT_FIX_H_

#ifdef __wasi__

// Only define if not already defined to avoid redefinition warnings
#ifndef V8_BASE_EXPORT
#define V8_BASE_EXPORT
#endif

#ifndef V8_EXPORT
#define V8_EXPORT
#endif

#ifndef V8_EXPORT_PRIVATE
#define V8_EXPORT_PRIVATE
#endif

#ifdef V8_NOINLINE
#undef V8_NOINLINE
#endif
#define V8_NOINLINE

#ifdef V8_WARN_UNUSED_RESULT
#undef V8_WARN_UNUSED_RESULT
#endif
#define V8_WARN_UNUSED_RESULT

#ifndef NON_EXPORTED_BASE
#define NON_EXPORTED_BASE(x) \
 public                      \
  x
#endif

#endif  // __wasi__

#endif  // V8_WASI_EXPORT_FIX_H_
