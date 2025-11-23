#ifdef __wasi__
// Avoid pulling standard headers from within namespace contexts; let each TU
// include what it needs at global scope.
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_CONFIG_H_
#define V8_CONFIG_H_

#ifdef __wasi__
// WASI-specific V8 configuration

// Architecture and OS detection
#define V8_WASI 1
#define V8_HOST_ARCH_32_BIT 1
#define V8_TARGET_ARCH_WASM 1
#define V8_OS_WASI 1

// Disable problematic features
// V8_ENABLE_SANDBOX is not supported on WASI - leave undefined
// For WASI we prefer these to be undefined so that any
// `#if V8_ENABLE_*` evaluates to 0 and `#ifdef V8_ENABLE_*` evaluates to false.
#undef V8_ENABLE_MAGLEV
#undef V8_ENABLE_TURBOFAN
#undef V8_ENABLE_SPARKPLUG
#define V8_COMPRESS_POINTERS 0
#define V8_31BIT_SMIS_ON_64BIT_ARCH 0
#define V8_ENABLE_POINTER_COMPRESSION 0

// Compiler attributes
#define V8_EXPORT
#define V8_EXPORT_PRIVATE  
#define V8_INLINE inline
#define V8_NOINLINE __attribute__((noinline))
#define V8_TRIVIAL_ABI
#define V8_LIKELY(x) __builtin_expect(!!(x), 1)
#define V8_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define V8_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define V8_NODISCARD [[nodiscard]]
#define V8_FALLTHROUGH [[fallthrough]]
#define V8_NO_UNIQUE_ADDRESS [[no_unique_address]]
#define V8_HAS_ATTRIBUTE_NO_UNIQUE_ADDRESS 1

// Null pointer checks
#if defined(__has_attribute)
#if __has_attribute(nonnull)
#define V8_NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
#else
#define V8_NONNULL(...)
#endif
#else
#define V8_NONNULL(...)
#endif

// V8_ASSUME(x) tells the compiler to assume that x is true
#ifdef __wasi__
// WASI: Use simpler form for constexpr compatibility
#define V8_ASSUME(x) ((void)0)
#else
#if defined(__has_builtin)
#if __has_builtin(__builtin_assume)
#define V8_ASSUME(x) __builtin_assume(!!(x))
#else
#define V8_ASSUME(x) do { if (!(x)) __builtin_unreachable(); } while (0)
#endif
#else
#define V8_ASSUME(x) do { if (!(x)) __builtin_unreachable(); } while (0)
#endif
#endif

// Additional compiler attributes
#if __has_attribute(preserve_most) && !defined(__wasi__)
#define V8_PRESERVE_MOST __attribute__((preserve_most))
#else
#define V8_PRESERVE_MOST
#endif

// C++20 constinit support
// For WASI builds, prefer the Clang attribute which is valid on declarations.
#if defined(__wasi__)
# if __has_attribute(require_constant_initialization)
#  define V8_CONSTINIT __attribute__((require_constant_initialization))
# else
#  define V8_CONSTINIT
# endif
#else
# if defined(__cpp_constinit) && __cpp_constinit >= 201907L
#  define V8_CONSTINIT constinit
# elif __has_attribute(require_constant_initialization)
#  define V8_CONSTINIT __attribute__((require_constant_initialization))
# else
#  define V8_CONSTINIT
# endif
#endif

// Constexpr support
#if __cplusplus >= 201402L
#define V8_CONST constexpr
#else
#define V8_CONST const
#endif

// Compiler detection
#define V8_CC_CLANG 1

// Detect common 64-bit host architectures so AtomicWord matches pointer size.
#if !defined(V8_HOST_ARCH_64_BIT)
#if defined(__x86_64__) || defined(_M_X64) || \
    defined(__aarch64__) || defined(_M_ARM64) || \
    defined(__ppc64__) || defined(__PPC64__) || \
    defined(__s390x__) || defined(__mips64) || defined(__mips64__) || \
    defined(__loongarch_lp64)
#define V8_HOST_ARCH_64_BIT 1
#endif
#endif

// Deprecated API suppression macros
#define START_ALLOW_USE_DEPRECATED()
#define END_ALLOW_USE_DEPRECATED()

// Exception specifications
#define V8_NOEXCEPT noexcept

#else
// Non-WASI fallback configuration used when building host tools during
// cross-compilation. Provide the minimal macro set expected by the headers
// above without pulling in the full upstream configuration.

#if defined(__APPLE__)
#define V8_OS_DARWIN 1
#define V8_OS_POSIX 1
#elif defined(__linux__)
#define V8_OS_LINUX 1
#define V8_OS_POSIX 1
#endif

#ifndef V8_EXPORT
#define V8_EXPORT
#endif
#ifndef V8_EXPORT_PRIVATE
#define V8_EXPORT_PRIVATE
#endif
#ifndef V8_INLINE
#define V8_INLINE inline
#endif
#ifndef V8_NOINLINE
#define V8_NOINLINE __attribute__((noinline))
#endif
#ifndef V8_WARN_UNUSED_RESULT
#define V8_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#endif
#ifndef V8_NODISCARD
#define V8_NODISCARD [[nodiscard]]
#endif
#ifndef V8_NOEXCEPT
#define V8_NOEXCEPT noexcept
#endif
#ifndef V8_LIKELY
#define V8_LIKELY(x) __builtin_expect(!!(x), 1)
#endif
#ifndef V8_UNLIKELY
#define V8_UNLIKELY(x) __builtin_expect(!!(x), 0)
#endif
#ifndef V8_ASSUME
#define V8_ASSUME(x) do { if (!(x)) __builtin_unreachable(); } while (0)
#endif
#ifndef V8_PRESERVE_MOST
#define V8_PRESERVE_MOST
#endif
#ifndef V8_CONSTINIT
#define V8_CONSTINIT constinit
#endif
#ifndef V8_CONST
#define V8_CONST constexpr
#endif
#ifndef V8_THROW_DEFAULT
#define V8_THROW_DEFAULT noexcept(false)
#endif
#ifndef V8_NO_UNIQUE_ADDRESS
#define V8_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

#define V8_CC_CLANG 1
#endif

#endif // V8_CONFIG_H_
#ifndef __wasi__
#ifndef kApiSystemPointerSize
#define kApiSystemPointerSize sizeof(void*)
#endif
#endif
