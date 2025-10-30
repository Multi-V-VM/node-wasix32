#ifdef __wasi__

#include "src/base/platform/platform.h"
#include <cstdlib>

namespace v8 {
namespace base {

// Stub implementations for WASI
void OS::Sleep(TimeDelta interval) {
  // WASI doesn't support sleep, just yield
}

void OS::Abort() {
  abort();
}

int OS::GetCurrentProcessId() {
  return 1; // Fixed PID for WASI
}

int OS::GetCurrentThreadId() {
  return 1; // Fixed thread ID for WASI
}

void* OS::Allocate(void* hint, size_t size, size_t alignment,
                   MemoryPermission access) {
  return aligned_alloc(alignment, size);
}

bool OS::Free(void* address, size_t size) {
  free(address);
  return true;
}

bool OS::SetPermissions(void* address, size_t size,
                        MemoryPermission access) {
  // WASI doesn't support memory permissions
  return true;
}

} // namespace base
} // namespace v8

#endif // __wasi__