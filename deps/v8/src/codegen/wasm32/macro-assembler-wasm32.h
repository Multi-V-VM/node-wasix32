// Copyright 2024 the V8 project authors. All rights reserved.
// Minimal WASM32 macro assembler stub - provides symbols for linking but
// actual code generation is not supported since WASM32 is an interpreter target.

#ifndef V8_CODEGEN_WASM32_MACRO_ASSEMBLER_WASM32_H_
#define V8_CODEGEN_WASM32_MACRO_ASSEMBLER_WASM32_H_

#include "src/codegen/macro-assembler-base.h"
#include "src/codegen/bailout-reason.h"
#include "src/codegen/wasm32/assembler-wasm32.h"
#include "src/execution/frame-constants.h"
#include "src/execution/frames.h"

namespace v8 {
namespace internal {

// Provide a minimal MoveCycleState for backend usage on wasm32.
struct MoveCycleState {};

class V8_EXPORT_PRIVATE MacroAssembler : public MacroAssemblerBase {
 public:
  using MacroAssemblerBase::MacroAssemblerBase;

  // Stack operations - all stubs
  void Push(Register) { nop(); }
  void Pop(Register) { nop(); }

  // Memory operations - all stubs
  void Move(Register, Register) { nop(); }
  void Move(Register, int32_t) { nop(); }
  void Move(Register, Tagged<Smi>) { nop(); }
  void Move(Register, ExternalReference) { nop(); }

  // Arithmetic operations - all stubs
  void Add(Register, Register, Register) { nop(); }
  void Sub(Register, Register, Register) { nop(); }
  void Mul(Register, Register, Register) { nop(); }

  // Comparison operations - stub
  void CompareAndBranch(Register, Register, Condition, Label*) { nop(); }

  // Function calls - all stubs
  void Call(Address) { nop(); }
  void Call(Register) { nop(); }
  void CallRuntime(Runtime::FunctionId) { nop(); }

  // Control flow - stubs
  void Jump(Label*) { nop(); }
  void Jump(Register) { nop(); }
  void Jump(Address, RelocInfo::Mode = RelocInfo::NO_INFO) { nop(); }
  void Jump(Handle<Code>, RelocInfo::Mode = RelocInfo::CODE_TARGET) { nop(); }
  void Jump(const ExternalReference&) { nop(); }
  void Jump(intptr_t, RelocInfo::Mode = RelocInfo::NO_INFO) { nop(); }

  void Bind(Label* label) { bind(label); }
  void Ret() { nop(); }

  // Stack frame operations - stubs
  void EnterFrame(StackFrame::Type) { nop(); }
  void LeaveFrame(StackFrame::Type) { nop(); }

  // Abort/Debug - stubs
  void Abort(AbortReason) { nop(); }
  void Trap() { nop(); }
  void CallBuiltin(Builtin) { nop(); }

  // Required for MacroAssemblerBase
  void RecordComment(const char*) {}
  void RequireCodeRange() {}

  static constexpr int kFramePointerRegister = 0;

  // Implement pure virtuals from MacroAssemblerBase as no-ops
  void LoadFromConstantsTable(Register, int) override { nop(); }
  void LoadRootRegisterOffset(Register, intptr_t) override { nop(); }
  void LoadRootRelative(Register, int32_t) override { nop(); }
  void StoreRootRelative(int32_t, Register) override { nop(); }
  void LoadRoot(Register, RootIndex) override { nop(); }

  // Backend helpers used by CodeGenerator
  void BindExceptionHandler(Label* label) { bind(label); }
  void CodeEntry() { nop(); }
  void LoopHeaderAlign() { nop(); }
  void CodeTargetAlign() { nop(); }
  void JumpIfEqual(Register, int, Label*) { nop(); }
  void JumpIfLessThan(Register, int, Label*) { nop(); }
  void jmp(Label*) { nop(); }
  void FinalizeJumpOptimizationInfo() {}
  void CallForDeoptimization(Builtin, int, Label*, DeoptimizeKind, Label*,
                             Label*) {
    nop();
  }

  // Map and Smi operations - stubs
  void LoadMap(Register, Register) { nop(); }
  void LoadCompressedMap(Register, Register) { nop(); }
  void SmiTag(Register, Register) { nop(); }
  void SmiTag(Register reg) { SmiTag(reg, reg); }
  void SmiUntag(Register, Register) { nop(); }
  void SmiUntag(Register reg) { SmiUntag(reg, reg); }
  void SmiToInt32(Register dst, Register src) { SmiUntag(dst, src); }
  void SmiToInt32(Register reg) { SmiUntag(reg, reg); }
  void AssertSmi(Register) { nop(); }
  void AssertNotSmi(Register) { nop(); }

  // Exception handling
  void ExceptionHandler(Label* label) { bind(label); }
  void ExceptionHandler() { nop(); }
};

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_MACRO_ASSEMBLER_WASM32_H_
