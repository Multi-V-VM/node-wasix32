#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_LIBPLATFORM_LIBPLATFORM_EXPORT_H_
#define V8_LIBPLATFORM_LIBPLATFORM_EXPORT_H_

#include "v8config.h"

#ifdef __wasi__
#define V8_PLATFORM_EXPORT V8_EXPORT
#else
#define V8_PLATFORM_EXPORT V8_EXPORT
#endif

#endif  // V8_LIBPLATFORM_LIBPLATFORM_EXPORT_H_
