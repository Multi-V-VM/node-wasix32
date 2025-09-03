#ifndef WASI_CPPHEAP_POINTER_TABLE_STUB_H_
#define WASI_CPPHEAP_POINTER_TABLE_STUB_H_

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

#ifdef __wasi__
// Minimal stub entry type to satisfy template instantiations on WASI builds.
struct CppHeapPointerTableEntry {
  inline void MakePointerEntry(Address /*value*/, CppHeapPointerTag /*tag*/, bool /*mark*/) {}
  inline Address GetPointer(CppHeapPointerTagRange /*tag_range*/) const { return 0; }
  inline void SetPointer(Address /*value*/, CppHeapPointerTag /*tag*/) {}
  inline bool HasPointer(CppHeapPointerTagRange /*tag_range*/) const { return false; }
  inline void MakeZappedEntry() {}
  inline void MakeFreelistEntry(uint32_t /*next_entry_index*/) {}
  inline uint32_t GetNextFreelistEntryIndex() const { return 0; }
  inline void MakeEvacuationEntry(Address /*handle_location*/) {}
  inline bool HasEvacuationEntry() const { return false; }
  inline void Evacuate(CppHeapPointerTableEntry& /*dest*/) {}
  inline void Mark() {}
  static constexpr bool IsWriteProtected = false;

 private:
  uint64_t payload_ = 0;  // keep size consistent with non-WASI builds
};

static_assert(sizeof(CppHeapPointerTableEntry) == 8, "WASI stub size must be 8");
#endif  // __wasi__

}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // WASI_CPPHEAP_POINTER_TABLE_STUB_H_
