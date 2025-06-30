// Platform implementation for WASI
#include "src/base/platform/platform.h"
#include "src/base/platform/wasi-platform-fix.h"

#ifdef __wasi__

#include <errno.h>
#include <time.h>
#include <unistd.h>

namespace v8 {
namespace base {

// Stub implementations for WASI
void OS::Sleep(TimeDelta interval) {
  usleep(static_cast<useconds_t>(interval.InMicroseconds()));
}

void OS::Abort() {
  abort();
}

void OS::PrepareForLowMemory() {
  // No-op for WASI
}

int OS::GetCurrentProcessId() {
  return 1; // WASI doesn't have process IDs
}

int OS::GetCurrentThreadId() {
  return 1; // WASI doesn't have thread IDs
}

void OS::ExitProcess(int exit_code) {
  exit(exit_code);
}

}  // namespace base
}  // namespace v8

#endif  // __wasi__
