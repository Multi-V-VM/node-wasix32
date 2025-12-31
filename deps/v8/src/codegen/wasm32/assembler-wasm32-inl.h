// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Minimal WASM32 assembler inline stubs - provides symbols for linking but
// actual code generation is not supported since WASM32 is an interpreter target.

#ifndef V8_CODEGEN_WASM32_ASSEMBLER_WASM32_INL_H_
#define V8_CODEGEN_WASM32_ASSEMBLER_WASM32_INL_H_

#include "src/codegen/assembler.h"
#include "src/codegen/wasm32/assembler-wasm32.h"
#include "src/codegen/reloc-info.h"
#include "src/objects/objects-inl.h"

namespace v8 {
namespace internal {

// CPU features - minimal stubs
inline bool CpuFeatures::SupportsOptimizer() { return false; }
inline bool CpuFeatures::SupportsWasmSimd128() { return false; }

// RelocInfo stubs for WASM32
void WritableRelocInfo::apply(intptr_t delta) {
  // No-op for WASM32
}

Address RelocInfo::target_address() {
  return Assembler::target_address_at(pc_, constant_pool_);
}

Address RelocInfo::target_address_address() {
  return pc_;
}

Address RelocInfo::constant_pool_entry_address() {
  UNREACHABLE();
}

int RelocInfo::target_address_size() {
  return kSystemPointerSize;
}

Tagged<HeapObject> RelocInfo::target_object(PtrComprCageBase cage_base) {
  return Cast<HeapObject>(Tagged<Object>(Assembler::target_address_at(pc_, constant_pool_)));
}

DirectHandle<HeapObject> RelocInfo::target_object_handle(Assembler* origin) {
  return DirectHandle<HeapObject>::FromSlot(reinterpret_cast<Address*>(
      Assembler::target_address_at(pc_, constant_pool_)));
}

JSDispatchHandle RelocInfo::js_dispatch_handle() {
  return JSDispatchHandle();
}

void WritableRelocInfo::set_target_object(Tagged<HeapObject> target,
                                          ICacheFlushMode icache_flush_mode) {
  Assembler::set_target_address_at(pc_, constant_pool_, target.ptr(), &jit_allocation_);
}

Address RelocInfo::target_external_reference() {
  return Assembler::target_address_at(pc_, constant_pool_);
}

void WritableRelocInfo::set_target_external_reference(
    Address target, ICacheFlushMode icache_flush_mode) {
  Assembler::set_target_address_at(pc_, constant_pool_, target, &jit_allocation_);
}

Address RelocInfo::target_internal_reference() {
  return Assembler::target_address_at(pc_, constant_pool_);
}

Address RelocInfo::target_internal_reference_address() {
  return pc_;
}

Builtin RelocInfo::target_builtin_at(Assembler* origin) {
  UNREACHABLE();
}

Address RelocInfo::target_off_heap_target() {
  return Assembler::target_address_at(pc_, constant_pool_);
}

WasmCodePointer RelocInfo::wasm_code_pointer_table_entry() const {
  // WASM32 stub - return empty value
  return WasmCodePointer{0};
}

void WritableRelocInfo::set_wasm_code_pointer_table_entry(
    WasmCodePointer target, ICacheFlushMode icache_flush_mode) {
  // No-op for WASM32
}

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_ASSEMBLER_WASM32_INL_H_
