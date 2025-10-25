#ifndef V8_SRC_BASE_LOGGING_H_
#define V8_SRC_BASE_LOGGING_H_

#ifdef __wasi__
#include <cstdlib>  // for abort()
#include <cstdio>   // for fprintf

// V8_INLINE macro
#ifndef V8_INLINE
#define V8_INLINE inline
#endif

// V8_Fatal and V8_Dcheck macros for WASI
#ifndef V8_Fatal
#define V8_Fatal(...) do { \
  fprintf(stderr, "V8 Fatal: "); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  abort(); \
} while (0)
#endif

#ifndef V8_Dcheck
#define V8_Dcheck(...) do { \
  fprintf(stderr, "V8 Dcheck: "); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
} while (0)
#endif

// Minimal logging stubs for WASI
#ifndef CHECK
#define CHECK(condition) ((void)0)
#endif

#ifndef DCHECK
#define DCHECK(condition) ((void)0)
#endif

// Provide a function-based form to avoid -Wunused-value when used as a
// statement, while still returning the value in expression contexts.
// Forwarding helpers to preserve value category and avoid copies.
#include <utility>
namespace v8 { namespace base { namespace detail {
template <typename T>
V8_INLINE T&& DcheckNotNull(T&& val) { return std::forward<T>(val); }
template <typename T>
V8_INLINE T&& CheckNotNull(T&& val) { return std::forward<T>(val); }
}}}  // namespace v8::base::detail

#ifndef DCHECK_NOT_NULL
#define DCHECK_NOT_NULL(val) (::v8::base::detail::DcheckNotNull(val))
#endif

#ifndef DCHECK_NULL
#define DCHECK_NULL(val) ((void)0)
#endif

#ifndef CHECK_NOT_NULL
#define CHECK_NOT_NULL(val) (::v8::base::detail::CheckNotNull(val))
#endif

#ifndef CHECK_NULL
#define CHECK_NULL(val) do { \
  if ((val) != nullptr) { \
    V8_Fatal("CHECK_NULL failed: %s is not null", #val); \
  } \
} while (0)
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

#ifndef GRACEFUL_FATAL
#define GRACEFUL_FATAL(...) FATAL(__VA_ARGS__)
#endif

#ifndef UNIMPLEMENTED
#define UNIMPLEMENTED() FATAL("UNIMPLEMENTED")
#endif

// Extended CHECK/DCHECK variants used throughout V8
#ifndef CHECK_WITH_MSG
#define CHECK_WITH_MSG(condition, message) CHECK(condition)
#endif

#ifndef DCHECK_WITH_MSG
#define DCHECK_WITH_MSG(condition, message) DCHECK(condition)
#endif

#ifndef DCHECK_WITH_MSG_AND_LOC
#define DCHECK_WITH_MSG_AND_LOC(condition, message, loc) DCHECK(condition)
#endif

#ifndef V8_GLIBC_PREREQ
#define V8_GLIBC_PREREQ(maj, min) 0
#endif

namespace v8 {
namespace base {

template <typename T>
inline const T& PrintCheckOperand(const T& value) { return value; }
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

#endif  // __wasi__
#endif  // V8_SRC_BASE_LOGGING_H_
