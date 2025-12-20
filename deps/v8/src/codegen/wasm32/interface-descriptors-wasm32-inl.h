// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_INTERFACE_DESCRIPTORS_WASM32_INL_H_
#define V8_CODEGEN_WASM32_INTERFACE_DESCRIPTORS_WASM32_INL_H_

#if V8_TARGET_ARCH_WASM32

#include "src/codegen/interface-descriptors.h"

namespace v8 {
namespace internal {

// Provide wasm32 defaults for descriptor register arrays.
constexpr auto CallInterfaceDescriptor::DefaultRegisterArray() {
  return RegisterArray(Register::r0(), Register::r1(), Register::r2(),
                       Register::r3(), Register::r4());
}

constexpr auto CallInterfaceDescriptor::DefaultDoubleRegisterArray() {
  return DoubleRegisterArray(DoubleRegister::d0(), DoubleRegister::d1(),
                             DoubleRegister::d2(), DoubleRegister::d3(),
                             DoubleRegister::d4());
}

constexpr auto CallInterfaceDescriptor::DefaultReturnRegisterArray() {
  return RegisterArray(Register::r0(), Register::r1());
}

constexpr auto CallInterfaceDescriptor::DefaultReturnDoubleRegisterArray() {
  return DoubleRegisterArray(DoubleRegister::d0(), DoubleRegister::d1());
}

// WASM32-specific descriptor implementations.
// Using virtual registers r0-r7 throughout.

// static
constexpr auto WriteBarrierDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r5(), Register::r4(),
                       Register::r2(), Register::r0(), Register::r3());
}

// static
constexpr Register LoadDescriptor::ReceiverRegister() { return Register::r1(); }
// static
constexpr Register LoadDescriptor::NameRegister() { return Register::r2(); }
// static
constexpr Register LoadDescriptor::SlotRegister() { return Register::r0(); }

// static
constexpr Register LoadWithVectorDescriptor::VectorRegister() { return Register::r3(); }

// static
constexpr Register KeyedLoadBaselineDescriptor::ReceiverRegister() { return Register::r1(); }
// static
constexpr Register KeyedLoadBaselineDescriptor::NameRegister() { return Register::r0(); }
// static
constexpr Register KeyedLoadBaselineDescriptor::SlotRegister() { return Register::r2(); }

// static
constexpr Register KeyedLoadWithVectorDescriptor::VectorRegister() { return Register::r3(); }

// static
constexpr Register EnumeratedKeyedLoadBaselineDescriptor::EnumIndexRegister() { return Register::r4(); }
// static
constexpr Register EnumeratedKeyedLoadBaselineDescriptor::CacheTypeRegister() { return Register::r5(); }
// static
constexpr Register EnumeratedKeyedLoadBaselineDescriptor::SlotRegister() { return Register::r2(); }

// static
constexpr Register KeyedHasICBaselineDescriptor::ReceiverRegister() { return Register::r0(); }
// static
constexpr Register KeyedHasICBaselineDescriptor::NameRegister() { return Register::r1(); }
// static
constexpr Register KeyedHasICBaselineDescriptor::SlotRegister() { return Register::r2(); }

// static
constexpr Register KeyedHasICWithVectorDescriptor::VectorRegister() { return Register::r3(); }

// static
constexpr Register LoadWithReceiverAndVectorDescriptor::LookupStartObjectRegister() { return Register::r4(); }

// static
constexpr Register StoreDescriptor::ReceiverRegister() { return Register::r1(); }
// static
constexpr Register StoreDescriptor::NameRegister() { return Register::r2(); }
// static
constexpr Register StoreDescriptor::ValueRegister() { return Register::r0(); }
// static
constexpr Register StoreDescriptor::SlotRegister() { return Register::r4(); }

// static
constexpr Register StoreWithVectorDescriptor::VectorRegister() { return Register::r3(); }

// static
constexpr Register DefineKeyedOwnDescriptor::FlagsRegister() { return Register::r5(); }

// static
constexpr Register StoreTransitionDescriptor::MapRegister() { return Register::r5(); }

// static
constexpr Register ApiGetterDescriptor::HolderRegister() { return Register::r0(); }
// static
constexpr Register ApiGetterDescriptor::CallbackRegister() { return Register::r3(); }

// static
constexpr Register GrowArrayElementsDescriptor::ObjectRegister() { return Register::r0(); }
// static
constexpr Register GrowArrayElementsDescriptor::KeyRegister() { return Register::r3(); }

// static
constexpr Register BaselineLeaveFrameDescriptor::ParamsSizeRegister() { return Register::r3(); }
// static
constexpr Register BaselineLeaveFrameDescriptor::WeightRegister() { return Register::r4(); }

// static
constexpr Register TypeConversionDescriptor::ArgumentRegister() { return Register::r0(); }

// static
constexpr auto TypeofDescriptor::registers() { return RegisterArray(Register::r0()); }

// static
constexpr Register MaglevOptimizeCodeOrTailCallOptimizedCodeSlotDescriptor::FlagsRegister() { return Register::r2(); }
// static
constexpr Register MaglevOptimizeCodeOrTailCallOptimizedCodeSlotDescriptor::FeedbackVectorRegister() { return Register::r5(); }
// static
constexpr Register MaglevOptimizeCodeOrTailCallOptimizedCodeSlotDescriptor::TemporaryRegister() { return Register::r4(); }

// static
constexpr auto CallTrampolineDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r0());
}

// static
constexpr auto CopyDataPropertiesWithExcludedPropertiesDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r0());
}

// static
constexpr auto CopyDataPropertiesWithExcludedPropertiesOnStackDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r0(), Register::r2());
}

// static
constexpr auto CallVarargsDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r0(), Register::r4(), Register::r2());
}

// static
constexpr auto CallForwardVarargsDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r0(), Register::r2());
}

// static
constexpr auto CallFunctionTemplateDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r2());
}

// static
constexpr auto CallFunctionTemplateGenericDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r2(), Register::r3());
}

// static
constexpr auto CallWithSpreadDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r0(), Register::r2());
}

// static
constexpr auto CallWithArrayLikeDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r2());
}

// static
constexpr auto ConstructVarargsDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r3(), Register::r0(), Register::r4(), Register::r2());
}

// static
constexpr auto ConstructForwardVarargsDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r3(), Register::r0(), Register::r2());
}

// static
constexpr auto ConstructWithSpreadDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r3(), Register::r0(), Register::r2());
}

// static
constexpr auto ConstructWithArrayLikeDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r3(), Register::r2());
}

// static
constexpr auto ConstructStubDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r3(), Register::r0());
}

// static
constexpr auto AbortDescriptor::registers() { return RegisterArray(Register::r1()); }

// static
constexpr auto CompareDescriptor::registers() { return RegisterArray(Register::r1(), Register::r0()); }

// static
constexpr auto Compare_BaselineDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r0(), Register::r2());
}

// static
constexpr auto BinaryOpDescriptor::registers() { return RegisterArray(Register::r1(), Register::r0()); }

// static
constexpr auto BinaryOp_BaselineDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r0(), Register::r2());
}

// static
constexpr auto BinarySmiOp_BaselineDescriptor::registers() {
  return RegisterArray(Register::r0(), Register::r1(), Register::r2());
}

// static
constexpr Register CallApiCallbackOptimizedDescriptor::ApiFunctionAddressRegister() { return Register::r1(); }
// static
constexpr Register CallApiCallbackOptimizedDescriptor::ActualArgumentsCountRegister() { return Register::r2(); }
// static
constexpr Register CallApiCallbackOptimizedDescriptor::FunctionTemplateInfoRegister() { return Register::r3(); }

// static
constexpr Register CallApiCallbackGenericDescriptor::ActualArgumentsCountRegister() { return Register::r2(); }
// static
constexpr Register CallApiCallbackGenericDescriptor::TopmostScriptHavingContextRegister() { return Register::r1(); }
// static
constexpr Register CallApiCallbackGenericDescriptor::FunctionTemplateInfoRegister() { return Register::r3(); }

// static
constexpr auto InterpreterDispatchDescriptor::registers() {
  return RegisterArray(Register::r0(), Register::r4(), Register::r5(), Register::r6());
}

// static
constexpr auto InterpreterPushArgsThenCallDescriptor::registers() {
  return RegisterArray(Register::r0(), Register::r2(), Register::r1());
}

// static
constexpr auto InterpreterPushArgsThenConstructDescriptor::registers() {
  return RegisterArray(Register::r0(), Register::r4(), Register::r1(), Register::r3(), Register::r2());
}

// static
constexpr auto ConstructForwardAllArgsDescriptor::registers() {
  return RegisterArray(Register::r1(), Register::r3());
}

// static
constexpr auto ResumeGeneratorDescriptor::registers() {
  return RegisterArray(Register::r0(), Register::r1());
}

// static
constexpr auto RunMicrotasksEntryDescriptor::registers() {
  return RegisterArray(Register::r0(), Register::r1());
}

// static
constexpr auto WasmJSToWasmWrapperDescriptor::registers() {
  return RegisterArray(Register::r7());
}

#if DEBUG
template <typename DerivedDescriptor>
void StaticCallInterfaceDescriptor<DerivedDescriptor>::
    VerifyArgumentRegisterCount(CallInterfaceDescriptorData* data,
                                int nof_expected_args) {
  // WASM32 doesn't use traditional registers, so skip this check
}
#endif  // DEBUG

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32

#endif  // V8_CODEGEN_WASM32_INTERFACE_DESCRIPTORS_WASM32_INL_H_
