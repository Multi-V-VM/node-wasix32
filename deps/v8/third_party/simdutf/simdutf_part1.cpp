#ifdef __wasi__
#include "simdutf_wasi.cpp"
#else

#include "simdutf.h"

namespace simdutf {

size_t convert_utf8_to_utf16(const char* input, size_t length,