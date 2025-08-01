// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_EXECUTION_WASM32_FRAME_CONSTANTS_WASM32_H_
#define V8_EXECUTION_WASM32_FRAME_CONSTANTS_WASM32_H_

#include "src/base/macros.h"
#include "src/codegen/register.h"
#include "src/execution/frame-constants.h"

namespace v8 {
namespace internal {

// WASM32 frame constants
class EntryFrameConstants : public AllStatic {
 public:
  // Total frame size includes return address and frame pointer
  static constexpr int kFixedFrameSizeFromFp = 2 * kSystemPointerSize;
};

class ExitFrameConstants : public TypedFrameConstants {
 public:
  static constexpr int kSPOffset = -1 * kSystemPointerSize;
  static constexpr int kCodeOffset = -2 * kSystemPointerSize;
  static constexpr int kPaddingOffset = -3 * kSystemPointerSize;
  static constexpr int kCallerPCOffset = 0 * kSystemPointerSize;
  static constexpr int kCallerFPOffset = 1 * kSystemPointerSize;
  static constexpr int kCallerSPOffset = 2 * kSystemPointerSize;
  
  static constexpr int kConstantPoolOffset = 0;  // No constant pool on WASM32
  static constexpr int kFixedFrameSizeFromFp = 3 * kSystemPointerSize;
};

class WasmExitFrameConstants : public TypedFrameConstants {
 public:
  static constexpr int kInstanceOffset = -1 * kSystemPointerSize;
  static constexpr int kFixedFrameSizeFromFp = 1 * kSystemPointerSize;
};

class JavaScriptFrameConstants : public AllStatic {
 public:
  // Fixed frame slots
  static constexpr int kFunctionOffset = -2 * kSystemPointerSize;
  static constexpr int kContextOffset = -1 * kSystemPointerSize;
  static constexpr int kLastParameterOffset = +2 * kSystemPointerSize;
  
  // Expression stack
  static constexpr int kLocal0Offset = -3 * kSystemPointerSize;
  static constexpr int kLastNonArgumentRegisterOffset = 0;
  static constexpr int kBytecodeOffsetRegisterOffset = 0;
  static constexpr int kRegisterFileFromFp = -3 * kSystemPointerSize;
  
  static constexpr int kExpressionsOffset = -3 * kSystemPointerSize;
  static constexpr int kFixedFrameSizeFromFp = 2 * kSystemPointerSize;
  static constexpr int kFixedSlotCount = 4;
};

// Constants for BuiltinContinuation frames
class BuiltinContinuationFrameConstants : public TypedFrameConstants {
 public:
  static constexpr int kFixedFrameSizeFromFp = 2 * kSystemPointerSize;
  static constexpr int kFixedSlotCount = 2;
};

// Interpreted frame constants
class InterpreterFrameConstants : public AllStatic {
 public:
  static constexpr int kFixedFrameSizeFromFp = 3 * kSystemPointerSize;
  static constexpr int kBytecodeOffsetFromFp = -1 * kSystemPointerSize;
  static constexpr int kBytecodeArrayFromFp = -2 * kSystemPointerSize;
  static constexpr int kRegisterFileFromFp = -3 * kSystemPointerSize;
  static constexpr int kExpressionsOffset = kRegisterFileFromFp;
  static constexpr int kLastParamFromFp = StandardFrameConstants::kCallerSPOffset;
  static constexpr int kBytecodeOffsetFromRegisterPointer = 1 * kSystemPointerSize;
  static constexpr int kBytecodeArrayFromRegisterPointer = 2 * kSystemPointerSize;
  static constexpr int kNewTargetFromRegisterPointer = 3 * kSystemPointerSize;
  static constexpr int kFunctionFromRegisterPointer = 4 * kSystemPointerSize;
  static constexpr int kContextFromRegisterPointer = 5 * kSystemPointerSize;
  static constexpr int kFixedFrameSizeFromRegisterPointer = 6 * kSystemPointerSize;
  static constexpr int kNumberOfExtraArgsFromRegisterPointer = 7 * kSystemPointerSize;
  static constexpr int kFirstParamFromRegisterPointer = 8 * kSystemPointerSize;
};

// Maglev frame constants
class MaglevFrameConstants : public StandardFrameConstants {
 public:
  static constexpr int kFixedFrameSizeFromFp = 2 * kSystemPointerSize;
  static constexpr int kLastParameterOffset = +2 * kSystemPointerSize;
  static constexpr int kStackGuardFrameSize = 1 * kSystemPointerSize;
};

// Unoptimized frame constants
class UnoptimizedFrameConstants : public StandardFrameConstants {
 public:
  static constexpr int kFixedFrameSizeFromFp = 2 * kSystemPointerSize;
  static constexpr int kLastParameterOffset = +2 * kSystemPointerSize;
};

}  // namespace internal
}  // namespace v8

#endif  // V8_EXECUTION_WASM32_FRAME_CONSTANTS_WASM32_H_