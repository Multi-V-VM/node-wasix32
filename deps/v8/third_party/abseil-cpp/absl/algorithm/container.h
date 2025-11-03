// Minimal absl/algorithm/container.h implementation for WASI build.
// Provides absl::c_* wrappers that forward to std algorithms using
// std::begin/std::end to avoid ADL pitfalls.

#ifndef ABSL_ALGORITHM_CONTAINER_H_
#define ABSL_ALGORITHM_CONTAINER_H_

#include <algorithm>
#include <iterator>

namespace absl {

// find / find_if / find_if_not
template <class C, class T>
auto c_find(C& c, const T& value) { return std::find(std::begin(c), std::end(c), value); }
template <class C, class Pred>
auto c_find_if(C& c, Pred p) { return std::find_if(std::begin(c), std::end(c), p); }
template <class C, class Pred>
auto c_find_if_not(C& c, Pred p) { return std::find_if_not(std::begin(c), std::end(c), p); }

// count / count_if
template <class C, class T>
auto c_count(const C& c, const T& value) { return std::count(std::begin(c), std::end(c), value); }
template <class C, class Pred>
auto c_count_if(const C& c, Pred p) { return std::count_if(std::begin(c), std::end(c), p); }

// any_of / all_of / none_of
template <class C, class Pred>
auto c_any_of(const C& c, Pred p) { return std::any_of(std::begin(c), std::end(c), p); }
template <class C, class Pred>
auto c_all_of(const C& c, Pred p) { return std::all_of(std::begin(c), std::end(c), p); }
template <class C, class Pred>
auto c_none_of(const C& c, Pred p) { return std::none_of(std::begin(c), std::end(c), p); }

// for_each
template <class C, class F>
auto c_for_each(C& c, F f) { return std::for_each(std::begin(c), std::end(c), f); }

// copy / copy_if / move
template <class C, class OutputIt>
auto c_copy(const C& c, OutputIt out) { return std::copy(std::begin(c), std::end(c), out); }
template <class C, class OutputIt, class Pred>
auto c_copy_if(const C& c, OutputIt out, Pred p) { return std::copy_if(std::begin(c), std::end(c), out, p); }
template <class C, class OutputIt>
auto c_move(C& c, OutputIt out) { return std::move(std::begin(c), std::end(c), out); }

// remove / remove_if / remove_copy / remove_copy_if
template <class C, class T>
auto c_remove(C& c, const T& value) { return std::remove(std::begin(c), std::end(c), value); }
template <class C, class Pred>
auto c_remove_if(C& c, Pred p) { return std::remove_if(std::begin(c), std::end(c), p); }
template <class C, class OutputIt, class T>
auto c_remove_copy(const C& c, OutputIt out, const T& value) { return std::remove_copy(std::begin(c), std::end(c), out, value); }
template <class C, class OutputIt, class Pred>
auto c_remove_copy_if(const C& c, OutputIt out, Pred p) { return std::remove_copy_if(std::begin(c), std::end(c), out, p); }

// replace / replace_if
template <class C, class T>
void c_replace(C& c, const T& old_value, const T& new_value) { std::replace(std::begin(c), std::end(c), old_value, new_value); }
template <class C, class Pred, class T>
void c_replace_if(C& c, Pred p, const T& new_value) { std::replace_if(std::begin(c), std::end(c), p, new_value); }

// fill
template <class C, class T>
void c_fill(C& c, const T& value) { std::fill(std::begin(c), std::end(c), value); }

// transform
template <class C, class OutputIt, class UnaryOp>
auto c_transform(const C& c, OutputIt out, UnaryOp op) { return std::transform(std::begin(c), std::end(c), out, op); }
template <class C1, class C2, class OutputIt, class BinaryOp>
auto c_transform(const C1& c1, const C2& c2, OutputIt out, BinaryOp op) { return std::transform(std::begin(c1), std::end(c1), std::begin(c2), out, op); }

// unique / unique_copy
template <class C>
auto c_unique(C& c) { return std::unique(std::begin(c), std::end(c)); }
template <class C, class BinaryPred>
auto c_unique(C& c, BinaryPred p) { return std::unique(std::begin(c), std::end(c), p); }
template <class C, class OutputIt>
auto c_unique_copy(const C& c, OutputIt out) { return std::unique_copy(std::begin(c), std::end(c), out); }

// reverse / reverse_copy
template <class C>
void c_reverse(C& c) { std::reverse(std::begin(c), std::end(c)); }
template <class C, class OutputIt>
auto c_reverse_copy(const C& c, OutputIt out) { return std::reverse_copy(std::begin(c), std::end(c), out); }

// rotate / rotate_copy
template <class C>
auto c_rotate(C& c, typename C::iterator middle) { return std::rotate(std::begin(c), middle, std::end(c)); }
template <class C, class OutputIt>
auto c_rotate_copy(const C& c, typename C::const_iterator middle, OutputIt out) { return std::rotate_copy(std::begin(c), middle, std::end(c), out); }

// sort / stable_sort / partial_sort / nth_element
template <class C>
void c_sort(C& c) { std::sort(std::begin(c), std::end(c)); }
template <class C, class Compare>
void c_sort(C& c, Compare comp) { std::sort(std::begin(c), std::end(c), comp); }
template <class C>
void c_stable_sort(C& c) { std::stable_sort(std::begin(c), std::end(c)); }
template <class C, class Compare>
void c_stable_sort(C& c, Compare comp) { std::stable_sort(std::begin(c), std::end(c), comp); }
template <class C>
void c_partial_sort(C& c, typename C::iterator middle) { std::partial_sort(std::begin(c), middle, std::end(c)); }
template <class C, class Compare>
void c_partial_sort(C& c, typename C::iterator middle, Compare comp) { std::partial_sort(std::begin(c), middle, std::end(c), comp); }
template <class C>
void c_nth_element(C& c, typename C::iterator nth) { std::nth_element(std::begin(c), nth, std::end(c)); }
template <class C, class Compare>
void c_nth_element(C& c, typename C::iterator nth, Compare comp) { std::nth_element(std::begin(c), nth, std::end(c), comp); }

// lower_bound / upper_bound / equal_range / binary_search
template <class C, class T>
auto c_lower_bound(C& c, const T& value) { return std::lower_bound(std::begin(c), std::end(c), value); }
template <class C, class T, class Compare>
auto c_lower_bound(C& c, const T& value, Compare comp) { return std::lower_bound(std::begin(c), std::end(c), value, comp); }
template <class C, class T>
auto c_upper_bound(C& c, const T& value) { return std::upper_bound(std::begin(c), std::end(c), value); }
template <class C, class T, class Compare>
auto c_upper_bound(C& c, const T& value, Compare comp) { return std::upper_bound(std::begin(c), std::end(c), value, comp); }
template <class C, class T>
auto c_equal_range(C& c, const T& value) { return std::equal_range(std::begin(c), std::end(c), value); }
template <class C, class T, class Compare>
auto c_equal_range(C& c, const T& value, Compare comp) { return std::equal_range(std::begin(c), std::end(c), value, comp); }
template <class C, class T>
auto c_binary_search(const C& c, const T& value) { return std::binary_search(std::begin(c), std::end(c), value); }

// min/max/minmax_element
template <class C>
auto c_min_element(C& c) { return std::min_element(std::begin(c), std::end(c)); }
template <class C, class Compare>
auto c_min_element(C& c, Compare comp) { return std::min_element(std::begin(c), std::end(c), comp); }
template <class C>
auto c_max_element(C& c) { return std::max_element(std::begin(c), std::end(c)); }
template <class C, class Compare>
auto c_max_element(C& c, Compare comp) { return std::max_element(std::begin(c), std::end(c), comp); }
template <class C>
auto c_minmax_element(C& c) { return std::minmax_element(std::begin(c), std::end(c)); }
template <class C, class Compare>
auto c_minmax_element(C& c, Compare comp) { return std::minmax_element(std::begin(c), std::end(c), comp); }

}  // namespace absl

#endif  // ABSL_ALGORITHM_CONTAINER_H_

