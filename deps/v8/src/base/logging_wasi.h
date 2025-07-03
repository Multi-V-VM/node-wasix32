#ifdef __wasi__
// WASI-specific logging implementation
#include <iostream>
#include <sstream>

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

#endif  // __wasi__
