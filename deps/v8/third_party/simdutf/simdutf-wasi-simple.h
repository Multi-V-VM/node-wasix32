#ifndef SIMDUTF_WASI_SIMPLE_H
#define SIMDUTF_WASI_SIMPLE_H

// Simplified simdutf for WASI - no concepts, minimal functionality

#include <cstddef>
#include <cstdint>

namespace simdutf {

// Basic result types
struct result {
  size_t count;
  bool error;

  result(size_t c = 0, bool e = false) : count(c), error(e) {}
};

// Basic encoding detection
enum encoding {
  UTF8 = 1,
  UTF16_LE = 2,
  UTF16_BE = 3,
  UTF32_LE = 4,
  UTF32_BE = 5
};

// Basic validation functions (stubs for WASI)
inline bool validate_utf8(const char* buf, size_t len) { return true; }
inline bool validate_utf16le(const char16_t* buf, size_t len) { return true; }
inline bool validate_utf16be(const char16_t* buf, size_t len) { return true; }
inline bool validate_utf32(const char32_t* buf, size_t len) { return true; }

// Basic conversion functions (stubs for WASI)
inline size_t convert_utf8_to_utf16le(const char* input, size_t length,
                                      char16_t* output) {
  return 0;  // Stub implementation
}

inline size_t convert_utf8_to_utf16be(const char* input, size_t length,
                                      char16_t* output) {
  return 0;  // Stub implementation
}

inline size_t convert_utf16le_to_utf8(const char16_t* input, size_t length,
                                      char* output) {
  return 0;  // Stub implementation
}

inline size_t convert_utf16be_to_utf8(const char16_t* input, size_t length,
                                      char* output) {
  return 0;  // Stub implementation
}

// Template-based versions (simplified without concepts)
template <typename InputType, typename OutputType>
inline size_t convert(const InputType* input, size_t length,
                      OutputType* output) {
  return 0;  // Stub implementation
}

// Span-like helpers for WASI
namespace detail {
template <typename T>
using input_span_of_byte_like = T;

template <typename T>
using output_span_of_byte_like = T;
}  // namespace detail

// Modern C++ interface stubs
template <typename InputSpan, typename OutputSpan>
inline result convert_utf8_to_utf16le(const InputSpan& input,
                                      OutputSpan&& output) {
  return result(0, false);
}

template <typename InputSpan, typename OutputSpan>
inline result convert_utf8_to_utf16be(const InputSpan& input,
                                      OutputSpan&& output) {
  return result(0, false);
}

template <typename InputSpan>
inline bool validate_utf8(const InputSpan& input) {
  return true;
}

template <typename InputSpan>
inline bool validate_utf16le(const InputSpan& input) {
  return true;
}

template <typename InputSpan>
inline bool validate_utf16be(const InputSpan& input) {
  return true;
}

// Latin1 conversion stubs
template <typename InputSpan, typename OutputSpan>
inline result convert_latin1_to_utf8(const InputSpan& input,
                                     OutputSpan&& output) {
  return result(0, false);
}

template <typename InputSpan, typename OutputSpan>
inline result convert_latin1_to_utf16le(const InputSpan& input,
                                        OutputSpan&& output) {
  return result(0, false);
}

template <typename InputSpan, typename OutputSpan>
inline result convert_latin1_to_utf16be(const InputSpan& input,
                                        OutputSpan&& output) {
  return result(0, false);
}

}  // namespace simdutf

#endif  // SIMDUTF_WASI_SIMPLE_H