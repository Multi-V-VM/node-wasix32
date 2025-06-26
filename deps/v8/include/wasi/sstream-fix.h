#ifndef WASI_SSTREAM_FIX_H
#define WASI_SSTREAM_FIX_H

#ifdef __wasi__

#include <sstream>
#include <string>

// Force explicit instantiation of string stream classes for WASI
// This resolves template instantiation errors

namespace std {
// Explicit template instantiations
extern template class basic_stringbuf<char, char_traits<char>, allocator<char>>;
extern template class basic_istringstream<char, char_traits<char>,
                                          allocator<char>>;
extern template class basic_ostringstream<char, char_traits<char>,
                                          allocator<char>>;
extern template class basic_stringstream<char, char_traits<char>,
                                         allocator<char>>;
}  // namespace std

#endif  // __wasi__
#endif  // WASI_SSTREAM_FIX_H