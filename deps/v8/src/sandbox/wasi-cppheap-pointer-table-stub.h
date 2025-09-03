#ifndef V8_SANDBOX_WASI_CPPHEAP_POINTER_TABLE_STUB_H_
#define V8_SANDBOX_WASI_CPPHEAP_POINTER_TABLE_STUB_H_

#ifdef __wasi__

#include <cstddef>
#include <cstdint>

namespace v8 {
namespace internal {

#ifndef V8_CPPHEAP_POINTER_HANDLE_TYPE_DEFINED
#define V8_CPPHEAP_POINTER_HANDLE_TYPE_DEFINED
using CppHeapPointerHandle = uint32_t;
#endif

#ifndef V8_NULL_CPPHEAP_POINTER_HANDLE_DEFINED
#define V8_NULL_CPPHEAP_POINTER_HANDLE_DEFINED
constexpr CppHeapPointerHandle kNullCppHeapPointerHandle = 0;
#endif

#ifndef V8_CPPHEAP_POINTER_TABLE_SIZE
#define V8_CPPHEAP_POINTER_TABLE_SIZE
constexpr size_t kCppHeapPointerTableReservationSize = 1024 * 1024;  // 1MB
#endif

#ifndef V8_MAX_CPPHEAP_POINTERS
#define V8_MAX_CPPHEAP_POINTERS
constexpr size_t kMaxCppHeapPointers = 65536;
#endif

#ifndef V8_CPPHEAP_POINTER_SHIFTS_DEFINED
#define V8_CPPHEAP_POINTER_SHIFTS_DEFINED
constexpr int kCppHeapPointerIndexShift = 0;
constexpr int kCppHeapPointerPayloadShift = 1;
constexpr int kCppHeapPointerTagShift = 1;
#endif

#ifndef V8_CPPHEAP_POINTER_MARKBIT_DEFINED
#define V8_CPPHEAP_POINTER_MARKBIT_DEFINED
constexpr uintptr_t kCppHeapPointerMarkBit = static_cast<uintptr_t>(0);
#endif

}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // V8_SANDBOX_WASI_CPPHEAP_POINTER_TABLE_STUB_H_

