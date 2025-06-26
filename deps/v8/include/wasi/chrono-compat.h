#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_CHRONO_COMPAT_H_
#define V8_WASI_CHRONO_COMPAT_H_

#ifdef __wasi__

#include <chrono>
#include <ratio>

// Provide chrono types in v8::std namespace that WASI libc++ might expect
namespace v8 {
namespace std {
namespace chrono {

// Import all the chrono types that V8 might need
using ::std::chrono::duration;
using ::std::chrono::high_resolution_clock;
using ::std::chrono::steady_clock;
using ::std::chrono::system_clock;
using ::std::chrono::time_point;

// Duration types
using ::std::chrono::hours;
using ::std::chrono::microseconds;
using ::std::chrono::milliseconds;
using ::std::chrono::minutes;
using ::std::chrono::nanoseconds;
using ::std::chrono::seconds;

// Additional types that might be missing
#ifdef __cpp_lib_chrono
using ::std::chrono::days;
using ::std::chrono::months;
using ::std::chrono::weeks;
using ::std::chrono::years;
#else
// Define days if not available
using days = duration<int, std::ratio<86400> >;
#endif

// Time point aliases
template <typename Duration>
using sys_time = time_point<system_clock, Duration>;

template <typename Duration>
using local_time = time_point<system_clock, Duration>;

using local_seconds = local_time<seconds>;
using local_days = local_time<days>;

// Fix WASI libc++ chrono issues
#ifndef _LIBCPP_HAS_NO_FILESYSTEM
struct file_clock {
  using rep = int64_t;
  using period = std::ratio<1, 1000000000>;  // nanoseconds
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<file_clock>;

  static constexpr bool is_steady = false;

  static time_point now() noexcept { return time_point{duration{0}}; }
};
#endif

}  // namespace chrono

// Also provide ratio types in v8::std
using ::std::micro;
using ::std::milli;
using ::std::nano;
using ::std::ratio;

}  // namespace std
}  // namespace v8

#endif  // __wasi__

#endif  // V8_WASI_CHRONO_COMPAT_H_