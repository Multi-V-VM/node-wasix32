#ifndef WASI_FILESYSTEM_STUBS_H_
#define WASI_FILESYSTEM_STUBS_H_

#ifdef __wasi__

#include <filesystem>
#include <system_error>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

namespace std {
namespace filesystem {

// WASI stub for status function
inline file_status status(const path& p, std::error_code& ec) noexcept {
  ec = std::make_error_code(std::errc::function_not_supported);
  return file_status(file_type::unknown);
}

inline file_status status(const path& p) {
  std::error_code ec;
  auto result = status(p, ec);
  if (ec) {
    throw filesystem_error("status not supported in WASI", p, ec);
  }
  return result;
}

// WASI stub for symlink_status function
inline file_status symlink_status(const path& p, std::error_code& ec) noexcept {
  ec = std::make_error_code(std::errc::function_not_supported);
  return file_status(file_type::unknown);
}

inline file_status symlink_status(const path& p) {
  std::error_code ec;
  auto result = symlink_status(p, ec);
  if (ec) {
    throw filesystem_error("symlink_status not supported in WASI", p, ec);
  }
  return result;
}

// WASI stub for remove_all function
inline uintmax_t remove_all(const path& p, std::error_code& ec) noexcept {
  ec = std::make_error_code(std::errc::function_not_supported);
  return static_cast<uintmax_t>(-1);
}

inline uintmax_t remove_all(const path& p) {
  std::error_code ec;
  auto result = remove_all(p, ec);
  if (ec) {
    throw filesystem_error("remove_all not supported in WASI", p, ec);
  }
  return result;
}

// WASI stub for equivalent function
inline bool equivalent(const path& p1, const path& p2, std::error_code& ec) noexcept {
  ec = std::make_error_code(std::errc::function_not_supported);
  return false;
}

inline bool equivalent(const path& p1, const path& p2) {
  std::error_code ec;
  auto result = equivalent(p1, p2, ec);
  if (ec) {
    throw filesystem_error("equivalent not supported in WASI", p1, p2, ec);
  }
  return result;
}

// WASI stub for copy_file function
inline bool copy_file(const path& from, const path& to, std::error_code& ec) noexcept {
  ec = std::make_error_code(std::errc::function_not_supported);
  return false;
}

inline bool copy_file(const path& from, const path& to) {
  std::error_code ec;
  auto result = copy_file(from, to, ec);
  if (ec) {
    throw filesystem_error("copy_file not supported in WASI", from, to, ec);
  }
  return result;
}

// WASI stub for read_symlink function
inline path read_symlink(const path& p, std::error_code& ec) {
  // Use POSIX readlink
  char buffer[PATH_MAX];
  ssize_t len = ::readlink(p.c_str(), buffer, sizeof(buffer) - 1);
  if (len == -1) {
    ec = std::error_code(errno, std::system_category());
    return path();
  }
  buffer[len] = '\0';
  ec.clear();
  return path(buffer);
}

inline path read_symlink(const path& p) {
  std::error_code ec;
  auto result = read_symlink(p, ec);
  if (ec) {
    throw filesystem_error("read_symlink failed", p, ec);
  }
  return result;
}

// WASI stub for is_regular_file function
inline bool is_regular_file(const path& p, std::error_code& ec) noexcept {
  struct stat st;
  if (::stat(p.c_str(), &st) != 0) {
    ec = std::error_code(errno, std::system_category());
    return false;
  }
  ec.clear();
  return S_ISREG(st.st_mode);
}

inline bool is_regular_file(const path& p) {
  std::error_code ec;
  auto result = is_regular_file(p, ec);
  if (ec) {
    throw filesystem_error("is_regular_file failed", p, ec);
  }
  return result;
}

// WASI stub for create_directory function
inline bool create_directory(const path& p, std::error_code& ec) noexcept {
  if (::mkdir(p.c_str(), 0755) == 0) {
    ec.clear();
    return true;
  }
  ec = std::error_code(errno, std::system_category());
  return false;
}

inline bool create_directory(const path& p) {
  std::error_code ec;
  auto result = create_directory(p, ec);
  if (ec) {
    throw filesystem_error("create_directory failed", p, ec);
  }
  return result;
}

// WASI stub for create_symlink function
inline void create_symlink(const path& target, const path& link, std::error_code& ec) noexcept {
  if (::symlink(target.c_str(), link.c_str()) != 0) {
    ec = std::error_code(errno, std::system_category());
  } else {
    ec.clear();
  }
}

inline void create_symlink(const path& target, const path& link) {
  std::error_code ec;
  create_symlink(target, link, ec);
  if (ec) {
    throw filesystem_error("create_symlink failed", target, link, ec);
  }
}

// WASI stub for create_directory_symlink function
inline void create_directory_symlink(const path& target, const path& link, std::error_code& ec) noexcept {
  // Same as create_symlink for WASI
  create_symlink(target, link, ec);
}

inline void create_directory_symlink(const path& target, const path& link) {
  std::error_code ec;
  create_directory_symlink(target, link, ec);
  if (ec) {
    throw filesystem_error("create_directory_symlink failed", target, link, ec);
  }
}

} // namespace filesystem
} // namespace std

#endif // __wasi__

#endif // WASI_FILESYSTEM_STUBS_H_