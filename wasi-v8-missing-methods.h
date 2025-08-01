#ifndef WASI_V8_MISSING_METHODS_H_
#define WASI_V8_MISSING_METHODS_H_

// This file provides missing V8 API method implementations for WASI builds
// Include this after v8.h but before any Node.js source files

#ifdef __wasi__

#include <cmath>

// Currently empty - methods will be added here as needed
// when we encounter actual missing method errors during linking

namespace v8 {

// Forward declarations
template<typename T> class Local;
class Value;
class Boolean;
class Number;
class String;
class Isolate;

// Missing ValueToBoolean function implementation
inline Local<Boolean> ValueToBoolean(Local<Value> value, Isolate* isolate) {
  // Simple stub implementation for WASI build
  // In a real V8 environment, this would do proper type conversion
  if (value.IsEmpty()) {
    return Boolean::New(isolate, false);
  }
  
  // For WASI build, provide a basic implementation
  // Check common cases that would be falsy in JavaScript
  if (value->IsNull() || value->IsUndefined()) {
    return Boolean::New(isolate, false);
  }
  if (value->IsBoolean()) {
    return value.As<Boolean>();
  }
  if (value->IsNumber()) {
    double num_value = value.As<v8::Number>()->Value();
    bool is_truthy = (num_value != 0.0) && !std::isnan(num_value);
    return Boolean::New(isolate, is_truthy);
  }
  if (value->IsString()) {
    Local<String> str = value.As<String>();
    bool is_truthy = str->Length() > 0;
    return Boolean::New(isolate, is_truthy);
  }
  
  // Default to true for objects and other types
  return Boolean::New(isolate, true);
}

} // namespace v8

#endif // __wasi__

#endif // WASI_V8_MISSING_METHODS_H_