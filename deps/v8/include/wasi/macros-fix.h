#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef WASI_MACROS_FIX_H
#define WASI_MACROS_FIX_H

#ifdef __wasi__

namespace v8 {
namespace base {

// Fix circular inheritance issue with is_trivially_destructible
template <typename T>
struct is_trivially_destructible {
    static constexpr bool value = ::std::is_trivially_destructible<T>::value;
};

// Fix is_trivially_copyable issue
template <typename T>
struct is_trivially_copyable {
    static constexpr bool value = ::std::is_trivially_copyable<T>::value;
};

} // namespace base
} // namespace v8

#endif // __wasi__
#endif // WASI_MACROS_FIX_H
