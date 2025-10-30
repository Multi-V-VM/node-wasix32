// Avoid pulling WASI std concept polyfills into this header to prevent
// accidental namespace std leakage when included under namespace scopes.
// WASI-specific platform definitions for V8
#ifndef V8_INCLUDE_WASI_V8_PLATFORM_WASI_H_
#define V8_INCLUDE_WASI_V8_PLATFORM_WASI_H_

#ifdef __wasi__

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// Constants for memory sizes
#ifndef GB
constexpr size_t WASI_GB = 1024 * 1024 * 1024;
#endif

// Debug and utility macros for WASI
#ifndef DCHECK
#define DCHECK(condition) ((void)0)
#endif
#ifndef DCHECK_GE  
#define DCHECK_GE(a, b) ((void)0)
#endif
#ifndef DCHECK_GT
#define DCHECK_GT(a, b) ((void)0)
#endif
#ifndef DCHECK_LT
#define DCHECK_LT(a, b) ((void)0)
#endif
#ifndef DCHECK_EQ
#define DCHECK_EQ(a, b) ((void)0)
#endif
#ifndef DCHECK_NE
#define DCHECK_NE(a, b) ((void)0)
#endif
#ifndef DCHECK_NOT_NULL
#define DCHECK_NOT_NULL(ptr) ((void)0)
#endif
#ifndef CHECK
#define CHECK(condition) ((void)0)
#endif
#ifndef CHECK_EQ
#define CHECK_EQ(a, b) ((void)0)
#endif
#ifndef UNREACHABLE
#define UNREACHABLE() ((void)0)
#endif

// Forward declarations  
namespace v8 {
namespace base {
  
  // Atomic types for WASI
  using Atomic32 = int32_t;
}

// V8 internal namespace constants for WASI
namespace internal {
#ifndef V8_WASI_SMI_CONSTANTS_DEFINED
  constexpr int kSmiTagSize = 1;
  constexpr int kSmiShiftSize = 0;
  constexpr int kSmiValueSize = 31;
#endif
#ifndef V8_WASI_CORE_SIZES_DEFINED
  constexpr int kSystemPointerSize = sizeof(void*);
  constexpr int kTaggedSize = sizeof(void*);
  using ExternalPointer_t = uintptr_t;
  using CppHeapPointer_t = uintptr_t;
  using IndirectPointerHandle = uint32_t;
#endif
}
}
// Forward-declare TaskPriority before use in interfaces below
namespace v8 { enum class TaskPriority; }
// Do not forward-declare or define libc++ internals in std:: on WASI.
// Creating or declaring std::* inside this header can accidentally happen
// while nested in other namespaces (e.g. namespace v8), leading to v8::std
// shadowing and widespread lookup failures in libc++ headers.

#if defined(__wasi__) && !defined(std)
// ensure std namespace resolution works properly in WASI
//   // Commented out to avoid conflicts
#endif
// Avoid redefining v8 task types here; use those from v8-task-full.h

// Avoid including libplatform tracing headers here. Including standard library
// headers from within this header can accidentally occur while a parent file
// has an open `namespace v8 {}` block, which would nest `namespace std` under
// `v8::std` and break libc++ lookups. Downstream users that need tracing should
// include `v8-platform-full.h` or libplatform headers at TU global scope.

// 枚举和类型定义
// MessageLoopBehavior, TaskPriority, and other platform types are provided
// by v8-platform-full.h; do not redeclare here.

namespace v8 {

namespace wasi {
// Empty implementation of unique_ptr for WASI
template <typename T>
class unique_ptr {
 public:
  typedef T* pointer;
  typedef T element_type;

  pointer ptr;

  unique_ptr() : ptr(nullptr) {}
  unique_ptr(T* p) : ptr(p) {}
  ~unique_ptr() {
    if (ptr) delete ptr;
  }

  T* get() const { return ptr; }
  T* release() {
    T* tmp = ptr;
    ptr = nullptr;
    return tmp;
  }
  void reset(T* p = nullptr) {
    if (ptr) delete ptr;
    ptr = p;
  }

  T& operator*() const { return *ptr; }
  T* operator->() const { return ptr; }
  explicit operator bool() const { return ptr != nullptr; }

  // Define move constructor and assignment
  unique_ptr(unique_ptr&& other) : ptr(other.release()) {}
  unique_ptr& operator=(unique_ptr&& other) {
    reset(other.release());
    return *this;
  }
};
}  // namespace wasi

namespace platform {
namespace tracing {
// Tracing types provided by libplatform headers; no WASI stubs here.
}  // namespace tracing
}  // namespace platform


}  // namespace v8

#endif  // __wasi__

#endif  // V8_INCLUDE_WASI_V8_PLATFORM_WASI_H_
