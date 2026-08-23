// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_ZONE_ZONE_CONTAINERS_H_
#define V8_ZONE_ZONE_CONTAINERS_H_

#include <deque>
#include <cstring>
#include <stdio.h>
#include <new>
#include <forward_list>
#include <initializer_list>
#include <utility>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>

// Abseil includes: prefer system path, fall back to V8's third_party layout.
#ifndef __wasi__
#if __has_include("absl/container/btree_map.h")
# include "absl/container/btree_map.h"
#else
# include "../../third_party/abseil-cpp/absl/container/btree_map.h"
#endif
#if __has_include("absl/container/flat_hash_map.h")
# include "absl/container/flat_hash_map.h"
#else
# include "../../third_party/abseil-cpp/absl/container/flat_hash_map.h"
#endif
#if __has_include("absl/container/flat_hash_set.h")
# include "absl/container/flat_hash_set.h"
#else
# include "../../third_party/abseil-cpp/absl/container/flat_hash_set.h"
#endif
#endif  // __wasi__
#include "src/base/hashing.h"
#include "src/base/intrusive-set.h"
#include "src/base/small-map.h"
#include "src/base/small-vector.h"
#include "src/zone/zone-allocator.h"
#include "src/utils/allocation.h"
#include "src/base/vector.h"

// WASI: Don't try to provide abseil fallbacks, as abseil headers
// may still be included from other files. The abseil-based wrapper
// classes will be disabled below.

namespace v8 {

namespace internal {

// Forward declarations for DirectHandle types
template <typename T>
class DirectHandle;
template <typename T>
class DirectHandleUnchecked;

// Ensure C library memory functions are visible in this namespace
using ::memcpy;
using ::memmove;

// A drop-in replacement for std::vector that uses a Zone for its allocations,
// and (contrary to a std::vector subclass with custom allocator) gives us
// precise control over its implementation and performance characteristics.
//
// When working on this code, keep the following rules of thumb in mind:
// - Everything between {data_} and {end_} (exclusive) is a live instance of T.
//   When writing to these slots, use the {CopyingOverwrite} or
//   {MovingOverwrite} helpers.
// - Everything between {end_} (inclusive) and {capacity_} (exclusive) is
//   considered uninitialized memory. When writing to these slots, use the
//   {CopyToNewStorage} or {MoveToNewStorage} helpers. Obviously, also use
//   these helpers to initialize slots in newly allocated backing stores.
// - When shrinking, call ~T on all slots between the new and the old position
//   of {end_} to maintain the above invariant. Also call ~T on all slots in
//   discarded backing stores.
// - The interface offered by {ZoneVector} should be a subset of
//   {std::vector}'s API, so that calling code doesn't need to be aware of
//   ZoneVector's implementation details and can assume standard C++ behavior.
//   (It's okay if we don't support everything that std::vector supports; we
//   can fill such gaps when use cases arise.)
template <typename T>
class ZoneVector {
 public:
  // Default constructs an empty non-owning vector.
  ZoneVector() : zone_(nullptr), data_(nullptr), end_(nullptr), capacity_(nullptr) {}
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<T*>;
  using const_reverse_iterator = std::reverse_iterator<const T*>;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  // Constructs an empty vector.
  explicit ZoneVector(Zone* zone)
      : zone_(zone), data_(nullptr), end_(nullptr), capacity_(nullptr) {}

  // Constructs a non-owning view over existing memory. The lifetime of the
  // underlying storage is managed externally. No deallocation is performed
  // in the destructor when constructed this way.
  ZoneVector(T* data, size_t length) : zone_(nullptr) {
    data_ = data;
    end_ = capacity_ = data ? (data + length) : nullptr;
  }

  // Allocate a raw buffer-backed vector using the global allocator. The
  // returned ZoneVector is a non-owning view that should be paired with
  // DeleteArray(ptr) by the caller when finished.
  static ZoneVector<T> New(size_t length) {
    T* buffer = NewArray<T>(length);
    return ZoneVector<T>(buffer, length);
  }

  // Construct a non-owning view from a base::Vector when both source and target
  // element types are layout-compatible.
  // Only allow conversion if const-ness is preserved (can't cast const to non-const).
  template <typename S,
            typename = std::enable_if_t<(sizeof(S) == sizeof(T)) &&
                                        (!std::is_const_v<S> || std::is_const_v<T>)>>
  ZoneVector(::v8::base::Vector<S> v) : zone_(nullptr) {
    data_ = reinterpret_cast<T*>(v.begin());
    end_ = capacity_ = data_ + v.size();
  }

  // Direct conversion from base::Vector of same type
  ZoneVector(::v8::base::Vector<T> v) : zone_(nullptr) {
    data_ = v.begin();
    end_ = capacity_ = data_ + v.size();
  }

  // Produce a sliced non-owning view advanced by offset elements.
  ZoneVector<T> operator+(size_t offset) const {
    DCHECK_LE(offset, size());
    ZoneVector<T> out = *this;
    out.data_ += offset;
    return out;
  }

  // Advance this non-owning view by offset elements in-place.
  ZoneVector<T>& operator+=(size_t offset) {
    DCHECK_LE(offset, size());
    data_ += offset;
    return *this;
  }

  // Constructs a new vector and fills it with {size} elements, each
  // constructed via the default constructor.
  ZoneVector(size_t size, Zone* zone) : zone_(zone) {
    data_ = size > 0 ? zone->AllocateArray<T>(size) : nullptr;
    end_ = capacity_ = data_ + size;
    for (T* p = data_; p < end_; p++) emplace(p);
  }

  // Constructs a new vector and fills it with {size} elements, each
  // having the value {def}.
  ZoneVector(size_t size, T def, Zone* zone) : zone_(zone) {
    data_ = size > 0 ? zone->AllocateArray<T>(size) : nullptr;
    end_ = capacity_ = data_ + size;
    for (T* p = data_; p < end_; p++) emplace(p, def);
  }

  // Constructs a new vector and fills it with the contents of the given
  // initializer list.
  ZoneVector(std::initializer_list<T> list, Zone* zone) : zone_(zone) {
    size_t size = list.size();
    if (size > 0) {
      data_ = zone->AllocateArray<T>(size);
      CopyToNewStorage(data_, list.begin(), list.end());
    } else {
      data_ = nullptr;
    }
    end_ = capacity_ = data_ + size;
  }

  // Constructs a new vector and fills it with the contents of the range
  // [first, last).
  template <class It,
            typename = typename std::iterator_traits<It>::iterator_category>
  ZoneVector(It first, It last, Zone* zone) : zone_(zone) {
    if constexpr (std::is_base_of_v<
                      std::random_access_iterator_tag,
                      typename std::iterator_traits<It>::iterator_category>) {
      size_t size = last - first;
      data_ = size > 0 ? zone->AllocateArray<T>(size) : nullptr;
      end_ = capacity_ = data_ + size;
      for (T* p = data_; p < end_; p++) emplace(p, *first++);
    } else {
      while (first != last) push_back(*first++);
    }
    DCHECK_EQ(first, last);
  }

  ZoneVector(const ZoneVector& other) V8_NOEXCEPT
      : zone_(other.zone_), data_(nullptr), end_(nullptr), capacity_(nullptr) {
    if (zone_ == nullptr) {
      data_ = other.data_;
      end_ = other.end_;
      capacity_ = other.capacity_;
      return;
    }

    size_t new_capacity = other.capacity();
    if (new_capacity == 0) return;
    data_ = zone_->AllocateArray<T>(new_capacity);
    end_ = data_ + other.size();
    capacity_ = data_ + new_capacity;
    CopyToNewStorage(data_, other.data_, other.end_);
  }

  ZoneVector(ZoneVector&& other) V8_NOEXCEPT
      : zone_(other.zone_),
        data_(other.data_),
        end_(other.end_),
        capacity_(other.capacity_) {
    other.data_ = other.end_ = other.capacity_ = nullptr;
  }

  ~ZoneVector() {
    for (T* p = data_; p < end_; p++) p->~T();
    if (zone_ && data_) zone_->DeleteArray(data_, capacity());
  }

  // Assignment operators.
  ZoneVector& operator=(const ZoneVector& other) V8_NOEXCEPT {
    // Self-assignment would cause undefined behavior in the !copy_assignable
    // branch, but likely indicates a bug in calling code anyway.
    DCHECK_NE(this, &other);
    T* src = other.data_;
    if (capacity() >= other.size() && zone_ == other.zone_) {
      T* dst = data_;
      if constexpr (std::is_trivially_copyable_v<T>) {
        size_t size = other.size();
        if (size) {
          void* d_void = static_cast<void*>(const_cast<std::remove_const_t<T>*>(dst));
          const void* s_void = static_cast<const void*>(src);
          ::memcpy(d_void, s_void, size * sizeof(T));
        }
        end_ = dst + size;
      } else if constexpr (std::is_copy_assignable_v<T>) {
        while (dst < end_ && src < other.end_) *dst++ = *src++;
        while (src < other.end_) emplace(dst++, *src++);
        T* old_end = end_;
        end_ = dst;
        for (T* p = end_; p < old_end; p++) p->~T();
      } else {
        for (T* p = data_; p < end_; p++) p->~T();
        while (src < other.end_) emplace(dst++, *src++);
        end_ = dst;
      }
    } else {
      for (T* p = data_; p < end_; p++) p->~T();
      if (data_) zone_->DeleteArray(data_, capacity());
      size_t new_cap = other.capacity();
      if (new_cap > 0) {
        data_ = zone_->AllocateArray<T>(new_cap);
        CopyToNewStorage(data_, other.data_, other.end_);
      } else {
        data_ = nullptr;
      }
      capacity_ = data_ + new_cap;
      end_ = data_ + other.size();
    }
    return *this;
  }

  ZoneVector& operator=(ZoneVector&& other) V8_NOEXCEPT {
    // Self-assignment would cause undefined behavior, and is probably a bug.
    DCHECK_NE(this, &other);
    // Move-assigning vectors from different zones would have surprising
    // lifetime semantics regardless of how we choose to implement it (keep
    // the old zone? Take the new zone?).
    if (zone_ == nullptr) {
      zone_ = other.zone_;
    } else {
      DCHECK_EQ(zone_, other.zone_);
    }
    if (data_ != nullptr) {
      for (T* p = data_; p < end_; p++) p->~T();
      if (zone_ != nullptr) zone_->DeleteArray(data_, capacity());
    }
    data_ = other.data_;
    end_ = other.end_;
    capacity_ = other.capacity_;
    // {other.zone_} may stay.
    other.data_ = other.end_ = other.capacity_ = nullptr;
    return *this;
  }

  ZoneVector& operator=(std::initializer_list<T> ilist) {
    clear();
    EnsureCapacity(ilist.size());
    CopyToNewStorage(data_, ilist.begin(), ilist.end());
    end_ = data_ + ilist.size();
    return *this;
  }

  void swap(ZoneVector<T>& other) noexcept {
    DCHECK_EQ(zone_, other.zone_);
    std::swap(data_, other.data_);
    std::swap(end_, other.end_);
    std::swap(capacity_, other.capacity_);
  }

  // Dispose of a non-zone-allocated buffer (created via New()).
  // For zone-allocated buffers, this is a no-op since the zone owns the memory.
  void Dispose() {
    if (zone_ == nullptr && data_ != nullptr) {
      DeleteArray(data_);
    }
    data_ = nullptr;
    end_ = nullptr;
    capacity_ = nullptr;
  }

  void resize(size_t new_size) {
    EnsureCapacity(new_size);
    T* new_end = data_ + new_size;
    for (T* p = end_; p < new_end; p++) emplace(p);
    for (T* p = new_end; p < end_; p++) p->~T();
    end_ = new_end;
  }

  void resize(size_t new_size, const T& value) {
    EnsureCapacity(new_size);
    T* new_end = data_ + new_size;
    for (T* p = end_; p < new_end; p++) emplace(p, value);
    for (T* p = new_end; p < end_; p++) p->~T();
    end_ = new_end;
  }

  void assign(size_t new_size, const T& value) {
    if (capacity() >= new_size) {
      T* new_end = data_ + new_size;
      T* assignable = data_ + std::min(size(), new_size);
      for (T* p = data_; p < assignable; p++) CopyingOverwrite(p, &value);
      for (T* p = assignable; p < new_end; p++) CopyToNewStorage(p, &value);
      for (T* p = new_end; p < end_; p++) p->~T();
      end_ = new_end;
    } else {
      clear();
      EnsureCapacity(new_size);
      T* new_end = data_ + new_size;
      for (T* p = data_; p < new_end; p++) emplace(p, value);
      end_ = new_end;
    }
  }

  void clear() {
    if (data_ == nullptr) {
      end_ = data_;
      return;
    }
    for (T* p = data_; p < end_; p++) p->~T();
    end_ = data_;
  }

  size_t size() const { return data_ == nullptr ? 0 : end_ - data_; }
  bool empty() const { return size() == 0; }
  size_t capacity() const {
    return data_ == nullptr ? 0 : capacity_ - data_;
  }
  void reserve(size_t new_cap) { EnsureCapacity(new_cap); }
  T* data() { return data_; }
  const T* data() const { return data_; }
  Zone* zone() const { return zone_; }

  // Implicit conversion to ZoneVector<const T> for view semantics
  template <typename U = T, typename = std::enable_if_t<!std::is_const_v<U>>>
  operator ZoneVector<const T>() const {
    return ZoneVector<const T>(data_, size());
  }

  // Compatibility methods for v8::base::Vector-like interface
  size_t length() const { return size(); }
  T* ptr() { return data(); }
  const T* ptr() const { return data(); }

  // Implicit conversion to v8::base::Vector<const U> for trivially-copyable,
  // layout-compatible element types. This enables seamless interop with APIs
  // that expect base::Vector views.
  template <typename U,
            typename = std::enable_if_t<std::is_trivially_copyable_v<T> &&
                                        std::is_trivially_copyable_v<U> &&
                                        (sizeof(U) == sizeof(T))>>
  operator ::v8::base::Vector<const U>() const {
    return {reinterpret_cast<const U*>(data_), size()};
  }

  template <typename U,
            typename = std::enable_if_t<std::is_trivially_copyable_v<T> &&
                                        std::is_trivially_copyable_v<U> &&
                                        (sizeof(U) == sizeof(T))>>
  operator ::v8::base::Vector<U>() {
    return {reinterpret_cast<U*>(data_), size()};
  }

  // Provide OverwriteWith to mirror base::Vector API used in V8.
  // Copies the contents of `other` into this view. Sizes must match.
  template <class U>
  void OverwriteWith(ZoneVector<U> other) {
    DCHECK_EQ(size(), other.size());
    T* dst = data_;
    const U* src = other.data();
    const U* src_end = src + other.size();
    // Use existing helper to perform optimal overwrite semantics.
    CopyingOverwrite(dst, src, src_end);
  }

  template <class U>
  void OverwriteWith(::v8::base::Vector<U> other) {
    DCHECK_EQ(size(), other.size());
    T* dst = data_;
    const U* src = other.begin();
    const U* src_end = other.end();
    CopyingOverwrite(dst, src, src_end);
  }

  // Cast method for type conversion (similar to v8::base::Vector::cast)
  template <typename S>
  static ZoneVector<T> cast(const ZoneVector<S>& input) {
    static_assert(std::is_trivial_v<S> && std::is_standard_layout_v<S>);
    static_assert(std::is_trivial_v<T> && std::is_standard_layout_v<T>);
    ZoneVector<T> result(input.zone());
    result.resize(input.size());
    if (input.size() > 0) {
      if constexpr (sizeof(S) == sizeof(T)) {
        ::memcpy(const_cast<void*>(static_cast<const void*>(result.data())),
                 static_cast<const void*>(input.data()),
                 input.size() * sizeof(S));
      } else {
        for (size_t i = 0; i < input.size(); ++i) {
          result[i] = static_cast<T>(input[i]);
        }
      }
    }
    return result;
  }

  // Overload for casting from base::Vector to ZoneVector (without zone)
  template <typename S>
  static ZoneVector<T> cast(const ::v8::base::Vector<S>& input) {
    static_assert(std::is_trivial_v<S> && std::is_standard_layout_v<S>);
    static_assert(std::is_trivial_v<T> && std::is_standard_layout_v<T>);
    // Note: This creates a ZoneVector with nullptr zone - caller must handle
    ZoneVector<T> result;
    result.reserve(input.size());
    if constexpr (sizeof(S) == sizeof(T)) {
      result.resize(input.size());
      if (input.size() > 0) {
        ::memcpy(const_cast<void*>(static_cast<const void*>(result.data())),
                 static_cast<const void*>(input.data()),
                 input.size() * sizeof(S));
      }
    } else {
      for (size_t i = 0; i < input.size(); ++i) {
        result.push_back(static_cast<T>(input[i]));
      }
    }
    return result;
  }

  T& at(size_t pos) {
    DCHECK_LT(pos, size());
    return data_[pos];
  }
  const T& at(size_t pos) const {
    DCHECK_LT(pos, size());
    return data_[pos];
  }

  T& operator[](size_t pos) { return at(pos); }
  const T& operator[](size_t pos) const { return at(pos); }

  T& front() {
    DCHECK_GT(end_, data_);
    return *data_;
  }
  const T& front() const {
    DCHECK_GT(end_, data_);
    return *data_;
  }

  T& back() {
    DCHECK_GT(end_, data_);
    return *(end_ - 1);
  }
  const T& back() const {
    DCHECK_GT(end_, data_);
    return *(end_ - 1);
  }
  // Compatibility aliases used by some code paths.
  T& first() { return front(); }
  const T& first() const { return front(); }
  T& last() { return back(); }
  const T& last() const { return back(); }

  T* begin() V8_NOEXCEPT { return data_; }
  const T* begin() const V8_NOEXCEPT { return data_; }
  const T* cbegin() const V8_NOEXCEPT { return data_; }

  T* end() V8_NOEXCEPT { return end_; }
  const T* end() const V8_NOEXCEPT { return end_; }
  const T* cend() const V8_NOEXCEPT { return end_; }

  // Subvector views similar to base::Vector
  ZoneVector<T> SubVector(size_t from, size_t to) const {
    DCHECK_LE(from, to);
    DCHECK_LE(to, size());
    return ZoneVector<T>(data_ + from, to - from);
  }
  ZoneVector<T> SubVectorFrom(size_t from) const { return SubVector(from, size()); }

  reverse_iterator rbegin() V8_NOEXCEPT {
    return ::std::make_reverse_iterator(end());
  }
  const_reverse_iterator rbegin() const V8_NOEXCEPT {
    return ::std::make_reverse_iterator(end());
  }
  const_reverse_iterator crbegin() const V8_NOEXCEPT {
    return ::std::make_reverse_iterator(cend());
  }
  reverse_iterator rend() V8_NOEXCEPT {
    return ::std::make_reverse_iterator(begin());
  }
  const_reverse_iterator rend() const V8_NOEXCEPT {
    return ::std::make_reverse_iterator(begin());
  }
  const_reverse_iterator crend() const V8_NOEXCEPT {
    return ::std::make_reverse_iterator(cbegin());
  }

  // Truncate the vector to new_size elements.
  void Truncate(size_t new_size) {
    DCHECK_LE(new_size, size());
    T* new_end = data_ + new_size;
    for (T* p = new_end; p < end_; ++p) p->~T();
    end_ = new_end;
  }

  void push_back(const T& value) {
    EnsureOneMoreCapacity();
    emplace(end_++, value);
  }
  void push_back(T&& value) { emplace_back(std::move(value)); }

  void pop_back() {
    DCHECK_GT(end_, data_);
    (--end_)->~T();
  }

  template <typename... Args>
  T& emplace_back(Args&&... args) {
    EnsureOneMoreCapacity();
    T* ptr = end_++;
    ::new (const_cast<void*>(static_cast<const void*>(ptr)))
        T(std::forward<Args>(args)...);
    return *ptr;
  }

  template <class It,
            typename = typename std::iterator_traits<It>::iterator_category>
  T* insert(const T* pos, It first, It last) {
    T* position;
    if constexpr (std::is_base_of_v<
                      std::random_access_iterator_tag,
                      typename std::iterator_traits<It>::iterator_category>) {
      DCHECK_LE(0, last - first);
      size_t count = last - first;
      size_t assignable;
      position = PrepareForInsertion(pos, count, &assignable);
      if constexpr (std::is_trivially_copyable_v<T>) {
        if (count > 0) {
          ::memcpy(static_cast<void*>(position),
                   static_cast<const void*>(first), count * sizeof(T));
        }
      } else {
        CopyingOverwrite(position, first, first + assignable);
        CopyToNewStorage(position + assignable, first + assignable, last);
      }
    } else if (pos == end()) {
      position = end_;
      while (first != last) {
        EnsureOneMoreCapacity();
        emplace(end_++, *first++);
      }
    } else {
      UNIMPLEMENTED();
      // We currently have no users of this case.
      // It could be implemented inefficiently as a combination of the two
      // cases above: while (first != last) { PrepareForInsertion(_, 1, _); }.
      // A more efficient approach would be to accumulate the input iterator's
      // results into a temporary vector first, then grow {this} only once
      // (by calling PrepareForInsertion(_, count, _)), then copy over the
      // accumulated elements.
    }
    return position;
  }
  T* insert(const T* pos, size_t count, const T& value) {
    size_t assignable;
    T* position = PrepareForInsertion(pos, count, &assignable);
    T* dst = position;
    T* stop = dst + assignable;
    while (dst < stop) {
      CopyingOverwrite(dst++, &value);
    }
    stop = position + count;
    while (dst < stop) emplace(dst++, value);
    return position;
  }

  T* erase(const T* pos) {
    DCHECK(data_ <= pos && pos <= end());
    if (pos == end()) return const_cast<T*>(pos);
    return erase(pos, 1);
  }
  T* erase(const T* first, const T* last) {
    DCHECK(data_ <= first && first <= last && last <= end());
    if (first == last) return const_cast<T*>(first);
    return erase(first, last - first);
  }

 private:
  static constexpr size_t kMinCapacity = 2;
  size_t NewCapacity(size_t minimum) {
    // We can ignore possible overflow here: on 32-bit platforms, if the
    // multiplication overflows, there's no better way to handle it than
    // relying on the "new_capacity < minimum" check; in particular, a
    // saturating multiplication would make no sense. On 64-bit platforms,
    // overflow is effectively impossible anyway.
    size_t new_capacity = data_ == capacity_ ? kMinCapacity : capacity() * 2;
    return new_capacity < minimum ? minimum : new_capacity;
  }

  V8_INLINE void EnsureOneMoreCapacity() {
    if (V8_LIKELY(end_ < capacity_)) return;
    Grow(capacity() + 1);
  }

  V8_INLINE void EnsureCapacity(size_t minimum) {
    if (V8_LIKELY(minimum <= capacity())) return;
    Grow(minimum);
  }

  V8_INLINE void CopyToNewStorage(T* dst, const T* src) { emplace(dst, *src); }

  V8_INLINE void MoveToNewStorage(T* dst, T* src) {
    if constexpr (std::is_move_constructible_v<T>) {
      emplace(dst, std::move(*src));
    } else {
      CopyToNewStorage(dst, src);
    }
  }

  V8_INLINE void CopyingOverwrite(T* dst, const T* src) {
    if constexpr (std::is_copy_assignable_v<T>) {
      *dst = *src;
    } else {
      dst->~T();
      CopyToNewStorage(dst, src);
    }
  }

  V8_INLINE void MovingOverwrite(T* dst, T* src) {
    if constexpr (std::is_move_assignable_v<T>) {
      *dst = std::move(*src);
    } else {
      CopyingOverwrite(dst, src);
    }
  }

#define EMIT_TRIVIAL_CASE(memcpy_function)                              \
  DCHECK_LE(src, src_end);                                              \
  if constexpr (std::is_trivially_copyable_v<T>) {                      \
    size_t count = src_end - src;                                       \
    /* Add V8_ASSUME to silence gcc null check warning. */              \
    V8_ASSUME(src != nullptr);                                          \
    void* d_void = static_cast<void*>(const_cast<std::remove_const_t<T>*>(dst)); \
    const void* s_void = static_cast<const void*>(src);                 \
    memcpy_function(d_void, s_void, count * sizeof(T));                 \
    return;                                                             \
  }

  V8_INLINE void CopyToNewStorage(T* dst, const T* src, const T* src_end) {
    EMIT_TRIVIAL_CASE(::memcpy)
    for (; src < src_end; dst++, src++) {
      CopyToNewStorage(dst, src);
    }
  }

  V8_INLINE void MoveToNewStorage(T* dst, T* src, const T* src_end) {
    EMIT_TRIVIAL_CASE(::memcpy)
    for (; src < src_end; dst++, src++) {
      MoveToNewStorage(dst, src);
      src->~T();
    }
  }

  V8_INLINE void CopyingOverwrite(T* dst, const T* src, const T* src_end) {
    EMIT_TRIVIAL_CASE(::memmove)
    for (; src < src_end; dst++, src++) {
      CopyingOverwrite(dst, src);
    }
  }

  V8_INLINE void MovingOverwrite(T* dst, T* src, const T* src_end) {
    EMIT_TRIVIAL_CASE(::memmove)
    for (; src < src_end; dst++, src++) {
      MovingOverwrite(dst, src);
    }
  }

#undef EMIT_TRIVIAL_CASE

  V8_NOINLINE V8_PRESERVE_MOST void Grow(size_t minimum) {
    T* old_data = data_;
    T* old_end = end_;
    size_t old_size = size();
    size_t new_capacity = NewCapacity(minimum);
    data_ = zone_->AllocateArray<T>(new_capacity);
    end_ = data_ + old_size;
    if (old_data) {
      MoveToNewStorage(data_, old_data, old_end);
      zone_->DeleteArray(old_data, capacity_ - old_data);
    }
    capacity_ = data_ + new_capacity;
  }

  T* PrepareForInsertion(const T* pos, size_t count, size_t* assignable) {
    DCHECK(data_ <= pos && pos <= end_);
    CHECK(std::numeric_limits<size_t>::max() - size() >= count);
    size_t index = pos - data_;
    size_t to_shift = end() - pos;
    DCHECK_EQ(index + to_shift, size());
    if (capacity() < size() + count) {
      *assignable = 0;  // Fresh memory is not assignable (must be constructed).
      T* old_data = data_;
      T* old_end = end_;
      size_t old_size = size();
      size_t new_capacity = NewCapacity(old_size + count);
      data_ = zone_->AllocateArray<T>(new_capacity);
      end_ = data_ + old_size + count;
      if (old_data) {
        MoveToNewStorage(data_, old_data, pos);
        MoveToNewStorage(data_ + index + count, const_cast<T*>(pos), old_end);
        zone_->DeleteArray(old_data, capacity_ - old_data);
      }
      capacity_ = data_ + new_capacity;
    } else {
      // There are two interesting cases: we're inserting more elements
      // than we're shifting (top), or the other way round (bottom).
      //
      // Old: [ABCDEFGHIJ___________]
      //       <--used--><--empty-->
      //
      // Case 1: index=7, count=8, to_shift=3
      // New: [ABCDEFGaaacccccHIJ___]
      //              <-><------>
      //               ↑    ↑ to be in-place constructed
      //               ↑
      //               assignable_slots
      //
      // Case 2: index=3, count=3, to_shift=7
      // New: [ABCaaaDEFGHIJ________]
      //          <-----><->
      //             ↑    ↑ to be in-place constructed
      //             ↑
      //             This range can be assigned. We report the first 3
      //             as {assignable_slots} to the caller, and use the other 4
      //             in the loop below.
      // Observe that the number of old elements that are moved to the
      // new end by in-place construction always equals {assignable_slots}.
      size_t assignable_slots = std::min(to_shift, count);
      *assignable = assignable_slots;
      if constexpr (std::is_trivially_copyable_v<T>) {
        if (to_shift > 0) {
          // Add V8_ASSUME to silence gcc null check warning.
          V8_ASSUME(pos != nullptr);
          memmove(const_cast<T*>(pos + count), pos, to_shift * sizeof(T));
        }
        end_ += count;
        return data_ + index;
      }
      // Construct elements in previously-unused area ("HIJ" in the example
      // above). This frees up assignable slots.
      T* dst = end_ + count;
      T* src = end_;
      for (T* stop = dst - assignable_slots; dst > stop;) {
        MoveToNewStorage(--dst, --src);
      }
      // Move (by assignment) elements into previously used area. This is
      // "DEFG" in "case 2" in the example above.
      DCHECK_EQ(src > pos, to_shift > count);
      DCHECK_IMPLIES(src > pos, dst == end_);
      while (src > pos) MovingOverwrite(--dst, --src);
      // Not destructing {src} here because that'll happen either in a
      // future iteration (when that spot becomes {dst}) or in {insert()}.
      end_ += count;
    }
    return data_ + index;
  }

  T* erase(const T* first, size_t count) {
    DCHECK(data_ <= first && first <= end());
    DCHECK_LE(count, end() - first);
    T* position = const_cast<T*>(first);
    MovingOverwrite(position, position + count, end());
    T* old_end = end();
    end_ -= count;
    for (T* p = end_; p < old_end; p++) p->~T();
    return position;
  }

  template <typename... Args>
  void emplace(T* target, Args&&... args) {
    ::new (const_cast<void*>(static_cast<const void*>(target)))
        T(std::forward<Args>(args)...);
  }

  Zone* zone_{nullptr};
  T* data_{nullptr};
  T* end_{nullptr};
  T* capacity_{nullptr};
};

template <class T>
bool operator==(const ZoneVector<T>& lhs, const ZoneVector<T>& rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T>
bool operator!=(const ZoneVector<T>& lhs, const ZoneVector<T>& rhs) {
  return !(lhs == rhs);
}

template <class T>
bool operator<(const ZoneVector<T>& lhs, const ZoneVector<T>& rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end());
}

// Comparison operators between ZoneVector and Vector
template <class T>
bool operator==(const ZoneVector<T>& lhs, const ::v8::base::Vector<T>& rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T>
bool operator==(const ::v8::base::Vector<T>& lhs, const ZoneVector<T>& rhs) {
  return rhs == lhs;
}

template <class T>
bool operator!=(const ZoneVector<T>& lhs, const ::v8::base::Vector<T>& rhs) {
  return !(lhs == rhs);
}

template <class T>
bool operator!=(const ::v8::base::Vector<T>& lhs, const ZoneVector<T>& rhs) {
  return !(lhs == rhs);
}

template <class T, class GetIntrusiveSetIndex>
class ZoneIntrusiveSet
    : public ::v8::base::IntrusiveSet<T, GetIntrusiveSetIndex, ZoneVector<T>> {
 public:
  explicit ZoneIntrusiveSet(Zone* zone, GetIntrusiveSetIndex index_functor = {})
      : ::v8::base::IntrusiveSet<T, GetIntrusiveSetIndex, ZoneVector<T>>(
            ZoneVector<T>(zone), std::move(index_functor)) {}
};
using ::v8::base::IntrusiveSetIndex;

// A wrapper subclass for std::deque to make it easy to construct one
// that uses a zone allocator.
template <typename T>
class ZoneDeque : public std::deque<T, RecyclingZoneAllocator<T>> {
 public:
  // Constructs an empty deque.
  explicit ZoneDeque(Zone* zone)
      : std::deque<T, RecyclingZoneAllocator<T>>(
            RecyclingZoneAllocator<T>(zone)) {}
};

// A wrapper subclass for std::list to make it easy to construct one
// that uses a zone allocator.
// TODO(all): This should be renamed to ZoneList once we got rid of our own
// home-grown ZoneList that actually is a ZoneVector.
template <typename T>
class ZoneLinkedList : public std::list<T, ZoneAllocator<T>> {
 public:
  // Constructs an empty list.
  explicit ZoneLinkedList(Zone* zone)
      : std::list<T, ZoneAllocator<T>>(ZoneAllocator<T>(zone)) {}
};

// A wrapper subclass for std::forward_list to make it easy to construct one
// that uses a zone allocator.
template <typename T>
class ZoneForwardList : public std::forward_list<T, ZoneAllocator<T>> {
 public:
  // Constructs an empty list.
  explicit ZoneForwardList(Zone* zone)
      : std::forward_list<T, ZoneAllocator<T>>(ZoneAllocator<T>(zone)) {}
};

// A wrapper subclass for std::priority_queue to make it easy to construct one
// that uses a zone allocator.
template <typename T, typename Compare = std::less<T>>
class ZonePriorityQueue
    : public std::priority_queue<T, ZoneVector<T>, Compare> {
 public:
  // Constructs an empty list.
  explicit ZonePriorityQueue(Zone* zone)
      : std::priority_queue<T, ZoneVector<T>, Compare>(Compare(),
                                                       ZoneVector<T>(zone)) {}
};

// A wrapper subclass for std::queue to make it easy to construct one
// that uses a zone allocator.
template <typename T>
class ZoneQueue : public std::queue<T, ZoneDeque<T>> {
 public:
  // Constructs an empty queue.
  explicit ZoneQueue(Zone* zone)
      : std::queue<T, ZoneDeque<T>>(ZoneDeque<T>(zone)) {}
};

// A wrapper subclass for std::stack to make it easy to construct one that uses
// a zone allocator.
template <typename T>
class ZoneStack : public std::stack<T, ZoneDeque<T>> {
 public:
  // Constructs an empty stack.
  explicit ZoneStack(Zone* zone)
      : std::stack<T, ZoneDeque<T>>(ZoneDeque<T>(zone)) {}
};

// A wrapper subclass for std::set to make it easy to construct one that uses
// a zone allocator.
template <typename K, typename Compare = std::less<K>>
class ZoneSet : public std::set<K, Compare, ZoneAllocator<K>> {
 public:
  // Constructs an empty set.
  explicit ZoneSet(Zone* zone)
      : std::set<K, Compare, ZoneAllocator<K>>(Compare(),
                                               ZoneAllocator<K>(zone)) {}
};

// A wrapper subclass for std::multiset to make it easy to construct one that
// uses a zone allocator.
template <typename K, typename Compare = std::less<K>>
class ZoneMultiset : public std::multiset<K, Compare, ZoneAllocator<K>> {
 public:
  // Constructs an empty multiset.
  explicit ZoneMultiset(Zone* zone)
      : std::multiset<K, Compare, ZoneAllocator<K>>(Compare(),
                                                    ZoneAllocator<K>(zone)) {}
};

// A wrapper subclass for std::map to make it easy to construct one that uses
// a zone allocator.
template <typename K, typename V, typename Compare = std::less<K>>
class ZoneMap
    : public std::map<K, V, Compare, ZoneAllocator<std::pair<const K, V>>> {
 public:
  // Constructs an empty map.
  explicit ZoneMap(Zone* zone)
      : std::map<K, V, Compare, ZoneAllocator<std::pair<const K, V>>>(
            Compare(), ZoneAllocator<std::pair<const K, V>>(zone)) {}
};

// A wrapper subclass for std::unordered_map to make it easy to construct one
// that uses a zone allocator.
template <typename K, typename V, typename Hash = ::v8::base::hash<K>,
          typename KeyEqual = std::equal_to<K>>
class ZoneUnorderedMap
    : public std::unordered_map<K, V, Hash, KeyEqual,
                                ZoneAllocator<std::pair<const K, V>>> {
 public:
  // Constructs an empty map.
  explicit ZoneUnorderedMap(Zone* zone, size_t bucket_count = 0)
      : std::unordered_map<K, V, Hash, KeyEqual,
                           ZoneAllocator<std::pair<const K, V>>>(
            bucket_count, Hash(), KeyEqual(),
            ZoneAllocator<std::pair<const K, V>>(zone)) {}
};

// A wrapper subclass for std::unordered_set to make it easy to construct one
// that uses a zone allocator.
template <typename K, typename Hash = ::v8::base::hash<K>,
          typename KeyEqual = std::equal_to<K>>
class ZoneUnorderedSet
    : public std::unordered_set<K, Hash, KeyEqual, ZoneAllocator<K>> {
 public:
  // Constructs an empty set.
  explicit ZoneUnorderedSet(Zone* zone, size_t bucket_count = 0)
      : std::unordered_set<K, Hash, KeyEqual, ZoneAllocator<K>>(
            bucket_count, Hash(), KeyEqual(), ZoneAllocator<K>(zone)) {}
};

// A wrapper subclass for std::multimap to make it easy to construct one that
// uses a zone allocator.
template <typename K, typename V, typename Compare = std::less<K>>
class ZoneMultimap
    : public std::multimap<K, V, Compare,
                           ZoneAllocator<std::pair<const K, V>>> {
 public:
  // Constructs an empty multimap.
  explicit ZoneMultimap(Zone* zone)
      : std::multimap<K, V, Compare, ZoneAllocator<std::pair<const K, V>>>(
            Compare(), ZoneAllocator<std::pair<const K, V>>(zone)) {}
};

// A wrapper subclass for base::SmallVector to make it easy to construct one
// that uses a zone allocator.
template <typename T, size_t kSize>
class SmallZoneVector : public ::v8::base::SmallVector<T, kSize, ZoneAllocator<T>> {
 public:
  // Constructs an empty small vector.
  explicit SmallZoneVector(Zone* zone)
      : ::v8::base::SmallVector<T, kSize, ZoneAllocator<T>>(ZoneAllocator<T>(zone)) {}

  explicit SmallZoneVector(size_t size, Zone* zone)
      : ::v8::base::SmallVector<T, kSize, ZoneAllocator<T>>(
            size, ZoneAllocator<T>(ZoneAllocator<T>(zone))) {}
};

// Used by SmallZoneMap below. Essentially a closure around placement-new of
// the "full" fallback ZoneMap. Called once SmallMap grows beyond kArraySize.
template <typename ZoneMap>
class ZoneMapInit {
 public:
  explicit ZoneMapInit(Zone* zone) : zone_(zone) {}
  void operator()(ZoneMap* map) const { ::new (static_cast<void*>(map)) ZoneMap(zone_); }

 private:
  Zone* zone_;
};

// A wrapper subclass for base::SmallMap to make it easy to construct one that
// uses a zone-allocated std::map as the fallback once the SmallMap outgrows
// its inline storage.
template <typename K, typename V, size_t kArraySize,
          typename Compare = std::less<K>, typename KeyEqual = std::equal_to<K>>
class SmallZoneMap
    : public ::v8::base::SmallMap<ZoneMap<K, V, Compare>, kArraySize, KeyEqual,
                            ZoneMapInit<ZoneMap<K, V, Compare>>> {
 public:
  explicit SmallZoneMap(Zone* zone)
      : ::v8::base::SmallMap<ZoneMap<K, V, Compare>, kArraySize, KeyEqual,
                       ZoneMapInit<ZoneMap<K, V, Compare>>>(
            ZoneMapInit<ZoneMap<K, V, Compare>>(zone)) {}
};

#ifndef __wasi__
// A wrapper subclass for absl::flat_hash_map to make it easy to construct one
// that uses a zone allocator. If you want to use a user-defined type as key
// (K), you'll need to define a AbslHashValue function for it (see
// https://abseil.io/docs/cpp/guides/hash).
template <typename K, typename V,
          typename Hash = typename absl::flat_hash_map<K, V>::hasher,
          typename KeyEqual =
              typename absl::flat_hash_map<K, V, Hash>::key_equal>
class ZoneAbslFlatHashMap
    : public absl::flat_hash_map<K, V, Hash, KeyEqual,
                                 ZoneAllocator<std::pair<const K, V>>> {
 public:
  // Constructs an empty map.
  explicit ZoneAbslFlatHashMap(Zone* zone, size_t bucket_count = 0)
      : absl::flat_hash_map<K, V, Hash, KeyEqual,
                            ZoneAllocator<std::pair<const K, V>>>(
            bucket_count, Hash(), KeyEqual(),
            ZoneAllocator<std::pair<const K, V>>(zone)) {}
};

// A wrapper subclass for absl::flat_hash_set to make it easy to construct one
// that uses a zone allocator. If you want to use a user-defined type as key
// (K), you'll need to define a AbslHashValue function for it (see
// https://abseil.io/docs/cpp/guides/hash).
template <typename K, typename Hash = typename absl::flat_hash_set<K>::hasher,
          typename KeyEqual = typename absl::flat_hash_set<K, Hash>::key_equal>
class ZoneAbslFlatHashSet
    : public absl::flat_hash_set<K, Hash, KeyEqual, ZoneAllocator<K>> {
 public:
  // Constructs an empty map.
  explicit ZoneAbslFlatHashSet(Zone* zone, size_t bucket_count = 0)
      : absl::flat_hash_set<K, Hash, KeyEqual, ZoneAllocator<K>>(
            bucket_count, Hash(), KeyEqual(), ZoneAllocator<K>(zone)) {}
};

// A wrapper subclass for absl::btree_map to make it easy to construct one
// that uses a zone allocator. If you want to use a user-defined type as key
// (K), you'll need to define a AbslHashValue function for it (see
// https://abseil.io/docs/cpp/guides/hash).
template <typename K, typename V, typename Compare = std::less<K>>
class ZoneAbslBTreeMap
    : public absl::btree_map<K, V, Compare,
                             ZoneAllocator<std::pair<const K, V>>> {
 public:
  // Constructs an empty map.
  explicit ZoneAbslBTreeMap(Zone* zone)
      : absl::btree_map<K, V, Compare, ZoneAllocator<std::pair<const K, V>>>(
            ZoneAllocator<std::pair<const K, V>>(zone)) {}
};
#else  // __wasi__
// WASI: Provide STL-based alternatives to abseil wrappers
template <typename K, typename V, typename Hash = ::v8::base::hash<K>,
          typename KeyEqual = std::equal_to<K>>
using ZoneAbslFlatHashMap = ZoneUnorderedMap<K, V, Hash, KeyEqual>;

template <typename K, typename Hash = ::v8::base::hash<K>,
          typename KeyEqual = std::equal_to<K>>
using ZoneAbslFlatHashSet = ZoneUnorderedSet<K, Hash, KeyEqual>;

template <typename K, typename V, typename Compare = std::less<K>>
using ZoneAbslBTreeMap = ZoneMap<K, V, Compare>;
#endif  // __wasi__

// Typedefs to shorten commonly used vectors.
using IntVector = ZoneVector<int>;

// Define Zone::AllocateZoneVector now that ZoneVector is available.
template <typename T>
inline ZoneVector<T> Zone::AllocateZoneVector(size_t length) {
  return ZoneVector<T>(length, this);
}

// CachedZoneVector is a ZoneVector that can be reused from a cache
// to avoid repeated allocations. The cache is external and passed by pointer.
template <typename T>
class CachedZoneVector : public ZoneVector<T> {
 public:
  // Constructor that takes a cache (stack of unused vectors)
  explicit CachedZoneVector(std::vector<ZoneVector<T>*>* cache)
      : ZoneVector<T>(cache->empty() ? nullptr : cache->back()->zone()),
        cache_(cache) {
    if (!cache->empty()) {
      // Reuse vector from cache
      ZoneVector<T>* reused = cache->back();
      cache->pop_back();
      this->swap(*reused);
      delete reused;
    }
  }

  ~CachedZoneVector() {
    // Return to cache if available
    if (cache_ && this->zone()) {
      ZoneVector<T>* to_cache = new ZoneVector<T>(this->zone());
      to_cache->swap(*this);
      cache_->push_back(to_cache);
    }
  }

  // Delete copy constructor and assignment
  CachedZoneVector(const CachedZoneVector&) = delete;
  CachedZoneVector& operator=(const CachedZoneVector&) = delete;

 private:
  std::vector<ZoneVector<T>*>* cache_;
};

// Specialization of ZoneVector for DirectHandle<T> to use DirectHandleUnchecked
// internally, since DirectHandle is stack-allocated only and cannot be used
// in heap/zone-allocated containers directly.
template <typename T>
class ZoneVector<DirectHandle<T>> {
 private:
  using InternalType = DirectHandleUnchecked<T>;
  ZoneVector<InternalType> storage_;

 public:
  using value_type = DirectHandle<T>;
  using reference = DirectHandle<T>&;
  using const_reference = const DirectHandle<T>&;
  using iterator = typename ZoneVector<InternalType>::iterator;
  using const_iterator = typename ZoneVector<InternalType>::const_iterator;
  using size_type = size_t;

  ZoneVector() : storage_() {}
  explicit ZoneVector(Zone* zone) : storage_(zone) {}
  ZoneVector(size_t size, Zone* zone) : storage_(size, zone) {}
  ZoneVector(std::initializer_list<value_type> init, Zone* zone) : storage_(zone) {
    storage_.reserve(init.size());
    for (const auto& item : init) {
      storage_.push_back(InternalType(item));
    }
  }

  // Constructor from base::Vector for compatibility (non-owning view)
  ZoneVector(::v8::base::Vector<value_type> vec)
      : storage_(reinterpret_cast<InternalType*>(vec.begin()), vec.size()) {}

  // Element access - reinterpret_cast is safe because DirectHandle and
  // DirectHandleUnchecked have the same layout
  reference operator[](size_t pos) {
    return reinterpret_cast<reference>(storage_[pos]);
  }
  const_reference operator[](size_t pos) const {
    return reinterpret_cast<const_reference>(storage_[pos]);
  }

  reference at(size_t pos) {
    return reinterpret_cast<reference>(storage_.at(pos));
  }
  const_reference at(size_t pos) const {
    return reinterpret_cast<const_reference>(storage_.at(pos));
  }

  // Iterators - use storage iterators which can be cast
  iterator begin() { return storage_.begin(); }
  const_iterator begin() const { return storage_.begin(); }
  iterator end() { return storage_.end(); }
  const_iterator end() const { return storage_.end(); }

  // Capacity
  bool empty() const { return storage_.empty(); }
  size_t size() const { return storage_.size(); }
  size_t length() const { return size(); }  // Compatibility alias
  size_t capacity() const { return storage_.capacity(); }
  void reserve(size_t n) { storage_.reserve(n); }
  void resize(size_t n) { storage_.resize(n); }

  // Front/back element access
  reference front() { return reinterpret_cast<reference>(storage_.front()); }
  const_reference front() const { return reinterpret_cast<const_reference>(storage_.front()); }
  reference back() { return reinterpret_cast<reference>(storage_.back()); }
  const_reference back() const { return reinterpret_cast<const_reference>(storage_.back()); }
  reference first() { return front(); }
  const_reference first() const { return front(); }
  reference last() { return back(); }
  const_reference last() const { return back(); }

  // Modifiers
  void push_back(const value_type& value) {
    storage_.push_back(InternalType(value));
  }
  void push_back(value_type&& value) {
    storage_.push_back(InternalType(std::move(value)));
  }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    storage_.emplace_back(std::forward<Args>(args)...);
  }

  void pop_back() { storage_.pop_back(); }
  void clear() { storage_.clear(); }

  // Other members
  void swap(ZoneVector& other) { storage_.swap(other.storage_); }
  Zone* zone() const { return storage_.zone(); }

  // Data access - returns pointer to underlying storage
  value_type* data() { return reinterpret_cast<value_type*>(storage_.data()); }
  const value_type* data() const { return reinterpret_cast<const value_type*>(storage_.data()); }
};

// Specialization of ZoneVector for const T - provides a read-only view
template <typename T>
class ZoneVector<const T> {
 private:
  const T* data_;
  size_t size_;

 public:
  using value_type = const T;
  using reference = const T&;
  using const_reference = const T&;
  using iterator = const T*;
  using const_iterator = const T*;
  using size_type = size_t;

  // Default constructor
  ZoneVector() : data_(nullptr), size_(0) {}

  // Constructor from pointer and size (non-owning view)
  ZoneVector(const T* data, size_t size) : data_(data), size_(size) {}

  // Constructor from pointer and size (with explicit cast from non-const)
  ZoneVector(T* data, size_t size) : data_(data), size_(size) {}

  // Constructor from base::Vector for implicit conversion
  ZoneVector(const ::v8::base::Vector<const T>& vec)
      : data_(vec.begin()), size_(vec.size()) {}

  // Constructor from base::Vector with compatible element type (e.g., uint8_t to char)
  template <typename S,
            typename = std::enable_if_t<sizeof(S) == sizeof(T) &&
                                        std::is_trivially_copyable_v<S> &&
                                        std::is_trivially_copyable_v<T>>>
  ZoneVector(const ::v8::base::Vector<S>& vec)
      : data_(reinterpret_cast<const T*>(vec.begin())), size_(vec.size()) {}

  // Copy from non-const ZoneVector
  ZoneVector(const ZoneVector<T>& other)
      : data_(other.data()), size_(other.size()) {}

  // Element access
  const_reference operator[](size_t pos) const {
    DCHECK_LT(pos, size_);
    return data_[pos];
  }

  const_reference at(size_t pos) const {
    DCHECK_LT(pos, size_);
    return data_[pos];
  }

  // Iterators
  using reverse_iterator = ::std::reverse_iterator<const_iterator>;
  using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

  const_iterator begin() const { return data_; }
  const_iterator end() const { return data_ + size_; }
  const_iterator cbegin() const { return data_; }
  const_iterator cend() const { return data_ + size_; }

  reverse_iterator rbegin() const { return ::std::make_reverse_iterator(end()); }
  reverse_iterator rend() const { return ::std::make_reverse_iterator(begin()); }
  const_reverse_iterator crbegin() const { return ::std::make_reverse_iterator(cend()); }
  const_reverse_iterator crend() const { return ::std::make_reverse_iterator(cbegin()); }

  // Capacity
  bool empty() const { return size_ == 0; }
  size_t size() const { return size_; }

  // Compatibility methods for v8::base::Vector-like interface
  size_t length() const { return size_; }

  // Front/back element access
  const_reference front() const {
    DCHECK_GT(size_, 0);
    return data_[0];
  }
  const_reference back() const {
    DCHECK_GT(size_, 0);
    return data_[size_ - 1];
  }
  const_reference first() const { return front(); }
  const_reference last() const { return back(); }

  // SubVector support
  ZoneVector<const T> SubVector(size_t from, size_t to) const {
    DCHECK_LE(from, to);
    DCHECK_LE(to, size_);
    return ZoneVector<const T>(data_ + from, to - from);
  }

  ZoneVector<const T> SubVectorFrom(size_t from) const {
    return SubVector(from, size_);
  }

  // Produce a sliced non-owning view advanced by offset elements.
  ZoneVector<const T> operator+(size_t offset) const {
    DCHECK_LE(offset, size_);
    return ZoneVector<const T>(data_ + offset, size_ - offset);
  }

  // Advance this non-owning view by offset elements in-place.
  ZoneVector<const T>& operator+=(size_t offset) {
    DCHECK_LE(offset, size_);
    data_ += offset;
    size_ -= offset;
    return *this;
  }

  // Data access
  const T* data() const { return data_; }

  // Implicit conversion to base::Vector<const T> for compatibility
  operator ::v8::base::Vector<const T>() const {
    return ::v8::base::Vector<const T>(data_, size_);
  }

  // Cast from base::Vector to ZoneVector<const T>
  template <typename S>
  static ZoneVector<const T> cast(const ::v8::base::Vector<S>& input) {
    static_assert(std::is_trivial_v<S> && std::is_standard_layout_v<S>);
    static_assert(std::is_trivial_v<T> && std::is_standard_layout_v<T>);
    if constexpr (sizeof(S) == sizeof(T)) {
      return ZoneVector<const T>(reinterpret_cast<const T*>(input.data()),
                                  input.size());
    } else {
      // For different sizes, we'd need to allocate, but this is a const view
      // so we can't. This case should not occur in practice for const views.
      UNREACHABLE();
    }
  }

  // Cast from ZoneVector to ZoneVector<const T> - only enabled when sizes match
  template <typename S,
            typename = std::enable_if_t<sizeof(S) == sizeof(T)>>
  static ZoneVector<const T> cast(const ZoneVector<S>& input) {
    return ZoneVector<const T>(reinterpret_cast<const T*>(input.data()),
                                input.size());
  }

  // Cast from ZoneVector<const S> to ZoneVector<const T> - handles reinterpret
  // casting bytes to wider types (e.g., uint8_t to uint16_t)
  template <typename S>
  static ZoneVector<const T> cast(const ZoneVector<const S>& input) {
    static_assert(std::is_trivially_copyable_v<S> && std::is_trivially_copyable_v<T>,
                  "Types must be trivially copyable for reinterpret cast");
    // Calculate the new size based on the ratio of element sizes
    size_t byte_size = input.size() * sizeof(S);
    size_t new_size = byte_size / sizeof(T);
    return ZoneVector<const T>(reinterpret_cast<const T*>(input.data()), new_size);
  }
};

}  // namespace internal
}  // namespace v8

// Hash function specialization for ZoneVector (must be outside v8 namespace)
#include "src/base/hashing.h"  // for hash_range
template <typename T>
inline size_t hash_value(v8::internal::ZoneVector<T> const& v) {
  return v8::base::hash_range(v.begin(), v.end());
}

#endif  // V8_ZONE_ZONE_CONTAINERS_H_
