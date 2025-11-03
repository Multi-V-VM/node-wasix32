// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_ATOMIC_UTILS_H_
#define V8_BASE_ATOMIC_UTILS_H_

#include <limits.h>

#include <atomic>
#include <type_traits>

#include "src/base/atomicops.h"
#include "src/base/macros.h"
#include "src/base/strong-alias.h"

namespace v8 {
namespace base {

// Deprecated. Use std::atomic<T> for new code.
// Flag using T atomically. Also accepts void* as T.
template <typename T>
class AtomicValue {
 public:
  AtomicValue() : value_(0) {}

  explicit AtomicValue(T initial)
      : value_(cast_helper<T>::to_storage_type(initial)) {}

  V8_INLINE T Value() const {
    return cast_helper<T>::to_return_type(::v8::base::Acquire_Load(&value_));
  }

  V8_INLINE void SetValue(T new_value) {
    ::v8::base::Release_Store(&value_, cast_helper<T>::to_storage_type(new_value));
  }

 private:
  static_assert(sizeof(T) <= sizeof(::v8::base::AtomicWord));

  template <typename S>
  struct cast_helper {
    static ::v8::base::AtomicWord to_storage_type(S value) {
      return static_cast<::v8::base::AtomicWord>(value);
    }
    static S to_return_type(::v8::base::AtomicWord value) {
      return static_cast<S>(value);
    }
  };

  template <typename S>
  struct cast_helper<S*> {
    static ::v8::base::AtomicWord to_storage_type(S* value) {
      return reinterpret_cast<::v8::base::AtomicWord>(value);
    }
    static S* to_return_type(::v8::base::AtomicWord value) {
      return reinterpret_cast<S*>(value);
    }
  };

  ::v8::base::AtomicWord value_;
};

// Forward declaration for width-to-atomic-type mapping used below.
template <int Width>
struct AtomicTypeFromByteWidth;

// Provides atomic operations for a values stored at some address.
template <typename TAtomicStorageType>
class AsAtomicImpl {
 public:
  using AtomicStorageType = TAtomicStorageType;

  template <typename T>
  static T SeqCst_Load(T* addr) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    static_assert(sizeof(T) <= sizeof(P));
    const volatile P* p = storage_ptr(addr);
    return from_atomic<T>(::v8::base::SeqCst_Load(p));
  }

  template <typename T>
  static T Acquire_Load(T* addr) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    static_assert(sizeof(T) <= sizeof(P));
    const volatile P* p = storage_ptr(addr);
    return from_atomic<T>(::v8::base::Acquire_Load(p));
  }

  template <typename T>
  static T Relaxed_Load(T* addr) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    static_assert(sizeof(T) <= sizeof(P));
    const volatile P* p = storage_ptr(addr);
    return from_atomic<T>(::v8::base::Relaxed_Load(p));
  }

  template <typename T>
  static void SeqCst_Store(T* addr,
                           typename ::std::remove_reference<T>::type new_value) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    static_assert(sizeof(T) <= sizeof(P));
    volatile P* p = storage_ptr(addr);
    ::v8::base::SeqCst_Store(p, to_atomic<T, P>(new_value));
  }

  template <typename T>
  static void Release_Store(T* addr,
                            typename ::std::remove_reference<T>::type new_value) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    static_assert(sizeof(T) <= sizeof(P));
    volatile P* p = storage_ptr(addr);
    ::v8::base::Release_Store(p, to_atomic<T, P>(new_value));
  }

  template <typename T>
  static void Relaxed_Store(T* addr,
                            typename ::std::remove_reference<T>::type new_value) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    static_assert(sizeof(T) <= sizeof(P));
    volatile P* p = storage_ptr(addr);
    ::v8::base::Relaxed_Store(p, to_atomic<T, P>(new_value));
  }

  template <typename T>
  static T SeqCst_Swap(T* addr,
                       typename ::std::remove_reference<T>::type new_value) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    static_assert(sizeof(T) <= sizeof(P));
    volatile P* p = storage_ptr(addr);
    return from_atomic<T>(::v8::base::SeqCst_AtomicExchange(p, to_atomic<T, P>(new_value)));
  }

  template <typename T>
  static T Release_CompareAndSwap(
      T* addr, typename ::std::remove_reference<T>::type old_value,
      typename ::std::remove_reference<T>::type new_value) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    static_assert(sizeof(T) <= sizeof(P));
    volatile P* p = storage_ptr(addr);
    return from_atomic<T>(::v8::base::Release_CompareAndSwap(
        p, to_atomic<T, P>(old_value), to_atomic<T, P>(new_value)));
  }

  template <typename T>
  static T Relaxed_CompareAndSwap(
      T* addr, typename ::std::remove_reference<T>::type old_value,
      typename ::std::remove_reference<T>::type new_value) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    static_assert(sizeof(T) <= sizeof(P));
    volatile P* p = storage_ptr(addr);
    return from_atomic<T>(::v8::base::Relaxed_CompareAndSwap(
        p, to_atomic<T, P>(old_value), to_atomic<T, P>(new_value)));
  }

  template <typename T>
  static T AcquireRelease_CompareAndSwap(
      T* addr, typename ::std::remove_reference<T>::type old_value,
      typename ::std::remove_reference<T>::type new_value) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    static_assert(sizeof(T) <= sizeof(P));
    volatile P* p = storage_ptr(addr);
    return from_atomic<T>(::v8::base::AcquireRelease_CompareAndSwap(
        p, to_atomic<T, P>(old_value), to_atomic<T, P>(new_value)));
  }

  template <typename T>
  static T SeqCst_CompareAndSwap(
      T* addr, typename ::std::remove_reference<T>::type old_value,
      typename ::std::remove_reference<T>::type new_value) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    static_assert(sizeof(T) <= sizeof(P));
    volatile P* p = storage_ptr(addr);
    return from_atomic<T>(::v8::base::SeqCst_CompareAndSwap(
        p, to_atomic<T, P>(old_value), to_atomic<T, P>(new_value)));
  }

  // Atomically sets bits selected by the mask to the given value.
  // Returns false if the bits are already set as needed.
  template <typename T>
  static bool Release_SetBits(T* addr, T bits, T mask) {
    static_assert(sizeof(T) <= sizeof(AtomicStorageType));
    DCHECK_EQ(bits & ~mask, static_cast<T>(0));
    T old_value = Relaxed_Load(addr);
    T new_value, old_value_before_cas;
    do {
      if ((old_value & mask) == bits) return false;
      new_value = (old_value & ~mask) | bits;
      old_value_before_cas = old_value;
      old_value = Release_CompareAndSwap(addr, old_value, new_value);
    } while (old_value != old_value_before_cas);
    return true;
  }

  // Atomically sets bits selected by the mask to the given value.
  // Returns false if the bits are already set as needed.
  template <typename T>
  static bool Relaxed_SetBits(T* addr, T bits, T mask) {
    static_assert(sizeof(T) <= sizeof(AtomicStorageType));
    DCHECK_EQ(bits & ~mask, static_cast<T>(0));
    T old_value = Relaxed_Load(addr);
    T new_value, old_value_before_cas;
    do {
      if ((old_value & mask) == bits) return false;
      new_value = (old_value & ~mask) | bits;
      old_value_before_cas = old_value;
      old_value = Relaxed_CompareAndSwap(addr, old_value, new_value);
    } while (old_value != old_value_before_cas);
    return true;
  }

 private:
  // Convert between user type T and atomic parameter type P.
  template <typename X>
  struct is_strong_alias : ::std::false_type {};
  template <typename Tag, typename U>
  struct is_strong_alias<base::StrongAlias<Tag, U>> : ::std::true_type {};

  template <typename T, typename P>
  static P to_atomic(typename ::std::remove_reference<T>::type value) {
    if constexpr (is_strong_alias<T>::value) {
      return static_cast<P>(value.value());
    } else if constexpr (::std::is_pointer_v<T>) {
      return static_cast<P>(reinterpret_cast<uintptr_t>(value));
    } else {
      return static_cast<P>(value);
    }
  }
  template <typename T, typename P>
  static T from_atomic(P value) {
    if constexpr (is_strong_alias<T>::value) {
      using Underlying = typename T::underlying_type;
      return T(static_cast<Underlying>(value));
    } else if constexpr (::std::is_pointer_v<T>) {
      return reinterpret_cast<T>(static_cast<uintptr_t>(value));
    } else {
      return static_cast<T>(value);
    }
  }

  template <typename T>
  static volatile typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type*
  storage_ptr(T* value) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    return reinterpret_cast<volatile P*>(value);
  }
  template <typename T>
  static const volatile typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type*
  storage_ptr(const T* value) {
    using P = typename ::v8::base::AtomicTypeFromByteWidth<sizeof(T)>::type;
    return reinterpret_cast<const volatile P*>(value);
  }
};

using AsAtomic8 = AsAtomicImpl<::v8::base::Atomic8>;
using AsAtomic16 = AsAtomicImpl<::v8::base::Atomic16>;
using AsAtomic32 = AsAtomicImpl<::v8::base::Atomic32>;
using AsAtomicWord = AsAtomicImpl<::v8::base::AtomicWord>;

template <int Width>
struct AtomicTypeFromByteWidth {};
template <>
struct AtomicTypeFromByteWidth<1> {
  using type = ::v8::base::Atomic8;
};
template <>
struct AtomicTypeFromByteWidth<2> {
  using type = ::v8::base::Atomic16;
};
template <>
struct AtomicTypeFromByteWidth<4> {
  using type = ::v8::base::Atomic32;
};
#if V8_HOST_ARCH_64_BIT
template <>
struct AtomicTypeFromByteWidth<8> {
  using type = base::Atomic64;
};
#endif

// This is similar to AsAtomicWord but it explicitly deletes functionality
// provided atomic access to bit representation of stored values.
#ifndef WASI_ATOMIC_POINTER_IMPL_DEFINED
template <typename TAtomicStorageType>
class AsAtomicPointerImpl : public AsAtomicImpl<TAtomicStorageType> {
 public:
  template <typename T>
  static bool SetBits(T* addr, T bits, T mask) = delete;
};
#define WASI_ATOMIC_POINTER_IMPL_DEFINED
#endif

using AsAtomicPointer = AsAtomicPointerImpl<::v8::base::AtomicWord>;

#if defined(__wasi__)
template <typename T>
inline void CheckedIncrement(
    ::std::atomic<T>* number, T amount,
    ::std::memory_order order = ::std::memory_order_seq_cst)
  requires ::std::is_unsigned<T>::value
{
  // On WASI, perform the atomic update without extra DCHECK semantics.
  // The DCHECK macros may be stubbed out, so just do the operation.
  number->fetch_add(amount, order);
}
#else
template <typename T>
inline void CheckedIncrement(
    ::std::atomic<T>* number, T amount,
    ::std::memory_order order = ::std::memory_order_seq_cst)
  requires ::std::is_unsigned<T>::value
{
  const T old = number->fetch_add(amount, order);
  DCHECK_GE(old + amount, old);
  USE(old);
}
#endif

#if defined(__wasi__)
template <typename T>
inline void CheckedDecrement(
    ::std::atomic<T>* number, T amount,
    ::std::memory_order order = ::std::memory_order_seq_cst)
  requires ::std::is_unsigned<T>::value
{
  // On WASI, perform the atomic update without extra DCHECK semantics.
  number->fetch_sub(amount, order);
}
#elif !defined(V8_TARGET_ARCH_WASM32)
template <typename T>
inline void CheckedDecrement(
    ::std::atomic<T>* number, T amount,
    ::std::memory_order order = ::std::memory_order_seq_cst)
  requires ::std::is_unsigned<T>::value
{
  const T old = number->fetch_sub(amount, order);
  DCHECK_GE(old, amount);
  USE(old);
}
#endif

template <typename T>
V8_INLINE ::std::atomic<T>* AsAtomicPtr(T* t) {
  static_assert(sizeof(T) == sizeof(::std::atomic<T>));
  static_assert(alignof(T) >= alignof(::std::atomic<T>));
  return reinterpret_cast<::std::atomic<T>*>(t);
}

template <typename T>
V8_INLINE const ::std::atomic<T>* AsAtomicPtr(const T* t) {
  static_assert(sizeof(T) == sizeof(::std::atomic<T>));
  static_assert(alignof(T) >= alignof(::std::atomic<T>));
  return reinterpret_cast<const ::std::atomic<T>*>(t);
}

}  // namespace base
}  // namespace v8


#endif  // V8_BASE_ATOMIC_UTILS_H_
