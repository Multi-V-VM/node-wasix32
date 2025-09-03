#ifndef WASI_V8_SANDBOX_STUBS_H_
#define WASI_V8_SANDBOX_STUBS_H_

// This file provides V8 sandbox-related stubs for WASI builds
// Include this before V8 headers that use sandbox features

#ifdef __wasi__

// Intentionally minimal: rely on definitions in deps/v8/include/wasi/nuclear-fix.h
// to avoid duplicate symbol or type mismatches.

#endif // __wasi__

#endif // WASI_V8_SANDBOX_STUBS_H_
