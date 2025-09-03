#ifndef V8_WASI_NAMESPACE_FIX_H_
#define V8_WASI_NAMESPACE_FIX_H_

#ifdef __wasi__

// Don't create namespace aliases here since types might not be defined yet
// Include nuclear-fix early to provide core WASI stubs/macros before other headers
#include "nuclear-fix.h"
// Instead, let the code that needs v8::internal::v8 handle the missing namespace

// Define deprecation marker early so headers that use it parse correctly.
#ifndef V8_DEPRECATE_SOON
#define V8_DEPRECATE_SOON(message)
#endif

#endif // __wasi__

#endif // V8_WASI_NAMESPACE_FIX_H_
