#ifndef WASI_V8_VALUE_METHODS_H
#define WASI_V8_VALUE_METHODS_H

// This file provides additional V8 Value methods for WASI builds

namespace v8 {

// Value methods that may be missing in WASI builds
inline MaybeLocal<String> Value::ToString(Local<Context> context) const {
    // Return empty string for WASI builds
    return MaybeLocal<String>();
}

inline MaybeLocal<Number> Value::ToNumber(Local<Context> context) const {
    // Return empty number for WASI builds
    return MaybeLocal<Number>();
}

inline MaybeLocal<Integer> Value::ToInteger(Local<Context> context) const {
    // Return empty integer for WASI builds
    return MaybeLocal<Integer>();
}

inline MaybeLocal<Object> Value::ToObject(Local<Context> context) const {
    // Return empty object for WASI builds
    return MaybeLocal<Object>();
}

} // namespace v8

#endif // WASI_V8_VALUE_METHODS_H
EOF < /dev/null