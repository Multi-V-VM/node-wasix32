#ifndef WASI_SYSTEM_COMPAT_H_
#define WASI_SYSTEM_COMPAT_H_

// This file provides system-specific compatibility for WASI builds
// Include this before any system calls in Node.js source files

#ifdef __wasi__

#include <sys/types.h>
#include <stdint.h>
#include <limits.h>

// Define rlim_t if not defined
#ifndef HAVE_RLIM_T
typedef unsigned long rlim_t;
#endif

// Define RLIM_INFINITY if not defined
#ifndef RLIM_INFINITY
#define RLIM_INFINITY ((rlim_t)-1)
#endif

// Resource limit definitions missing in WASI
#ifndef RLIMIT_NOFILE
#define RLIMIT_NOFILE 7  // Maximum number of open file descriptors
#endif

// Network service flags missing in WASI
#ifndef NI_NUMERICSERV
#define NI_NUMERICSERV 0x08  // Don't resolve service names
#endif

// Resource limit structure for WASI
#ifndef HAVE_STRUCT_RLIMIT
struct rlimit {
  rlim_t rlim_cur;  // Soft limit
  rlim_t rlim_max;  // Hard limit
};
#endif

// Stub implementations for resource limit functions
inline int getrlimit(int resource, struct rlimit *rlim) {
  // Stub implementation - return unlimited resources
  if (rlim) {
    rlim->rlim_cur = RLIM_INFINITY;
    rlim->rlim_max = RLIM_INFINITY;
  }
  return 0;
}

inline int setrlimit(int resource, const struct rlimit *rlim) {
  // Stub implementation - always succeed
  return 0;
}

// Filesystem compatibility for C++17 features
#ifdef __cplusplus
#include <string>
#include <vector>

namespace std {
namespace filesystem {

// Simple path class for WASI
class path {
 private:
  std::string path_str;
  
 public:
  path() = default;
  path(const char* p) : path_str(p) {}
  path(const std::string& p) : path_str(p) {}
  
  const char* c_str() const { return path_str.c_str(); }
  std::string string() const { return path_str; }
  
  path& operator/=(const path& p) {
    if (!path_str.empty() && path_str.back() != '/') {
      path_str += '/';
    }
    path_str += p.path_str;
    return *this;
  }
  
  friend path operator/(const path& lhs, const path& rhs) {
    path result = lhs;
    result /= rhs;
    return result;
  }
};

// Simple current_path implementation for WASI
inline path current_path() {
  // WASI doesn't have a traditional current working directory
  // Return root directory as default
  return path("/");
}

// Add other filesystem functions as needed

} // namespace filesystem
} // namespace std
#endif // __cplusplus

// Platform-specific timestamp function
inline double SystemClockTimeMillis() {
  // Simple timestamp implementation for WASI
  // In real WASI environments, you would use the WASI clock API
  // For now, return a monotonic counter
  static double counter = 0.0;
  return counter += 1.0;
}

// cppgc::HeapStatistics extensions for missing members
namespace cppgc {

class SpaceStatistics {
 public:
  const char* name;
  size_t physical_size_bytes;
  size_t used_size_bytes;
  std::vector<PageStatistics> page_stats;
};

class PageStatistics {
 public:
  size_t physical_size_bytes;
  size_t used_size_bytes;
  std::vector<ObjectStatsEntry> object_statistics;
};

class ObjectStatsEntry {
 public:
  const char* type_name;
  size_t allocated_bytes;
  size_t object_count;
};

// Extend HeapStatistics with missing members
inline void ExtendHeapStatistics(HeapStatistics* stats) {
  // This is a placeholder - the actual HeapStatistics needs these members
}

} // namespace cppgc

#endif // __wasi__

#endif // WASI_SYSTEM_COMPAT_H_