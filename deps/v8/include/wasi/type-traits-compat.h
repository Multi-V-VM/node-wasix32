#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef WASI_TYPE_TRAITS_COMPAT_H
#define WASI_TYPE_TRAITS_COMPAT_H

#ifdef __wasi__

#include <cstdint>
#include <cstring>
#include <type_traits>

// Only provide v8::base types without any std namespace pollution
namespace v8 {
namespace base {

// Provide the missing type traits that V8 needs
template <typename T>
struct is_standard_layout {
  static constexpr bool value = __is_standard_layout(T);
};

template <typename T>
struct is_trivial {
  static constexpr bool value = __is_trivial(T);
};

// Forward to global std for available traits - NO namespace pollution
template <typename T>
struct is_trivially_copyable {
  static constexpr bool value = ::std::is_trivially_copyable<T>::value;
};

template <typename T>
struct is_trivially_destructible {
  static constexpr bool value = ::std::is_trivially_destructible<T>::value;
};

template <typename T>
struct is_trivially_constructible {
  static constexpr bool value = ::std::is_trivially_constructible<T>::value;
};

template <typename T>
struct is_copy_constructible {
  static constexpr bool value = ::std::is_copy_constructible<T>::value;
};

template <typename T>
struct is_move_constructible {
  static constexpr bool value = ::std::is_move_constructible<T>::value;
};

template <typename T>
struct is_copy_assignable {
  static constexpr bool value = ::std::is_copy_assignable<T>::value;
};

template <typename T>
struct is_move_assignable {
  static constexpr bool value = ::std::is_move_assignable<T>::value;
};

template <typename T>
using remove_reference_t = typename ::std::remove_reference<T>::type;

template <typename T>
using remove_cv_t = typename ::std::remove_cv<T>::type;

// Important: Do NOT create any "std" namespace inside v8
// This avoids all the namespace pollution issues

}  // namespace base
}  // namespace v8

// Also define platform-specific types
namespace v8 {

class PageAllocator {
 public:
  class SharedMemory {
   public:
    virtual ~SharedMemory() = default;
    virtual void* Map(void* address, size_t length) = 0;
    virtual void Unmap(void* address, size_t length) = 0;
  };

  virtual ~PageAllocator() = default;
  virtual std::unique_ptr<SharedMemory> AllocateSharedPages(
      size_t length, const void* hint = nullptr) = 0;
};

}  // namespace v8

#endif  // __wasi__
#endif  // WASI_TYPE_TRAITS_COMPAT_H