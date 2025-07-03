#ifndef V8_BASE_HASHING_WASI_FIX_H_
#define V8_BASE_HASHING_WASI_FIX_H_

// Alternative implementations for WASI that don't use C++20 concepts

template <typename T>
typename std::enable_if<std::is_enum<T>::value, size_t>::type
hash_value(T v) {
  return hash_value(static_cast<std::underlying_type_t<T>>(v));
}

// SFINAE-based alternative to concept
template <typename T>
auto hash_value(const T& v) -> decltype(v.hash_value()) {
  return v.hash_value();
}

// Hashable concept alternative
template <typename T, typename = void>
struct is_hashable : std::false_type {};

template <typename T>
struct is_hashable<T, std::void_t<decltype(hash_value(std::declval<const T&>()))>>
    : std::true_type {};

template <typename T>
inline constexpr bool is_hashable_v = is_hashable<T>::value;

#endif  // V8_BASE_HASHING_WASI_FIX_H_
