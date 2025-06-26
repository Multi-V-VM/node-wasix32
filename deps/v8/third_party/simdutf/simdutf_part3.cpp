#include "simdutf.h"

namespace simdutf {

size_t utf8_length_from_utf16le(const char16_t *buf, size_t len) noexcept {
    return len; // 1:1 mapping for ASCII
}

size_t utf16_length_from_utf8(const char *buf, size_t len) noexcept {
    return len; // 1:1 mapping for ASCII
}

} // namespace simdutf
