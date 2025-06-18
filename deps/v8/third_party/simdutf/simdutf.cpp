// Minimal stub implementation for WASI environment
#include "simdutf.h"

// Ultra minimal implementation - bare functions only
namespace simdutf {

// Implementation
namespace {
  // Simplified implementation class
  class implementation {
  public:
    implementation() {}
    virtual ~implementation() {}
    
    // Minimal implementations of required functions
    virtual size_t utf8_to_utf16(const char* input, size_t length, char16_t* output) const {
      return 0;
    }
    
    virtual size_t utf16_to_utf8(const char16_t* input, size_t length, char* output) const {
      return 0;
    }
  };

  // Return a singleton implementation
  const implementation* get_implementation() {
    static implementation impl;
    return &impl;
  }
}

// Public API implementations
size_t utf8_to_utf16(const char* input, size_t length, char16_t* output) {
  return get_implementation()->utf8_to_utf16(input, length, output);
}

size_t utf16_to_utf8(const char16_t* input, size_t length, char* output) {
  return get_implementation()->utf16_to_utf8(input, length, output);
}

} // namespace simdutf
