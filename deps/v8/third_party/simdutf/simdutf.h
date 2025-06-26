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

struct result {
    size_t count;
    int error;
};

namespace error_code {
    constexpr int SUCCESS = 0;
    constexpr int INVALID_UTF8 = 1;
}

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
    result res;
    res.count = length;
    res.error = error_code::SUCCESS;
    for (size_t i = 0; i < length; ++i) {
        output[i] = input[i];
    }
    return res;
}

}  // namespace simdutf

#endif // __wasi__
#endif // SIMDUTF_H_WASI
