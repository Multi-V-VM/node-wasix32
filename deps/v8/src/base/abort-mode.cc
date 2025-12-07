// WASI and cross-compilation stub for abort-mode symbols
// This provides minimal definitions for code generators and host tools built with WASI flags
#if defined(__wasi__) || !defined(V8_COMPRESS_POINTERS)
namespace v8 {
namespace base {
// Minimal enum declaration
enum class AbortMode { kDefault };
// Provide minimal symbol for linker
AbortMode g_abort_mode = AbortMode::kDefault;
}  // namespace base
}  // namespace v8
#endif