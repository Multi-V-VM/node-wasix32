#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_V8_TRACED_HANDLE_H_
#define V8_V8_TRACED_HANDLE_H_

#include "v8config.h"

namespace v8 {

// 为 WASI 提供最小的追踪句柄支持
template <class T>
class TracedHandle {
 public:
  TracedHandle() = default;
  ~TracedHandle() = default;

  // 禁用拷贝和赋值
  TracedHandle(const TracedHandle&) = delete;
  TracedHandle& operator=(const TracedHandle&) = delete;

  // 移动构造和赋值
  TracedHandle(TracedHandle&&) = default;
  TracedHandle& operator=(TracedHandle&&) = default;

  bool IsEmpty() const { return true; }
  void Reset() {}

  T* operator->() const { return nullptr; }
  T* operator*() const { return nullptr; }
};

// 全局追踪句柄
template <class T>
class TracedGlobal : public TracedHandle<T> {
 public:
  TracedGlobal() = default;
  ~TracedGlobal() = default;
};

}  // namespace v8

#endif  // V8_V8_TRACED_HANDLE_H_
