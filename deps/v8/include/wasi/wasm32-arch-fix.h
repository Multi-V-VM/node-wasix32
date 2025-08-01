#ifndef V8_INCLUDE_WASI_WASM32_ARCH_FIX_H_
#define V8_INCLUDE_WASI_WASM32_ARCH_FIX_H_

// WASM32 architecture definition for V8 WASI builds
#ifdef __wasi__

// Define WASM32 target architecture
#ifndef V8_TARGET_ARCH_WASM32
#define V8_TARGET_ARCH_WASM32 1
#endif

// Architecture-specific constants
#define V8_HOST_ARCH_WASM32 1

// Register definitions for WASM32
namespace v8 {
namespace internal {

// Register constants for WASM32
constexpr int kNumRegisters = 0;  // WASM32 has no general-purpose registers
constexpr int kNumDoubleRegisters = 0;

// Stack pointer and frame pointer definitions
constexpr int kSPRegister = -1;  // No stack pointer register in WASM32
constexpr int kFPRegister = -1;  // No frame pointer register

// Calling convention constants
constexpr int kMaxRegularParameters = 8;
constexpr int kMaxDoubleParameters = 8;

} // namespace internal
} // namespace v8

// Architecture feature detection
#define V8_TARGET_LITTLE_ENDIAN 1
#define V8_TARGET_BIG_ENDIAN 0

// Memory alignment - only define if not already defined
#ifndef V8_TARGET_ARCH_STORES_RETURN_ADDRESS_ON_STACK
#define V8_TARGET_ARCH_STORES_RETURN_ADDRESS_ON_STACK 0
#endif

// WASM32-specific macros
#define V8_WASM32_ARCH 1

#endif // __wasi__

#endif // V8_INCLUDE_WASI_WASM32_ARCH_FIX_H_