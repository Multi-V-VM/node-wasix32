#ifndef WASI_V8_MISSING_TYPES_H_
#define WASI_V8_MISSING_TYPES_H_

#include <vector>

namespace v8 {

// Forward declarations
template <typename T> class Local;

namespace internal {

// FixedArray stub for WASI
class FixedArray {
 public:
  FixedArray() = default;
  ~FixedArray() = default;
  
  int length() const { return 0; }
  void* get(int index) { return nullptr; }
  void set(int index, void* value) {}
};

// PerformCastCheck function
template <typename T>
inline void PerformCastCheck(T* data) {
  // No-op for WASI builds
}

// BackingStoreBase stub
class BackingStoreBase {
 public:
  BackingStoreBase() = default;
  virtual ~BackingStoreBase() = default;
};

} // namespace internal

// LocalVector template
template <typename T>
using LocalVector = std::vector<Local<T>>;

} // namespace v8

#endif  // WASI_V8_MISSING_TYPES_H_