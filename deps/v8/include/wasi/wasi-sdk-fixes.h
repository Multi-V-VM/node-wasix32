#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef WASI_SDK_FIXES_H
#define WASI_SDK_FIXES_H

#ifdef __wasi__

// Workaround for WASI SDK file_clock.h bug
// The WASI SDK has a syntax error in file_clock.h with "namespace std = ::std;"
// inside struct
#define _LIBCPP_HAS_NO_FILESYSTEM_LIBRARY 1

// Prevent inclusion of problematic headers
#ifdef __has_include
#if __has_include(<__chrono/file_clock.h>)
#define _LIBCPP_CHRONO_FILE_CLOCK_H
namespace std {
namespace chrono {
struct file_clock {
  using rep = int64_t;
  using period = std::nano;
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<file_clock>;

  static constexpr bool is_steady = false;

  static time_point now() noexcept { return time_point{duration{0}}; }
};
}  // namespace chrono
}  // namespace std
#endif
#endif

// Include chrono workaround first to prevent WASI SDK bug
#include "chrono-workaround.h"

// Include other safe headers
#include <algorithm>
#include <cstdint>
#include <functional>
#include <type_traits>

// Provide bit_cast in v8::base namespace
namespace v8 {
namespace base {

// Provide bit_cast function for WASI
template <typename To, typename From>
constexpr To bit_cast(const From& from) noexcept {
  static_assert(sizeof(To) == sizeof(From),
                "bit_cast requires same size types");
  static_assert(std::is_trivially_copyable_v<To>,
                "To must be trivially copyable");
  static_assert(std::is_trivially_copyable_v<From>,
                "From must be trivially copyable");

#if __has_builtin(__builtin_bit_cast)
  return __builtin_bit_cast(To, from);
#else
  // Fallback implementation
  union {
    From f;
    To t;
  } u;
  u.f = from;
  return u.t;
#endif
}

// Forward standard library functions to global namespace to avoid v8::std
// pollution
using ::std::copy;
using ::std::copy_backward;
using ::std::count;
using ::std::fill;
using ::std::fill_n;
using ::std::find;
using ::std::max;
using ::std::min;

}  // namespace base
}  // namespace v8

// Fix for PageAllocator::SharedMemory
namespace v8 {

class PageAllocator {
 public:
  class SharedMemory {
   public:
    virtual ~SharedMemory() = default;
    virtual void* Map(void* address, size_t length) = 0;
    virtual void Unmap(void* address, size_t length) = 0;
    virtual size_t GetSize() const = 0;
  };

  virtual ~PageAllocator() = default;
  virtual std::unique_ptr<SharedMemory> AllocateSharedPages(size_t size) {
    return nullptr;
  }
};

}  // namespace v8

#endif  // __wasi__
#endif  // WASI_SDK_FIXES_H