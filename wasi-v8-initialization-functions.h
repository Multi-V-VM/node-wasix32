#ifndef WASI_V8_INITIALIZATION_FUNCTIONS_H_
#define WASI_V8_INITIALIZATION_FUNCTIONS_H_

// Functions needed for v8-initialization.h
namespace v8 {
namespace internal {

// These functions might be needed before nuclear-fix.h is included
// Use guards to prevent redefinition
#ifndef V8_INCLUDE_WASI_NUCLEAR_FIX_H_
inline bool PointerCompressionIsEnabled() { return false; }
inline bool SandboxIsEnabled() { return false; }
#endif
// SmiValuesAre31Bits is already defined in v8-wasi-compat.h

} // namespace internal
} // namespace v8

#endif // WASI_V8_INITIALIZATION_FUNCTIONS_H_