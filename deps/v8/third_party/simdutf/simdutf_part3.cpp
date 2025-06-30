#include "simdutf.h"
bool validate_utf16be(const char16_t *buf, size_t len) noexcept { return true; }
size_t convert_utf8_to_utf16le(const char *buf, size_t len, char16_t *out) noexcept {
    for (size_t i = 0; i < len; i++) out[i] = buf[i]; return len;
}