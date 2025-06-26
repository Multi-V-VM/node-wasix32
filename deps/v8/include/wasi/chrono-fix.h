#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifdef __wasi__
#ifndef WASI_CHRONO_FIX_H_
#define WASI_CHRONO_FIX_H_

// Define missing chrono types for WASI
namespace std {
namespace chrono {

// Stub for missing types
template<typename T>
struct __is_time_point : false_type {};

template<typename Clock, typename Duration>
struct __is_time_point<time_point<Clock, Duration>> : true_type {};

}  // namespace chrono
}  // namespace std

#endif  // WASI_CHRONO_FIX_H_
#endif  // __wasi__
