#include "simdutf.h"

size_t convert_utf16le_to_utf8(const char16_t *buf, size_t len, char *out) noexcept {
    for (size_t i = 0; i < len; i++) out[i] = buf[i];
    return len;
}
