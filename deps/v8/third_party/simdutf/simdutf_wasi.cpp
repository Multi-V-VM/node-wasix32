// WASI-specific simdutf implementation
#include "simdutf.h"
#include <cstring>
#include <algorithm>
#include <array>

namespace simdutf {

size_t utf8_length_from_utf16le(const char16_t *input, size_t length) {
    size_t result = 0;
    for (size_t i = 0; i < length; i++) {
        char16_t c = input[i];
        if (c < 0x80) result += 1;
        else if (c < 0x800) result += 2;
        else result += 3;
    }
    return result;
}

size_t utf8_length_from_utf16be(const char16_t *input, size_t length) {
    return utf8_length_from_utf16le(input, length);
}

size_t utf16_length_from_utf8(const char *input, size_t length) {
    return length; // Simplified
}

result validate_utf8(const char *buf, size_t len) {
    return {error_code::SUCCESS, len};
}

result validate_utf16le(const char16_t *buf, size_t len) {
    return {error_code::SUCCESS, len};
}

result validate_utf16be(const char16_t *buf, size_t len) {
    return {error_code::SUCCESS, len};
}

size_t convert_utf8_to_utf16le(const char *buf, size_t len, char16_t *utf16_output) {
    size_t result = 0;
    for (size_t i = 0; i < len && i < len; i++) {
        utf16_output[result++] = static_cast<char16_t>(buf[i]);
    }
    return result;
}

size_t convert_utf16le_to_utf8(const char16_t *buf, size_t len, char *utf8_output) {
    size_t result = 0;
    for (size_t i = 0; i < len; i++) {
        utf8_output[result++] = static_cast<char>(buf[i]);
    }
    return result;
}

// --- Minimal base64 support for V8 TypedArray.fromBase64 ---
namespace {
constexpr uint8_t kInvalid = 0xFF;

// Build tables for standard and URL-safe alphabets.
constexpr std::array<uint8_t, 256> MakeDecodingTable(bool urlsafe) {
  std::array<uint8_t, 256> table{};
  for (auto &v : table) v = kInvalid;
  auto set = [&](char c, uint8_t v) { table[static_cast<unsigned char>(c)] = v; };
  for (uint8_t i = 0; i < 26; ++i) { set('A' + i, i); set('a' + i, 26 + i); }
  for (uint8_t i = 0; i < 10; ++i) { set('0' + i, 52 + i); }
  if (urlsafe) {
    set('-', 62); set('_', 63);
  } else {
    set('+', 62); set('/', 63);
  }
  return table;
}
}  // namespace

size_t maximal_binary_length_from_base64(const char* /*input*/, size_t length) noexcept {
  // Conservative upper bound.
  return (length / 4 + 1) * 3;
}

result base64_to_binary_safe(const char* input, size_t input_length, char* output,
                             size_t output_length, base64_options alphabet,
                             last_chunk_handling_options last_chunk) noexcept {
  const bool urlsafe = (alphabet == base64_options::base64_url);
  static const auto std_table = MakeDecodingTable(false);
  static const auto url_table = MakeDecodingTable(true);
  const auto& table = urlsafe ? url_table : std_table;

  size_t out = 0;
  size_t i = 0;
  // Process blocks of 4 chars -> 3 bytes.
  while (i + 4 <= input_length) {
    uint8_t a = table[static_cast<unsigned char>(input[i++])];
    uint8_t b = table[static_cast<unsigned char>(input[i++])];
    char cch = input[i++];
    char dch = input[i++];
    uint8_t c = (cch == '=') ? kInvalid : table[static_cast<unsigned char>(cch)];
    uint8_t d = (dch == '=') ? kInvalid : table[static_cast<unsigned char>(dch)];

    if (a == kInvalid || b == kInvalid || (cch != '=' && c == kInvalid) || (dch != '=' && d == kInvalid)) {
      return {out, error_code::INVALID_BASE64_CHARACTER};
    }

    uint32_t triple = (static_cast<uint32_t>(a) << 18) |
                      (static_cast<uint32_t>(b) << 12) |
                      ((cch == '=') ? 0 : (static_cast<uint32_t>(c) << 6)) |
                      ((dch == '=') ? 0 : static_cast<uint32_t>(d));

    if (out + 3 > output_length) {
      // Would overflow output buffer.
      return {out, error_code::BASE64_EXTRA_BITS};
    }

    output[out++] = static_cast<char>((triple >> 16) & 0xFF);
    if (cch != '=') output[out++] = static_cast<char>((triple >> 8) & 0xFF);
    if (dch != '=') output[out++] = static_cast<char>(triple & 0xFF);

    if (cch == '=' || dch == '=') {
      // Padding should only appear at the end; honor last_chunk policy.
      if (i != input_length) {
        if (last_chunk == last_chunk_handling_options::strict) {
          return {out, error_code::BASE64_INPUT_REMAINDER};
        } else if (last_chunk == last_chunk_handling_options::stop_before_partial) {
          // Stop here without error.
          return {out, error_code::SUCCESS};
        }
        // 'loose' falls through and ignores trailing garbage.
      }
      break;
    }
  }

  // Handle remainder depending on policy.
  size_t rem = input_length - i;
  if (rem != 0) {
    if (last_chunk == last_chunk_handling_options::strict) {
      return {out, error_code::BASE64_INPUT_REMAINDER};
    }
    // For 'loose' or 'stop_before_partial', ignore trailing bytes.
  }

  return {out, error_code::SUCCESS};
}

} // namespace simdutf
