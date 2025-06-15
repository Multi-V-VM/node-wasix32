/* WebAssembly/WASI config for V8 */
#define V8_TARGET_ARCH_WASM 1
#define V8_TARGET_ARCH_32_BIT 1
#define V8_ATOMIC_OBJECT_FIELD_WRITES 1
/* Remove architecture detection errors */
#undef V8_HOST_ARCH_ARM
#undef V8_HOST_ARCH_ARM64
#undef V8_HOST_ARCH_IA32
#undef V8_HOST_ARCH_X64
#define V8_HOST_ARCH_WASM 1
#define V8_HAVE_TARGET_OS 1
#define V8_TARGET_OS_WASI 1
