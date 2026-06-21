#ifndef V8_VALUE_INLINE_IMPL_H_
#define V8_VALUE_INLINE_IMPL_H_

#include "v8-value.h"
#include "v8-internal.h"
#include "v8-maybe.h"

namespace v8 {

// Just is already defined in v8-maybe.h, no need to redefine

namespace internal {

inline internal::Address WasiTaggedValueFromApiValue(const Value* value) {
  return reinterpret_cast<internal::Address>(value);
}

inline bool WasiHasStrongHeapObjectTag(internal::Address value) {
  return value > 1 && ((value & 1) == 1);
}

inline int WasiOddballKindFromTaggedValue(internal::Address value) {
  static constexpr int kHeapObjectTag = 1;
  static constexpr int kHeapObjectMapOffset = 0;
  static constexpr int kMapInstanceTypeOffset = 8;
  static constexpr int kOddballKindOffset = 24;
  static constexpr uint16_t kOddballType = 131;
  static constexpr int kSmiTagSize = 1;

  if (!WasiHasStrongHeapObjectTag(value)) return -1;
  internal::Address object = value - kHeapObjectTag;
  internal::Address map =
      *reinterpret_cast<const internal::Address*>(object + kHeapObjectMapOffset);
  if (!WasiHasStrongHeapObjectTag(map)) return -1;
  internal::Address map_object = map - kHeapObjectTag;
  uint16_t instance_type =
      *reinterpret_cast<const uint16_t*>(map_object + kMapInstanceTypeOffset);
  if (instance_type != kOddballType) return -1;
  internal::Address kind_smi =
      *reinterpret_cast<const internal::Address*>(object + kOddballKindOffset);
  return static_cast<int>(static_cast<intptr_t>(kind_smi) >> kSmiTagSize);
}

}  // namespace internal

// Inline implementations for v8::Value methods
inline bool Value::IsUndefined() const {
  return internal::WasiOddballKindFromTaggedValue(
             internal::WasiTaggedValueFromApiValue(this)) == 4;
}

inline bool Value::IsNull() const {
  return internal::WasiOddballKindFromTaggedValue(
             internal::WasiTaggedValueFromApiValue(this)) == 3;
}

inline bool Value::IsNullOrUndefined() const {
  return IsNull() || IsUndefined();
}

inline bool Value::IsString() const {
  return IsName() && !IsSymbol();
}

// Add missing Value methods that Node.js expects
inline Maybe<int64_t> Value::IntegerValue(Local<Context> context) const {
  // For WASI stub, return a default value
  return Just<int64_t>(0);
}

inline Maybe<int32_t> Value::Int32Value(Local<Context> context) const {
  // For WASI stub, return a default value
  return Just<int32_t>(0);
}

inline Maybe<uint32_t> Value::Uint32Value(Local<Context> context) const {
  // For WASI stub, return a default value
  return Just<uint32_t>(0);
}

inline Maybe<double> Value::NumberValue(Local<Context> context) const {
  // For WASI stub, return a default value
  return Just<double>(0.0);
}

// WASI stubs for Full* methods
inline bool Value::FullIsUndefined() const { return IsUndefined(); }
inline bool Value::FullIsNull() const { return IsNull(); }
inline bool Value::FullIsTrue() const { return IsTrue(); }
inline bool Value::FullIsFalse() const { return IsFalse(); }
inline bool Value::FullIsString() const { return IsString(); }
inline bool Value::IsPrimitive() const {
  return IsUndefined() || IsNull() || IsBoolean() || IsNumber() || IsString() || IsSymbol() || IsBigInt();
}
inline bool Value::IsWasmMemoryMapDescriptor() const { return false; }

// WASI stubs for conversion methods
inline MaybeLocal<Number> Value::ToNumber(Local<Context> context) const {
  return MaybeLocal<Number>();
}

inline MaybeLocal<String> Value::ToDetailString(Local<Context> context) const {
  if (IsString()) {
    return MaybeLocal<String>(
        Local<String>::FromAddress(reinterpret_cast<internal::Address>(this)));
  }
  return MaybeLocal<String>();
}

inline Local<String> Value::TypeOf(Isolate* isolate) const {
  return Local<String>();
}

inline MaybeLocal<Boolean> Value::InstanceOf(Local<Context> context, Local<Object> object) const {
  return MaybeLocal<Boolean>();
}

inline MaybeLocal<BigInt> Value::ToBigInt(Local<Context> context) const {
  return MaybeLocal<BigInt>();
}

inline MaybeLocal<Primitive> Value::ToPrimitive(Local<Context> context) const {
  return MaybeLocal<Primitive>();
}

inline MaybeLocal<Numeric> Value::ToNumeric(Local<Context> context) const {
  return MaybeLocal<Numeric>();
}

inline Local<Boolean> Value::ToBoolean(Isolate* isolate) const {
  return Local<Boolean>();
}

inline MaybeLocal<String> Value::ToString(Local<Context> context) const {
  if (IsString()) {
    return MaybeLocal<String>(
        Local<String>::FromAddress(reinterpret_cast<internal::Address>(this)));
  }
  return MaybeLocal<String>();
}

inline MaybeLocal<Integer> Value::ToInteger(Local<Context> context) const {
  return MaybeLocal<Integer>();
}

inline MaybeLocal<Int32> Value::ToInt32(Local<Context> context) const {
  return MaybeLocal<Int32>();
}

inline MaybeLocal<Uint32> Value::ToUint32(Local<Context> context) const {
  return MaybeLocal<Uint32>();
}

inline MaybeLocal<Uint32> Value::ToArrayIndex(Local<Context> context) const {
  return MaybeLocal<Uint32>();
}

inline Maybe<bool> Value::Equals(Local<Context> context, Local<Value> that) const {
  return Just(this == *that);
}

inline uint32_t Value::GetHash() {
  return 0;
}

// WASI: Wasm type check methods
inline bool Value::IsWasmMemoryObject() const { return false; }
inline bool Value::IsWasmModuleObject() const { return false; }
inline bool Value::IsWasmNull() const { return false; }

} // namespace v8
#endif // V8_VALUE_INLINE_IMPL_H_
