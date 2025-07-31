#ifndef WASI_FILESYSTEM_STUBS_H_
#define WASI_FILESYSTEM_STUBS_H_

#ifdef __wasi__

#include <string>
#include <filesystem>

namespace std {
namespace filesystem {

// Add missing filesystem functions for WASI
inline bool equivalent(const path& p1, const path& p2) {
  // Simple comparison for WASI
  return p1 == p2;
}

inline bool is_directory(const path& p) {
  // For WASI, we'll need to use stat or similar
  // For now, return false as a stub
  return false;
}

inline path current_path() {
  // Return root path for WASI
  return path("/");
}

} // namespace filesystem
} // namespace std

#endif // __wasi__

#endif // WASI_FILESYSTEM_STUBS_H_