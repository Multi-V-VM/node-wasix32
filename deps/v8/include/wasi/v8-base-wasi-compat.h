// Copyright 2024 the V8 project authors. All rights reserved.
// WASI compatibility layer for V8 base types
#ifndef V8_INCLUDE_WASI_V8_BASE_WASI_COMPAT_H_
#define V8_INCLUDE_WASI_V8_BASE_WASI_COMPAT_H_

#ifdef __wasi__

// This header provides WASI-compatible implementations of V8 base types
// It should be included AFTER the regular V8 headers to avoid conflicts

namespace v8 {
namespace base {

// Only define these if the original headers haven't defined them
#ifndef V8_BASE_PLATFORM_MUTEX_H_

// Forward declaration for friend class
class ConditionVariable;

// Simple Mutex stub for WASI
class Mutex {
 public:
  Mutex() 
#ifdef DEBUG
    : level_(0) 
#endif
  {}
  ~Mutex() {
#ifdef DEBUG
    DCHECK_EQ(0, level_);
#endif
  }
  
  void Lock() { 
    /* No-op for WASI */ 
#ifdef DEBUG
    AssertUnheldAndMark();
#endif
  }
  
  void Unlock() { 
#ifdef DEBUG
    AssertHeldAndUnmark();
#endif
    /* No-op for WASI */ 
  }
  
  bool TryLock() { 
#ifdef DEBUG
    if (level_ == 0) {
      AssertUnheldAndMark();
      return true;
    }
    return false;
#else
    return true;
#endif
  }
  
  void AssertHeld() const {
#ifdef DEBUG
    DCHECK_EQ(1, level_);
#endif
  }

 private:
  friend class ConditionVariable;
  
  void AssertHeldAndUnmark() {
#ifdef DEBUG
    DCHECK_EQ(1, level_);
    level_--;
#endif
  }
  
  void AssertUnheldAndMark() {
#ifdef DEBUG
    DCHECK_EQ(0, level_);
    level_++;
#endif
  }
  
  // For compatibility with absl::ConditionVariable expectations
  void* native_handle_;
  
#ifdef DEBUG
  int level_;
#endif
  
  Mutex(const Mutex&) = delete;
  Mutex& operator=(const Mutex&) = delete;
};

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

// LockGuard is an alias for MutexGuard<Mutex>
using LockGuard = MutexGuard<Mutex>;

#endif // V8_BASE_PLATFORM_MUTEX_H_

// bit_cast template
#ifndef V8_BASE_MACROS_H_
template<typename Dest, typename Source>
inline Dest bit_cast(Source const& source) {
  static_assert(sizeof(Dest) == sizeof(Source), "Size mismatch in bit_cast");
  Dest dest;
  memcpy(&dest, &source, sizeof(dest));
  return dest;
}
#endif // V8_BASE_MACROS_H_

// Vector and Address types
#ifndef V8_BASE_VECTOR_H_
using Address = uintptr_t;
constexpr Address kNullAddress = 0;

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
  
  // SubVector method
  Vector<T> SubVector(size_t from, size_t to) const {
    return Vector<T>(start_ + from, to - from);
  }
  
 private:
  T* start_;
  size_t length_;
};
#endif // V8_BASE_VECTOR_H_

}  // namespace base
}  // namespace v8

#endif  // __wasi__

#endif  // V8_INCLUDE_WASI_V8_BASE_WASI_COMPAT_H_