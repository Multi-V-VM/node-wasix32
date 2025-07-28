#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
// WASI 环境的辅助定义
#ifndef V8_INCLUDE_WASI_HELPERS_H_
#define V8_INCLUDE_WASI_HELPERS_H_

#include <cstddef>

namespace v8 {

// 简化的 HandleScope 实现
#ifndef V8_HANDLESCOPE_DEFINED
#define V8_HANDLESCOPE_DEFINED
class HandleScope {
 public:
  explicit HandleScope(void* isolate) {}
  ~HandleScope() = default;
};

// 简化的 EscapableHandleScope 实现
class EscapableHandleScope : public HandleScope {
 public:
  explicit EscapableHandleScope(void* isolate) : HandleScope(isolate) {}
  ~EscapableHandleScope() = default;
  
  template <class T>
  T* Escape(T* value) { return value; }
};
#endif // V8_HANDLESCOPE_DEFINED

// 其他必要的辅助类和函数...

} // namespace v8

#endif // V8_INCLUDE_WASI_HELPERS_H_
