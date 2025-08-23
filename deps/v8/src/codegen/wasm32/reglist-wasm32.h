#ifndef V8_CODEGEN_WASM32_REGLIST_WASM32_H_
#define V8_CODEGEN_WASM32_REGLIST_WASM32_H_

#include "src/codegen/register-base.h"
#include "src/codegen/wasm32/register-wasm32.h"
#include "src/codegen/wasm32/wasi-v8-reglist-fix.h"

namespace v8 {
namespace internal {

// For WASM32, we only define these if not already defined by IA32
#if defined(V8_TARGET_ARCH_IA32)
// Use IA32 register definitions
#include "src/codegen/ia32/register-ia32.h"
#else
// Define WASM32-specific registers
// This would be the WASM32 register definitions
#endif

}  // namespace internal
}  // namespace v8

#endif  // V8_CODEGEN_WASM32_REGLIST_WASM32_H_
