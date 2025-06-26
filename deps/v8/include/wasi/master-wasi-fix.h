#ifndef MASTER_WASI_FIX_H
#define MASTER_WASI_FIX_H

#ifdef __wasi__

// Include standard headers first
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

// Disable all the problematic features
#define _LIBCPP_HAS_NO_FILESYSTEM_LIBRARY 1
#define _LIBCPP_HAS_NO_INCOMPLETE_CHRONO 1
#undef _LIBCPP_HAS_FILE_TIME

// Define missing debug macros
#define DCHECK_NOT_NULL(ptr) ((void)0)
#define DCHECK(condition) ((void)0)
#define DCHECK_EQ(a, b) ((void)0)
#define DCHECK_NE(a, b) ((void)0)
#define DCHECK_LT(a, b) ((void)0)
#define DCHECK_LE(a, b) ((void)0)
#define DCHECK_GT(a, b) ((void)0)
#define DCHECK_GE(a, b) ((void)0)
#define CHECK_NOT_NULL(ptr) (ptr)
#define CHECK_EQ(a, b) ((void)0)
#define CHECK_NE(a, b) ((void)0)
#define CHECK_LT(a, b) ((void)0)
#define CHECK_LE(a, b) ((void)0)
#define CHECK_GT(a, b) ((void)0)
#define CHECK_GE(a, b) ((void)0)
#define FATAL(...) do { fprintf(stderr, __VA_ARGS__); abort(); } while(0)
#define IMMEDIATE_CRASH() abort()

// Define missing bit operations
namespace v8 { namespace base { namespace bits {
inline uint32_t RotateRight32(uint32_t value, uint8_t shift) {
  return (value >> shift) | (value << (32 - shift));
}
inline bool IsPowerOfTwo(uint32_t value) {
  return value && !(value & (value - 1));
}
}}}

// Define missing V8 internal symbols
namespace v8 { namespace internal {
constexpr int kSmiTagSize = 1;
constexpr int kSmiShiftSize = 0;
constexpr int kSmiValueSize = 31;
constexpr int kSystemPointerSize = sizeof(void*);
constexpr int kTaggedSize = sizeof(void*);
using ExternalPointer_t = uintptr_t;
using CppHeapPointer_t = uintptr_t;
using IndirectPointerHandle = uint32_t;
}}

// Define missing platform functions
namespace v8 { namespace base {
inline bool MainThreadIsCurrentThread() { return true; }
inline int64_t ClockNow(int clock_id) { return 0; }
}}

// Stub missing template functions
namespace v8 { namespace base {
template<typename T> std::string PrintCheckOperand(T) { return "value"; }
}}

// Define essential macros
#define V8_EXPORT
#define V8_PLATFORM_EXPORT
#define NON_EXPORTED_BASE(x) x
#define DISABLE_CFI_ICALL

#endif // __wasi__
#endif // MASTER_WASI_FIX_H
