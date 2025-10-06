#ifndef WASI_FILESYSTEM_STUBS_H_
#define WASI_FILESYSTEM_STUBS_H_

#ifdef __wasi__

#include <filesystem>

#if defined(_LIBCPP_HAS_NO_FILESYSTEM)

namespace std {
namespace filesystem {

inline bool equivalent(const path& p1, const path& p2) { return p1 == p2; }

inline bool is_directory(const path&) { return false; }

inline path current_path() { return path("/"); }

}  // namespace filesystem
}  // namespace std

#endif  // defined(_LIBCPP_HAS_NO_FILESYSTEM)

#endif  // __wasi__

#endif // WASI_FILESYSTEM_STUBS_H_
