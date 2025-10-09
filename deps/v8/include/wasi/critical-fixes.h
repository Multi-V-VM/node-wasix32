#ifndef WASI_CRITICAL_FIXES_H
#define WASI_CRITICAL_FIXES_H

#ifdef __wasi__
// Close any open v8 namespace before including standard library headers
}  // close any v8 namespace if open
#endif

#include <cstddef>
#include <cstdint>

// Critical V8 internal symbols that are missing in WASI builds
namespace v8 {
namespace internal {

// Smi (Small Integer) constants
#ifndef V8_WASI_SMI_CONSTANTS_DEFINED
constexpr int kSmiTagSize = 1;
constexpr int kSmiShiftSize = 0;
constexpr int kSmiValueSize = 31;
constexpr int kSystemPointerSize = sizeof(void*);
constexpr int kTaggedSize = sizeof(void*);
#define V8_WASI_SMI_CONSTANTS_DEFINED 1
#endif

// Pointer types for WASI
#ifndef V8_WASI_POINTER_TYPEDEFS_DEFINED
using ExternalPointer_t = uintptr_t;
using CppHeapPointer_t = uintptr_t;
using IndirectPointerHandle = uint32_t;
#define V8_WASI_POINTER_TYPEDEFS_DEFINED 1
#endif

// Debug macros
#define DCHECK_NOT_NULL(ptr) ((void)0)
#define DCHECK(condition) ((void)0)
#define CHECK(condition) ((void)0)

}  // namespace internal
}  // namespace v8

#ifdef __wasi__
// Re-open the v8 namespace if it was closed at the beginning
namespace v8 {
#endif

#endif // WASI_CRITICAL_FIXES_H
