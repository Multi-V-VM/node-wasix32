#ifndef V8_SRC_BASE_LOGGING_H_
#define V8_SRC_BASE_LOGGING_H_

#include <cstdlib>  // for abort()

// Minimal logging stubs for WASI
#ifndef CHECK
#define CHECK(condition) ((void)0)
#endif

#ifndef DCHECK
#define DCHECK(condition) ((void)0)
#endif

#ifndef DCHECK_NOT_NULL
#define DCHECK_NOT_NULL(val) (val)
#endif

#ifndef DCHECK_NULL
#define DCHECK_NULL(val) ((void)0)
#endif

#ifndef CHECK_NOT_NULL
#define CHECK_NOT_NULL(val) (val)
#endif

#ifndef CHECK_EQ
#define CHECK_EQ(a, b) ((void)0)
#endif

#ifndef CHECK_NE
#define CHECK_NE(a, b) ((void)0)
#endif

#ifndef CHECK_GT
#define CHECK_GT(a, b) ((void)0)
#endif

#ifndef CHECK_GE
#define CHECK_GE(a, b) ((void)0)
#endif

#ifndef CHECK_LT
#define CHECK_LT(a, b) ((void)0)
#endif

#ifndef CHECK_LE
#define CHECK_LE(a, b) ((void)0)
#endif

#ifndef DCHECK_EQ
#define DCHECK_EQ(a, b) ((void)0)
#endif

#ifndef DCHECK_NE
#define DCHECK_NE(a, b) ((void)0)
#endif

#ifndef DCHECK_GT
#define DCHECK_GT(a, b) ((void)0)
#endif

#ifndef DCHECK_GE
#define DCHECK_GE(a, b) ((void)0)
#endif

#ifndef DCHECK_LT
#define DCHECK_LT(a, b) ((void)0)
#endif

#ifndef DCHECK_LE
#define DCHECK_LE(a, b) ((void)0)
#endif

#ifndef DCHECK_IMPLIES
#define DCHECK_IMPLIES(a, b) ((void)0)
#endif

#ifndef UNREACHABLE
#define UNREACHABLE() __builtin_unreachable()
#endif

#ifndef FATAL
#define FATAL(...) abort()
#endif

#ifndef UNIMPLEMENTED
#define UNIMPLEMENTED() FATAL("UNIMPLEMENTED")
#endif

#ifndef V8_GLIBC_PREREQ
#define V8_GLIBC_PREREQ(maj, min) 0
#endif

namespace v8 {
namespace base {
constexpr const char* kUnreachableCodeMessage = "Unreachable code";

// Out-of-memory error types
enum class OOMType {
  kProcess,      // General process out of memory
  kJavaScript    // JavaScript heap out of memory
};

// Fatal out-of-memory handler
[[noreturn]] inline void FatalOOM(OOMType type, const char* location) {
  // In WASI, we simply abort on OOM
  abort();
}

}  // namespace base
}  // namespace v8

#endif  // V8_SRC_BASE_LOGGING_H_
