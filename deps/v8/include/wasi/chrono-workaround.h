#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef WASI_CHRONO_WORKAROUND_H
#define WASI_CHRONO_WORKAROUND_H

#ifdef __wasi__

// Prevent inclusion of broken file_clock.h
#define _LIBCPP_CHRONO_FILE_CLOCK_H 1
#define _LIBCPP_HAS_NO_FILESYSTEM_LIBRARY 1

// Pre-define safe chrono types before any inclusion
#include <cstdint>
#include <ratio>

namespace std {
namespace chrono {

// Minimal safe file_clock implementation
struct file_clock {
  using rep = int64_t;
  using period = nano;
  using duration = chrono::duration<rep, period>;
  using time_point = chrono::time_point<file_clock>;

  static constexpr bool is_steady = false;

  static time_point now() noexcept { return time_point{duration{0}}; }

  template <typename Duration>
  static time_point<file_clock, Duration> from_sys(
      const time_point<system_clock, Duration>& t) {
    return time_point<file_clock, Duration>{t.time_since_epoch()};
  }

  template <typename Duration>
  static time_point<system_clock, Duration> to_sys(
      const time_point<file_clock, Duration>& t) {
    return time_point<system_clock, Duration>{t.time_since_epoch()};
  }
};

template <typename Duration>
using file_time = time_point<file_clock, Duration>;

}  // namespace chrono

namespace filesystem {
struct _FilesystemClock {
  using rep = int64_t;
  using period = std::nano;
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<_FilesystemClock>;

  static constexpr bool is_steady = false;
};
}  // namespace filesystem

}  // namespace std

// Now it's safe to include chrono
#include <chrono>

#endif  // __wasi__
#endif  // WASI_CHRONO_WORKAROUND_H