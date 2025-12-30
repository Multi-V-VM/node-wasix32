#if defined(__wasi__) || defined(V8_USING_WASI_SHIMS)
// WASI-specific logging implementation
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

namespace v8 {
namespace base {

// Note: CheckMessageStream is now defined in logging.h, don't redefine here
// class CheckMessageStream { ... };

// Simple template for operator<< support
// Removed to avoid ambiguity - use the member function instead

// WASI stub for stack trace printing
using StackTracePrinter = void (*)(void);

inline void SetPrintStackTrace(StackTracePrinter printer) {
  // WASI stub - no-op
  (void)printer;
}

}  // namespace base
}  // namespace v8

// Note: V8_Fatal and V8_Dcheck are defined as proper functions in logging.h
// Do NOT define them as macros here - it would break the function declarations

#ifndef CHECK_NULL
#define CHECK_NULL(val) do { \
  if ((val) == nullptr) { \
    V8_Fatal("CHECK_NULL failed: %s is null", #val); \
  } \
} while (0)
#endif

#ifndef DCHECK_NOT_NULL
#define DCHECK_NOT_NULL(val) (val)
#endif

#ifndef DCHECK_BOUNDS
// In release builds, DCHECK_BOUNDS is a no-op that just evaluates the arguments
#define DCHECK_BOUNDS(index, limit) ((void)0)
#endif

#ifndef DCHECK_EQ
// In release builds, DCHECK_EQ is a no-op that just evaluates the arguments
#define DCHECK_EQ(a, b) ((void)0)
#endif

#ifndef DCHECK_NE
// In release builds, DCHECK_NE is a no-op that just evaluates the arguments
#define DCHECK_NE(a, b) ((void)0)
#endif

#ifndef DCHECK_LT
// In release builds, DCHECK_LT is a no-op that just evaluates the arguments
#define DCHECK_LT(a, b) ((void)0)
#endif

#ifndef DCHECK_LE
// In release builds, DCHECK_LE is a no-op that just evaluates the arguments
#define DCHECK_LE(a, b) ((void)0)
#endif

#ifndef DCHECK_GT
// In release builds, DCHECK_GT is a no-op that just evaluates the arguments
#define DCHECK_GT(a, b) ((void)0)
#endif

#ifndef DCHECK_GE
// In release builds, DCHECK_GE is a no-op that just evaluates the arguments
#define DCHECK_GE(a, b) ((void)0)
#endif

#ifndef DCHECK
// Generic DCHECK - no-op in release builds
#define DCHECK(condition) ((void)0)
#endif

#ifndef DCHECK_IMPLIES
// DCHECK_IMPLIES - no-op in release builds
#define DCHECK_IMPLIES(a, b) ((void)0)
#endif

#ifndef DCHECK_WITH_MSG_AND_LOC
// DCHECK_WITH_MSG_AND_LOC - no-op in release builds
// Used for type cast checks with source location tracking
#define DCHECK_WITH_MSG_AND_LOC(condition, message, loc) ((void)0)
#endif

#ifndef DCHECK_WITH_MSG
// DCHECK_WITH_MSG - no-op in release builds
#define DCHECK_WITH_MSG(condition, message) ((void)0)
#endif


#endif  // defined(__wasi__) || defined(V8_USING_WASI_SHIMS)
