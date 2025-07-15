// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_CODEGEN_WASM32_INTERFACE_DESCRIPTORS_WASM32_INL_H_
#define V8_CODEGEN_WASM32_INTERFACE_DESCRIPTORS_WASM32_INL_H_

#if V8_TARGET_ARCH_WASM32

#include "src/base/template-utils.h"
#include "src/codegen/interface-descriptors.h"
#include "src/execution/frames.h"

namespace v8 {
namespace internal {

constexpr auto CallInterfaceDescriptor::DefaultRegisterArray() {
  auto registers = RegisterArray(r0(), r1(), r2(), r3(), r4());
  return registers;
}

constexpr auto CallInterfaceDescriptor::DefaultDoubleRegisterArray() {
  auto registers = DoubleRegisterArray(d0(), d1(), d2(), d3(), d4());
  return registers;
}

constexpr auto CallInterfaceDescriptor::DefaultReturnRegisterArray() {
  auto registers = RegisterArray(r0(), r1());
  return registers;
}

constexpr auto CallInterfaceDescriptor::DefaultReturnDoubleRegisterArray() {
  auto registers = DoubleRegisterArray(d0(), d1());
  return registers;
}

// We define the association between CallDescriptors::Key and the specialized
// Builtin Descriptor here. This allows us to generate most of the call
// descriptors with the same BuiltinDescriptor.
#define DEF_TI(name, ...) \
  typedef name##Descriptor Builtin_##name##_InterfaceDescriptor;
INTERFACE_DESCRIPTOR_LIST(DEF_TI)
#undef DEF_TI

// Void descriptor
template <>
struct CallInterfaceDescriptorFor<CallDescriptors::Void> {
  using type = VoidDescriptor;
};

// Fast API call descriptors
template <>
struct CallInterfaceDescriptorFor<CallDescriptors::FastApiCall> {
  using type = FastApiCallDescriptor<kTarget>;
};

template <>
struct CallInterfaceDescriptorFor<CallDescriptors::FastApiCallNoOptions> {
  using type = FastApiCallDescriptor<kTarget, kHolder, kData, kIsolate>;
};

// Load descriptors
static constexpr inline Register LoadDescriptor::ReceiverRegister() {
  return r1();
}
static constexpr inline Register LoadDescriptor::NameRegister() {
  return r2();
}
static constexpr inline Register LoadDescriptor::SlotRegister() {
  return r0();
}

static constexpr inline Register LoadGlobalDescriptor::SlotRegister() {
  return r2();
}

static constexpr inline Register LoadWithVectorDescriptor::VectorRegister() {
  return r3();
}

static constexpr inline Register KeyedLoadWithVectorDescriptor::VectorRegister() {
  return r3();
}

static constexpr inline Register KeyedHasICDescriptor::VectorRegister() {
  return r3();
}

static constexpr inline Register
LoadGlobalWithVectorDescriptor::VectorRegister() {
  return r3();
}

// Store descriptors
static constexpr inline Register StoreDescriptor::ReceiverRegister() {
  return r1();
}
static constexpr inline Register StoreDescriptor::NameRegister() {
  return r2();
}
static constexpr inline Register StoreDescriptor::ValueRegister() {
  return r0();
}
static constexpr inline Register StoreDescriptor::SlotRegister() {
  return r4();
}

static constexpr inline Register StoreWithVectorDescriptor::VectorRegister() {
  return r3();
}

static constexpr inline Register StoreGlobalDescriptor::SlotRegister() {
  return r4();
}

static constexpr inline Register
StoreGlobalWithVectorDescriptor::VectorRegister() {
  return r3();
}

static constexpr inline Register KeyedStoreGenericDescriptor::VectorRegister() {
  return r4();
}

static constexpr inline Register InterpreterDispatchDescriptor::kAccumulatorRegister() {
  return r0();
}
static constexpr inline Register InterpreterDispatchDescriptor::kBytecodeOffsetRegister() {
  return r2();
}
static constexpr inline Register InterpreterDispatchDescriptor::kBytecodeArrayRegister() {
  return r3();
}
static constexpr inline Register InterpreterDispatchDescriptor::kDispatchTableRegister() {
  return r4();
}

static constexpr inline Register ApiGetterDescriptor::HolderRegister() {
  return r0();
}
static constexpr inline Register ApiGetterDescriptor::CallbackRegister() {
  return r3();
}

static constexpr inline Register GrowArrayElementsDescriptor::ObjectRegister() {
  return r0();
}
static constexpr inline Register GrowArrayElementsDescriptor::KeyRegister() {
  return r3();
}

static constexpr inline Register BaselineLeaveFrameDescriptor::ParamsSizeRegister() {
  return r3();
}
static constexpr inline Register BaselineLeaveFrameDescriptor::WeightRegister() {
  return r4();
}

static constexpr inline Register TypeConversionDescriptor::ArgumentRegister() {
  return r0();
}

static constexpr inline Register
    TypeConversion_Float32ToFloat64_Descriptor::SourceRegister() {
  return r0();
}

static constexpr inline auto TypeofDescriptor::registers() {
  return RegisterArray(r0());
}

static constexpr inline auto CallTrampolineDescriptor::registers() {
  return RegisterArray(r1(), r0());
}

static constexpr inline auto CallVarargsDescriptor::registers() {
  return RegisterArray(r0(), r4(), r3());
}

static constexpr inline auto CallForwardVarargsDescriptor::registers() {
  return RegisterArray(r1(), r0());
}

static constexpr inline auto CallFunctionTemplateDescriptor::registers() {
  return RegisterArray(r1(), r3(), r0());
}

static constexpr inline auto CallFunctionTemplateGenericDescriptor::registers() {
  return RegisterArray(r1(), r3(), r0(), r4());
}

static constexpr inline auto CallWithSpreadDescriptor::registers() {
  return RegisterArray(r1(), r0(), r4());
}

static constexpr inline auto CallWithSpread_BaselineDescriptor::registers() {
  return RegisterArray(r1(), r0(), r4(), r3());
}

static constexpr inline auto CallWithSpread_WithFeedbackDescriptor::registers() {
  return RegisterArray(r1(), r0(), r4(), r3(), r2());
}

static constexpr inline auto CallWithArrayLikeDescriptor::registers() {
  return RegisterArray(r1(), r2());
}

static constexpr inline auto CallWithArrayLike_WithFeedbackDescriptor::registers() {
  return RegisterArray(r1(), r2(), r3(), r4());
}

static constexpr inline auto ConstructVarargsDescriptor::registers() {
  return RegisterArray(r1(), r3(), r0(), r4());
}

static constexpr inline auto ConstructForwardVarargsDescriptor::registers() {
  return RegisterArray(r1(), r3(), r0());
}

static constexpr inline auto ConstructWithSpreadDescriptor::registers() {
  return RegisterArray(r1(), r3(), r0(), r4());
}

static constexpr inline auto ConstructWithSpread_BaselineDescriptor::registers() {
  return RegisterArray(r1(), r3(), r0(), r4(), kJavaScriptCallSlotCountRegister);
}

static constexpr inline auto ConstructWithArrayLikeDescriptor::registers() {
  return RegisterArray(r1(), r3(), r2());
}

static constexpr inline auto ConstructWithArrayLike_WithFeedbackDescriptor::registers() {
  return RegisterArray(r1(), r3(), r2(), r4());
}

static constexpr inline auto ConstructStubDescriptor::registers() {
  return RegisterArray(r1(), r3(), r0());
}

static constexpr inline auto AbortDescriptor::registers() {
  return RegisterArray(r1());
}

static constexpr inline auto CompareDescriptor::registers() {
  return RegisterArray(r1(), r0());
}

static constexpr inline auto Compare_BaselineDescriptor::registers() {
  return RegisterArray(r1(), r0(), r3());
}

static constexpr inline auto BinaryOpDescriptor::registers() {
  return RegisterArray(r1(), r0());
}

static constexpr inline auto BinaryOp_BaselineDescriptor::registers() {
  return RegisterArray(r1(), r0(), r3());
}

static constexpr inline auto BinarySmiOp_BaselineDescriptor::registers() {
  return RegisterArray(r0(), r1(), r3());
}

static constexpr inline auto ApiCallbackDescriptor::registers() {
  return RegisterArray(r1(), r2(), r3(), r0());
}

static constexpr inline auto InterpreterPushArgsThenCallDescriptor::registers() {
  return RegisterArray(r0(), r2(), r1());
}

static constexpr inline auto InterpreterPushArgsThenConstructDescriptor::registers() {
  return RegisterArray(r0(), r4(), r1(), r3());
}

static constexpr inline auto ResumeGeneratorDescriptor::registers() {
  return RegisterArray(r0(), r1());
}

static constexpr inline auto RunMicrotasksEntryDescriptor::registers() {
  return RegisterArray(r0(), r1());
}

static constexpr inline auto WasmFloat32ToNumberDescriptor::registers() {
  return RegisterArray();
}

static constexpr inline auto WasmFloat64ToTaggedDescriptor::registers() {
  return RegisterArray();
}

static constexpr inline auto WasmJSToWasmWrapperDescriptor::registers() {
  return RegisterArray();
}

static constexpr inline auto WasmI32AtomicWait32Descriptor::registers() {
  return RegisterArray(r0(), r1(), r2(), r3());
}

static constexpr inline auto WasmI64AtomicWait32Descriptor::registers() {
  return RegisterArray(r0(), r1(), r2(), r3(), r4());
}

static constexpr inline auto CloneObjectWithVectorDescriptor::registers() {
  return RegisterArray(r1(), r2(), r3(), r4());
}

static constexpr inline auto V8_EXPORT_PRIVATE BinaryOp_WithFeedbackDescriptor::registers() {
  return RegisterArray(r1(), r0(), r2(), r3(), r4());
}

static constexpr inline auto CallTrampoline_BaselineDescriptor::registers() {
  return RegisterArray(r1(), r0(), r3());
}

static constexpr inline auto CallTrampoline_WithFeedbackDescriptor::registers() {
  return CallTrampoline_BaselineDescriptor::registers();
}

static constexpr inline auto ConstructStub_BaselineDescriptor::registers() {
  return RegisterArray(r1(), r3(), r0(), kJavaScriptCallSlotCountRegister);
}

static constexpr inline auto ConstructStub_WithFeedbackDescriptor::registers() {
  return ConstructStub_BaselineDescriptor::registers();
}

static constexpr inline auto FastNewObjectDescriptor::registers() {
  return RegisterArray(r1(), r3());
}

static constexpr inline auto WriteBarrierDescriptor::registers() {
  return RegisterArray(r1(), r5(), r4(), r2(), r0(), r3());
}

static constexpr inline auto IndirectPointerWriteBarrierDescriptor::registers() {
  return RegisterArray(r1(), r0(), r5(), r4(), r2());
}

static constexpr inline auto ForInPrepareDescriptor::registers() {
  return RegisterArray(r0(), r1());
}

static constexpr inline auto GetIteratorStackParameterDescriptor::registers() {
  return RegisterArray(r0());
}

static constexpr inline auto UnaryOp_BaselineDescriptor::registers() {
  return RegisterArray(r0(), r1(), r2());
}

static constexpr inline auto UnaryOp_WithFeedbackDescriptor::registers() {
  return RegisterArray(r0(), r1(), r2(), r3());
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32

#endif  // V8_CODEGEN_WASM32_INTERFACE_DESCRIPTORS_WASM32_INL_H_