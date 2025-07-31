#ifndef WASI_SYSTEM_COMPAT_H_
#define WASI_SYSTEM_COMPAT_H_

// This file provides system-specific compatibility for WASI builds
// Include this before any system calls in Node.js source files

#ifdef __wasi__

#include <sys/types.h>
#include <stdint.h>
#include <limits.h>

// Check if we need to include sys/resource.h or define our own types
#include <sys/resource.h>

// If sys/resource.h doesn't provide these, define them
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

// The WASI SDK now provides getrlimit and setrlimit in sys/resource.h
// so we don't need stub implementations

// Filesystem compatibility for C++17 features
#ifdef __cplusplus
#include <string>
#include <vector>

// Note: std::filesystem is provided by WASI SDK
// Don't redefine it here to avoid conflicts
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

// Forward declarations to fix ordering issues
class ObjectStatsEntry;
class PageStatistics;

class ObjectStatsEntry {
 public:
  const char* type_name;
  size_t allocated_bytes;
  size_t object_count;
};

class PageStatistics {
 public:
  size_t physical_size_bytes;
  size_t used_size_bytes;
  std::vector<ObjectStatsEntry> object_statistics;
};

class SpaceStatistics {
 public:
  const char* name;
  size_t physical_size_bytes;
  size_t used_size_bytes;
  std::vector<PageStatistics> page_stats;
};

} // namespace cppgc

// v8::HeapStatistics extensions
namespace v8 {

// Forward declaration
class HeapStatistics;

// Extend HeapStatistics with missing members
inline void ExtendHeapStatistics(HeapStatistics* stats) {
  // This is a placeholder - the actual HeapStatistics needs these members
}

} // namespace v8

#endif // __wasi__

#endif // WASI_SYSTEM_COMPAT_H_