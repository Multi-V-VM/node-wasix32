#ifndef WASI_SIMDUTF_COMPAT_H_
#define WASI_SIMDUTF_COMPAT_H_

// This file provides simdutf compatibility for WASI builds
// Include this before any simdutf usage in Node.js source files

#ifdef __wasi__

#include <cstring>  // for strchr
#include <vector>   // for std::vector

namespace simdutf {

// Error codes for simdutf compatibility
enum class error_code {
  SUCCESS = 0,
  INVALID_BASE64_CHARACTER,
  BASE64_INPUT_REMAINDER,
  // Add other error codes as needed
};

// Result type for WASI compatibility
struct result {
  // Success/error status
  error_code error;
  
  // Number of bytes processed
  size_t count;
  
  result(error_code e = error_code::SUCCESS, size_t c = 0) : error(e), count(c) {}
  
  operator bool() const { return error == error_code::SUCCESS; }
};

// Result with error type for validation functions
struct result_with_error {
  bool valid;
  error_code error;
  
  result_with_error(bool v = true, error_code e = error_code::SUCCESS) 
    : valid(v), error(e) {}
};

// Base64 types
enum base64_type {
  base64_default = 0,
  base64_url = 1
};

// Base64 decoding for WASI
inline result base64_to_binary(const char* input, size_t length, char* output) {
  // Simple base64 decoding implementation for WASI
  static const char base64_chars[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  
  size_t output_length = 0;
  
  for (size_t i = 0; i < length; i += 4) {
    if (i + 4 > length) {
      return result(error_code::BASE64_INPUT_REMAINDER, output_length);
    }
    
    // Decode 4 base64 characters to 3 bytes
    uint32_t val = 0;
    for (int j = 0; j < 4; j++) {
      char c = input[i + j];
      if (c == '=') {
        val = val << 6;
      } else {
        const char* pos = strchr(base64_chars, c);
        if (!pos) {
          return result(error_code::INVALID_BASE64_CHARACTER, output_length);
        }
        val = (val << 6) | (pos - base64_chars);
      }
    }
    
    if (output) {
      output[output_length++] = (val >> 16) & 0xFF;
      if (input[i + 2] != '=') {
        output[output_length++] = (val >> 8) & 0xFF;
      }
      if (input[i + 3] != '=') {
        output[output_length++] = val & 0xFF;
      }
    }
  }
  
  return result(error_code::SUCCESS, output_length);
}

// Overloaded version that accepts base64_type parameter
inline result base64_to_binary(const char* input, size_t length, char* output, base64_type type) {
  // For now, ignore the type parameter and use the same implementation
  return base64_to_binary(input, length, output);
}

// Overloaded version for char16_t* input
inline result base64_to_binary(const char16_t* input, size_t length, char* output) {
  // Convert char16_t to char for processing (assumes ASCII base64 chars)
  std::vector<char> temp(length);
  for (size_t i = 0; i < length; ++i) {
    temp[i] = static_cast<char>(input[i] & 0xFF);
  }
  return base64_to_binary(temp.data(), length, output);
}

// Overloaded version for char16_t* input with base64_type
inline result base64_to_binary(const char16_t* input, size_t length, char* output, base64_type type) {
  // For now, ignore the type parameter
  return base64_to_binary(input, length, output);
}

// Latin1 to UTF8 conversion for WASI
inline size_t convert_latin1_to_utf8_safe(const char* input, size_t length, 
                                         char* utf8_output, size_t utf8_length) {
  size_t pos = 0;
  size_t utf8_pos = 0;
  
  while (pos < length && utf8_pos < utf8_length) {
    unsigned char c = static_cast<unsigned char>(input[pos]);
    
    if (c < 0x80) {
      // ASCII character
      if (utf8_pos >= utf8_length) break;
      utf8_output[utf8_pos++] = c;
    } else {
      // Latin-1 character (0x80-0xFF) -> 2-byte UTF-8
      if (utf8_pos + 1 >= utf8_length) break;
      utf8_output[utf8_pos++] = 0xC0 | (c >> 6);
      utf8_output[utf8_pos++] = 0x80 | (c & 0x3F);
    }
    pos++;
  }
  
  return utf8_pos;
}

// Additional simdutf functions may be added here as needed

} // namespace simdutf

#endif // __wasi__

#endif // WASI_SIMDUTF_COMPAT_H_