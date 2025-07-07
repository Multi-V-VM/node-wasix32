#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
// Copyright 2021 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_V8_VALUE_H_
#define INCLUDE_V8_VALUE_H_

#include "v8-data.h"          // NOLINT(build/include_directory)
#include "v8config.h"         // NOLINT(build/include_directory)

namespace v8 {

class Isolate;

/**
 * The superclass of all JavaScript values and objects.
 */
class V8_EXPORT Value : public Data {
 public:
  /**
   * Returns true if this value is the undefined value.  See ECMA-262
   * 4.3.10.
   */
  V8_INLINE bool IsUndefined() const;

  /**
   * Returns true if this value is the null value.  See ECMA-262
   * 4.3.11.
   */
  V8_INLINE bool IsNull() const;

  /**
   * Returns true if this value is either the null or the undefined value.
   * See ECMA-262
   * 4.3.11. and 4.3.12
   */
  V8_INLINE bool IsNullOrUndefined() const;

  /**
   * Returns true if this value is true.
   */
  bool IsTrue() const;

  /**
   * Returns true if this value is false.
   */
  bool IsFalse() const;

  /**
   * Returns true if this value is a symbol or a string.
   */
  bool IsName() const;

  /**
   * Returns true if this value is an instance of the String type.
   * See ECMA-262 8.4.
   */
  V8_INLINE bool IsString() const;

  /**
   * Returns true if this value is a symbol.
   */
  bool IsSymbol() const;

  /**
   * Returns true if this value is a function.
   */
  bool IsFunction() const;

  /**
   * Returns true if this value is an array. Note that it will return false for
   * an Proxy for an array.
   */
  bool IsArray() const;

  /**
   * Returns true if this value is an object.
   */
  bool IsObject() const;

  /**
   * Returns true if this value is a bigint.
   */
  bool IsBigInt() const;

  /**
   * Returns true if this value is boolean.
   */
  bool IsBoolean() const;

  /**
   * Returns true if this value is a number.
   */
  bool IsNumber() const;

  /**
   * Returns true if this value is an `External` object.
   */
  bool IsExternal() const;

  /**
   * Returns true if this value is a 32-bit signed integer.
   */
  bool IsInt32() const;

  /**
   * Returns true if this value is a 32-bit unsigned integer.
   */
  bool IsUint32() const;

  /**
   * Returns true if this value is a Date.
   */
  bool IsDate() const;

  /**
   * Returns true if this value is an Arguments object.
   */
  bool IsArgumentsObject() const;

  /**
   * Returns true if this value is a BigInt object.
   */
  bool IsBigIntObject() const;

  /**
   * Returns true if this value is a Boolean object.
   */
  bool IsBooleanObject() const;

  /**
   * Returns true if this value is a Number object.
   */
  bool IsNumberObject() const;

  /**
   * Returns true if this value is a String object.
   */
  bool IsStringObject() const;

  /**
   * Returns true if this value is a Symbol object.
   */
  bool IsSymbolObject() const;

  /**
   * Returns true if this value is a NativeError.
   */
  bool IsNativeError() const;

  /**
   * Returns true if this value is a RegExp.
   */
  bool IsRegExp() const;

  /**
   * Returns true if this value is an async function.
   */
  bool IsAsyncFunction() const;

  /**
   * Returns true if this value is a Generator function.
   */
  bool IsGeneratorFunction() const;

  /**
   * Returns true if this value is a Generator object (iterator).
   */
  bool IsGeneratorObject() const;

  /**
   * Returns true if this value is a Promise.
   */
  bool IsPromise() const;

  /**
   * Returns true if this value is a Map.
   */
  bool IsMap() const;

  /**
   * Returns true if this value is a Set.
   */
  bool IsSet() const;

  /**
   * Returns true if this value is a Map Iterator.
   */
  bool IsMapIterator() const;

  /**
   * Returns true if this value is a Set Iterator.
   */
  bool IsSetIterator() const;

  /**
   * Returns true if this value is a WeakMap.
   */
  bool IsWeakMap() const;

  /**
   * Returns true if this value is a WeakSet.
   */
  bool IsWeakSet() const;

  /**
   * Returns true if this value is a WeakRef.
   */
  bool IsWeakRef() const;

  /**
   * Returns true if this value is an ArrayBuffer.
   */
  bool IsArrayBuffer() const;

  /**
   * Returns true if this value is an ArrayBufferView.
   */
  bool IsArrayBufferView() const;

  /**
   * Returns true if this value is one of TypedArrays.
   */
  bool IsTypedArray() const;

  /**
   * Returns true if this value is an Uint8Array.
   */
  bool IsUint8Array() const;

  /**
   * Returns true if this value is an Uint8ClampedArray.
   */
  bool IsUint8ClampedArray() const;

  /**
   * Returns true if this value is an Int8Array.
   */
  bool IsInt8Array() const;

  /**
   * Returns true if this value is an Uint16Array.
   */
  bool IsUint16Array() const;

  /**
   * Returns true if this value is an Int16Array.
   */
  bool IsInt16Array() const;

  /**
   * Returns true if this value is an Uint32Array.
   */
  bool IsUint32Array() const;

  /**
   * Returns true if this value is an Int32Array.
   */
  bool IsInt32Array() const;

  /**
   * Returns true if this value is a Float16Array.
   */
  bool IsFloat16Array() const;

  /**
   * Returns true if this value is a Float32Array.
   */
  bool IsFloat32Array() const;

  /**
   * Returns true if this value is a Float64Array.
   */
  bool IsFloat64Array() const;

  /**
   * Returns true if this value is a BigInt64Array.
   */
  bool IsBigInt64Array() const;

  /**
   * Returns true if this value is a BigUint64Array.
   */
  bool IsBigUint64Array() const;

  /**
   * Returns true if this value is a DataView.
   */
  bool IsDataView() const;

  /**
   * Returns true if this value is a SharedArrayBuffer.
   */
  bool IsSharedArrayBuffer() const;

  /**
   * Returns true if this value is a JavaScript Proxy.
   */
  bool IsProxy() const;

  /**
   * Returns true if this value is a WasmMemoryObject.
   */
  bool IsWasmMemoryObject() const;

  /**
   * Returns true if this value is a WasmModuleObject.
   */
  bool IsWasmModuleObject() const;

  /**
   * Returns true if this value is the WasmNull object.
   */
  bool IsWasmNull() const;

  /**
   * Returns true if this value is a Module Namespace Object.
   */
  bool IsModuleNamespaceObject() const;

  /**
   * Cast functions
   */
  V8_INLINE static Value* Cast(Data* data) {
#ifdef V8_ENABLE_CHECKS
    CheckCast(data);
#endif
    return static_cast<Value*>(data);
  }

 private:
  static void CheckCast(Data* that);
};

}  // namespace v8

#endif  // INCLUDE_V8_VALUE_H_