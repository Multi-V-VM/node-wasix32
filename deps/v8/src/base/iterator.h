// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_ITERATOR_H_
#define V8_BASE_ITERATOR_H_

#include <iterator>
#include <utility>

namespace v8 {
namespace base {

template <class Category, class Type, class Diff = std::ptrdiff_t,
          class Pointer = Type*, class Reference = Type&>
struct iterator {
  using iterator_category = Category;
  using value_type = Type;
  using difference_type = Diff;
  using pointer = Pointer;
  using reference = Reference;
};

// The intention of the base::iterator_range class is to encapsulate two
// iterators so that the range defined by the iterators can be used like
// a regular STL container (actually only a subset of the full container
// functionality is available usually).
template <typename ForwardIterator>
class iterator_range {
 public:
  using iterator = ForwardIterator;
  using const_iterator = ForwardIterator;
  using pointer = typename std::iterator_traits<iterator>::pointer;
  using reference = typename std::iterator_traits<iterator>::reference;
  using value_type = typename std::iterator_traits<iterator>::value_type;
  using difference_type =
      typename std::iterator_traits<iterator>::difference_type;

  iterator_range() : begin_(), end_() {}
  iterator_range(ForwardIterator begin, ForwardIterator end)
      : begin_(begin), end_(end) {}

  iterator begin() const { return begin_; }
  iterator end() const { return end_; }
  const_iterator cbegin() const { return begin_; }
  const_iterator cend() const { return end_; }
  auto rbegin() const { return std::make_reverse_iterator(end_); }
  auto rend() const { return std::make_reverse_iterator(begin_); }

  bool empty() const { return cbegin() == cend(); }

  // Random Access iterators only.
  reference operator[](difference_type n) { return begin()[n]; }
  difference_type size() const { return cend() - cbegin(); }

 private:
  const_iterator const begin_;
  const_iterator const end_;
};

template <typename ForwardIterator>
auto make_iterator_range(ForwardIterator begin, ForwardIterator end) {
  return iterator_range<ForwardIterator>{begin, end};
}

template <class T>
struct DerefPtrIterator : base::iterator<std::bidirectional_iterator_tag, T> {
  T* const* ptr;

  explicit DerefPtrIterator(T* const* ptr) : ptr(ptr) {}

  T& operator*() { return **ptr; }
  DerefPtrIterator& operator++() {
    ++ptr;
    return *this;
  }
  DerefPtrIterator& operator--() {
    --ptr;
    return *this;
  }
  bool operator!=(DerefPtrIterator other) { return ptr != other.ptr; }
};

// {Reversed} returns a container adapter usable in a range-based "for"
// statement for iterating a reversible container in reverse order.
//
// Example:
//
//   std::vector<int> v = ...;
//   for (int i : base::Reversed(v)) {
//     // iterates through v from back to front
//   }
//
// The signature avoids binding to temporaries (T&& / const T&) on purpose. The
// lifetime of a temporary would not extend to a range-based for loop using it.
template <typename T>
auto Reversed(T& t) {
  return make_iterator_range(std::rbegin(t), std::rend(t));
}

// This overload of `Reversed` is safe even when the argument is a temporary,
// because we rely on the wrapped iterators instead of the `iterator_range`
// object itself.
template <typename T>
auto Reversed(const iterator_range<T>& t) {
  return make_iterator_range(std::rbegin(t), std::rend(t));
}

// {IterateWithoutLast} returns a container adapter usable in a range-based
// "for" statement for iterating all elements without the last in a forward
// order. It performs a check whether the container is empty.
//
// Example:
//
//   std::vector<int> v = ...;
//   for (int i : base::IterateWithoutLast(v)) {
//     // iterates through v front to --back
//   }
//
// The signature avoids binding to temporaries, see the remark in {Reversed}.
template <typename T>
auto IterateWithoutLast(T& t) {
  DCHECK_NE(std::begin(t), std::end(t));
  auto new_end = std::end(t);
  return make_iterator_range(std::begin(t), --new_end);
}

template <typename T>
auto IterateWithoutLast(const iterator_range<T>& t) {
  iterator_range<T> range_copy = {t.begin(), t.end()};
  return IterateWithoutLast(range_copy);
}

// {IterateWithoutFirst} returns a container adapter usable in a range-based
// "for" statement for iterating all elements except the first.
//
// Example:
//
//   std::vector<int> v = ...;
//   for (int i : base::IterateWithoutFirst(v)) {
//     // iterates through v from ++front to back
//   }
//
// The signature avoids binding to temporaries, see the remark in {Reversed}.
template <typename T>
auto IterateWithoutFirst(T& t) {
  DCHECK_NE(std::begin(t), std::end(t));
  auto new_begin = std::begin(t);
  return make_iterator_range(++new_begin, std::end(t));
}

template <typename T>
auto IterateWithoutFirst(const iterator_range<T>& t) {
  iterator_range<T> range_copy = {t.begin(), t.end()};
  return IterateWithoutFirst(range_copy);
}

// zip - iterate over two containers in parallel, yielding pairs of elements.
//
// Example:
//   std::vector<int> a = {1, 2, 3};
//   std::vector<char> b = {'a', 'b', 'c'};
//   for (auto&& [x, y] : base::zip(a, b)) {
//     // x = 1, y = 'a', then x = 2, y = 'b', etc.
//   }
template <typename Iter1, typename Iter2>
class ZipIterator {
 public:
  using value_type = std::pair<typename std::iterator_traits<Iter1>::reference,
                               typename std::iterator_traits<Iter2>::reference>;
  using difference_type = std::ptrdiff_t;
  using pointer = void;
  using reference = value_type;
  using iterator_category = std::forward_iterator_tag;

  ZipIterator(Iter1 it1, Iter2 it2) : it1_(it1), it2_(it2) {}

  value_type operator*() const { return {*it1_, *it2_}; }

  ZipIterator& operator++() {
    ++it1_;
    ++it2_;
    return *this;
  }

  ZipIterator operator++(int) {
    ZipIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const ZipIterator& other) const { return it1_ == other.it1_; }
  bool operator!=(const ZipIterator& other) const { return !(*this == other); }

 private:
  Iter1 it1_;
  Iter2 it2_;
};

template <typename Container1, typename Container2>
class ZipRange {
 public:
  using iterator = ZipIterator<decltype(std::begin(std::declval<Container1&>())),
                               decltype(std::begin(std::declval<Container2&>()))>;

  ZipRange(Container1& c1, Container2& c2)
      : begin_(std::begin(c1), std::begin(c2)),
        end_(std::end(c1), std::end(c2)) {}

  iterator begin() const { return begin_; }
  iterator end() const { return end_; }

 private:
  iterator begin_;
  iterator end_;
};

template <typename Container1, typename Container2>
auto zip(Container1& c1, Container2& c2) {
  return ZipRange<Container1, Container2>(c1, c2);
}

template <typename Container1, typename Container2>
auto zip(const Container1& c1, const Container2& c2) {
  return ZipRange<const Container1, const Container2>(c1, c2);
}

}  // namespace base
}  // namespace v8

#endif  // V8_BASE_ITERATOR_H_
