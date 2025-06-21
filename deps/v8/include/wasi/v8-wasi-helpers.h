#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
// WASI 环境的辅助定义
#ifndef V8_INCLUDE_WASI_HELPERS_H_
#define V8_INCLUDE_WASI_HELPERS_H_

#include <cstddef>

namespace v8 {

// 基本的 StackAllocated 模板类实现
template <bool check_statically_enabled = false>
class StackAllocated {
 public:
  StackAllocated() = default;
  virtual ~StackAllocated() = default;
};

// 简化的 HandleScope 实现
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

// 简化的 Local 模板类
template <class T>
class Local {
 public:
  Local() : ptr_(nullptr) {}
  explicit Local(T* val) : ptr_(val) {}
  
  bool IsEmpty() const { return ptr_ == nullptr; }
  T* operator->() const { return ptr_; }
  T* operator*() const { return ptr_; }
  
 private:
  T* ptr_;
};

// 其他必要的辅助类和函数...

} // namespace v8

#endif // V8_INCLUDE_WASI_HELPERS_H_
