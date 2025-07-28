#ifndef V8_INCLUDE_WASI_V8_DEPRECATION_FIX_H_
#define V8_INCLUDE_WASI_V8_DEPRECATION_FIX_H_

// Fix V8_DEPRECATE_SOON macro issues for WASI build

// Define V8_DEPRECATE_SOON as empty for WASI to avoid syntax errors
#ifndef V8_DEPRECATE_SOON
#define V8_DEPRECATE_SOON(message) 
#endif

#endif  // V8_INCLUDE_WASI_V8_DEPRECATION_FIX_H_