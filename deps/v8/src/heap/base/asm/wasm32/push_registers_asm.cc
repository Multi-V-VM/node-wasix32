// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Push all callee-saved registers to get them on the stack for conservative
// stack scanning.
//
// WebAssembly (WASM32) implementation for stack scanning.
// This implementation follows the virtual ISA model where WASM doesn't have
// direct register access, so we provide a C++ implementation that captures
// the current execution context.

#include <cstdint>
#include <cstring>

// WebAssembly doesn't have inline assembly or direct register access.
// This implementation uses C++ to provide equivalent functionality.
extern "C" void PushAllRegistersAndIterateStack(
    void* stack,
    void* stack_visitor,
    void (*callback)(void*, void*, void*)) {
  // In WebAssembly, we don't have direct access to registers.
  // The runtime manages the virtual machine state internally.
  // We capture the current stack pointer and pass it to the callback.
  
  // Get current stack pointer - this is platform-specific
  // For WASM, we approximate by taking the address of a local variable
  volatile uintptr_t stack_marker = 0;
  void* current_sp = const_cast<void*>(reinterpret_cast<const void*>(&stack_marker));
  
  // In a real WASM implementation, the runtime would provide access to:
  // - Linear memory base and size
  // - Current stack pointer within linear memory
  // - Function table references
  // - Global variables
  
  // For conservative stack scanning in WASM, we need to scan:
  // 1. The shadow stack (if enabled)
  // 2. The linear memory stack region
  // 3. Any host-managed state
  
  // Call the callback with the stack information
  // The callback will perform the actual stack scanning
  callback(stack, stack_visitor, current_sp);
  
  // Note: In production WASM engines like V8's Liftoff or TurboFan,
  // stack scanning would be integrated with the engine's internal
  // representation of the WASM stack and local variables.
}

// Alternative implementation using WASM intrinsics if available
#ifdef __wasm__
extern "C" void PushAllRegistersAndIterateStack_WASM(
    void* stack,
    void* stack_visitor,
    void (*callback)(void*, void*, void*)) __attribute__((export_name("PushAllRegistersAndIterateStack")));

extern "C" void PushAllRegistersAndIterateStack_WASM(
    void* stack,
    void* stack_visitor,
    void (*callback)(void*, void*, void*)) {
  // In pure WASM, we would use:
  // - __builtin_wasm_memory_size(0) for memory size
  // - __builtin_frame_address(0) for frame pointer equivalent
  // - Local variables are already on the WASM stack
  
  void* frame_address = __builtin_frame_address(0);
  callback(stack, stack_visitor, frame_address);
}
#endif