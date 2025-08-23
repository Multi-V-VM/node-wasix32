#ifndef V8_SRC_WASI_V8_BASE_FIXES_H_
#define V8_SRC_WASI_V8_BASE_FIXES_H_

#include <atomic>
#include <cstdint>
#include <cstring>
#include <memory>
#include <type_traits>
#include <vector>

namespace v8 {
namespace base {

// Add Address to base namespace
using Address = ::Address;

// BitWidth function
namespace bits {
  template<typename T>
  constexpr int BitWidth(T value) {
    if (value == 0) return 0;
    int width = 0;
    while (value) {
      value >>= 1;
      width++;
    }
    return width;
  }
  
}

// has_type_v template helper  
template<typename T, typename... Ts>
struct has_type;

template<typename T>
struct has_type<T> : std::false_type {};

template<typename T, typename Head, typename... Tail>
struct has_type<T, Head, Tail...> 
    : std::conditional_t<std::is_same_v<T, Head>, std::true_type, has_type<T, Tail...>> {};

template<typename T, typename... Ts>
inline constexpr bool has_type_v = has_type<T, Ts...>::value;

// Remove Vector alias - conflicts with base/vector.h

}  // namespace base
}  // namespace v8

#endif  // V8_SRC_WASI_V8_BASE_FIXES_H_
