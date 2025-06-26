                             char16_t* output) {
  for (size_t i = 0; i < length; ++i) {
    output[i] = static_cast<char16_t>(input[i]);
  }
  return length;
}

result convert_utf8_to_latin1_with_errors(const char* input, size_t length,
                                          char* output) {