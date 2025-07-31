#ifndef WASI_V8_API_ADDITIONS_H
#define WASI_V8_API_ADDITIONS_H

// This file provides additional V8 API definitions for WASI builds
// It should be included AFTER v8.h to extend the V8 namespace

#include <vector>
#include <initializer_list>
#include <cstddef>

// Include necessary V8 headers first
#include "deps/v8/include/v8-value.h"
#include "deps/v8/include/v8-object.h"
#include "deps/v8/include/v8-local-handle.h"
#include "deps/v8/include/v8-primitive.h"

// This header includes our comprehensive V8 API stubs
#include "wasi-v8-api-stubs.h"

namespace v8 {

// Fix for LocalVector constructor issues
template<typename T>
inline LocalVector<T> MakeLocalVector(Isolate* isolate, 
                                     std::initializer_list<Local<T>> init) {
    // Create LocalVector and populate it
    LocalVector<T> result(isolate);
    result.reserve(init.size());
    for (const auto& item : init) {
        result.push_back(item);
    }
    return result;
}

// Value inline methods are now defined in wasi/v8-value-inline-impl.h
// Note: QuickIsString is not implemented for WASI - it should be defined in v8-value.h

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

// Note: V8 Value methods are now implemented directly in v8-value.h with __wasi__ guards

#endif // WASI_V8_API_ADDITIONS_H