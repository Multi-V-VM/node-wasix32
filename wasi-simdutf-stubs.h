#ifndef WASI_SIMDUTF_STUBS_H_
#define WASI_SIMDUTF_STUBS_H_

#ifdef __wasi__

#include <cstddef>
#include <cstdint>
#include "wasi-simdutf-compat.h"  // Include this to get the result struct

namespace simdutf {

// Character encoding validation functions
inline result_with_error validate_ascii_with_errors(const char* buf, size_t len) {
  // Simple ASCII validation
  for (size_t i = 0; i < len; i++) {
    if (static_cast<unsigned char>(buf[i]) > 127) {
      return result_with_error(false, error_code::INVALID_BASE64_CHARACTER);
    }
  }
  return result_with_error(true, error_code::SUCCESS);
}

// UTF conversion length calculation functions
inline size_t utf8_length_from_utf16(const char16_t* input, size_t length) {
  // Simple approximation: UTF-8 can be up to 3 bytes per UTF-16 unit
  return length * 3;
}

inline size_t utf8_length_from_latin1(const char* input, size_t length) {
  // Latin1 to UTF-8 can be up to 2 bytes per character
  return length * 2;
}

inline size_t utf16_length_from_utf8(const char* input, size_t length) {
  // UTF-8 to UTF-16: worst case is 1 UTF-16 unit per UTF-8 byte
  return length;
}

// UTF conversion functions
inline size_t convert_utf16_to_utf8(const char16_t* input, size_t length, char* output) {
  // Basic stub implementation - copy as-is (incorrect but compilable)
  for (size_t i = 0; i < length; i++) {
    output[i] = static_cast<char>(input[i] & 0xFF);
  }
  return length;
}

// Convert UTF16 to Latin1
inline size_t convert_utf16_to_latin1(const char16_t* input, size_t length, char* output) {
  // Simple conversion - just copy lower bytes (Latin1 is 8-bit)
  for (size_t i = 0; i < length; i++) {
    output[i] = static_cast<char>(input[i] & 0xFF);
  }
  return length;
}

// Convert Latin1 to UTF16 little-endian
inline size_t convert_latin1_to_utf16le(const char* input, size_t length, char16_t* output) {
  // Latin1 maps directly to first 256 Unicode code points
  for (size_t i = 0; i < length; i++) {
    output[i] = static_cast<char16_t>(static_cast<unsigned char>(input[i]));
  }
  return length;
}

// Convert UTF8 to UTF16 little-endian
inline size_t convert_utf8_to_utf16le(const char* input, size_t length, char16_t* output) {
  // Simple stub - just treat as Latin1
  return convert_latin1_to_utf16le(input, length, output);
}

// Base64 encoding/decoding functions
// base64_type is defined in wasi-simdutf-compat.h

inline size_t base64_length_from_binary(size_t length, base64_type type = base64_default) {
  // Base64 encoding: 4 characters for every 3 bytes, rounded up
  return ((length + 2) / 3) * 4;
}

inline size_t maximal_binary_length_from_base64(const char* input, size_t length) {
  // Base64 decoding: 3 bytes for every 4 characters
  return (length / 4) * 3;
}

inline size_t maximal_binary_length_from_base64(const char16_t* input, size_t length) {
  // Base64 decoding: 3 bytes for every 4 characters (same as char version)
  return (length / 4) * 3;
}

inline size_t binary_to_base64(const char* input, size_t length, char* output, base64_type type = base64_default) {
  // Stub implementation - just return expected length
  return base64_length_from_binary(length, type);
}

// Error codes are defined in wasi-simdutf-compat.h

// Base64 decoding with safety checks
inline result base64_to_binary_safe(const char* input, size_t length, char* output, base64_type type = base64_default) {
  // Simple stub implementation
  size_t output_len = maximal_binary_length_from_base64(input, length);
  if (output_len > 0) {
    // Simulate successful decoding
    return result(error_code::SUCCESS, output_len);
  }
  return result(error_code::INVALID_BASE64_CHARACTER, 0);
}

// Overload that takes output length
inline result base64_to_binary_safe(const char* input, size_t length, char* output, size_t& output_length, base64_type type = base64_default) {
  // Simple stub implementation
  output_length = maximal_binary_length_from_base64(input, length);
  if (output_length > 0) {
    // Simulate successful decoding
    return result(error_code::SUCCESS, output_length);
  }
  return result(error_code::INVALID_BASE64_CHARACTER, 0);
}

// Overload for char16_t input
inline result base64_to_binary_safe(const char16_t* input, size_t length, char* output, size_t& output_length, base64_type type = base64_default) {
  // Simple stub implementation
  output_length = maximal_binary_length_from_base64(static_cast<const char*>(nullptr), length);
  if (output_length > 0) {
    // Simulate successful decoding
    return result(error_code::SUCCESS, output_length);
  }
  return result(error_code::INVALID_BASE64_CHARACTER, 0);
}

// Overload for char16_t without output_length
inline result base64_to_binary_safe(const char16_t* input, size_t length, char* output, base64_type type = base64_default) {
  // Simple stub implementation
  size_t output_len = maximal_binary_length_from_base64(static_cast<const char*>(nullptr), length);
  if (output_len > 0) {
    // Simulate successful decoding
    return result(error_code::SUCCESS, output_len);
  }
  return result(error_code::INVALID_BASE64_CHARACTER, 0);
}

// Convert Latin-1 to UTF-8
inline size_t convert_latin1_to_utf8(const char* src, size_t len, char* dst) {
  size_t dst_len = 0;
  for (size_t i = 0; i < len; i++) {
    unsigned char c = static_cast<unsigned char>(src[i]);
    if (c < 0x80) {
      dst[dst_len++] = static_cast<char>(c);
    } else {
      dst[dst_len++] = static_cast<char>(0xC0 | (c >> 6));
      dst[dst_len++] = static_cast<char>(0x80 | (c & 0x3F));
    }
  }
  return dst_len;
}

// UTF-8 length from Latin-1 - removed duplicate, already defined above

// Additional simdutf functions
inline size_t utf8_length_from_utf16le(const char16_t* input, size_t length) {
  // UTF-16LE is same as UTF-16 for length calculation
  return utf8_length_from_utf16(input, length);
}

inline size_t convert_utf16le_to_utf8(const char16_t* input, size_t length, char* output) {
  // UTF-16LE is same as UTF-16 for conversion
  return convert_utf16_to_utf8(input, length, output);
}

inline bool validate_utf8(const char* input, size_t length) {
  // Simple UTF-8 validation stub
  return true;
}

inline result_with_error validate_utf8_with_errors(const char* input, size_t length) {
  // Simple validation - always succeed for stub
  return result_with_error(true, error_code::SUCCESS);
}

inline bool validate_ascii(const char* input, size_t length) {
  // Check if all bytes are ASCII
  for (size_t i = 0; i < length; i++) {
    if (static_cast<unsigned char>(input[i]) > 127) {
      return false;
    }
  }
  return true;
}

inline size_t convert_utf8_to_utf16(const char* input, size_t length, char16_t* output) {
  // Basic stub implementation
  size_t output_len = 0;
  for (size_t i = 0; i < length && input[i]; i++) {
    output[output_len++] = static_cast<char16_t>(static_cast<unsigned char>(input[i]));
  }
  return output_len;
}

// Version macro
#define SIMDUTF_VERSION "5.7.0"

} // namespace simdutf

#endif // __wasi__

#endif // WASI_SIMDUTF_STUBS_H_