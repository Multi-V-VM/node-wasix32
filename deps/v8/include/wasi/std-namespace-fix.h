#ifndef V8_INCLUDE_WASI_STD_NAMESPACE_FIX_H_
#define V8_INCLUDE_WASI_STD_NAMESPACE_FIX_H_

// Include ranges compatibility layer first
#ifdef __wasi__
#include "wasi/ranges-compat.h"
#include "wasi/crdtp-namespace-fix.h"
#endif

// Ensure global std namespace is accessible
namespace std {}

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
#include <iomanip>
#include <stdexcept>
#include <cstdlib>

namespace v8 {
namespace std {

// Forward string type aliases
using ::std::string;
using ::std::wstring;
using ::std::u8string;
using ::std::u16string;
using ::std::u32string;
using ::std::string_view;
using ::std::size_t;

// Forward use_facet template
template<typename _Facet>
inline const _Facet& use_facet(const ::std::locale& __l) {
    return ::std::use_facet<_Facet>(__l);
}

// Forward __put_character_sequence
template<typename _CharT, typename _Traits>
inline ::std::basic_ostream<_CharT, _Traits>&
__put_character_sequence(::std::basic_ostream<_CharT, _Traits>& __os, 
                        const _CharT* __str, size_t __len) {
    return ::std::__put_character_sequence(__os, __str, __len);
}

// Forward manipulators
using ::std::noskipws;
using ::std::skipws;

// Math functions
using ::std::signbit;
using ::std::isnan;
using ::std::memset;
using ::std::uninitialized_fill_n;
using ::std::logical_not;

// Memory utilities
template<typename T, typename Deleter = ::std::default_delete<T>> using unique_ptr = ::std::unique_ptr<T, Deleter>;
template<typename T> using shared_ptr = ::std::shared_ptr<T>;
template<typename T> using weak_ptr = ::std::weak_ptr<T>;
template<typename T> using enable_shared_from_this = ::std::enable_shared_from_this<T>;

// Atomic types
template<typename T> using atomic = ::std::atomic<T>;
using atomic_bool = ::std::atomic_bool;
using atomic_uint = ::std::atomic_uint;

// Atomic functions
template<typename T>
inline T atomic_fetch_add_explicit(::std::atomic<T>* ptr, T value, ::std::memory_order order) {
    return ::std::atomic_fetch_add_explicit(ptr, value, order);
}

template<typename T>
inline T atomic_exchange_explicit(::std::atomic<T>* ptr, T value, ::std::memory_order order) {
    return ::std::atomic_exchange_explicit(ptr, value, order);
}

template<typename T>
inline bool atomic_compare_exchange_strong_explicit(::std::atomic<T>* ptr, T* expected, T desired, 
                                                   ::std::memory_order success, ::std::memory_order failure) {
    return ::std::atomic_compare_exchange_strong_explicit(ptr, expected, desired, success, failure);
}

// Mutex types
using mutex = ::std::mutex;
template<typename T> using lock_guard = ::std::lock_guard<T>;

// Memory order constants
using ::std::memory_order_relaxed;
using ::std::memory_order_consume;
using ::std::memory_order_acquire;
using ::std::memory_order_release;
using ::std::memory_order_acq_rel;
using ::std::memory_order_seq_cst;

// Numeric limits
template<typename T> using numeric_limits = ::std::numeric_limits<T>;

// Container types
template<typename T, typename Alloc = ::std::allocator<T>> 
using vector = ::std::vector<T, Alloc>;

// Tuple type
template<typename... Types>
using tuple = ::std::tuple<Types...>;

// Array type
template<typename T, size_t N>
using array = ::std::array<T, N>;

template<typename Key, typename T, typename Hash = ::std::hash<Key>, 
         typename KeyEqual = ::std::equal_to<Key>, 
         typename Alloc = ::std::allocator<::std::pair<const Key, T>>>
using unordered_map = ::std::unordered_map<Key, T, Hash, KeyEqual, Alloc>;

template<typename Key, typename T, typename Hash = ::std::hash<Key>, 
         typename KeyEqual = ::std::equal_to<Key>, 
         typename Alloc = ::std::allocator<::std::pair<const Key, T>>>
using unordered_multimap = ::std::unordered_multimap<Key, T, Hash, KeyEqual, Alloc>;

template<typename T, typename Container = ::std::deque<T>>
using queue = ::std::queue<T, Container>;

template<typename T, typename Container = ::std::deque<T>>
using stack = ::std::stack<T, Container>;

template<typename T, typename Compare = ::std::less<T>, typename Alloc = ::std::allocator<T>>
using set = ::std::set<T, Compare, Alloc>;

template<typename Key, typename T, typename Compare = ::std::less<Key>, typename Alloc = ::std::allocator<::std::pair<const Key, T>>>
using map = ::std::map<Key, T, Compare, Alloc>;

template<typename T, typename Alloc = ::std::allocator<T>>
using list = ::std::list<T, Alloc>;

template<typename Key, typename T, typename Compare = ::std::less<Key>, typename Alloc = ::std::allocator<::std::pair<const Key, T>>>
using multimap = ::std::multimap<Key, T, Compare, Alloc>;

// Chrono namespace additions
namespace chrono {
    using ::std::chrono::duration;
    using ::std::chrono::time_point;
    using ::std::chrono::system_clock;
    using ::std::chrono::steady_clock;
    using ::std::chrono::high_resolution_clock;
    
    // Common duration types
    using ::std::chrono::nanoseconds;
    using ::std::chrono::microseconds;
    using ::std::chrono::milliseconds;
    using ::std::chrono::seconds;
    using ::std::chrono::minutes;
    using ::std::chrono::hours;
}

// Type traits
template<typename T> using is_trivially_copyable = ::std::is_trivially_copyable<T>;
template<typename T> using is_trivially_destructible = ::std::is_trivially_destructible<T>;

// Utility templates
template<typename T> using optional = ::std::optional<T>;
using nullopt_t = ::std::nullopt_t;
inline constexpr nullopt_t nullopt = ::std::nullopt;

// Initializer list
template<typename T>
using initializer_list = ::std::initializer_list<T>;

// declval function template
template<typename T>
typename ::std::add_rvalue_reference<T>::type declval() noexcept;

// nullptr_t type
using nullptr_t = ::std::nullptr_t;

// nothrow symbols
using nothrow_t = ::std::nothrow_t;
inline constexpr nothrow_t nothrow = ::std::nothrow;

// Tuple utilities
template<size_t I, typename T>
constexpr auto get(T&& t) -> decltype(::std::get<I>(::std::forward<T>(t))) {
    return ::std::get<I>(::std::forward<T>(t));
}

// Integer sequences
template<typename T, T... Ints>
using integer_sequence = ::std::integer_sequence<T, Ints...>;

template<size_t... Ints>
using index_sequence = ::std::index_sequence<Ints...>;

template<size_t N>
using make_index_sequence = ::std::make_index_sequence<N>;

template<typename... T>
using index_sequence_for = ::std::index_sequence_for<T...>;

// Forward is a function template, not a type alias
template<typename T>
constexpr T&& forward(typename ::std::remove_reference<T>::type& t) noexcept {
    return ::std::forward<T>(t);
}

template<typename T>
constexpr T&& forward(typename ::std::remove_reference<T>::type&& t) noexcept {
    return ::std::forward<T>(t);
}

// Additional utility functions
template<typename T, typename... Args>
inline ::std::unique_ptr<T> make_unique(Args&&... args) {
    return ::std::make_unique<T>(::std::forward<Args>(args)...);
}

// move function
template<typename T>
constexpr typename ::std::remove_reference<T>::type&& move(T&& t) noexcept {
    return ::std::move(t);
}

// Type traits
template<typename T> using is_unsigned = ::std::is_unsigned<T>;
template<bool B, typename T = void> using enable_if = ::std::enable_if<B, T>;
template<bool B, typename T = void> using enable_if_t = ::std::enable_if_t<B, T>;
template<typename From, typename To> using is_convertible = ::std::is_convertible<From, To>;
template<typename From, typename To> constexpr bool is_convertible_v = ::std::is_convertible_v<From, To>;
template<typename T, T v> using integral_constant = ::std::integral_constant<T, v>;
template<typename... Ts> using void_t = ::std::void_t<Ts...>;
template<typename T> using underlying_type = ::std::underlying_type<T>;
template<typename T> using underlying_type_t = ::std::underlying_type_t<T>;
template<std::size_t I, typename T> using tuple_element = ::std::tuple_element<I, T>;
template<std::size_t I, typename T> using tuple_element_t = ::std::tuple_element_t<I, T>;
template<typename Base, typename Derived> using is_base_of = ::std::is_base_of<Base, Derived>;
template<typename T> using is_enum = ::std::is_enum<T>;
using false_type = ::std::false_type;
using true_type = ::std::true_type;
// Add missing conjunction, disjunction, and negation
template<typename... Args> using conjunction = ::std::conjunction<Args...>;
template<typename... Args> constexpr bool conjunction_v = ::std::conjunction_v<Args...>;
template<typename... Args> using disjunction = ::std::disjunction<Args...>;
template<typename... Args> constexpr bool disjunction_v = ::std::disjunction_v<Args...>;
template<typename B> using negation = ::std::negation<B>;
template<typename B> constexpr bool negation_v = ::std::negation_v<B>;
template<typename T> using is_integral = ::std::is_integral<T>;
template<typename T> constexpr bool is_integral_v = ::std::is_integral_v<T>;
template<typename T> using remove_reference = ::std::remove_reference<T>;
template<typename T> using remove_reference_t = ::std::remove_reference_t<T>;
template<typename T> using remove_cv = ::std::remove_cv<T>;
template<typename T> using remove_cv_t = ::std::remove_cv_t<T>;
template<typename T> using remove_cvref = ::std::remove_cvref<T>;
template<typename T> using remove_cvref_t = ::std::remove_cvref_t<T>;
template<typename T> using is_const = ::std::is_const<T>;
template<typename T> constexpr bool is_const_v = ::std::is_const_v<T>;
template<typename T> using is_trivial = ::std::is_trivial<T>;
template<typename T> constexpr bool is_trivial_v = ::std::is_trivial_v<T>;
template<typename T> using is_standard_layout = ::std::is_standard_layout<T>;
template<typename T> constexpr bool is_standard_layout_v = ::std::is_standard_layout_v<T>;
template<typename T> using is_signed = ::std::is_signed<T>;
template<typename T, typename U> using is_same = ::std::is_same<T, U>;
template<typename T, typename U> constexpr bool is_same_v = ::std::is_same_v<T, U>;
template<typename T> using is_floating_point = ::std::is_floating_point<T>;
template<typename T> constexpr bool is_floating_point_v = ::std::is_floating_point_v<T>;
template<typename Base, typename Derived> constexpr bool is_base_of_v = ::std::is_base_of_v<Base, Derived>;
template<typename T> constexpr bool is_copy_assignable_v = ::std::is_copy_assignable_v<T>;
template<typename T> using is_arithmetic = ::std::is_arithmetic<T>;
template<typename T> using is_destructible = ::std::is_destructible<T>;
template<typename T> using is_copy_constructible = ::std::is_copy_constructible<T>;
template<typename T> using is_copy_assignable = ::std::is_copy_assignable<T>;
template<typename... T> using is_constructible = ::std::is_constructible<T...>;
template<typename... T> constexpr bool is_constructible_v = ::std::is_constructible_v<T...>;
template<typename T> using is_default_constructible = ::std::is_default_constructible<T>;
template<typename T> constexpr bool is_default_constructible_v = ::std::is_default_constructible_v<T>;
template<typename T> using is_void = ::std::is_void<T>;
template<typename T> using make_signed = ::std::make_signed<T>;
template<typename T> using make_signed_t = ::std::make_signed_t<T>;
template<typename T> using make_unsigned = ::std::make_unsigned<T>;
template<typename T> using make_unsigned_t = ::std::make_unsigned_t<T>;
template<bool B, typename T, typename F> using conditional = ::std::conditional<B, T, F>;
template<bool B, typename T, typename F> using conditional_t = ::std::conditional_t<B, T, F>;
template<typename T> using remove_const = ::std::remove_const<T>;
template<typename T> using remove_const_t = ::std::remove_const_t<T>;
template<typename T> using is_array = ::std::is_array<T>;
template<bool B> using bool_constant = ::std::bool_constant<B>;
template<typename T> using tuple_size = ::std::tuple_size<T>;
template<typename T> constexpr size_t tuple_size_v = ::std::tuple_size_v<T>;

// bit_cast template
#if __cplusplus >= 202002L && __has_builtin(__builtin_bit_cast)
template<typename To, typename From>
constexpr To bit_cast(const From& from) {
    return __builtin_bit_cast(To, from);
}
#else
template<typename To, typename From>
inline To bit_cast(const From& from) {
    static_assert(sizeof(To) == sizeof(From), "Size mismatch in bit_cast");
    To to;
    ::std::memcpy(&to, &from, sizeof(To));
    return to;
}
#endif

// to_address function
template<typename Ptr>
inline auto to_address(Ptr const& p) noexcept -> decltype(::std::to_address(p)) {
    return ::std::to_address(p);
}

template<typename T>
inline T* to_address(T* p) noexcept {
    return p;
}

// data and size functions for containers
template<typename C>
auto data(C& c) -> decltype(c.data()) {
    return c.data();
}

template<typename C>
auto data(const C& c) -> decltype(c.data()) {
    return c.data();
}

template<typename T, size_t N>
constexpr T* data(T (&array)[N]) noexcept {
    return array;
}

template<typename E>
constexpr const E* data(::std::initializer_list<E> il) noexcept {
    return il.begin();
}

template<typename C>
auto size(const C& c) -> decltype(c.size()) {
    return c.size();
}

template<typename T, size_t N>
constexpr size_t size(const T (&)[N]) noexcept {
    return N;
}

// Algorithms
using ::std::max;
using ::std::min;

// Stream types
using ostream = ::std::ostream;
using istream = ::std::istream;
using stringstream = ::std::stringstream;
using ostringstream = ::std::ostringstream;
using streambuf = ::std::streambuf;
template<typename CharT, typename Traits = ::std::char_traits<CharT>>
using basic_streambuf = ::std::basic_streambuf<CharT, Traits>;

// Size type
using size_t = ::std::size_t;
using ptrdiff_t = ::std::ptrdiff_t;

// String types  
template<typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
using basic_string = ::std::basic_string<CharT, Traits, Alloc>;

template<typename CharT, typename Traits = ::std::char_traits<CharT>>
using basic_string_view = ::std::basic_string_view<CharT, Traits>;

using u8string_view = ::std::u8string_view;

// Endian enum
using endian = ::std::endian;

// Type traits
template<typename T> using is_function = ::std::is_function<T>;

// Container types
template<typename T, typename Hash = ::std::hash<T>, 
         typename KeyEqual = ::std::equal_to<T>, 
         typename Alloc = ::std::allocator<T>>
using unordered_set = ::std::unordered_set<T, Hash, KeyEqual, Alloc>;

// Function objects
template<typename T> using function = ::std::function<T>;
template<typename T> using equal_to = ::std::equal_to<T>;
template<typename T> using less = ::std::less<T>;

// Iterator functions
template<typename C>
auto begin(C& c) -> decltype(::std::begin(c)) {
    return ::std::begin(c);
}

template<typename C>
auto begin(const C& c) -> decltype(::std::begin(c)) {
    return ::std::begin(c);
}

template<typename C>
auto end(C& c) -> decltype(::std::end(c)) {
    return ::std::end(c);
}

template<typename C>
auto end(const C& c) -> decltype(::std::end(c)) {
    return ::std::end(c);
}

// Hash template
template<typename T> using hash = ::std::hash<T>;

// Pair template
template<typename T1, typename T2> using pair = ::std::pair<T1, T2>;

// make_pair function
template<typename T1, typename T2>
constexpr auto make_pair(T1&& a, T2&& b) -> decltype(::std::make_pair(::std::forward<T1>(a), ::std::forward<T2>(b))) {
    return ::std::make_pair(::std::forward<T1>(a), ::std::forward<T2>(b));
}

// Iterator tags
using input_iterator_tag = ::std::input_iterator_tag;
using output_iterator_tag = ::std::output_iterator_tag;
using forward_iterator_tag = ::std::forward_iterator_tag;
using bidirectional_iterator_tag = ::std::bidirectional_iterator_tag;
using random_access_iterator_tag = ::std::random_access_iterator_tag;
using contiguous_iterator_tag = ::std::contiguous_iterator_tag;

// Iterator types and traits
template<typename T> using iterator_traits = ::std::iterator_traits<T>;
template<typename Iterator>
using reverse_iterator = ::std::reverse_iterator<Iterator>;

template<typename Iterator>
auto make_reverse_iterator(Iterator it) -> decltype(::std::make_reverse_iterator(it)) {
    return ::std::make_reverse_iterator(it);
}

// String functions
inline size_t strlen(const char* str) {
    return ::strlen(str);
}

// Math functions
using ::std::ceil;
using ::std::floor;
using ::std::round;
using ::std::swap;
using ::std::isnan;
using ::std::isinf;
using ::std::isfinite;
using ::std::fmod;
using ::std::trunc;
using ::std::abs;

// Iterator functions
using ::std::next;
using ::std::prev;
using ::std::advance;

// IO stream types and constants
namespace ios {
    using fmtflags = ::std::ios::fmtflags;
    constexpr auto boolalpha = ::std::ios::boolalpha;
    constexpr auto dec = ::std::ios::dec;
    constexpr auto fixed = ::std::ios::fixed;
    constexpr auto hex = ::std::ios::hex;
    constexpr auto internal = ::std::ios::internal;
    constexpr auto left = ::std::ios::left;
    constexpr auto oct = ::std::ios::oct;
    constexpr auto right = ::std::ios::right;
    constexpr auto scientific = ::std::ios::scientific;
    constexpr auto showbase = ::std::ios::showbase;
    constexpr auto showpoint = ::std::ios::showpoint;
    constexpr auto showpos = ::std::ios::showpos;
    constexpr auto skipws = ::std::ios::skipws;
    constexpr auto unitbuf = ::std::ios::unitbuf;
    constexpr auto uppercase = ::std::ios::uppercase;
}

// to_string functions
inline string to_string(int val) {
    return ::std::to_string(val);
}

inline string to_string(long val) {
    return ::std::to_string(val);
}

inline string to_string(long long val) {
    return ::std::to_string(val);
}

inline string to_string(unsigned val) {
    return ::std::to_string(val);
}

inline string to_string(unsigned long val) {
    return ::std::to_string(val);
}

inline string to_string(unsigned long long val) {
    return ::std::to_string(val);
}

// String conversion functions
inline int stoi(const string& str, size_t* pos = nullptr, int base = 10) {
    return ::std::stoi(str, pos, base);
}

inline double stod(const string& str, size_t* pos = nullptr) {
    return ::std::stod(str, pos);
}

inline unsigned long long stoull(const string& str, size_t* pos = nullptr, int base = 10) {
    return ::std::stoull(str, pos, base);
}

// System functions
using ::std::exit;

// Exception types
using exception = ::std::exception;
using invalid_argument = ::std::invalid_argument;
using out_of_range = ::std::out_of_range;
using runtime_error = ::std::runtime_error;
using logic_error = ::std::logic_error;

// Algorithm functions
template<typename InputIt, typename UnaryPredicate>
bool all_of(InputIt first, InputIt last, UnaryPredicate p) {
    return ::std::all_of(first, last, p);
}

template<typename InputIt, typename UnaryPredicate>
bool any_of(InputIt first, InputIt last, UnaryPredicate p) {
    return ::std::any_of(first, last, p);
}

template<typename InputIt, typename UnaryPredicate>
bool none_of(InputIt first, InputIt last, UnaryPredicate p) {
    return ::std::none_of(first, last, p);
}

template<typename ForwardIt, typename UnaryPredicate>
ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p) {
    return ::std::remove_if(first, last, p);
}

template<typename BidirIt>
void reverse(BidirIt first, BidirIt last) {
    ::std::reverse(first, last);
}

template<typename InputIt, typename OutputIt, typename UnaryOp>
OutputIt transform(InputIt first, InputIt last, OutputIt d_first, UnaryOp op) {
    return ::std::transform(first, last, d_first, op);
}

// back_inserter is causing ambiguity - use the std version directly
using ::std::back_inserter;

template<typename InputIt, typename OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
    return ::std::copy(first, last, d_first);
}

template<typename InputIt, typename OutputIt, typename Size>
OutputIt copy_n(InputIt first, Size count, OutputIt result) {
    return ::std::copy_n(first, count, result);
}

template<typename InputIt, typename ForwardIt>
ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first) {
    return ::std::uninitialized_copy(first, last, d_first);
}

template<typename InputIt1, typename InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
    return ::std::equal(first1, last1, first2);
}

template<typename OutputIt, typename Size, typename T>
OutputIt fill_n(OutputIt first, Size count, const T& value) {
    return ::std::fill_n(first, count, value);
}

template<typename T>
inline ::std::unique_ptr<T> make_unique_for_overwrite() {
    return ::std::make_unique_for_overwrite<T>();
}

// Additional algorithms
using ::std::distance;
using ::std::find;
using ::std::replace_if;
using ::std::unique_copy;
using ::std::min_element;
using ::std::tie;
using ::std::replace;
using ::std::copy_if;
using ::std::sort;
using ::std::fill;
using ::std::uninitialized_value_construct_n;

// Memory utilities
using ::std::addressof;

// Additional type utilities
template<typename T>
inline auto __to_unsigned_like(T val) -> decltype(::std::__to_unsigned_like(val)) {
    return ::std::__to_unsigned_like(val);
}

// Atomic functions
using ::std::atomic_fetch_add_explicit;
using ::std::atomic_exchange_explicit;
using ::std::atomic_compare_exchange_strong_explicit;
using ::std::atomic_fetch_and_explicit;
using ::std::atomic_store_explicit;
using ::std::atomic_load_explicit;
using ::std::atomic_thread_fence;
using ::std::memory_order_seq_cst;

// Stream manipulators
using ::std::setprecision;
using ::std::hex;
using ::std::dec;
using ::std::isdigit;
using ::std::setfill;
using ::std::setw;
using ::std::endl;

// Quoted manipulator
template<typename CharT, typename Traits, typename Allocator>
inline auto __quoted(const ::std::basic_string<CharT, Traits, Allocator>& s,
                     CharT delim = CharT('"'), CharT escape = CharT('\\')) {
    return ::std::quoted(s, delim, escape);
}

template<typename CharT, typename Traits, typename Allocator>
inline auto __quoted(::std::basic_string<CharT, Traits, Allocator>& s,
                     CharT delim = CharT('"'), CharT escape = CharT('\\')) {
    return ::std::quoted(s, delim, escape);
}

// Standard streams
using ::std::cerr;
using ::std::cout;
using ::std::cin;

// Tuple utilities
using ::std::ignore;
using ::std::make_tuple;

// Character classification functions
using ::std::isspace;
using ::std::isalpha;
using ::std::isalnum;
using ::std::isxdigit;
using ::std::toupper;
using ::std::tolower;

// Locale functions
using ::std::locale;

// String literals namespace
namespace string_literals {
    using namespace ::std::string_literals;
}

// C++20 concepts
#if __cplusplus >= 202002L
template<typename T, typename U>
concept same_as = ::std::same_as<T, U>;

template<typename From, typename To>
concept convertible_to = ::std::convertible_to<From, To>;

template<typename Derived, typename Base>
concept derived_from = ::std::derived_from<Derived, Base>;
#endif

// Smart pointer factories
template<typename T, typename... Args>
inline ::std::shared_ptr<T> make_shared(Args&&... args) {
    return ::std::make_shared<T>(::std::forward<Args>(args)...);
}

// Functional utilities
template<typename Fn, typename... Args>
inline auto invoke(Fn&& fn, Args&&... args) -> decltype(::std::invoke(::std::forward<Fn>(fn), ::std::forward<Args>(args)...)) {
    return ::std::invoke(::std::forward<Fn>(fn), ::std::forward<Args>(args)...);
}

}  // namespace std
}  // namespace v8

// Do not create aliases in ::std::ranges to avoid conflicts with standard library

#endif  // V8_INCLUDE_WASI_STD_NAMESPACE_FIX_H_

