// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_EXECUTION_WASM32_FRAME_CONSTANTS_WASM32_H_
#define V8_EXECUTION_WASM32_FRAME_CONSTANTS_WASM32_H_

#include "src/base/bits.h"
#include "src/base/macros.h"
#include "src/codegen/register.h"
#include "src/execution/frame-constants.h"

namespace v8 {
namespace internal {

// WASM32-specific frame constants
class EntryFrameConstants : public AllStatic {
 public:
  // Total frame size includes return address and frame pointer
  static constexpr int kFixedFrameSizeFromFp = 2 * kSystemPointerSize;

  // Offsets for storing FP and PC of fast API calls
  static constexpr int kNextExitFrameFPOffset = -3 * kSystemPointerSize;
  static constexpr int kNextFastCallFrameFPOffset = -4 * kSystemPointerSize;
  static constexpr int kNextFastCallFramePCOffset = -5 * kSystemPointerSize;

  // Root register value offset
  static constexpr int kRootRegisterValueOffset = +2 * kSystemPointerSize;

  // Arguments for JSEntry and JSConstructEntry
  static constexpr int kNewTargetArgOffset = +3 * kSystemPointerSize;
  static constexpr int kFunctionArgOffset = +4 * kSystemPointerSize;
  static constexpr int kReceiverArgOffset = +5 * kSystemPointerSize;
  static constexpr int kArgcOffset = +6 * kSystemPointerSize;
  static constexpr int kArgvOffset = +7 * kSystemPointerSize;

  // For JSRunMicrotasksEntry
  static constexpr int kMicrotaskQueueArgOffset = +3 * kSystemPointerSize;
};

class WasmLiftoffSetupFrameConstants : public TypedFrameConstants {
 public:
  // Number of gp parameters, without the instance
  static constexpr int kNumberOfSavedGpParamRegs = 2;
  static constexpr int kNumberOfSavedFpParamRegs = 4;

  // Instance spill offset
  static constexpr int kInstanceSpillOffset =
      TYPED_FRAME_PUSHED_VALUE_OFFSET(1);

  static constexpr int kParameterSpillsOffset[] = {
      TYPED_FRAME_PUSHED_VALUE_OFFSET(2), TYPED_FRAME_PUSHED_VALUE_OFFSET(3)};

  // SP-relative offsets
  static constexpr int kWasmInstanceDataOffset = 2 * kSystemPointerSize;
  static constexpr int kDeclaredFunctionIndexOffset = 1 * kSystemPointerSize;
  static constexpr int kNativeModuleOffset = 0;
};

class WasmLiftoffFrameConstants : public TypedFrameConstants {
 public:
  static constexpr int kFeedbackVectorOffset = 3 * kSystemPointerSize;
  static constexpr int kInstanceDataOffset = 2 * kSystemPointerSize;
};

class WasmDebugBreakFrameConstants : public TypedFrameConstants {
 public:
  // WASM32 has limited registers compared to other architectures
  // Virtual register set for debugging support
  static constexpr int kNumPushedGpRegisters = 4;
  static constexpr int kNumPushedFpRegisters = 4;

  // Register lists for push/pop operations (r0-r3 for GP, d0-d3 for FP)
  static constexpr RegList kPushedGpRegs = {Register::r0(), Register::r1(),
                                             Register::r2(), Register::r3()};
  static constexpr DoubleRegList kPushedFpRegs = {
      DoubleRegister::d0(), DoubleRegister::d1(),
      DoubleRegister::d2(), DoubleRegister::d3()};

  static constexpr int kLastPushedGpRegisterOffset =
      -kFixedFrameSizeFromFp - kNumPushedGpRegisters * kSystemPointerSize;
  static constexpr int kLastPushedFpRegisterOffset =
      kLastPushedGpRegisterOffset - kNumPushedFpRegisters * kSimd128Size;

  // Offsets are fp-relative.
  static int GetPushedGpRegisterOffset(int reg_code) {
    DCHECK_NE(0, kPushedGpRegs.bits() & (1 << reg_code));
    uint32_t lower_regs =
        kPushedGpRegs.bits() & ((uint32_t{1} << reg_code) - 1);
    return kLastPushedGpRegisterOffset +
           base::bits::CountPopulation(lower_regs) * kSystemPointerSize;
  }

  static int GetPushedFpRegisterOffset(int reg_code) {
    DCHECK_NE(0, kPushedFpRegs.bits() & (1 << reg_code));
    uint32_t lower_regs =
        kPushedFpRegs.bits() & ((uint32_t{1} << reg_code) - 1);
    return kLastPushedFpRegisterOffset +
           base::bits::CountPopulation(lower_regs) * kDoubleSize;
  }
};

}  // namespace internal
}  // namespace v8

#endif  // V8_EXECUTION_WASM32_FRAME_CONSTANTS_WASM32_H_
