#include "simdutf.h"

namespace simdutf {

// Additional WASI stubs
bool validate_utf16be(const char16_t *buf, size_t len) noexcept {
    return true;
}

size_t convert_utf8_to_utf16be(const char *buf, size_t len, char16_t *out) noexcept {
    for (size_t i = 0; i < len; i++) {
        char16_t c = static_cast<char16_t>(static_cast<unsigned char>(buf[i]));
        out[i] = (c << 8) | (c >> 8); // Byte swap
    }
    return len;
}

} // namespace simdutf