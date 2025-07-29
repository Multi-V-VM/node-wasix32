#ifndef WASI_NODE_COMPAT_H_
#define WASI_NODE_COMPAT_H_

#ifdef __wasi__

// Compatibility helpers for WASI build
namespace v8 {
class Isolate;

// Static method extensions for Isolate
namespace IsolateCompat {
inline Isolate* GetCurrent() { return nullptr; }
} // namespace IsolateCompat

// Redefine static calls to use our compat functions
#define V8_ISOLATE_GET_CURRENT() v8::IsolateCompat::GetCurrent()

} // namespace v8

// Compatibility for node.h
#define NODE_V8_UNIXTIME(date) 0.0

// std::filesystem compatibility for WASI
#include <filesystem>
#include <system_error>
#include <sys/stat.h>
#include <cstdio>
#include <unistd.h>

namespace std {
namespace filesystem {

// Add missing filesystem functions for WASI
inline path absolute(const path& p) {
  // Simple implementation - just return the path as-is
  // In WASI, all paths are already absolute or relative to the preopened directories
  return p;
}

inline path absolute(const path& p, std::error_code& ec) {
  ec.clear();
  return p;
}

inline bool create_directories(const path& p) {
  // For WASI, we'll need to implement this using WASI APIs
  // For now, return false to indicate failure
  return false;
}

inline bool create_directories(const path& p, std::error_code& ec) {
  ec = std::make_error_code(std::errc::function_not_supported);
  return false;
}

// Check if a file or directory exists
inline bool exists(const path& p) {
  struct stat st;
  return ::stat(p.c_str(), &st) == 0;
}

inline bool exists(const path& p, std::error_code& ec) {
  struct stat st;
  if (::stat(p.c_str(), &st) == 0) {
    ec.clear();
    return true;
  }
  ec = std::error_code(errno, std::system_category());
  return false;
}

// Remove a file or empty directory
inline bool remove(const path& p) {
  // Try to remove as file first
  if (::remove(p.c_str()) == 0) {
    return true;
  }
  // If that fails, try rmdir for directories
  if (::rmdir(p.c_str()) == 0) {
    return true;
  }
  return false;
}

inline bool remove(const path& p, std::error_code& ec) {
  if (::remove(p.c_str()) == 0 || ::rmdir(p.c_str()) == 0) {
    ec.clear();
    return true;
  }
  ec = std::error_code(errno, std::system_category());
  return false;
}

// Copy a file - declare the error_code version first
inline void copy_file(const path& from, const path& to, std::error_code& ec) {
  FILE* src = ::fopen(from.c_str(), "rb");
  if (!src) {
    ec = std::error_code(errno, std::system_category());
    return;
  }
  
  FILE* dst = ::fopen(to.c_str(), "wb");
  if (!dst) {
    ::fclose(src);
    ec = std::error_code(errno, std::system_category());
    return;
  }
  
  // Copy file contents
  char buffer[8192];
  size_t n;
  while ((n = ::fread(buffer, 1, sizeof(buffer), src)) > 0) {
    if (::fwrite(buffer, 1, n, dst) != n) {
      ec = std::error_code(errno, std::system_category());
      ::fclose(src);
      ::fclose(dst);
      return;
    }
  }
  
  ::fclose(src);
  ::fclose(dst);
  ec.clear();
}

// Copy a file - throwing version
inline void copy_file(const path& from, const path& to) {
  std::error_code ec;
  copy_file(from, to, ec);
  if (ec) {
    throw filesystem_error("cannot copy file", from, to, ec);
  }
}

// Check if path is a directory
inline bool is_directory(const path& p) {
  struct stat st;
  if (::stat(p.c_str(), &st) != 0) {
    return false;
  }
  return S_ISDIR(st.st_mode);
}

inline bool is_directory(const path& p, std::error_code& ec) {
  struct stat st;
  if (::stat(p.c_str(), &st) != 0) {
    ec = std::error_code(errno, std::system_category());
    return false;
  }
  ec.clear();
  return S_ISDIR(st.st_mode);
}

// WASI stub for weakly_canonical - simplifies paths but doesn't resolve symlinks fully
inline path weakly_canonical(const path& p) {
  // For WASI, we can't fully resolve symlinks, so just normalize the path
  // This is a simplified implementation
  path result;
  for (const auto& component : p) {
    if (component == ".") {
      // Skip current directory markers
      continue;
    } else if (component == "..") {
      // Go up one level if possible
      if (!result.empty() && result.filename() != "..") {
        result = result.parent_path();
      } else {
        result /= component;
      }
    } else {
      result /= component;
    }
  }
  return result.empty() ? path(".") : result;
}

inline path weakly_canonical(const path& p, std::error_code& ec) {
  ec.clear();
  try {
    return weakly_canonical(p);
  } catch (...) {
    ec = std::make_error_code(std::errc::invalid_argument);
    return path();
  }
}

} // namespace filesystem
} // namespace std

#else // !__wasi__

#define V8_ISOLATE_GET_CURRENT() v8::Isolate::GetCurrent()

#endif // __wasi__

#endif // WASI_NODE_COMPAT_H_