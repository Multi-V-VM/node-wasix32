// WASI-specific platform definitions for V8
#ifndef V8_INCLUDE_WASI_V8_PLATFORM_WASI_H_
#define V8_INCLUDE_WASI_V8_PLATFORM_WASI_H_

#include <memory>

namespace v8 {

// Forward declarations of classes used in v8-platform.h
class Task;
class IdleTask;
class SourceLocation;

namespace wasi {
// Empty implementation of unique_ptr for WASI
template <typename T>
class unique_ptr {
 public:
  typedef T* pointer;
  typedef T element_type;

  pointer ptr;

  unique_ptr() : ptr(nullptr) {}
  unique_ptr(T* p) : ptr(p) {}
  ~unique_ptr() { if (ptr) delete ptr; }

  T* get() const { return ptr; }
  T* release() {
    T* tmp = ptr;
    ptr = nullptr;
    return tmp;
  }
  void reset(T* p = nullptr) {
    if (ptr) delete ptr;
    ptr = p;
  }

  T& operator*() const { return *ptr; }
  T* operator->() const { return ptr; }
  explicit operator bool() const { return ptr != nullptr; }

  // Define move constructor and assignment
  unique_ptr(unique_ptr&& other) : ptr(other.release()) {}
  unique_ptr& operator=(unique_ptr&& other) {
    reset(other.release());
    return *this;
  }
};
}  // namespace wasi
}  // namespace v8

#endif  // V8_INCLUDE_WASI_V8_PLATFORM_WASI_H_
