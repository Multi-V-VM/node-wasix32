#ifndef V8_SRC_WASI_V8_BASE_FIXES_H_
#define V8_SRC_WASI_V8_BASE_FIXES_H_

#include <atomic>
#include <cstdint>
#include <cstring>
#include <memory>
#include <type_traits>
#include <vector>

#include "src/base/vector.h"

namespace v8 {
namespace base {

// Provide a permissive Address alias for WASI builds
using Address = uintptr_t;

// BitWidth function
// Note: bit utilities are provided in src/base/bits.h for WASI builds.

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

// RecursiveMutex and RandomNumberGenerator are already defined in V8 base classes
// src/base/platform/mutex.h and src/base/utils/random-number-generator.h
// Don't redefine them here

}  // namespace base

// Mirror the upstream convenience alias `v8::Vector` -> `v8::base::Vector`.
template <typename T>
using Vector = base::Vector<T>;

}  // namespace v8

#endif  // V8_SRC_WASI_V8_BASE_FIXES_H_
