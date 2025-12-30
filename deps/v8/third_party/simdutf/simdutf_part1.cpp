#include "simdutf.h"

namespace simdutf {

// Validation functions - simple fallback implementations
bool validate_utf8(const char* buf, size_t len) noexcept {
  for (size_t i = 0; i < len; ++i) {
    unsigned char c = buf[i];
    if (c < 0x80) continue;
    size_t extra_bytes;
    if ((c & 0xE0) == 0xC0) extra_bytes = 1;
    else if ((c & 0xF0) == 0xE0) extra_bytes = 2;
    else if ((c & 0xF8) == 0xF0) extra_bytes = 3;
    else return false;
    if (i + extra_bytes >= len) return false;
    for (size_t j = 1; j <= extra_bytes; ++j) {
      if ((buf[i + j] & 0xC0) != 0x80) return false;
    }
    i += extra_bytes;
  }
  return true;
}

bool validate_utf16le(const char16_t* buf, size_t len) noexcept {
  for (size_t i = 0; i < len; ++i) {
    char16_t c = buf[i];
    if (c >= 0xD800 && c < 0xDC00) {
      if (i + 1 >= len) return false;
      char16_t c2 = buf[i + 1];
      if (c2 < 0xDC00 || c2 >= 0xE000) return false;
      ++i;
    } else if (c >= 0xDC00 && c < 0xE000) {
      return false;
    }
  }
  return true;
}

bool validate_utf16be(const char16_t* buf, size_t len) noexcept {
  return validate_utf16le(buf, len);  // Simplified
}

bool validate_utf32(const char32_t* buf, size_t len) noexcept {
  for (size_t i = 0; i < len; ++i) {
    if (buf[i] > 0x10FFFF) return false;
  }
  return true;
}

// Length functions
size_t utf8_length_from_utf16le(const char16_t* input, size_t length) noexcept {
  size_t result = 0;
  for (size_t i = 0; i < length; ++i) {
    char16_t c = input[i];
    if (c < 0x80) result += 1;
    else if (c < 0x800) result += 2;
    else if (c >= 0xD800 && c < 0xDC00) {
      result += 4;
      ++i;  // Skip surrogate pair
    } else result += 3;
  }
  return result;
}

size_t utf8_length_from_utf16be(const char16_t* input, size_t length) noexcept {
  return utf8_length_from_utf16le(input, length);
}

size_t utf16_length_from_utf8(const char* input, size_t length) noexcept {
  size_t result = 0;
  for (size_t i = 0; i < length; ++i) {
    unsigned char c = input[i];
    if ((c & 0x80) == 0) { result += 1; }
    else if ((c & 0xE0) == 0xC0) { result += 1; i += 1; }
    else if ((c & 0xF0) == 0xE0) { result += 1; i += 2; }
    else if ((c & 0xF8) == 0xF0) { result += 2; i += 3; }  // Surrogate pair
  }
  return result;
}

// Conversion functions
size_t convert_utf8_to_utf16le(const char* input, size_t length, char16_t* output) noexcept {
  size_t j = 0;
  for (size_t i = 0; i < length;) {
    unsigned char c = input[i];
    uint32_t codepoint;
    if ((c & 0x80) == 0) {
      codepoint = c;
      ++i;
    } else if ((c & 0xE0) == 0xC0) {
      codepoint = (c & 0x1F) << 6;
      codepoint |= (input[i+1] & 0x3F);
      i += 2;
    } else if ((c & 0xF0) == 0xE0) {
      codepoint = (c & 0x0F) << 12;
      codepoint |= (input[i+1] & 0x3F) << 6;
      codepoint |= (input[i+2] & 0x3F);
      i += 3;
    } else if ((c & 0xF8) == 0xF0) {
      codepoint = (c & 0x07) << 18;
      codepoint |= (input[i+1] & 0x3F) << 12;
      codepoint |= (input[i+2] & 0x3F) << 6;
      codepoint |= (input[i+3] & 0x3F);
      i += 4;
    } else {
      ++i;
      continue;
    }
    if (codepoint <= 0xFFFF) {
      output[j++] = static_cast<char16_t>(codepoint);
    } else {
      codepoint -= 0x10000;
      output[j++] = static_cast<char16_t>(0xD800 | (codepoint >> 10));
      output[j++] = static_cast<char16_t>(0xDC00 | (codepoint & 0x3FF));
    }
  }
  return j;
}

size_t convert_utf8_to_utf16be(const char* input, size_t length, char16_t* output) noexcept {
  return convert_utf8_to_utf16le(input, length, output);
}

size_t convert_utf16le_to_utf8(const char16_t* input, size_t length, char* output) noexcept {
  size_t j = 0;
  for (size_t i = 0; i < length;) {
    uint32_t codepoint;
    char16_t c = input[i++];
    if (c >= 0xD800 && c < 0xDC00) {
      if (i < length) {
        char16_t c2 = input[i++];
        codepoint = 0x10000 + ((c & 0x3FF) << 10) + (c2 & 0x3FF);
      } else {
        continue;
      }
    } else {
      codepoint = c;
    }
    if (codepoint < 0x80) {
      output[j++] = static_cast<char>(codepoint);
    } else if (codepoint < 0x800) {
      output[j++] = static_cast<char>(0xC0 | (codepoint >> 6));
      output[j++] = static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint < 0x10000) {
      output[j++] = static_cast<char>(0xE0 | (codepoint >> 12));
      output[j++] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
      output[j++] = static_cast<char>(0x80 | (codepoint & 0x3F));
    } else {
      output[j++] = static_cast<char>(0xF0 | (codepoint >> 18));
      output[j++] = static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
      output[j++] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
      output[j++] = static_cast<char>(0x80 | (codepoint & 0x3F));
    }
  }
  return j;
}

size_t convert_utf16be_to_utf8(const char16_t* input, size_t length, char* output) noexcept {
  return convert_utf16le_to_utf8(input, length, output);
}

result convert_utf8_to_latin1_with_errors(const char* input, size_t length, char* output) noexcept {
  for (size_t i = 0; i < length; ++i) {
    unsigned char c = input[i];
    if (c < 0x80) {
      output[i] = static_cast<char>(c);
    } else if ((c & 0xE0) == 0xC0 && i + 1 < length) {
      uint32_t codepoint = (c & 0x1F) << 6;
      codepoint |= (input[i+1] & 0x3F);
      if (codepoint > 0xFF) {
        return {i, error_code::INVALID_UTF8};
      }
      output[i] = static_cast<char>(codepoint);
      ++i;  // Skip the continuation byte
    } else {
      return {i, error_code::INVALID_UTF8};
    }
  }
  return {length, error_code::SUCCESS};
}

// Base64 functions
size_t maximal_binary_length_from_base64(const char* input, size_t length) noexcept {
  return (length / 4) * 3 + 3;
}

static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char base64_url_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

static int decode_base64_char(char c, base64_options alphabet) {
  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= 'a' && c <= 'z') return c - 'a' + 26;
  if (c >= '0' && c <= '9') return c - '0' + 52;
  if (alphabet == base64_options::base64_url || alphabet == base64_options::base64_url_with_padding) {
    if (c == '-') return 62;
    if (c == '_') return 63;
  } else {
    if (c == '+') return 62;
    if (c == '/') return 63;
  }
  return -1;
}

result base64_to_binary_safe(const char* input, size_t input_length, char* output,
                             size_t output_length, base64_options alphabet,
                             last_chunk_handling_options /*last_chunk*/) noexcept {
  size_t i = 0, j = 0;
  while (i < input_length) {
    if (input[i] == '=' || input[i] == '\n' || input[i] == '\r' || input[i] == ' ') {
      ++i;
      continue;
    }
    int a = decode_base64_char(input[i], alphabet);
    if (a < 0) return {j, error_code::INVALID_BASE64_CHARACTER};
    ++i;
    if (i >= input_length) break;
    while (i < input_length && (input[i] == '\n' || input[i] == '\r' || input[i] == ' ')) ++i;
    if (i >= input_length) break;
    int b = decode_base64_char(input[i], alphabet);
    if (b < 0 && input[i] != '=') return {j, error_code::INVALID_BASE64_CHARACTER};
    ++i;
    if (j >= output_length) return {j, error_code::OUTPUT_BUFFER_TOO_SMALL};
    if (b < 0) break;
    output[j++] = static_cast<char>((a << 2) | (b >> 4));
    while (i < input_length && (input[i] == '\n' || input[i] == '\r' || input[i] == ' ')) ++i;
    if (i >= input_length || input[i] == '=') continue;
    int c = decode_base64_char(input[i], alphabet);
    if (c < 0 && input[i] != '=') return {j, error_code::INVALID_BASE64_CHARACTER};
    ++i;
    if (j >= output_length) return {j, error_code::OUTPUT_BUFFER_TOO_SMALL};
    if (c < 0) break;
    output[j++] = static_cast<char>(((b & 0x0F) << 4) | (c >> 2));
    while (i < input_length && (input[i] == '\n' || input[i] == '\r' || input[i] == ' ')) ++i;
    if (i >= input_length || input[i] == '=') continue;
    int d = decode_base64_char(input[i], alphabet);
    if (d < 0 && input[i] != '=') return {j, error_code::INVALID_BASE64_CHARACTER};
    ++i;
    if (j >= output_length) return {j, error_code::OUTPUT_BUFFER_TOO_SMALL};
    if (d < 0) break;
    output[j++] = static_cast<char>(((c & 0x03) << 6) | d);
  }
  return {j, error_code::SUCCESS};
}

result base64_to_binary_safe(const char16_t* input, size_t input_length, char* output,
                             size_t output_length, base64_options alphabet,
                             last_chunk_handling_options last_chunk) noexcept {
  // Convert to char and call the char version
  size_t i = 0, j = 0;
  while (i < input_length) {
    if (input[i] == '=' || input[i] == '\n' || input[i] == '\r' || input[i] == ' ') {
      ++i;
      continue;
    }
    int a = decode_base64_char(static_cast<char>(input[i]), alphabet);
    if (a < 0) return {j, error_code::INVALID_BASE64_CHARACTER};
    ++i;
    if (i >= input_length) break;
    while (i < input_length && (input[i] == '\n' || input[i] == '\r' || input[i] == ' ')) ++i;
    if (i >= input_length) break;
    int b = decode_base64_char(static_cast<char>(input[i]), alphabet);
    if (b < 0 && input[i] != '=') return {j, error_code::INVALID_BASE64_CHARACTER};
    ++i;
    if (j >= output_length) return {j, error_code::OUTPUT_BUFFER_TOO_SMALL};
    if (b < 0) break;
    output[j++] = static_cast<char>((a << 2) | (b >> 4));
    while (i < input_length && (input[i] == '\n' || input[i] == '\r' || input[i] == ' ')) ++i;
    if (i >= input_length || input[i] == '=') continue;
    int c = decode_base64_char(static_cast<char>(input[i]), alphabet);
    if (c < 0 && input[i] != '=') return {j, error_code::INVALID_BASE64_CHARACTER};
    ++i;
    if (j >= output_length) return {j, error_code::OUTPUT_BUFFER_TOO_SMALL};
    if (c < 0) break;
    output[j++] = static_cast<char>(((b & 0x0F) << 4) | (c >> 2));
    while (i < input_length && (input[i] == '\n' || input[i] == '\r' || input[i] == ' ')) ++i;
    if (i >= input_length || input[i] == '=') continue;
    int d = decode_base64_char(static_cast<char>(input[i]), alphabet);
    if (d < 0 && input[i] != '=') return {j, error_code::INVALID_BASE64_CHARACTER};
    ++i;
    if (j >= output_length) return {j, error_code::OUTPUT_BUFFER_TOO_SMALL};
    if (d < 0) break;
    output[j++] = static_cast<char>(((c & 0x03) << 6) | d);
  }
  return {j, error_code::SUCCESS};
}

size_t base64_length_from_binary(size_t length, base64_options /*alphabet*/) noexcept {
  return ((length + 2) / 3) * 4;
}

size_t binary_to_base64(const char* input, size_t length, char* output,
                        base64_options alphabet) noexcept {
  const char* chars = (alphabet == base64_options::base64_url ||
                       alphabet == base64_options::base64_url_with_padding)
                      ? base64_url_chars : base64_chars;
  size_t i = 0, j = 0;
  while (i + 2 < length) {
    unsigned char a = input[i++];
    unsigned char b = input[i++];
    unsigned char c = input[i++];
    output[j++] = chars[a >> 2];
    output[j++] = chars[((a & 0x03) << 4) | (b >> 4)];
    output[j++] = chars[((b & 0x0F) << 2) | (c >> 6)];
    output[j++] = chars[c & 0x3F];
  }
  if (i < length) {
    unsigned char a = input[i++];
    output[j++] = chars[a >> 2];
    if (i < length) {
      unsigned char b = input[i++];
      output[j++] = chars[((a & 0x03) << 4) | (b >> 4)];
      output[j++] = chars[(b & 0x0F) << 2];
    } else {
      output[j++] = chars[(a & 0x03) << 4];
      output[j++] = '=';
    }
    output[j++] = '=';
  }
  return j;
}

}  // namespace simdutf
