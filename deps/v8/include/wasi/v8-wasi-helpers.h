#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
// WASI 环境的辅助定义
#ifndef V8_INCLUDE_WASI_HELPERS_H_
#define V8_INCLUDE_WASI_HELPERS_H_

#include <cstddef>

namespace v8 {

// HandleScope is now defined in v8-handlescope-fix.h
// Remove duplicate definition to avoid conflicts

// 其他必要的辅助类和函数...

} // namespace v8

#endif // V8_INCLUDE_WASI_HELPERS_H_
