/* libuv WASI stubs - minimal implementation for undefined symbols */

#include "uv.h"
#include "../internal.h"  /* internal.h is in the parent directory */
#include <stdint.h>
#include <time.h>
#include <errno.h>

/* High-resolution time stub */
uint64_t uv__hrtime(uv_clocktype_t type) {
  struct timespec ts;
  /* For WASI, we ignore the clock type and always use CLOCK_MONOTONIC */
  (void)type;
  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
    return 0;
  }
  return (uint64_t)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

/* Platform-specific loop initialization */
int uv__platform_loop_init(uv_loop_t* loop) {
  /* WASI doesn't need platform-specific initialization */
  return 0;
}

/* Platform-specific loop cleanup */
void uv__platform_loop_delete(uv_loop_t* loop) {
  /* WASI doesn't need platform-specific cleanup */
}

/* I/O polling stub - WASI doesn't support traditional polling */
void uv__io_poll(uv_loop_t* loop, int timeout) {
  /* In WASI, we can't do traditional I/O polling
     For now, just sleep if timeout is positive */
  if (timeout > 0) {
    struct timespec ts;
    ts.tv_sec = timeout / 1000;
    ts.tv_nsec = (timeout % 1000) * 1000000;
    nanosleep(&ts, NULL);
  }
}

/* File descriptor validation */
int uv__io_check_fd(uv_loop_t* loop, int fd) {
  /* WASI has limited FD support, assume valid for now */
  return 0;
}

/* Invalidate file descriptor */
void uv__platform_invalidate_fd(uv_loop_t* loop, int fd) {
  /* No-op for WASI */
}

/* Fork handling - WASI doesn't support fork */
int uv__io_fork(uv_loop_t* loop) {
  errno = ENOSYS;
  return -ENOSYS;
}

/* File system events - not supported in WASI */
void uv__fs_event_close(uv_fs_event_t* handle) {
  /* No-op for WASI */
}

/* Network interface name to index - stub for WASI */
unsigned int if_nametoindex(const char *ifname) {
  /* WASI doesn't have network interfaces */
  return 0;
}

/* Network interface address functions - stubs for WASI */
struct ifaddrs {
  struct ifaddrs *ifa_next;
  char *ifa_name;
  unsigned int ifa_flags;
  struct sockaddr *ifa_addr;
  struct sockaddr *ifa_netmask;
  union {
    struct sockaddr *ifu_broadaddr;
    struct sockaddr *ifu_dstaddr;
  } ifa_ifu;
  void *ifa_data;
};

int getifaddrs(struct ifaddrs **ifap) {
  /* WASI doesn't support network interfaces */
  *ifap = NULL;
  errno = ENOSYS;
  return -1;
}

void freeifaddrs(struct ifaddrs *ifa) {
  /* No-op for WASI */
  (void)ifa;
}

/* Setup args - simplified for WASI */
char** uv_setup_args(int argc, char** argv) {
  /* WASI doesn't need special args setup */
  return argv;
}
