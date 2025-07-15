// Copyright 2024 the V8 project authors. All rights reserved.
// WASI build base type includes
#ifndef V8_INCLUDE_WASI_V8_BASE_INCLUDES_H_
#define V8_INCLUDE_WASI_V8_BASE_INCLUDES_H_

#ifdef __wasi__

// When building for WASI, we need to ensure V8's base types are available
// This header provides the necessary includes or stubs

// Check if we're in a context where V8 base headers are available
#ifdef V8_BASE_BUILD

// Include the actual V8 base headers
#include "src/base/macros.h"
#include "src/base/platform/mutex.h"
#include "src/base/vector.h"

#else

// Provide minimal stubs for contexts where V8 base headers aren't available
#include <cstring>
#include <cstdint>
#include <cstddef>

namespace v8 {
namespace base {

// Only define if not already defined
#ifndef V8_WASI_BASE_STUBS_DEFINED
#define V8_WASI_BASE_STUBS_DEFINED

// bit_cast implementation
template <class Dest, class Source>
inline Dest bit_cast(Source const& source) {
  static_assert(sizeof(Dest) == sizeof(Source), "Size mismatch in bit_cast");
  Dest dest;
  std::memcpy(&dest, &source, sizeof(dest));
  return dest;
}

// Forward declarations
class Mutex;
class ConditionVariable;

// MutexGuard template
template<typename MutexType>
class MutexGuard {
 public:
  explicit MutexGuard(MutexType* mutex) : mutex_(mutex) {
    if (mutex_) mutex_->Lock();
  }
  ~MutexGuard() {
    if (mutex_) mutex_->Unlock();
  }
  
 private:
  MutexType* mutex_;
  MutexGuard(const MutexGuard&) = delete;
  MutexGuard& operator=(const MutexGuard&) = delete;
};

// Simple Mutex stub
class Mutex {
 public:
  Mutex() {}
  ~Mutex() {}
  void Lock() {}
  void Unlock() {}
  bool TryLock() { return true; }
  void AssertHeld() const {}
  
 private:
  friend class ConditionVariable;
  void AssertHeldAndUnmark() {}
  void AssertUnheldAndMark() {}
  
  void* native_handle_;
  
  Mutex(const Mutex&) = delete;
  Mutex& operator=(const Mutex&) = delete;
};

// LockGuard alias
using LockGuard = MutexGuard<Mutex>;

// Vector template
template<typename T>
class Vector {
 public:
  Vector() : start_(nullptr), length_(0) {}
  Vector(T* data, size_t length) : start_(data), length_(length) {}
  
  T* begin() const { return start_; }
  T* end() const { return start_ + length_; }
  size_t length() const { return length_; }
  bool is_empty() const { return length_ == 0; }
  
  T& operator[](size_t index) { return start_[index]; }
  const T& operator[](size_t index) const { return start_[index]; }
  
  T* data() const { return start_; }
  
  Vector<T> SubVector(size_t from, size_t to) const {
    return Vector<T>(start_ + from, to - from);
  }
  
 private:
  T* start_;
  size_t length_;
};

#endif // V8_WASI_BASE_STUBS_DEFINED

}  // namespace base
}  // namespace v8

#endif // V8_BASE_BUILD

#endif // __wasi__

#endif // V8_INCLUDE_WASI_V8_BASE_INCLUDES_H_