// WASI-specific abort-mode symbols to satisfy linker requirements
#ifdef __wasi__
namespace v8 {
namespace base {
// Minimal enum declaration for WASI
enum class AbortMode { kDefault };
// Provide minimal symbol
AbortMode g_abort_mode = AbortMode::kDefault;
}  // namespace base
}  // namespace v8
#endif