#ifndef WASI_RANGES_COMPAT_H_
#define WASI_RANGES_COMPAT_H_

// Compatibility layer for C++20 ranges with WASI SDK
// The WASI SDK appears to have incomplete ranges support, so we provide
// minimal definitions to allow compilation

#include <iterator>
#include <type_traits>
#include <utility>

// Disable actual ranges functionality for WASI builds
#define RANGES_DISABLED_FOR_WASI 1

namespace v8 {
namespace std {

// Additional type traits that might be missing
template<typename T>
constexpr bool __is_std_initializer_list = false;

template<typename T>
constexpr bool __is_std_initializer_list<::std::initializer_list<T>> = true;

inline namespace __2 {
namespace ranges {

// Dummy implementations to satisfy compilation
// These are not functional but allow the code to compile

// Basic concepts
template<typename T>
concept range = false;

template<typename T>
concept view = false;

template<typename T>
concept borrowed_range = false;

template<typename T>
concept common_range = false;

template<typename T>
concept viewable_range = false;

template<typename T>
concept sized_range = false;

template<typename T>
concept contiguous_range = false;

template<typename T>
concept random_access_range = false;

template<typename T>
concept forward_range = false;

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
template<typename T>
concept contiguous_iterator = false;

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
template<typename R, typename T>
concept __span_compatible_range = false;

// __simple_view concept helper
template<typename T>
concept __simple_view = false;

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

// Make ranges types available directly in v8::std
namespace ranges {
using __2::ranges::range;
using __2::ranges::view;
using __2::ranges::borrowed_range;
using __2::ranges::common_range;
using __2::ranges::viewable_range;
using __2::ranges::sized_range;
using __2::ranges::contiguous_range;
using __2::ranges::random_access_range;
using __2::ranges::forward_range;
using __2::ranges::ref_view;
using __2::ranges::owning_view;
using __2::ranges::common_view;
using __2::ranges::iterator_t;
using __2::ranges::range_reference_t;
using __2::ranges::range_difference_t;
using __2::ranges::iter_reference_t;
using __2::ranges::contiguous_iterator;
using __2::ranges::__range_adaptor_closure;
using __2::ranges::enable_borrowed_range;
using __2::ranges::begin;
using __2::ranges::end;
using __2::ranges::distance;
using __2::ranges::next;
using __2::ranges::data;
using __2::ranges::size;
using __2::ranges::__span_compatible_range;
using __2::ranges::__simple_view;
using __2::ranges::enable_view;
using __2::ranges::elements_view;
}

} // namespace std
} // namespace v8

#endif // WASI_RANGES_COMPAT_H_