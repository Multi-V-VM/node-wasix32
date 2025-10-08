#ifdef __wasi__
// WASI-specific logging implementation
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

namespace v8 {
namespace base {

class CheckMessageStream {
 public:
  CheckMessageStream() = default;
  template<typename T>
  CheckMessageStream& operator<<(const T& value) {
    stream_ << value;
    return *this;
  }
  
  std::string str() const { return stream_.str(); }
  
 private:
  std::ostringstream stream_;
};

// Simple template for operator<< support
// Removed to avoid ambiguity - use the member function instead

}  // namespace base
}  // namespace v8

// V8_Fatal and V8_Dcheck macros for WASI
#ifndef V8_Fatal
#define V8_Fatal(...) do { \
  fprintf(stderr, "V8 Fatal: "); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  abort(); \
} while (0)
#endif

#ifndef V8_Dcheck
#define V8_Dcheck(...) do { \
  fprintf(stderr, "V8 Dcheck: "); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
} while (0)
#endif

#ifndef CHECK_NULL
#define CHECK_NULL(val) do { \
  if ((val) == nullptr) { \
    V8_Fatal("CHECK_NULL failed: %s is null", #val); \
  } \
} while (0)
#endif

#ifndef DCHECK_NOT_NULL
#define DCHECK_NOT_NULL(val) (val)
#endif

#endif  // __wasi__
