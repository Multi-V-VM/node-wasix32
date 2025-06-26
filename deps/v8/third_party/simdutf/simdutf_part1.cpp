#include "simdutf.h"

namespace simdutf {

// WASI stub implementations
bool validate_utf8(const char *buf, size_t len) noexcept {
    return true; // Accept all input for WASI stub
}

size_t convert_utf8_to_utf16le(const char *buf, size_t len, char16_t *out) noexcept {
    for (size_t i = 0; i < len; i++) {
        out[i] = static_cast<char16_t>(static_cast<unsigned char>(buf[i]));
    }
    return len;
}

size_t convert_utf8_to_utf16(const char* input, size_t length,