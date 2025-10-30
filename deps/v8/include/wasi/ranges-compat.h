#ifndef WASI_RANGES_COMPAT_H_
#define WASI_RANGES_COMPAT_H_

// Compatibility layer for C++20 ranges with WASI SDK
// The WASI SDK appears to have incomplete ranges support, so we provide
// minimal definitions to allow compilation

#include <iterator>
#include <type_traits>
#include <utility>

// If native <ranges> is available, do not provide a polyfill to avoid
// ambiguous references with libc++'s std::ranges.
#if defined(__has_include)
#  if __has_include(<ranges>)
#    define V8_WASI_HAS_NATIVE_RANGES 1
#  endif
#endif

#ifdef __cpp_concepts
#include <concepts>
#define V8_WASI_HAS_CONCEPTS 1
#else
#define V8_WASI_HAS_CONCEPTS 0
#endif

#if !V8_WASI_HAS_CONCEPTS
#define V8_WASI_DECLARE_CONCEPT_1(name) \
  template <typename T> inline constexpr bool name = false
#define V8_WASI_DECLARE_CONCEPT_2(name) \
  template <typename T, typename U> inline constexpr bool name = false
#else
#define V8_WASI_DECLARE_CONCEPT_1(name) \
  template <typename T> concept name = false
#define V8_WASI_DECLARE_CONCEPT_2(name) \
  template <typename T, typename U> concept name = false
#endif

#if !defined(V8_WASI_HAS_NATIVE_RANGES) && defined(V8_WASI_STD_POLYFILLS_GLOBAL_SCOPE)
// Disable actual ranges functionality for WASI builds
#define RANGES_DISABLED_FOR_WASI 1

namespace std {

// Additional type traits that might be missing
template<typename T>
constexpr bool __is_std_initializer_list = false;

template<typename T>
constexpr bool __is_std_initializer_list<::std::initializer_list<T>> = true;

inline namespace __v8_wasi_polyfill {
namespace ranges {

// Dummy implementations to satisfy compilation
// These are not functional but allow the code to compile

// Basic concepts (stubbed to `false` for compat builds)
V8_WASI_DECLARE_CONCEPT_1(range);
V8_WASI_DECLARE_CONCEPT_1(view);
V8_WASI_DECLARE_CONCEPT_1(borrowed_range);
V8_WASI_DECLARE_CONCEPT_1(common_range);
V8_WASI_DECLARE_CONCEPT_1(viewable_range);
V8_WASI_DECLARE_CONCEPT_1(sized_range);
V8_WASI_DECLARE_CONCEPT_1(contiguous_range);
V8_WASI_DECLARE_CONCEPT_1(random_access_range);
V8_WASI_DECLARE_CONCEPT_1(forward_range);

// iterator_t helper
template<typename T>
using iterator_t = void*;

// range_reference_t helper - return a proper type instead of void
template<typename T>
using range_reference_t = typename T::value_type&;

// range_difference_t helper
template<typename T>
using range_difference_t = ::std::ptrdiff_t;

// iter_reference_t helper
template<typename T>
using iter_reference_t = typename T::value_type&;

// contiguous_iterator concept
V8_WASI_DECLARE_CONCEPT_1(contiguous_iterator);

// Dummy ref_view
template<typename R>
class ref_view {
public:
    ref_view(R&) {}
};

// Dummy owning_view
template<typename R>
class owning_view {
public:
    owning_view() = default;
    owning_view(R&&) {}
};

// Dummy common_view
template<typename V>
class common_view {
public:
    common_view() = default;
    common_view(V) {}
};

// view_interface base
template<typename Derived>
class view_interface {};

// views namespace
namespace views {
    template<typename R>
    using all_t = R;
}

// range_adaptor_closure
template<typename T>
struct __range_adaptor_closure {};

// enable_borrowed_range trait
template<typename T>
inline constexpr bool enable_borrowed_range = false;

// begin() function for ranges
template<typename R>
inline auto begin(R&& r) -> decltype(::std::begin(::std::forward<R>(r))) {
    return ::std::begin(::std::forward<R>(r));
}

// end() function for ranges
template<typename R>
inline auto end(R&& r) -> decltype(::std::end(::std::forward<R>(r))) {
    return ::std::end(::std::forward<R>(r));
}

// distance() function for ranges
template<typename R>
inline auto distance(R&& r) -> decltype(::std::distance(::std::begin(r), ::std::end(r))) {
    return ::std::distance(::std::begin(r), ::std::end(r));
}

// next() function for ranges
template<typename It, typename D>
inline auto next(It it, D dist) -> decltype(::std::next(it, dist)) {
    return ::std::next(it, dist);
}

template<typename It, typename D, typename S>
inline auto next(It it, D dist, S sentinel) -> It {
    // Simple implementation that respects sentinel
    while (dist > 0 && it != sentinel) {
        ++it;
        --dist;
    }
    return it;
}

// data() function for ranges
template<typename R>
inline auto data(R&& r) -> decltype(::std::data(::std::forward<R>(r))) {
    return ::std::data(::std::forward<R>(r));
}

// size() function for ranges
template<typename R>
inline auto size(R&& r) -> decltype(::std::size(::std::forward<R>(r))) {
    return ::std::size(::std::forward<R>(r));
}

// __span_compatible_range concept helper
V8_WASI_DECLARE_CONCEPT_2(__span_compatible_range);

// __simple_view concept helper
V8_WASI_DECLARE_CONCEPT_1(__simple_view);

// enable_view trait
template<typename T>
inline constexpr bool enable_view = false;

// elements_view stub
template<typename V, ::std::size_t N>
class elements_view {
public:
    elements_view() = default;
    elements_view(V base) : base_(base) {}
    
    size_t size() const { return 0; }
    auto begin() const { return base_.begin(); }
    auto end() const { return base_.end(); }
    
private:
    V base_;
};

} // namespace ranges
} // namespace __2

// Make ranges types available directly in std::ranges
namespace ranges {
using __v8_wasi_polyfill::ranges::range;
using __v8_wasi_polyfill::ranges::view;
using __v8_wasi_polyfill::ranges::borrowed_range;
using __v8_wasi_polyfill::ranges::common_range;
using __v8_wasi_polyfill::ranges::viewable_range;
using __v8_wasi_polyfill::ranges::sized_range;
using __v8_wasi_polyfill::ranges::contiguous_range;
using __v8_wasi_polyfill::ranges::random_access_range;
using __v8_wasi_polyfill::ranges::forward_range;
using __v8_wasi_polyfill::ranges::ref_view;
using __v8_wasi_polyfill::ranges::owning_view;
using __v8_wasi_polyfill::ranges::common_view;
using __v8_wasi_polyfill::ranges::iterator_t;
using __v8_wasi_polyfill::ranges::range_reference_t;
using __v8_wasi_polyfill::ranges::range_difference_t;
using __v8_wasi_polyfill::ranges::iter_reference_t;
using __v8_wasi_polyfill::ranges::contiguous_iterator;
using __v8_wasi_polyfill::ranges::__range_adaptor_closure;
using __v8_wasi_polyfill::ranges::enable_borrowed_range;
using __v8_wasi_polyfill::ranges::begin;
using __v8_wasi_polyfill::ranges::end;
using __v8_wasi_polyfill::ranges::distance;
using __v8_wasi_polyfill::ranges::next;
using __v8_wasi_polyfill::ranges::data;
using __v8_wasi_polyfill::ranges::size;
using __v8_wasi_polyfill::ranges::__span_compatible_range;
using __v8_wasi_polyfill::ranges::__simple_view;
using __v8_wasi_polyfill::ranges::enable_view;
using __v8_wasi_polyfill::ranges::elements_view;
}

} // namespace std

#undef V8_WASI_DECLARE_CONCEPT_1
#undef V8_WASI_DECLARE_CONCEPT_2
#undef V8_WASI_HAS_CONCEPTS
#endif // !V8_WASI_HAS_NATIVE_RANGES && V8_WASI_STD_POLYFILLS_GLOBAL_SCOPE

#endif // WASI_RANGES_COMPAT_H_
