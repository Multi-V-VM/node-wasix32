#ifndef WASI_V8_INITIALIZATION_FUNCTIONS_H_
#define WASI_V8_INITIALIZATION_FUNCTIONS_H_

// Functions needed for v8-initialization.h
namespace v8 {
namespace internal {

// These functions are needed by v8-initialization.h
inline bool PointerCompressionIsEnabled() { return false; }
inline bool SandboxIsEnabled() { return false; }
// SmiValuesAre31Bits is already defined in v8-wasi-compat.h

} // namespace internal
} // namespace v8

#endif // WASI_V8_INITIALIZATION_FUNCTIONS_H_