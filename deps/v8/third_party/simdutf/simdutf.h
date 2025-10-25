// WASI-compatible simdutf header
#ifndef SIMDUTF_H_WASI
#define SIMDUTF_H_WASI

#include <cstddef>
#include <cstdint>

#ifdef __wasi__

// Define the macro if not already defined
#ifndef simdutf_warn_unused
#define simdutf_warn_unused
#endif

namespace simdutf {

// Minimal structures and enums to satisfy V8 usage on WASI.
// Keep signatures compatible with V8's expectations.
enum class error_code : int {
    SUCCESS = 0,
    INVALID_UTF8 = 1,
    INVALID_BASE64_CHARACTER = 2,
    BASE64_INPUT_REMAINDER = 3,
    BASE64_EXTRA_BITS = 4,
    OUTPUT_BUFFER_TOO_SMALL = 5,
};

struct result {
    size_t count;
    error_code error;
};

// Base64 option enums used by TypedArray builtins in newer V8 versions.
enum class base64_options {
  base64_default,
  base64_url,
  // Additional options used by newer V8 call sites
  base64_default_no_padding,
  base64_url_with_padding
};
enum class last_chunk_handling_options { loose, strict, stop_before_partial };
// Backward-compatible constants
constexpr base64_options base64_default = base64_options::base64_default;
constexpr base64_options base64_url = base64_options::base64_url;

// Function declarations (these are implemented in simdutf_wasi.cpp)
simdutf_warn_unused bool validate_utf8(const char* buf, size_t len) noexcept;
simdutf_warn_unused bool validate_utf16le(const char16_t* buf, size_t len) noexcept;
simdutf_warn_unused bool validate_utf16be(const char16_t* buf, size_t len) noexcept;
simdutf_warn_unused bool validate_utf32(const char32_t* buf, size_t len) noexcept;

simdutf_warn_unused size_t utf8_length_from_utf16le(const char16_t* input, size_t length) noexcept;
simdutf_warn_unused size_t utf8_length_from_utf16be(const char16_t* input, size_t length) noexcept;
simdutf_warn_unused size_t utf16_length_from_utf8(const char* input, size_t length) noexcept;

simdutf_warn_unused size_t convert_utf8_to_utf16le(const char* input, size_t length, char16_t* output) noexcept;
simdutf_warn_unused size_t convert_utf8_to_utf16be(const char* input, size_t length, char16_t* output) noexcept;
simdutf_warn_unused size_t convert_utf16le_to_utf8(const char16_t* input, size_t length, char* output) noexcept;
simdutf_warn_unused size_t convert_utf16be_to_utf8(const char16_t* input, size_t length, char* output) noexcept;

// Aliases for compatibility
inline size_t convert_utf8_to_utf16(const char* input, size_t length, char16_t* output) noexcept {
    return convert_utf8_to_utf16le(input, length, output);
}

inline result convert_utf8_to_latin1_with_errors(const char* input, size_t length, char* output) noexcept {
    for (size_t i = 0; i < length; ++i) output[i] = input[i];
    return {length, error_code::SUCCESS};
}

// Minimal base64 API used by V8; implemented in simdutf_wasi.cpp
size_t maximal_binary_length_from_base64(const char* input, size_t length) noexcept;

// Safe base64 decode into a caller-provided buffer. Overloads accept either
// 8-bit or 16-bit character inputs (UTF-16 with ASCII contents).
result base64_to_binary_safe(const char* input, size_t input_length, char* output,
                             size_t output_length, base64_options alphabet,
                             last_chunk_handling_options last_chunk) noexcept;
result base64_to_binary_safe(const char16_t* input, size_t input_length, char* output,
                             size_t output_length, base64_options alphabet,
                             last_chunk_handling_options last_chunk) noexcept;

// Minimal base64 encode support used by TypedArray.prototype.toBase64.
size_t base64_length_from_binary(size_t length, base64_options alphabet) noexcept;
size_t binary_to_base64(const char* input, size_t length, char* output,
                        base64_options alphabet) noexcept;

}  // namespace simdutf

#endif // __wasi__
#endif // SIMDUTF_H_WASI
