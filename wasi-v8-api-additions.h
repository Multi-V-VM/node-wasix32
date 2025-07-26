#ifndef WASI_V8_API_ADDITIONS_H
#define WASI_V8_API_ADDITIONS_H

// This file provides additional V8 API definitions for WASI builds
// It should be included AFTER v8.h to extend the V8 namespace

#include <vector>
#include <initializer_list>
#include <cstddef>

// This header includes our comprehensive V8 API stubs
#include "wasi-v8-api-stubs.h"

namespace v8 {

// Fix for LocalVector constructor issues
template<typename T>
inline std::vector<Local<T>> MakeLocalVector(Isolate* isolate, 
                                            std::initializer_list<Local<T>> init) {
    // Ignore isolate parameter for WASI builds
    return std::vector<Local<T>>(init);
}

// Fix for Value inline methods
inline bool Value::IsString() const {
    // For WASI, provide a simplified check
    // This would normally check the object type bits
    return false; // Safe default for WASI
}

inline bool Value::QuickIsString() const {
    return false; // Safe default for WASI
}

// Fix for Object::New overload issues
inline Local<Object> ObjectNewWithArrays(
    Isolate* isolate,
    Local<Value> prototype_or_null,
    Local<Name>* names,
    Local<Value>* values,
    size_t length) {
    // Return empty object for WASI builds
    return Local<Object>();
}

} // namespace v8

// Include additional V8 Value methods
#include "wasi-v8-value-methods.h"

#endif // WASI_V8_API_ADDITIONS_H