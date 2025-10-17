#ifndef V8_INCLUDE_WASI_STD_NAMESPACE_FIX_H_
#define V8_INCLUDE_WASI_STD_NAMESPACE_FIX_H_

// Ensure global std namespace is accessible
namespace std {}

// Include ranges compatibility layer after ensuring std exists
#ifdef __wasi__
#include "wasi/ranges-compat.h"
#include "wasi/crdtp-namespace-fix.h"
#endif

// Forward declare v8 types to avoid namespace conflicts
namespace v8 {
  class Isolate;
  template<typename T> class Local;
  class String;
  class Object;
  class Array;
  class Value;
  class Context;
  template<typename T> class Maybe;
  class EmbedderGraph;
}

#include <locale>
#include <iomanip>
#include <ostream>
#include <chrono>
#include <string>
#include <string_view>
#include <memory>
#include <atomic>
#include <vector>
#include <unordered_map>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <limits>
#include <optional>
#include <type_traits>
#include <utility>
#include <functional>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <tuple>
#include <sstream>
#include <initializer_list>
#include <iterator>
#include <list>
#include <forward_list>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>
#include <cmath>
#include <mutex>
#include <cstddef>
#include <array>
#include <stack>
#include <cstdint>
#include <new>
#include <bit>

#if !defined(__cpp_lib_endian)
namespace std {
enum class endian {
    little = 0,
    big = 1,
#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && defined(__ORDER_LITTLE_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    native = big
#else
    native = little
#endif
#else
    native = little
#endif
};
}  // namespace std
#endif

#if !defined(__cpp_lib_ranges)
namespace std {
struct contiguous_iterator_tag : random_access_iterator_tag {};
}  // namespace std
#endif

// Do NOT create nested aliases like v8::std or v8::v8::std.
// They introduce a nested `v8::v8` namespace and break qualified lookups
// such as `v8::String` inside the v8 namespace. Standard library symbols
// should be referenced as `::std::...` or `std::...` from any namespace.


// Do not create aliases in ::std::ranges to avoid conflicts with standard library

#endif  // V8_INCLUDE_WASI_STD_NAMESPACE_FIX_H_
