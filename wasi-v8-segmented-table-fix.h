#ifndef WASI_V8_SEGMENTED_TABLE_FIX_H_
#define WASI_V8_SEGMENTED_TABLE_FIX_H_

// Fix for missing SegmentedTable types

namespace v8 {
namespace internal {

// Forward declare SegmentedTable
template<typename Entry, size_t kMaxCapacity>
class SegmentedTable;

namespace wasm {
struct WasmCodePointerTableEntry;
}

// Specialize SegmentedTable for WasmCodePointerTableEntry
template<>
class SegmentedTable<wasm::WasmCodePointerTableEntry, 268435456> {
 public:
  // Define the missing FreelistHead type
  struct FreelistHead {
    uint32_t next;
    uint32_t size;
    
    FreelistHead() : next(0), size(0) {}
    FreelistHead(uint32_t n, uint32_t s) : next(n), size(s) {}
  };
  
  using Entry = wasm::WasmCodePointerTableEntry;
  static constexpr size_t kMaxCapacity = 268435456;
  
  // Disable sandbox requirement for WASI
  static constexpr bool kUseContiguousMemory = true;
};

} // namespace internal
} // namespace v8

// Fix V8_ENABLE_SANDBOX_BOOL for WASI
#ifndef V8_ENABLE_SANDBOX_BOOL
#define V8_ENABLE_SANDBOX_BOOL false
#endif

#endif // WASI_V8_SEGMENTED_TABLE_FIX_H_