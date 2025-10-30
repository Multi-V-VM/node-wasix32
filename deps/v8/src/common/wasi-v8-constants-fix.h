#ifndef WASI_V8_CONSTANTS_FIX_H_
#define WASI_V8_CONSTANTS_FIX_H_

// Missing constants and macros for WASI V8 build

// Missing CHECK macros
#ifndef CHECK_IMPLIES
#define CHECK_IMPLIES(a, b) ((!(a)) || (b))
#endif

#ifndef CONSTEXPR_UNREACHABLE
#define CONSTEXPR_UNREACHABLE() __builtin_unreachable()
#endif

// Execution policies for parallel algorithms (C++17) - provide minimal stubs
// when the libc++ headers bundled with the WASI SDK do not expose them yet.
#if !defined(__cpp_lib_execution) && defined(V8_WASI_STD_POLYFILLS_GLOBAL_SCOPE)
namespace std {
namespace execution {
struct sequenced_policy {};
struct parallel_policy {};
struct parallel_unsequenced_policy {};

inline constexpr sequenced_policy seq{};
inline constexpr parallel_policy par{};
inline constexpr parallel_unsequenced_policy par_unseq{};
}  // namespace execution
}  // namespace std
#endif

#endif  // WASI_V8_CONSTANTS_FIX_H_
