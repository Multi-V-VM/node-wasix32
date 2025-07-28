#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_CHRONO_COMPAT_H_
#define V8_WASI_CHRONO_COMPAT_H_

#ifdef __wasi__

#include <chrono>
#include <ratio>

// Provide only filesystem clock support without v8::std namespace pollution
namespace v8 {
namespace internal {

// Fix WASI libc++ chrono issues for filesystem
#ifndef _LIBCPP_HAS_NO_FILESYSTEM
struct WasiFileClock {
  using rep = int64_t;
  using period = ::std::ratio<1, 1000000000>;  // nanoseconds
  using duration = ::std::chrono::duration<rep, period>;
  using time_point = ::std::chrono::time_point<WasiFileClock>;

  static constexpr bool is_steady = false;
  static time_point now() noexcept { return time_point{duration{0}}; }
};

// Helper aliases that avoid std namespace conflicts
using filesystem_clock = WasiFileClock;
using file_time_type = WasiFileClock::time_point;
#endif

}  // namespace internal
}  // namespace v8

#endif  // __wasi__

#endif  // V8_WASI_CHRONO_COMPAT_H_