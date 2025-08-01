#ifndef WASI_FILESYSTEM_STUBS_H_
#define WASI_FILESYSTEM_STUBS_H_

#ifdef __wasi__

#include <filesystem>
#include <system_error>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

// Add our stub implementations to std::filesystem
namespace std::filesystem {

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

// Overload with copy_options
inline bool copy_file(const path& from, const path& to, copy_options options, std::error_code& ec) noexcept {
  ec = std::make_error_code(std::errc::function_not_supported);
  return false;
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

// WASI stub for create_directories function
inline bool create_directories(const path& p, std::error_code& ec) noexcept {
  // Simple implementation - doesn't handle parent directories
  return create_directory(p, ec);
}

inline bool create_directories(const path& p) {
  std::error_code ec;
  auto result = create_directories(p, ec);
  if (ec) {
    throw filesystem_error("create_directories failed", p, ec);
  }
  return result;
}

// WASI stub for remove function
inline bool remove(const path& p, std::error_code& ec) noexcept {
  if (::remove(p.c_str()) == 0) {
    ec.clear();
    return true;
  }
  ec = std::error_code(errno, std::system_category());
  return false;
}

inline bool remove(const path& p) {
  std::error_code ec;
  auto result = remove(p, ec);
  if (ec) {
    throw filesystem_error("remove failed", p, ec);
  }
  return result;
}

// WASI stub for exists function
inline bool exists(const path& p, std::error_code& ec) noexcept {
  struct stat st;
  if (::stat(p.c_str(), &st) == 0) {
    ec.clear();
    return true;
  }
  if (errno == ENOENT) {
    ec.clear();
    return false;
  }
  ec = std::error_code(errno, std::system_category());
  return false;
}

inline bool exists(const path& p) {
  std::error_code ec;
  auto result = exists(p, ec);
  if (ec) {
    throw filesystem_error("exists failed", p, ec);
  }
  return result;
}

// WASI stub for absolute function
inline path absolute(const path& p, std::error_code& ec) {
  char* abs_path = ::realpath(p.c_str(), nullptr);
  if (abs_path) {
    path result(abs_path);
    ::free(abs_path);
    ec.clear();
    return result;
  }
  ec = std::error_code(errno, std::system_category());
  return path();
}

inline path absolute(const path& p) {
  std::error_code ec;
  auto result = absolute(p, ec);
  if (ec) {
    throw filesystem_error("absolute failed", p, ec);
  }
  return result;
}

// WASI stub for weakly_canonical function
inline path weakly_canonical(const path& p, std::error_code& ec) {
  // Simple implementation - just returns absolute path
  return absolute(p, ec);
}

inline path weakly_canonical(const path& p) {
  std::error_code ec;
  auto result = weakly_canonical(p, ec);
  if (ec) {
    throw filesystem_error("weakly_canonical failed", p, ec);
  }
  return result;
}

// WASI stub for is_directory function
inline bool is_directory(const path& p, std::error_code& ec) noexcept {
  struct stat st;
  if (::stat(p.c_str(), &st) != 0) {
    ec = std::error_code(errno, std::system_category());
    return false;
  }
  ec.clear();
  return S_ISDIR(st.st_mode);
}

inline bool is_directory(const path& p) {
  std::error_code ec;
  auto result = is_directory(p, ec);
  if (ec) {
    throw filesystem_error("is_directory failed", p, ec);
  }
  return result;
}

// WASI stub for copy_symlink function
inline void copy_symlink(const path& from, const path& to, std::error_code& ec) noexcept {
  char buffer[PATH_MAX];
  ssize_t len = ::readlink(from.c_str(), buffer, sizeof(buffer) - 1);
  if (len != -1) {
    buffer[len] = '\0';
    if (::symlink(buffer, to.c_str()) == 0) {
      ec.clear();
      return;
    }
  }
  ec = std::error_code(errno, std::system_category());
}

inline void copy_symlink(const path& from, const path& to) {
  std::error_code ec;
  copy_symlink(from, to, ec);
  if (ec) {
    throw filesystem_error("copy_symlink failed", from, to, ec);
  }
}

// WASI stub for directory_entry
class directory_entry {
public:
  directory_entry() noexcept = default;
  
  const path& path() const noexcept {
    static std::filesystem::path empty_path;
    return empty_path;
  }
  
  bool is_symlink() const {
    return false;  // WASI stub - no symlink support
  }
  
  bool is_directory() const {
    return false;  // WASI stub
  }
  
  bool is_regular_file() const {
    return false;  // WASI stub
  }
};

// WASI stub for directory_iterator
class directory_iterator {
public:
  directory_iterator() noexcept = default;
  explicit directory_iterator(const path& p) {
    // WASI stub - no directory iteration support
  }
  directory_iterator(const path& p, std::error_code& ec) noexcept {
    // WASI stub - no directory iteration support
    ec = std::make_error_code(std::errc::not_supported);
  }
  
  bool operator==(const directory_iterator& rhs) const noexcept {
    return true; // Always equal in stub
  }
  
  bool operator!=(const directory_iterator& rhs) const noexcept {
    return false; // Never different in stub
  }
  
  directory_iterator& operator++() {
    return *this;
  }
  
  // operator* to dereference the iterator
  directory_entry operator*() const {
    // Return an empty directory_entry for WASI stub
    return directory_entry();
  }
  
  directory_iterator begin() const { return directory_iterator(); }
  directory_iterator end() const { return directory_iterator(); }
};

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

} // namespace std::filesystem

#endif // __wasi__

#endif // WASI_FILESYSTEM_STUBS_H_