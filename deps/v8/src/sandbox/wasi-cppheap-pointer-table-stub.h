#ifndef V8_SANDBOX_WASI_CPPHEAP_POINTER_TABLE_STUB_H_
#define V8_SANDBOX_WASI_CPPHEAP_POINTER_TABLE_STUB_H_

#ifdef __wasi__

#include <atomic>
#include <cstdint>
#include "include/v8-sandbox.h"

namespace v8 {
namespace internal {

// WASI stub for CppHeapPointerTableEntry that ensures 8-byte size
struct CppHeapPointerTableEntry {
  // Stub methods for WASI compatibility
  inline void MakePointerEntry(Address value, CppHeapPointerTag tag, bool mark_as_alive) {
    // WASI stub - no-op
  }

  inline Address GetPointer(CppHeapPointerTagRange tag_range) const {
    return 0;  // WASI stub
  }

  inline void SetPointer(Address value, CppHeapPointerTag tag) {
    // WASI stub - no-op
  }

  inline bool HasPointer(CppHeapPointerTagRange tag_range) const {
    return false;  // WASI stub
  }

  inline void MakeZappedEntry() {
    // WASI stub - no-op
  }

  inline void MakeFreelistEntry(uint32_t next_entry_index) {
    // WASI stub - no-op
  }

  inline uint32_t GetNextFreelistEntryIndex() const {
    return 0;  // WASI stub
  }

  inline void MakeEvacuationEntry(Address handle_location) {
    // WASI stub - no-op
  }

  inline bool HasEvacuationEntry() const {
    return false;  // WASI stub
  }

  inline void Evacuate(CppHeapPointerTableEntry& dest) {
    // WASI stub - no-op
  }

  inline void Mark() {
    // WASI stub - no-op
  }

  static constexpr bool IsWriteProtected = false;

private:
  // Ensure the entry is exactly 8 bytes as expected by V8
  // Use uint64_t to guarantee 8-byte size regardless of platform
  std::atomic<uint64_t> data_;
};

// Verify the size assertion will pass
static_assert(sizeof(CppHeapPointerTableEntry) == 8, 
              "CppHeapPointerTableEntry must be exactly 8 bytes");

} // namespace internal
} // namespace v8

#endif // __wasi__

#endif // V8_SANDBOX_WASI_CPPHEAP_POINTER_TABLE_STUB_H_