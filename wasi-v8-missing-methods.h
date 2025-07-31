#ifndef WASI_V8_MISSING_METHODS_H_
#define WASI_V8_MISSING_METHODS_H_

// This file provides missing V8 API method implementations for WASI builds
// Include this after v8.h but before any Node.js source files

#ifdef __wasi__

// Currently empty - methods will be added here as needed
// when we encounter actual missing method errors during linking

namespace v8 {
// Add any missing V8 method implementations here
} // namespace v8

#endif // __wasi__

#endif // WASI_V8_MISSING_METHODS_H_