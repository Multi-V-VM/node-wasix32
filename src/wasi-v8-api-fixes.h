#ifndef SRC_WASI_V8_API_FIXES_H_
#define SRC_WASI_V8_API_FIXES_H_

#ifdef __wasi__

#include "v8.h"

// JustVoid is now provided by v8-maybe-specialization.h
// No additional fix needed here

// V8 Persistent handle fixes
namespace v8 {
namespace internal {
// Helper to make persistent handle API work with WASI
template<typename T>
inline T* AddressOf(const T& ref) {
  return const_cast<T*>(&ref);
}
}
}

// Override problematic Persistent methods
#define V8_PERSISTENT_BASE_GET_FIX 1

// Add ptr() method to Local<T>
template <typename T>
struct LocalPtrFix {
  static internal::Address* ptr(const Local<T>& local) {
    return reinterpret_cast<internal::Address*>(const_cast<T*>(*local));
  }
};

}  // namespace v8

// Fix UNREACHABLE macro for WASI
#ifdef UNREACHABLE
#undef UNREACHABLE
#endif
#define UNREACHABLE(...) \
  do { \
    fprintf(stderr, "UNREACHABLE: %s\n", ##__VA_ARGS__); \
    __builtin_unreachable(); \
  } while (0)

namespace v8 {

// simdutf function name fixes
namespace simdutf {
// Map old function names to new ones
inline auto validate_utf8_with_errors(const char* data, size_t length) {
  // Return a simple result struct
  struct result {
    bool valid;
    size_t pos;
  };
  // Basic UTF-8 validation
  for (size_t i = 0; i < length; i++) {
    unsigned char c = data[i];
    if (c > 0x7F) {
      // Simplified: just check for invalid high bits
      if ((c & 0xC0) == 0x80) {
        return result{false, i};
      }
    }
  }
  return result{true, length};
}

inline size_t convert_latin1_to_utf8(const char* src, size_t len, char* dst) {
  // Simple Latin-1 to UTF-8 conversion
  size_t written = 0;
  for (size_t i = 0; i < len; i++) {
    unsigned char c = src[i];
    if (c < 0x80) {
      dst[written++] = c;
    } else {
      dst[written++] = 0xC0 | (c >> 6);
      dst[written++] = 0x80 | (c & 0x3F);
    }
  }
  return written;
}
}

#endif // __wasi__

#endif // SRC_WASI_V8_API_FIXES_H_