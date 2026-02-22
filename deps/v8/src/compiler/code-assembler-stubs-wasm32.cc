// Copyright 2024 the V8 project authors. All rights reserved.
// Comprehensive CodeAssembler stub implementations for WASM32 target.
// WASM32 is an interpreter target that does not use native code generation.

#if V8_TARGET_ARCH_WASM32

#include "src/compiler/code-assembler.h"
#include "src/compiler/pipeline.h"
#include "src/compiler/compiler-source-position-table.h"
#include "src/builtins/builtins.h"
#include "src/codegen/macro-assembler.h"
#include "src/codegen/compiler.h"
#include "src/execution/isolate.h"
#include "src/api/api.h"
#include "src/handles/handles.h"
#include "src/heap/factory.h"
#include "src/roots/roots.h"
#include "src/runtime/runtime.h"
#include "src/objects/smi.h"
#include "src/objects/code.h"

namespace v8 {
namespace internal {
namespace compiler {

// ============================================================================
// CodeAssembler constructor/destructor stubs
// ============================================================================

CodeAssembler::~CodeAssembler() = default;

// ============================================================================
// CodeAssembler basic accessors
// ============================================================================

Isolate* CodeAssembler::isolate() const { return nullptr; }
Zone* CodeAssembler::zone() const { return nullptr; }
Factory* CodeAssembler::factory() const { return nullptr; }
Builtin CodeAssembler::builtin() { return Builtin::kNoBuiltinId; }
bool CodeAssembler::Is32() const { return true; }
bool CodeAssembler::Is64() const { return false; }
bool CodeAssembler::Word32ShiftIsSafe() const { return false; }
bool CodeAssembler::IsFloat64RoundDownSupported() const { return false; }
bool CodeAssembler::IsFloat64RoundUpSupported() const { return false; }
bool CodeAssembler::IsFloat64RoundTiesEvenSupported() const { return false; }
bool CodeAssembler::IsFloat64RoundTruncateSupported() const { return false; }
bool CodeAssembler::IsInt32AbsWithOverflowSupported() const { return false; }
bool CodeAssembler::IsIntPtrAbsWithOverflowSupported() const { return false; }
bool CodeAssembler::IsTruncateFloat64ToFloat16RawBitsSupported() const { return false; }
bool CodeAssembler::HasDynamicJSParameterCount() { return false; }
bool CodeAssembler::IsMapOffsetConstant(Node*) { return false; }
bool CodeAssembler::IsNullConstant(TNode<Object>) { return false; }
bool CodeAssembler::IsUndefinedConstant(TNode<Object>) { return false; }

const std::vector<FileAndLine>& CodeAssembler::GetMacroSourcePositionStack() const {
  static std::vector<FileAndLine> empty_vector;
  return empty_vector;
}

TNode<Uint16T> CodeAssembler::DynamicJSParameterCount() { return TNode<Uint16T>(); }

// ============================================================================
// Constants
// ============================================================================

TNode<Int32T> CodeAssembler::Int32Constant(int32_t value) { return TNode<Int32T>(); }
TNode<Int32T> CodeAssembler::UniqueInt32Constant(int value) { return TNode<Int32T>(); }
TNode<Int64T> CodeAssembler::Int64Constant(int64_t value) { return TNode<Int64T>(); }
TNode<IntPtrT> CodeAssembler::IntPtrConstant(intptr_t value) { return TNode<IntPtrT>(); }
TNode<IntPtrT> CodeAssembler::UniqueIntPtrConstant(intptr_t value) { return TNode<IntPtrT>(); }
TNode<Float32T> CodeAssembler::Float32Constant(double value) { return TNode<Float32T>(); }
TNode<Float64T> CodeAssembler::Float64Constant(double value) { return TNode<Float64T>(); }
TNode<Smi> CodeAssembler::SmiConstant(int value) { return TNode<Smi>(); }
TNode<Smi> CodeAssembler::SmiConstant(Tagged<Smi> value) { return TNode<Smi>(); }
TNode<Number> CodeAssembler::NumberConstant(double value) { return TNode<Number>(); }
TNode<String> CodeAssembler::StringConstant(const char* str) { return TNode<String>(); }
TNode<TaggedIndex> CodeAssembler::TaggedIndexConstant(intptr_t value) { return TNode<TaggedIndex>(); }
TNode<ExternalReference> CodeAssembler::ExternalConstant(ExternalReference ref) { return TNode<ExternalReference>(); }
TNode<ExternalReference> CodeAssembler::IsolateField(IsolateFieldId id) { return TNode<ExternalReference>(); }

// ============================================================================
// Parameters and control flow
// ============================================================================

Node* CodeAssembler::UntypedParameter(int index) { return nullptr; }
TNode<Context> CodeAssembler::GetJSContextParameter() { return TNode<Context>(); }

void CodeAssembler::Bind(Label* label) {}
void CodeAssembler::Goto(Label* label) {}
void CodeAssembler::GotoIf(TNode<IntegralT> condition, Label* label, GotoHint hint) {}
void CodeAssembler::GotoIfNot(TNode<IntegralT> condition, Label* label, GotoHint hint) {}
void CodeAssembler::Branch(TNode<IntegralT> condition, Label* true_label, Label* false_label, BranchHint hint) {}
void CodeAssembler::Branch(TNode<BoolT> condition, const std::function<void()>& true_body, const std::function<void()>& false_body) {}
void CodeAssembler::Branch(TNode<BoolT> condition, Label* true_label, const std::function<void()>& false_body) {}
void CodeAssembler::Switch(Node* index, Label* default_label, const int32_t* case_values, Label** case_labels, size_t case_count) {}
void CodeAssembler::Unreachable() {}
void CodeAssembler::DebugBreak() {}
void CodeAssembler::BreakOnNode(int node_id) {}

// ============================================================================
// Return statements
// ============================================================================

void CodeAssembler::Return(TNode<Object> value) {}
void CodeAssembler::Return(TNode<Object> value1, TNode<Object> value2) {}
void CodeAssembler::Return(TNode<Word32T> value1, TNode<Object> value2) {}
void CodeAssembler::Return(TNode<Word32T> value1, TNode<Word32T> value2) {}
void CodeAssembler::Return(TNode<WordT> value) {}
void CodeAssembler::Return(TNode<WordT> value1, TNode<WordT> value2) {}
void CodeAssembler::Return(TNode<Float32T> value) {}
void CodeAssembler::Return(TNode<Float64T> value) {}
void CodeAssembler::Return(TNode<Int32T> value) {}
void CodeAssembler::Return(TNode<Uint32T> value) {}
void CodeAssembler::ReturnIf(TNode<BoolT> condition, TNode<Object> value) {}
void CodeAssembler::PopAndReturn(Node* pop, Node* value) {}
void CodeAssembler::PopAndReturn(Node* pop, Node* value1, Node* value2, Node* value3, Node* value4) {}

// ============================================================================
// Type conversions
// ============================================================================

TNode<IntPtrT> CodeAssembler::ChangeInt32ToIntPtr(TNode<Word32T> value) { return TNode<IntPtrT>(); }
TNode<UintPtrT> CodeAssembler::ChangeUint32ToWord(TNode<Word32T> value) { return TNode<UintPtrT>(); }
TNode<Uint64T> CodeAssembler::ChangeUint32ToUint64(TNode<Word32T> value) { return TNode<Uint64T>(); }
TNode<Float64T> CodeAssembler::ChangeInt32ToFloat64(TNode<Int32T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::ChangeUint32ToFloat64(TNode<Word32T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::ChangeInt64ToFloat64(TNode<Int64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::ChangeUintPtrToFloat64(TNode<UintPtrT> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::ChangeFloat32ToFloat64(TNode<Float32T> value) { return TNode<Float64T>(); }
TNode<IntPtrT> CodeAssembler::ChangeFloat64ToIntPtr(TNode<Float64T> value) { return TNode<IntPtrT>(); }
TNode<UintPtrT> CodeAssembler::ChangeFloat64ToUintPtr(TNode<Float64T> value) { return TNode<UintPtrT>(); }
TNode<Uint32T> CodeAssembler::ChangeFloat64ToUint32(TNode<Float64T> value) { return TNode<Uint32T>(); }
TNode<Int32T> CodeAssembler::TruncateFloat32ToInt32(TNode<Float32T> value) { return TNode<Int32T>(); }
TNode<Float32T> CodeAssembler::TruncateFloat64ToFloat32(TNode<Float64T> value) { return TNode<Float32T>(); }
TNode<Int64T> CodeAssembler::TruncateFloat64ToInt64(TNode<Float64T> value) { return TNode<Int64T>(); }
TNode<Uint32T> CodeAssembler::TruncateFloat64ToWord32(TNode<Float64T> value) { return TNode<Uint32T>(); }
TNode<Int32T> CodeAssembler::TruncateInt64ToInt32(TNode<Int64T> value) { return TNode<Int32T>(); }
TNode<Float16RawBitsT> CodeAssembler::TruncateFloat64ToFloat16RawBits(TNode<Float64T> value) { return TNode<Float16RawBitsT>(); }
TNode<Float32T> CodeAssembler::RoundInt32ToFloat32(TNode<Int32T> value) { return TNode<Float32T>(); }
TNode<Float64T> CodeAssembler::RoundIntPtrToFloat64(Node* value) { return TNode<Float64T>(); }
TNode<Int32T> CodeAssembler::RoundFloat64ToInt32(TNode<Float64T> value) { return TNode<Int32T>(); }

// ============================================================================
// Bitcast operations
// ============================================================================

TNode<IntPtrT> CodeAssembler::BitcastTaggedToWord(TNode<Object> value) { return TNode<IntPtrT>(); }
TNode<IntPtrT> CodeAssembler::BitcastTaggedToWordForTagAndSmiBits(TNode<AnyTaggedT> value) { return TNode<IntPtrT>(); }
TNode<IntPtrT> CodeAssembler::BitcastMaybeObjectToWord(TNode<MaybeObject> value) { return TNode<IntPtrT>(); }
TNode<Object> CodeAssembler::BitcastWordToTagged(TNode<WordT> value) { return TNode<Object>(); }
TNode<Smi> CodeAssembler::BitcastWordToTaggedSigned(TNode<WordT> value) { return TNode<Smi>(); }
TNode<Uint32T> CodeAssembler::BitcastFloat32ToInt32(TNode<Float32T> value) { return TNode<Uint32T>(); }
TNode<Int64T> CodeAssembler::BitcastFloat64ToInt64(TNode<Float64T> value) { return TNode<Int64T>(); }
TNode<Float32T> CodeAssembler::BitcastInt32ToFloat32(TNode<Word32T> value) { return TNode<Float32T>(); }
TNode<Float64T> CodeAssembler::BitcastInt64ToFloat64(TNode<Int64T> value) { return TNode<Float64T>(); }

// ============================================================================
// Word32 operations
// ============================================================================

TNode<Word32T> CodeAssembler::Int32Add(TNode<Word32T> left, TNode<Word32T> right) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::Int32Sub(TNode<Word32T> left, TNode<Word32T> right) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::Int32Mul(TNode<Word32T> left, TNode<Word32T> right) { return TNode<Word32T>(); }
TNode<Int32T> CodeAssembler::Int32Div(TNode<Int32T> left, TNode<Int32T> right) { return TNode<Int32T>(); }
TNode<Int32T> CodeAssembler::Int32Mod(TNode<Int32T> left, TNode<Int32T> right) { return TNode<Int32T>(); }
TNode<Uint32T> CodeAssembler::Uint32Div(TNode<Uint32T> left, TNode<Uint32T> right) { return TNode<Uint32T>(); }
TNode<Uint32T> CodeAssembler::Uint32Mod(TNode<Uint32T> left, TNode<Uint32T> right) { return TNode<Uint32T>(); }
TNode<Word32T> CodeAssembler::Word32And(TNode<Word32T> left, TNode<Word32T> right) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::Word32Or(TNode<Word32T> left, TNode<Word32T> right) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::Word32Xor(TNode<Word32T> left, TNode<Word32T> right) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::Word32Shl(TNode<Word32T> value, TNode<Word32T> shift) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::Word32Shr(TNode<Word32T> value, int shift) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::Word32Shr(TNode<Word32T> value, TNode<Word32T> shift) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::Word32Sar(TNode<Word32T> value, int shift) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::Word32Sar(TNode<Word32T> value, TNode<Word32T> shift) { return TNode<Word32T>(); }
TNode<Int32T> CodeAssembler::Word32Clz(TNode<Word32T> value) { return TNode<Int32T>(); }
TNode<Word32T> CodeAssembler::Word32BitwiseNot(TNode<Word32T> value) { return TNode<Word32T>(); }
TNode<BoolT> CodeAssembler::Word32BinaryNot(TNode<Word32T> value) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Word32Equal(TNode<Word32T> left, TNode<Word32T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Word32NotEqual(TNode<Word32T> left, TNode<Word32T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Int32LessThan(TNode<Word32T> left, TNode<Word32T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Int32LessThanOrEqual(TNode<Word32T> left, TNode<Word32T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Int32GreaterThan(TNode<Word32T> left, TNode<Word32T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Int32GreaterThanOrEqual(TNode<Word32T> left, TNode<Word32T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Uint32LessThan(TNode<Word32T> left, TNode<Word32T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Uint32LessThanOrEqual(TNode<Word32T> left, TNode<Word32T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Uint32GreaterThan(TNode<Word32T> left, TNode<Word32T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Uint32GreaterThanOrEqual(TNode<Word32T> left, TNode<Word32T> right) { return TNode<BoolT>(); }
TNode<PairT<Int32T, BoolT>> CodeAssembler::Int32AddWithOverflow(TNode<Int32T> left, TNode<Int32T> right) { return TNode<PairT<Int32T, BoolT>>(); }
TNode<PairT<Int32T, BoolT>> CodeAssembler::Int32SubWithOverflow(TNode<Int32T> left, TNode<Int32T> right) { return TNode<PairT<Int32T, BoolT>>(); }
TNode<PairT<Int32T, BoolT>> CodeAssembler::Int32MulWithOverflow(TNode<Int32T> left, TNode<Int32T> right) { return TNode<PairT<Int32T, BoolT>>(); }
TNode<PairT<Int32T, BoolT>> CodeAssembler::Int32AbsWithOverflow(TNode<Int32T> value) { return TNode<PairT<Int32T, BoolT>>(); }
TNode<PairT<Word32T, Word32T>> CodeAssembler::Int32PairAdd(TNode<Word32T> a_low, TNode<Word32T> a_high, TNode<Word32T> b_low, TNode<Word32T> b_high) { return TNode<PairT<Word32T, Word32T>>(); }
TNode<PairT<Word32T, Word32T>> CodeAssembler::Int32PairSub(TNode<Word32T> a_low, TNode<Word32T> a_high, TNode<Word32T> b_low, TNode<Word32T> b_high) { return TNode<PairT<Word32T, Word32T>>(); }

// ============================================================================
// Word64 operations
// ============================================================================

TNode<Word64T> CodeAssembler::Int64Add(TNode<Word64T> left, TNode<Word64T> right) { return TNode<Word64T>(); }
TNode<Word64T> CodeAssembler::Int64Sub(TNode<Word64T> left, TNode<Word64T> right) { return TNode<Word64T>(); }
TNode<Word64T> CodeAssembler::Int64Mul(TNode<Word64T> left, TNode<Word64T> right) { return TNode<Word64T>(); }
TNode<Word64T> CodeAssembler::Word64And(TNode<Word64T> left, TNode<Word64T> right) { return TNode<Word64T>(); }
TNode<Word64T> CodeAssembler::Word64Xor(TNode<Word64T> left, TNode<Word64T> right) { return TNode<Word64T>(); }
TNode<Word64T> CodeAssembler::Word64Shr(TNode<Word64T> value, TNode<Word64T> shift) { return TNode<Word64T>(); }
TNode<Word64T> CodeAssembler::Word64Sar(TNode<Word64T> value, TNode<Word64T> shift) { return TNode<Word64T>(); }
TNode<BoolT> CodeAssembler::Word64Equal(TNode<Word64T> left, TNode<Word64T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Uint64LessThan(TNode<Word64T> left, TNode<Word64T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Uint64GreaterThan(TNode<Word64T> left, TNode<Word64T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Uint64GreaterThanOrEqual(TNode<Word64T> left, TNode<Word64T> right) { return TNode<BoolT>(); }

// ============================================================================
// WordT (pointer-sized) operations
// ============================================================================

TNode<WordT> CodeAssembler::IntPtrAdd(TNode<WordT> left, TNode<WordT> right) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::IntPtrSub(TNode<WordT> left, TNode<WordT> right) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::IntPtrMul(TNode<WordT> left, TNode<WordT> right) { return TNode<WordT>(); }
TNode<IntPtrT> CodeAssembler::IntPtrDiv(TNode<IntPtrT> left, TNode<IntPtrT> right) { return TNode<IntPtrT>(); }
TNode<IntPtrT> CodeAssembler::IntPtrMod(TNode<IntPtrT> left, TNode<IntPtrT> right) { return TNode<IntPtrT>(); }
TNode<WordT> CodeAssembler::WordAnd(TNode<WordT> left, TNode<WordT> right) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::WordOr(TNode<WordT> left, TNode<WordT> right) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::WordXor(TNode<WordT> left, TNode<WordT> right) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::WordNot(TNode<WordT> value) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::WordShl(TNode<WordT> value, int shift) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::WordShl(TNode<WordT> value, TNode<IntegralT> shift) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::WordShr(TNode<WordT> value, int shift) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::WordShr(TNode<WordT> value, TNode<IntegralT> shift) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::WordSar(TNode<WordT> value, int shift) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::WordSar(TNode<WordT> value, TNode<IntegralT> shift) { return TNode<WordT>(); }
TNode<WordT> CodeAssembler::WordSarShiftOutZeros(TNode<WordT> value, TNode<IntegralT> shift) { return TNode<WordT>(); }
TNode<BoolT> CodeAssembler::WordEqual(TNode<WordT> left, TNode<WordT> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::WordNotEqual(TNode<WordT> left, TNode<WordT> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::IntPtrLessThan(TNode<WordT> left, TNode<WordT> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::IntPtrLessThanOrEqual(TNode<WordT> left, TNode<WordT> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::IntPtrGreaterThan(TNode<WordT> left, TNode<WordT> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::IntPtrGreaterThanOrEqual(TNode<WordT> left, TNode<WordT> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::IntPtrEqual(TNode<WordT> left, TNode<WordT> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::UintPtrLessThan(TNode<WordT> left, TNode<WordT> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::UintPtrLessThanOrEqual(TNode<WordT> left, TNode<WordT> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::UintPtrGreaterThan(TNode<WordT> left, TNode<WordT> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::UintPtrGreaterThanOrEqual(TNode<WordT> left, TNode<WordT> right) { return TNode<BoolT>(); }
TNode<PairT<IntPtrT, BoolT>> CodeAssembler::IntPtrAddWithOverflow(TNode<IntPtrT> left, TNode<IntPtrT> right) { return TNode<PairT<IntPtrT, BoolT>>(); }
TNode<PairT<IntPtrT, BoolT>> CodeAssembler::IntPtrSubWithOverflow(TNode<IntPtrT> left, TNode<IntPtrT> right) { return TNode<PairT<IntPtrT, BoolT>>(); }
TNode<PairT<IntPtrT, BoolT>> CodeAssembler::IntPtrMulWithOverflow(TNode<IntPtrT> left, TNode<IntPtrT> right) { return TNode<PairT<IntPtrT, BoolT>>(); }

// ============================================================================
// Float operations
// ============================================================================

TNode<Float32T> CodeAssembler::Float32Abs(TNode<Float32T> value) { return TNode<Float32T>(); }
TNode<Float32T> CodeAssembler::Float32Add(TNode<Float32T> left, TNode<Float32T> right) { return TNode<Float32T>(); }
TNode<Float32T> CodeAssembler::Float32Sub(TNode<Float32T> left, TNode<Float32T> right) { return TNode<Float32T>(); }
TNode<Float32T> CodeAssembler::Float32Mul(TNode<Float32T> left, TNode<Float32T> right) { return TNode<Float32T>(); }
TNode<BoolT> CodeAssembler::Float32Equal(TNode<Float32T> left, TNode<Float32T> right) { return TNode<BoolT>(); }

TNode<Float64T> CodeAssembler::Float64Abs(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Add(TNode<Float64T> left, TNode<Float64T> right) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Sub(TNode<Float64T> left, TNode<Float64T> right) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Mul(TNode<Float64T> left, TNode<Float64T> right) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Div(TNode<Float64T> left, TNode<Float64T> right) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Mod(TNode<Float64T> left, TNode<Float64T> right) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Neg(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Sqrt(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Pow(TNode<Float64T> base, TNode<Float64T> exp) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Max(TNode<Float64T> left, TNode<Float64T> right) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Min(TNode<Float64T> left, TNode<Float64T> right) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Sin(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Cos(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Tan(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Asin(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Acos(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Atan(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Atan2(TNode<Float64T> y, TNode<Float64T> x) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Sinh(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Cosh(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Tanh(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Asinh(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Acosh(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Atanh(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Exp(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Expm1(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Log(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Log1p(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Log2(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Log10(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64Cbrt(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64RoundDown(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64RoundUp(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64RoundTruncate(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64RoundTiesEven(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64SilenceNaN(TNode<Float64T> value) { return TNode<Float64T>(); }
TNode<Uint32T> CodeAssembler::Float64ExtractHighWord32(TNode<Float64T> value) { return TNode<Uint32T>(); }
TNode<Uint32T> CodeAssembler::Float64ExtractLowWord32(TNode<Float64T> value) { return TNode<Uint32T>(); }
TNode<Float64T> CodeAssembler::Float64InsertHighWord32(TNode<Float64T> value, TNode<Word32T> high) { return TNode<Float64T>(); }
TNode<Float64T> CodeAssembler::Float64InsertLowWord32(TNode<Float64T> value, TNode<Word32T> low) { return TNode<Float64T>(); }
TNode<BoolT> CodeAssembler::Float64Equal(TNode<Float64T> left, TNode<Float64T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Float64NotEqual(TNode<Float64T> left, TNode<Float64T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Float64LessThan(TNode<Float64T> left, TNode<Float64T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Float64LessThanOrEqual(TNode<Float64T> left, TNode<Float64T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Float64GreaterThan(TNode<Float64T> left, TNode<Float64T> right) { return TNode<BoolT>(); }
TNode<BoolT> CodeAssembler::Float64GreaterThanOrEqual(TNode<Float64T> left, TNode<Float64T> right) { return TNode<BoolT>(); }

// ============================================================================
// Load/Store operations
// ============================================================================

TNode<Object> CodeAssembler::LoadRoot(RootIndex root_index) { return TNode<Object>(); }
TNode<AnyTaggedT> CodeAssembler::LoadRootMapWord(RootIndex root_index) { return TNode<AnyTaggedT>(); }
TNode<Object> CodeAssembler::LoadFullTagged(Node* base) { return TNode<Object>(); }
TNode<Object> CodeAssembler::LoadFullTagged(Node* base, TNode<IntPtrT> offset) { return TNode<Object>(); }
TNode<RawPtrT> CodeAssembler::LoadFramePointer() { return TNode<RawPtrT>(); }
TNode<RawPtrT> CodeAssembler::LoadParentFramePointer() { return TNode<RawPtrT>(); }
TNode<RawPtrT> CodeAssembler::StackSlotPtr(int size, int alignment) { return TNode<RawPtrT>(); }
TNode<RawPtrT> CodeAssembler::LoadPointerFromRootRegister(TNode<IntPtrT> offset) { return TNode<RawPtrT>(); }
TNode<Object> CodeAssembler::LoadTaggedFromRootRegister(TNode<IntPtrT> offset) { return TNode<Object>(); }
Node* CodeAssembler::LoadProtectedPointerFromObject(TNode<Object> object, TNode<IntPtrT> offset) { return nullptr; }

Node* CodeAssembler::Load(MachineType type, Node* base) { return nullptr; }
Node* CodeAssembler::Load(MachineType type, Node* base, Node* offset) { return nullptr; }
Node* CodeAssembler::LoadFromObject(MachineType type, TNode<Object> object, TNode<IntPtrT> offset) { return nullptr; }

void CodeAssembler::Store(Node* base, Node* offset, Node* value) {}
void CodeAssembler::StoreNoWriteBarrier(MachineRepresentation rep, Node* base, Node* value) {}
void CodeAssembler::StoreNoWriteBarrier(MachineRepresentation rep, Node* base, Node* offset, Node* value) {}
void CodeAssembler::UnsafeStoreNoWriteBarrier(MachineRepresentation rep, Node* base, Node* value) {}
void CodeAssembler::UnsafeStoreNoWriteBarrier(MachineRepresentation rep, Node* base, Node* offset, Node* value) {}
void CodeAssembler::StoreFullTaggedNoWriteBarrier(TNode<RawPtrT> base, TNode<Object> value) {}
void CodeAssembler::StoreFullTaggedNoWriteBarrier(TNode<RawPtrT> base, TNode<IntPtrT> offset, TNode<Object> value) {}
void CodeAssembler::StoreToObject(MachineRepresentation rep, TNode<Object> object, TNode<IntPtrT> offset, Node* value, StoreToObjectWriteBarrier write_barrier) {}
void CodeAssembler::StoreRoot(RootIndex root_index, TNode<Object> value) {}
void CodeAssembler::StoreEphemeronKey(Node* base, Node* offset, Node* value) {}
void CodeAssembler::OptimizedStoreField(MachineRepresentation rep, TNode<HeapObject> object, int offset, Node* value) {}
void CodeAssembler::OptimizedStoreFieldAssertNoWriteBarrier(MachineRepresentation rep, TNode<HeapObject> object, int offset, Node* value) {}
void CodeAssembler::OptimizedStoreFieldUnsafeNoWriteBarrier(MachineRepresentation rep, TNode<HeapObject> object, int offset, Node* value) {}
void CodeAssembler::OptimizedStoreMap(TNode<HeapObject> object, TNode<Map> map) {}

TNode<HeapObject> CodeAssembler::OptimizedAllocate(TNode<IntPtrT> size, AllocationType allocation_type) { return TNode<HeapObject>(); }

// ============================================================================
// Atomic operations
// ============================================================================

Node* CodeAssembler::AtomicLoad(MachineType type, AtomicMemoryOrder order, TNode<RawPtrT> base, TNode<WordT> offset) { return nullptr; }
void CodeAssembler::AtomicStore(MachineRepresentation rep, AtomicMemoryOrder order, TNode<RawPtrT> base, TNode<WordT> offset, TNode<Word32T> value) {}
void CodeAssembler::AtomicStore64(AtomicMemoryOrder order, TNode<RawPtrT> base, TNode<WordT> offset, TNode<UintPtrT> value_low, TNode<UintPtrT> value_high) {}
TNode<Word32T> CodeAssembler::AtomicAdd(MachineType type, TNode<RawPtrT> base, TNode<UintPtrT> offset, TNode<Word32T> value) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::AtomicSub(MachineType type, TNode<RawPtrT> base, TNode<UintPtrT> offset, TNode<Word32T> value) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::AtomicAnd(MachineType type, TNode<RawPtrT> base, TNode<UintPtrT> offset, TNode<Word32T> value) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::AtomicOr(MachineType type, TNode<RawPtrT> base, TNode<UintPtrT> offset, TNode<Word32T> value) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::AtomicXor(MachineType type, TNode<RawPtrT> base, TNode<UintPtrT> offset, TNode<Word32T> value) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::AtomicExchange(MachineType type, TNode<RawPtrT> base, TNode<UintPtrT> offset, TNode<Word32T> value) { return TNode<Word32T>(); }
TNode<Word32T> CodeAssembler::AtomicCompareExchange(MachineType type, TNode<RawPtrT> base, TNode<WordT> offset, TNode<Word32T> old_value, TNode<Word32T> new_value) { return TNode<Word32T>(); }

// 64-bit atomic operation template instantiations
template<> TNode<PairT<IntPtrT, IntPtrT>> CodeAssembler::AtomicLoad64<PairT<IntPtrT, IntPtrT>>(AtomicMemoryOrder, TNode<RawPtrT>, TNode<WordT>) { return TNode<PairT<IntPtrT, IntPtrT>>(); }
template<> TNode<PairT<UintPtrT, UintPtrT>> CodeAssembler::AtomicLoad64<PairT<UintPtrT, UintPtrT>>(AtomicMemoryOrder, TNode<RawPtrT>, TNode<WordT>) { return TNode<PairT<UintPtrT, UintPtrT>>(); }
template<> TNode<PairT<IntPtrT, IntPtrT>> CodeAssembler::AtomicAdd64<PairT<IntPtrT, IntPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<IntPtrT, IntPtrT>>(); }
template<> TNode<PairT<UintPtrT, UintPtrT>> CodeAssembler::AtomicAdd64<PairT<UintPtrT, UintPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<UintPtrT, UintPtrT>>(); }
template<> TNode<PairT<IntPtrT, IntPtrT>> CodeAssembler::AtomicSub64<PairT<IntPtrT, IntPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<IntPtrT, IntPtrT>>(); }
template<> TNode<PairT<UintPtrT, UintPtrT>> CodeAssembler::AtomicSub64<PairT<UintPtrT, UintPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<UintPtrT, UintPtrT>>(); }
template<> TNode<PairT<IntPtrT, IntPtrT>> CodeAssembler::AtomicAnd64<PairT<IntPtrT, IntPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<IntPtrT, IntPtrT>>(); }
template<> TNode<PairT<UintPtrT, UintPtrT>> CodeAssembler::AtomicAnd64<PairT<UintPtrT, UintPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<UintPtrT, UintPtrT>>(); }
template<> TNode<PairT<IntPtrT, IntPtrT>> CodeAssembler::AtomicOr64<PairT<IntPtrT, IntPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<IntPtrT, IntPtrT>>(); }
template<> TNode<PairT<UintPtrT, UintPtrT>> CodeAssembler::AtomicOr64<PairT<UintPtrT, UintPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<UintPtrT, UintPtrT>>(); }
template<> TNode<PairT<IntPtrT, IntPtrT>> CodeAssembler::AtomicXor64<PairT<IntPtrT, IntPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<IntPtrT, IntPtrT>>(); }
template<> TNode<PairT<UintPtrT, UintPtrT>> CodeAssembler::AtomicXor64<PairT<UintPtrT, UintPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<UintPtrT, UintPtrT>>(); }
template<> TNode<PairT<IntPtrT, IntPtrT>> CodeAssembler::AtomicExchange64<PairT<IntPtrT, IntPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<IntPtrT, IntPtrT>>(); }
template<> TNode<PairT<UintPtrT, UintPtrT>> CodeAssembler::AtomicExchange64<PairT<UintPtrT, UintPtrT>>(TNode<RawPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<UintPtrT, UintPtrT>>(); }
template<> TNode<PairT<IntPtrT, IntPtrT>> CodeAssembler::AtomicCompareExchange64<PairT<IntPtrT, IntPtrT>>(TNode<RawPtrT>, TNode<WordT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<IntPtrT, IntPtrT>>(); }
template<> TNode<PairT<UintPtrT, UintPtrT>> CodeAssembler::AtomicCompareExchange64<PairT<UintPtrT, UintPtrT>>(TNode<RawPtrT>, TNode<WordT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>, TNode<UintPtrT>) { return TNode<PairT<UintPtrT, UintPtrT>>(); }

// ============================================================================
// Call operations
// ============================================================================

Node* CodeAssembler::CallRuntimeImpl(Runtime::FunctionId function, TNode<Object> context, std::initializer_list<TNode<Object>> args) { return nullptr; }
void CodeAssembler::TailCallRuntimeImpl(Runtime::FunctionId function, TNode<Int32T> arity, TNode<Object> context, std::initializer_list<TNode<Object>> args) {}
void CodeAssembler::TailCallStubImpl(const CallInterfaceDescriptor& descriptor, TNode<Code> target, TNode<Object> context, std::initializer_list<Node*> args) {}
Node* CodeAssembler::CallStubRImpl(StubCallMode call_mode, const CallInterfaceDescriptor& descriptor, TNode<Object> target, TNode<Object> context, std::initializer_list<Node*> args) { return nullptr; }
void CodeAssembler::TailCallStubThenBytecodeDispatchImpl(const CallInterfaceDescriptor& descriptor, Node* target, Node* context, std::initializer_list<Node*> args) {}
Node* CodeAssembler::CallJSStubImpl(const CallInterfaceDescriptor& descriptor, TNode<Object> target, TNode<Object> context, TNode<Object> function, std::optional<TNode<Object>> new_target, TNode<Int32T> argc, std::optional<TNode<JSDispatchHandleT>> dispatch_handle, std::initializer_list<Node*> args) { return nullptr; }
void CodeAssembler::TailCallJSCode(TNode<Code> code, TNode<Context> context, TNode<JSFunction> function, TNode<Object> new_target, TNode<Int32T> argc, TNode<JSDispatchHandleT> dispatch_handle) {}

Node* CodeAssembler::CallCFunction(Node* function, std::optional<MachineType> return_type, std::initializer_list<std::pair<MachineType, Node*>> args) { return nullptr; }
Node* CodeAssembler::CallCFunctionWithoutFunctionDescriptor(Node* function, MachineType return_type, std::initializer_list<std::pair<MachineType, Node*>> args) { return nullptr; }
Node* CodeAssembler::CallCFunctionWithCallerSavedRegisters(Node* function, MachineType return_type, SaveFPRegsMode mode, std::initializer_list<std::pair<MachineType, Node*>> args) { return nullptr; }

// ============================================================================
// Bytecode dispatch
// ============================================================================

template<>
void CodeAssembler::TailCallBytecodeDispatch<TNode<Object>, TNode<IntPtrT>, TNode<BytecodeArray>, TNode<ExternalReference>>(
    const CallInterfaceDescriptor& descriptor, TNode<RawPtrT> target,
    TNode<Object> arg1, TNode<IntPtrT> arg2, TNode<BytecodeArray> arg3, TNode<ExternalReference> arg4) {}

// ============================================================================
// Utility methods
// ============================================================================

bool CodeAssembler::TryToInt32Constant(TNode<IntegralT> node, int32_t* out_value) { return false; }
bool CodeAssembler::TryToIntPtrConstant(TNode<IntegralT> node, intptr_t* out_value) { return false; }
bool CodeAssembler::TryToIntPtrConstant(TNode<Smi> node, intptr_t* out_value) { return false; }
bool CodeAssembler::TryToSmiConstant(TNode<Smi> node, Tagged<Smi>* out_value) { return false; }

Node* CodeAssembler::Projection(int index, Node* value) { return nullptr; }
TNode<BoolT> CodeAssembler::StackPointerGreaterThan(TNode<WordT> value) { return TNode<BoolT>(); }
void CodeAssembler::StaticAssert(TNode<BoolT> condition, const char* message) {}
void CodeAssembler::AbortCSADcheck(Node* message) {}

TNode<RawPtrT> CodeAssembler::SwitchToTheCentralStackIfNeeded() { return TNode<RawPtrT>(); }
void CodeAssembler::SwitchFromTheCentralStack(TNode<RawPtrT> old_sp) {}
void CodeAssembler::SetDynamicJSParameterCount(TNode<Uint16T> count) {}

void CodeAssembler::RegisterCallGenerationCallbacks(const std::function<void()>& before, const std::function<void()>& after) {}
void CodeAssembler::UnregisterCallGenerationCallbacks() {}
void CodeAssembler::PushSourcePosition() {}
void CodeAssembler::PopSourcePosition() {}

TNode<HeapObject> CodeAssembler::UntypedHeapConstantNoHole(Handle<HeapObject> object) { return TNode<HeapObject>(); }

// ============================================================================
// CodeAssemblerState
// ============================================================================

void CodeAssemblerState::SetInitialDebugInformation(const char* msg, const char* file, int line) {}

// ============================================================================
// CodeAssemblerVariable
// ============================================================================

CodeAssemblerVariable::CodeAssemblerVariable(CodeAssembler* assembler, MachineRepresentation rep)
    : impl_(nullptr), state_(nullptr) {}

CodeAssemblerVariable::CodeAssemblerVariable(CodeAssembler* assembler, MachineRepresentation rep, Node* initial_value)
    : impl_(nullptr), state_(nullptr) {}

#if DEBUG
CodeAssemblerVariable::CodeAssemblerVariable(CodeAssembler* assembler, AssemblerDebugInfo debug_info, MachineRepresentation rep)
    : impl_(nullptr), state_(nullptr) {}

CodeAssemblerVariable::CodeAssemblerVariable(CodeAssembler* assembler, AssemblerDebugInfo debug_info, MachineRepresentation rep, Node* initial_value)
    : impl_(nullptr), state_(nullptr) {}
#endif

CodeAssemblerVariable::~CodeAssemblerVariable() {}

void CodeAssemblerVariable::Bind(Node* value) {}
Node* CodeAssemblerVariable::value() const { return nullptr; }
MachineRepresentation CodeAssemblerVariable::rep() const { return MachineRepresentation::kNone; }
bool CodeAssemblerVariable::IsBound() const { return false; }

// ============================================================================
// CodeAssemblerLabel
// ============================================================================

CodeAssemblerLabel::CodeAssemblerLabel(CodeAssembler* assembler, size_t vars_count,
                                       CodeAssemblerVariable* const* vars,
                                       CodeAssemblerLabel::Type type)
    : bound_(false), merge_count_(0), state_(nullptr), label_(nullptr) {}

CodeAssemblerLabel::~CodeAssemblerLabel() {}

// ============================================================================
// CodeAssemblerParameterizedLabelBase
// ============================================================================

void CodeAssemblerParameterizedLabelBase::AddInputs(std::vector<Node*> inputs) {}

Node* CodeAssemblerParameterizedLabelBase::CreatePhi(MachineRepresentation rep, const std::vector<Node*>& inputs) { return nullptr; }

const std::vector<Node*>& CodeAssemblerParameterizedLabelBase::CreatePhis(std::vector<MachineRepresentation> reps) {
  static std::vector<Node*> empty_vector;
  return empty_vector;
}

// ============================================================================
// ScopedExceptionHandler
// ============================================================================

ScopedExceptionHandler::ScopedExceptionHandler(CodeAssembler* assembler,
                                               CodeAssemblerExceptionHandlerLabel* label)
    : has_handler_(false), assembler_(nullptr), compatibility_label_(nullptr), label_(nullptr), exception_(nullptr) {}

ScopedExceptionHandler::ScopedExceptionHandler(CodeAssembler* assembler, CodeAssemblerLabel* label,
                                               TypedCodeAssemblerVariable<Object>* exception)
    : has_handler_(false), assembler_(nullptr), compatibility_label_(nullptr), label_(nullptr), exception_(nullptr) {}

ScopedExceptionHandler::~ScopedExceptionHandler() {}

// ============================================================================
// BuiltinCompilationScheduler
// ============================================================================

CodeAssembler::BuiltinCompilationScheduler::~BuiltinCompilationScheduler() {}

void CodeAssembler::BuiltinCompilationScheduler::AwaitAndFinalizeCurrentBatch(Isolate* isolate) {}

void CodeAssembler::BuiltinCompilationScheduler::CompileCode(
    Isolate* isolate, std::unique_ptr<TurbofanCompilationJob> job) {
  // WASI: We can't actually compile TF/TS builtins (no native code generation).
  // Just count them as installed - they'll keep their placeholder code.
  builtins_installed_count_++;
}

// ============================================================================
// Pipeline stubs
// ============================================================================

std::unique_ptr<TurbofanCompilationJob> Pipeline::NewCSLinkageCodeStubBuiltinCompilationJob(
    Isolate* isolate, Builtin builtin,
    std::function<void(CodeAssemblerState*)> generator,
    std::function<void(Builtin, Handle<Code>)> set_builtin_code,
    const AssemblerOptions& assembler_options,
    CallDescriptors::Key call_descriptor_key, const char* name,
    const ProfileDataFromFile* profile_data, int stub_key) {
  return nullptr;
}

std::unique_ptr<TurbofanCompilationJob> Pipeline::NewJSLinkageCodeStubBuiltinCompilationJob(
    Isolate* isolate, Builtin builtin,
    std::function<void(CodeAssemblerState*)> generator,
    std::function<void(Builtin, Handle<Code>)> set_builtin_code,
    const AssemblerOptions& assembler_options,
    int argc, const char* name, const ProfileDataFromFile* profile_data,
    int stub_key) {
  return nullptr;
}

std::unique_ptr<TurbofanCompilationJob> Pipeline::NewBytecodeHandlerCompilationJob(
    Isolate* isolate, Builtin builtin,
    std::function<void(CodeAssemblerState*)> generator,
    std::function<void(Builtin, Handle<Code>)> set_builtin_code,
    const AssemblerOptions& assembler_options, const char* name,
    const ProfileDataFromFile* profile_data, int stub_key) {
  return nullptr;
}

}  // namespace compiler

// ============================================================================
// HandleScopeImplementer static member
// ============================================================================

const size_t HandleScopeImplementer::kEnteredContextsOffset = 0;

// ============================================================================
// Builtins stubs
// ============================================================================

void Builtins::Generate_InterpreterOnStackReplacement_ToBaseline(MacroAssembler* masm) {
  // WASM32 does not support OSR to baseline
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
