// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_ASSEMBLER_WASM32_H_
#define V8_CODEGEN_WASM32_ASSEMBLER_WASM32_H_

#include <memory>

#include "src/codegen/assembler.h"
#include "src/codegen/wasm32/constants-wasm32.h"
#include "src/codegen/wasm32/register-wasm32.h"

namespace v8 {
namespace internal {

class SafepointTableBuilder;

// Minimal WASM32 Assembler stub - provides linker symbols but
// actual code generation is not supported.
class V8_EXPORT_PRIVATE Assembler : public AssemblerBase {
 public:
  explicit Assembler(const AssemblerOptions& options,
                     std::unique_ptr<AssemblerBuffer> buffer = {});
  // For compatibility with assemblers that require a zone.
  Assembler(const MaybeAssemblerZone&, const AssemblerOptions& options,
            std::unique_ptr<AssemblerBuffer> buffer = {})
      : Assembler(options, std::move(buffer)) {}
  ~Assembler() override;

  // GetCode emits any pending code and fills the descriptor
  static constexpr int kNoHandlerTable = 0;
  static constexpr SafepointTableBuilder* kNoSafepointTable = nullptr;
  void GetCode(LocalIsolate* isolate, CodeDesc* desc,
               SafepointTableBuilder* safepoint_table_builder = kNoSafepointTable,
               int handler_table_offset = kNoHandlerTable);
  void GetCode(Isolate* isolate, CodeDesc* desc);

  // Required by codegen infrastructure
  void MaybeEmitOutOfLineConstantPool() {}
  void bind(Label* L);
  void nop();
  void GrowBuffer();

  // Position info
  int pc_offset() const { return static_cast<int>(pc_ - buffer_start_); }
  uint8_t* pc() const { return pc_; }

  // Code patching
  static void PatchBranchOffset(uint8_t* pc, int32_t offset);
  void RecordComment(const char* comment);
  void RecordDeoptReason(DeoptimizeReason reason, uint32_t node_id,
                         SourcePosition position, int id);

  // Static methods required by RelocInfo
  static Address target_address_at(Address pc, Address constant_pool);
  static void set_target_address_at(Address pc, Address constant_pool,
                                    Address target,
                                    WritableJitAllocation* jit_allocation = nullptr,
                                    ICacheFlushMode icache_flush_mode = FLUSH_ICACHE_IF_NEEDED);
  static uint32_t uint32_constant_at(Address pc, Address constant_pool);
  static void set_uint32_constant_at(
      Address pc, Address constant_pool, uint32_t new_constant,
      WritableJitAllocation* jit_allocation = nullptr,
      ICacheFlushMode icache_flush_mode = FLUSH_ICACHE_IF_NEEDED);
  static void deserialization_set_target_internal_reference_at(
      Address pc, Address target, WritableJitAllocation& jit_allocation,
      RelocInfo::Mode mode = RelocInfo::INTERNAL_REFERENCE);

  // Code comments
  int WriteCodeComments();
  void RecordRelocInfo(RelocInfo::Mode rmode, intptr_t data = 0);

  // Constants
  static constexpr int kGap = 32;

  // Buffer checks
  void CheckBufferSpace() {
    if (pc_ + kGap >= buffer_end_) GrowBuffer();
  }
  void CheckSpace(int space_needed) {
    if (pc_ + space_needed >= buffer_end_) GrowBuffer();
  }

  // Alignment
  void DataAlign(int m) {
    while ((pc_offset() & (m - 1)) != 0) nop();
  }
  void Align(int m) {
    while ((pc_offset() & (m - 1)) != 0) nop();
  }

  // Data emission
  void db(uint8_t data) {
    CheckSpace(1);
    if (pc_) *pc_++ = data;
  }
  void dd(uint32_t data) {
    CheckSpace(4);
    if (pc_) {
      *reinterpret_cast<uint32_t*>(pc_) = data;
      pc_ += 4;
    }
  }
  void dq(uint64_t data) {
    CheckSpace(8);
    if (pc_) {
      *reinterpret_cast<uint64_t*>(pc_) = data;
      pc_ += 8;
    }
  }
  void dp(uintptr_t data) { dd(static_cast<uint32_t>(data)); }

  // Unused on this architecture
  bool use_real_constant_pool() const { return false; }

 protected:
  int32_t branch_offset(Label* L);
  int32_t jump_offset(Label* L);

 private:
  uint8_t* buffer_start_ = nullptr;
  uint8_t* buffer_end_ = nullptr;
  uint8_t* pc_ = nullptr;
};

class V8_EXPORT_PRIVATE EnsureSpace {
 public:
  explicit EnsureSpace(Assembler* assembler) : assembler_(assembler) {
    assembler_->CheckBufferSpace();
  }
 private:
  Assembler* const assembler_;
};

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_ASSEMBLER_WASM32_H_
