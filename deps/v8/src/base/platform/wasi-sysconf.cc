#ifdef __wasi__
#include <errno.h>

extern "C" {
long sysconf(int name) {
  if (name == 30) return 4096;  // _SC_PAGESIZE
  if (name == 84) return 1;      // _SC_NPROCESSORS_ONLN
  errno = EINVAL;
  return -1;
}
}
#endif
