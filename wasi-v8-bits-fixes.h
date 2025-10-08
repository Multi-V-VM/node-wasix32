#ifndef WASI_V8_BITS_FIXES_H_
#define WASI_V8_BITS_FIXES_H_

#ifdef __wasi__

#include <array>
#include <cstdint>
#include <type_traits>
#include <utility>

// Include V8 base headers
#include "src/base/bit-field.h"
#include "src/base/bits.h"

// Add make_array to v8::base namespace
namespace v8::base {

#ifndef V8_WASI_BASE_MAKE_ARRAY_DEFINED
#define V8_WASI_BASE_MAKE_ARRAY_DEFINED

template <size_t N, typename F>
constexpr auto make_array(F&& f) -> ::std::array<decltype(f(0)), N> {
  return []<size_t... I>(F&& fn, ::std::index_sequence<I...>) {
    return ::std::array<decltype(fn(0)), N>{{fn(I)...}};
  }(::std::forward<F>(f), ::std::make_index_sequence<N>{});
}

#endif  // V8_WASI_BASE_MAKE_ARRAY_DEFINED

}  // namespace v8::base

#endif  // __wasi__

#endif  // WASI_V8_BITS_FIXES_H_
