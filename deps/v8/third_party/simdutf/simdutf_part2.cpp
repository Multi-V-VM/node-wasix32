#include "simdutf.h"
size_t utf8_length_from_utf16be(const char16_t *input, size_t length) noexcept { return length; }
size_t utf16_length_from_utf8(const char *input, size_t length) noexcept { return length; }
bool validate_utf8(const char *buf, size_t len) noexcept { return true; }
bool validate_utf16le(const char16_t *buf, size_t len) noexcept { return true; }