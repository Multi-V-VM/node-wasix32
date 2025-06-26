#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_WASI_PLATFORM_TYPES_H_
#define V8_WASI_PLATFORM_TYPES_H_

#ifdef __wasi__

#include <cstdint>

namespace v8 {
namespace base {

// WASI platform types
using PlatformSharedMemoryHandle = int;

class Platform {
 public:
  virtual ~Platform() = default;
  // Minimal platform interface for WASI
};

}  // namespace base
}  // namespace v8

#endif // __wasi__

#endif // V8_WASI_PLATFORM_TYPES_H_
