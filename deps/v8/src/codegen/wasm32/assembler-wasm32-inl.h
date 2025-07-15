// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_ASSEMBLER_WASM32_INL_H_
#define V8_CODEGEN_WASM32_ASSEMBLER_WASM32_INL_H_

#include "src/codegen/assembler.h"
#include "src/codegen/wasm32/assembler-wasm32.h"
#include "src/debug/debug.h"
#include "src/objects/objects-inl.h"

namespace v8 {
namespace internal {

// Static register constants
const int kRegisterCodeByAllocationIndex[] = {
    // Allocatable registers in allocation order
    r0().code(),  r1().code(),  r2().code(),  r3().code(),
    r4().code(),  r5().code(),  r6().code(),  r7().code(),
    r8().code(),  r9().code(),  r10().code(), r11().code(),
    r12().code()
};

const char* const RegisterNames[] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
    "sp", "fp", "link"
};

const char* const FloatRegisterNames[] = {
    "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
    "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15"
};

const char* const DoubleRegisterNames[] = {
    "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
    "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15"
};

const char* const Simd128RegisterNames[] = {
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "s12", "s13", "s14", "s15"
};

// Register name accessors
const char* Register::AllocationIndexToString(int index) {
  DCHECK(index >= 0 && index < kAllocatableGeneralRegisters.Count());
  return RegisterNames[kRegisterCodeByAllocationIndex[index]];
}

const char* Register::Mnemonic(int code) {
  DCHECK(code >= 0 && code < kNumRegisters);
  return RegisterNames[code];
}

const char* FloatRegister::AllocationIndexToString(int index) {
  DCHECK(index >= 0 && index < kNumRegisters);
  return FloatRegisterNames[index];
}

const char* FloatRegister::Mnemonic(int code) {
  DCHECK(code >= 0 && code < kNumRegisters);
  return FloatRegisterNames[code];
}

const char* DoubleRegister::AllocationIndexToString(int index) {
  DCHECK(index >= 0 && index < kNumRegisters);
  return DoubleRegisterNames[index];
}

const char* DoubleRegister::Mnemonic(int code) {
  DCHECK(code >= 0 && code < kNumRegisters);
  return DoubleRegisterNames[code];
}

const char* Simd128Register::AllocationIndexToString(int index) {
  DCHECK(index >= 0 && index < kNumRegisters);
  return Simd128RegisterNames[index];
}

const char* Simd128Register::Mnemonic(int code) {
  DCHECK(code >= 0 && code < kNumRegisters);
  return Simd128RegisterNames[code];
}

// CPU features
bool CpuFeatures::SupportsOptimizer() {
  // WASM32 always supports optimization
  return true;
}

bool CpuFeatures::SupportsWasmSimd128() {
  return IsSupported(WASM32_SIMD);
}

// Operand encoding helpers
int ToNumber(Register reg) {
  DCHECK(reg.is_valid());
  return reg.code();
}

// Memory operand class for WASM32
class MemOperand {
 public:
  explicit MemOperand(Register base, int32_t offset = 0)
      : base_(base), offset_(offset) {
    DCHECK(is_int12(offset));
  }

  Register base() const { return base_; }
  int32_t offset() const { return offset_; }

 private:
  Register base_;
  int32_t offset_;
};

// RelocInfo functions
void RelocInfo::apply(intptr_t delta) {
  // WASM32 doesn't use relocation for code movement
  UNREACHABLE();
}

Address RelocInfo::target_address() {
  DCHECK(IsCodeTargetMode(rmode_) || IsNearBuiltinEntry(rmode_));
  return Assembler::target_address_from_return_address(pc_);
}

Address RelocInfo::target_address_address() {
  DCHECK(HasTargetAddressAddress());
  // We don't support this on WASM32
  UNREACHABLE();
}

Address RelocInfo::constant_pool_entry_address() {
  UNREACHABLE();
}

int RelocInfo::target_address_size() {
  return kPointerSize;
}

HeapObject RelocInfo::target_object(PtrComprCageBase cage_base) {
  DCHECK(IsCodeTarget(rmode_) || IsFullEmbeddedObject(rmode_));
  return HeapObject::cast(Object(target_address()));
}

Handle<HeapObject> RelocInfo::target_object_handle(Assembler* origin) {
  DCHECK(IsCodeTarget(rmode_) || IsFullEmbeddedObject(rmode_));
  return origin->code_target_object_handle_at(pc_);
}

void RelocInfo::set_target_object(Heap* heap, HeapObject target,
                                  WriteBarrierMode write_barrier_mode,
                                  ICacheFlushMode icache_flush_mode) {
  DCHECK(IsCodeTarget(rmode_) || IsFullEmbeddedObject(rmode_));
  Assembler::set_target_address_at(pc_, constant_pool_, target.ptr(),
                                   icache_flush_mode);
  if (!host().is_null() && !v8_flags.disable_write_barriers) {
    WriteBarrierForCode(host(), this, target, write_barrier_mode);
  }
}

Address RelocInfo::target_internal_reference() {
  DCHECK(rmode_ == INTERNAL_REFERENCE);
  return Memory<Address>(pc_);
}

Address RelocInfo::target_internal_reference_address() {
  DCHECK(rmode_ == INTERNAL_REFERENCE);
  return pc_;
}

Builtin RelocInfo::target_builtin_at(Assembler* origin) {
  DCHECK(IsNearBuiltinEntry(rmode_));
  return Builtins::FromInt(ReadUnalignedValue<int32_t>(pc_));
}

Address RelocInfo::target_off_heap_target() {
  DCHECK(IsOffHeapTarget(rmode_));
  return Assembler::target_address_from_return_address(pc_);
}

void RelocInfo::WipeOut() {
  DCHECK(IsFullEmbeddedObject(rmode_) || IsCodeTarget(rmode_) ||
         IsExternalReference(rmode_) || IsInternalReference(rmode_) ||
         IsOffHeapTarget(rmode_));
  if (IsFullEmbeddedObject(rmode_)) {
    Memory<Address>(pc_) = kNullAddress;
  } else if (IsCodeTarget(rmode_) || IsOffHeapTarget(rmode_)) {
    Assembler::set_target_address_at(pc_, constant_pool_,
                                     kNullAddress);
  } else if (IsExternalReference(rmode_)) {
    Memory<Address>(pc_) = kNullAddress;
  } else {
    DCHECK(IsInternalReference(rmode_));
    Memory<Address>(pc_) = kNullAddress;
  }
}

// Assembler static functions
Address Assembler::target_address_from_return_address(Address pc) {
  // Assuming the return address is after a call instruction
  return Memory<uint32_t>(pc - kWasm32InstrSize);
}

void Assembler::set_target_compressed_address_at(
    Address pc, Address constant_pool, Tagged_t target,
    ICacheFlushMode icache_flush_mode) {
  Assembler::set_target_address_at(pc, constant_pool,
                                   static_cast<Address>(target),
                                   icache_flush_mode);
}

Tagged_t Assembler::target_compressed_address_at(Address pc,
                                                 Address constant_pool) {
  return static_cast<Tagged_t>(target_address_at(pc, constant_pool));
}

Handle<Object> Assembler::code_target_object_handle_at(Address pc,
                                                       Address constant_pool) {
  Address address = target_address_at(pc, constant_pool);
  return Handle<Object>(reinterpret_cast<Address*>(address));
}

Handle<HeapObject> Assembler::compressed_embedded_object_handle_at(
    Address pc, Address constant_pool) {
  return Handle<HeapObject>::cast(code_target_object_handle_at(pc, constant_pool));
}

Address Assembler::target_address_at(Address pc, Address constant_pool) {
  // Read the target address from the instruction stream
  uint32_t* instr_ptr = reinterpret_cast<uint32_t*>(pc);
  uint32_t instr = *instr_ptr;
  
  // Extract immediate offset from instruction
  int32_t offset = static_cast<int32_t>((instr & Instruction::kImmMask) >> 
                                        Instruction::kImmShift);
  // Sign extend
  if (offset & 0x800) {
    offset |= 0xFFFFF000;
  }
  
  return pc + offset;
}

void Assembler::set_target_address_at(Address pc, Address constant_pool,
                                     Address target,
                                     ICacheFlushMode icache_flush_mode) {
  int32_t offset = target - pc;
  PatchBranchOffset(reinterpret_cast<uint8_t*>(pc), offset);
  
  if (icache_flush_mode != SKIP_ICACHE_FLUSH) {
    FlushInstructionCache(pc, kWasm32InstrSize);
  }
}

void Assembler::deserialization_set_special_target_at(
    Address instruction_payload, Tagged<Code> code, Address target) {
  set_target_address_at(instruction_payload,
                       !code.is_null() ? code->constant_pool() : kNullAddress,
                       target);
}

int Assembler::deserialization_special_target_size(
    Address instruction_payload) {
  return kPointerSize;
}

void Assembler::deserialization_set_target_internal_reference_at(
    Address pc, Address target, RelocInfo::Mode mode) {
  Memory<Address>(pc) = target;
}

// CPU-specific functions
void Assembler::FlushInstructionCache(Address start, size_t size) {
  // No instruction cache on WASM32 virtual ISA
}

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_ASSEMBLER_WASM32_INL_H_