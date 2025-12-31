// Copyright 2024 the V8 project authors. All rights reserved.
// Minimal WASM32 assembler stub - provides symbols for linking but
// actual code generation is not supported since WASM32 is an interpreter target.

#if V8_TARGET_ARCH_WASM32

#include "src/codegen/wasm32/assembler-wasm32.h"

#include "src/base/cpu.h"
#include "src/codegen/assembler-inl.h"
#include "src/codegen/wasm32/assembler-wasm32-inl.h"
#include "src/execution/isolate.h"

namespace v8 {
namespace internal {

// Assembler implementation - minimal stubs
Assembler::Assembler(const AssemblerOptions& options,
                     std::unique_ptr<AssemblerBuffer> buffer)
    : AssemblerBase(options, std::move(buffer)) {
  buffer_start_ = buffer_ ? reinterpret_cast<uint8_t*>(buffer_->start()) : nullptr;
  pc_ = buffer_start_;
  buffer_end_ = buffer_ ? buffer_start_ + buffer_->size() : nullptr;
}

Assembler::~Assembler() = default;

void Assembler::GetCode(LocalIsolate* isolate, CodeDesc* desc,
                        SafepointTableBuilder* safepoint_table_builder,
                        int handler_table_offset) {
  desc->buffer = buffer_ ? buffer_->start() : nullptr;
  desc->buffer_size = buffer_ ? buffer_->size() : 0;
  desc->instr_size = pc_offset();
  desc->reloc_size = 0;
  desc->constant_pool_offset = 0;
  desc->constant_pool_size = 0;
  desc->origin = this;
  desc->unwinding_info_size = 0;
  desc->unwinding_info = nullptr;
}

void Assembler::GetCode(Isolate* isolate, CodeDesc* desc) {
  GetCode(isolate ? isolate->main_thread_local_isolate() : nullptr, desc);
}

void Assembler::bind(Label* L) {
  if (L) L->bind_to(pc_offset());
}

void Assembler::nop() {
  // Emit a placeholder byte
  if (buffer_start_ && pc_) {
    CheckBufferSpace();
    *pc_++ = 0;
  }
}

void Assembler::GrowBuffer() {
  if (!buffer_) return;

  int old_size = buffer_->size();
  int new_size = old_size + (old_size >> 1);
  if (new_size < old_size + 256) new_size = old_size + 256;

  std::unique_ptr<AssemblerBuffer> new_buffer = buffer_->Grow(new_size);

  uint8_t* new_start = reinterpret_cast<uint8_t*>(new_buffer->start());
  int offset = static_cast<int>(pc_ - buffer_start_);
  buffer_start_ = new_start;
  pc_ = new_start + offset;
  buffer_end_ = new_start + new_size;

  buffer_ = std::move(new_buffer);
}

void Assembler::PatchBranchOffset(uint8_t* pc, int32_t offset) {
  // Stub - no actual patching needed for WASM32
}

void Assembler::RecordComment(const char* comment) {
  // Stub - comments are ignored for WASM32
}

int32_t Assembler::branch_offset(Label* L) {
  return L ? L->pos() - pc_offset() : 0;
}

int32_t Assembler::jump_offset(Label* L) {
  return L ? L->pos() - pc_offset() : 0;
}

// Static methods required by RelocInfo
Address Assembler::target_address_at(Address pc, Address constant_pool) {
  // Read the target address from the instruction stream
  return *reinterpret_cast<Address*>(pc);
}

void Assembler::set_target_address_at(Address pc, Address constant_pool,
                                      Address target,
                                      WritableJitAllocation* jit_allocation,
                                      ICacheFlushMode icache_flush_mode) {
  // Write the target address to the instruction stream
  *reinterpret_cast<Address*>(pc) = target;
}

uint32_t Assembler::uint32_constant_at(Address pc, Address constant_pool) {
  return *reinterpret_cast<uint32_t*>(pc);
}

void Assembler::set_uint32_constant_at(Address pc, Address constant_pool,
                                       uint32_t new_constant,
                                       WritableJitAllocation* jit_allocation,
                                       ICacheFlushMode icache_flush_mode) {
  *reinterpret_cast<uint32_t*>(pc) = new_constant;
}

void Assembler::deserialization_set_target_internal_reference_at(
    Address pc, Address target, WritableJitAllocation& jit_allocation,
    RelocInfo::Mode mode) {
  jit_allocation.WriteUnalignedValue<Address>(pc, target);
}

void Assembler::RecordRelocInfo(RelocInfo::Mode rmode, intptr_t data) {
  // Stub - no relocation info for WASM32
}

bool RelocInfo::IsInConstantPool() { return false; }

uint32_t RelocInfo::wasm_call_tag() const {
  DCHECK(rmode_ == WASM_CALL || rmode_ == WASM_STUB_CALL);
  return 0;
}

// kApplyMask - modes affected by apply. For WASM32 interpreter target, no patching needed.
const int RelocInfo::kApplyMask = 0;

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
