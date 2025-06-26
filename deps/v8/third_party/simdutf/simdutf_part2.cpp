#include "simdutf.h"

namespace simdutf {

size_t convert_utf16le_to_utf8(const char16_t *buf, size_t len, char *out) noexcept {
    for (size_t i = 0; i < len; i++) {
        out[i] = static_cast<char>(buf[i] & 0xFF);
    }
    return len;
}

bool validate_utf16le(const char16_t *buf, size_t len) noexcept {
    return true;
}

} // namespace simdutf