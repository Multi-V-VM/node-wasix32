/* Copyright libuv contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/* Enable stubs for WASI or when UV_WASI_STUBS is defined (for wasm32 builds) */
#if defined(__wasi__) || defined(UV_WASI_STUBS)

#include "uv.h"
#include "internal.h"
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <sys/types.h>

// Stub implementations for WASI - Only implement missing functions


unsigned int if_nametoindex(const char *ifname) {
  return 0;
}

char* if_indextoname(unsigned int ifindex, char *ifname) {
  // WASI doesn't support network interfaces
  return NULL;
}

uint64_t uv__hrtime(uv_clocktype_t type) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

void uv__fs_event_close(uv_fs_event_t* handle) {
  // No-op for WASI - fs events not supported
}

int uv_fs_event_init(uv_loop_t* loop, uv_fs_event_t* handle) {
  return UV_ENOSYS;
}

int uv_fs_event_start(uv_fs_event_t* handle, uv_fs_event_cb cb,
                      const char* filename, unsigned int flags) {
  return UV_ENOSYS;
}

int uv_fs_event_stop(uv_fs_event_t* handle) {
  return UV_ENOSYS;
}

void uv__io_poll(uv_loop_t* loop, int timeout) {
  struct uv__queue* q;
  struct pollfd* poll_fds;
  uv__io_t** poll_watchers;
  uv__io_t* w;
  unsigned int fd;
  unsigned int i;
  unsigned int count;
  int result;

  while (!uv__queue_empty(&loop->watcher_queue)) {
    q = uv__queue_head(&loop->watcher_queue);
    uv__queue_remove(q);
    uv__queue_init(q);
    w = uv__queue_data(q, uv__io_t, watcher_queue);
    w->events = w->pevents;
  }

  count = loop->nfds;
  if (count == 0) {
    if (timeout > 0) {
      /*
       * Do not wait until the next timer deadline here. uv_check callbacks
       * can create the first I/O watcher (Node uses this for native
       * immediates), so a long wait would postpone the connect until after
       * user timers have already fired. Yield briefly and let the next loop
       * turn run the check phase.
       */
      struct timespec ts = {0, 1000000};
      nanosleep(&ts, NULL);
    }
    SAVE_ERRNO(uv__update_time(loop));
    return;
  }

  poll_fds = uv__malloc(count * sizeof(*poll_fds));
  poll_watchers = uv__malloc(count * sizeof(*poll_watchers));
  if (poll_fds == NULL || poll_watchers == NULL)
    abort();

  i = 0;
  for (fd = 0; fd < loop->nwatchers && i < count; fd++) {
    w = loop->watchers[fd];
    if (w == NULL || w->pevents == 0)
      continue;
    poll_fds[i].fd = w->fd;
    poll_fds[i].events = w->pevents;
    poll_fds[i].revents = 0;
    poll_watchers[i] = w;
    i++;
  }
  count = i;

  /*
   * WASIX advances socket readiness only while poll is allowed to wait. A
   * zero-time poll therefore cannot observe a nonblocking connect completion.
   * Keep libuv's timer deadline intact, but give active I/O enough time to
   * reach the WASIX readiness scheduler when the loop requests a
   * nonblocking turn.
   */
  result = poll(poll_fds, count, timeout == 0 ? 1000 : timeout);
  SAVE_ERRNO(uv__update_time(loop));

  if (result >= 0) {
    for (i = 0; i < count; i++) {
      unsigned int events;

      events = poll_fds[i].revents;
      if (events == 0)
        events = poll_watchers[i]->pevents & (POLLIN | POLLOUT);
      if (events == 0)
        continue;
      w = poll_watchers[i];
      fd = poll_fds[i].fd;
      if (fd >= loop->nwatchers || loop->watchers[fd] != w)
        continue;
      events &= w->pevents | POLLERR | POLLHUP | POLLNVAL;
      if (events != 0)
        w->cb(loop, w, events);
    }
  }


  uv__free(poll_watchers);
  uv__free(poll_fds);
}

void uv__platform_invalidate_fd(uv_loop_t* loop, int fd) {
  // No-op for WASI
}

int uv__platform_loop_init(uv_loop_t* loop) {
  return 0;
}

void uv__platform_loop_delete(uv_loop_t* loop) {
  // No-op for WASI
}

int uv__io_fork(uv_loop_t* loop) {
  return 0;
}

int uv__io_check_fd(uv_loop_t* loop, int fd) {
  return 0;
}

// Remove duplicate functions that are already implemented elsewhere
// TCP and UDP functions are implemented in tcp.c and udp.c

// Signal stubs - use weak symbols to avoid conflicts
__attribute__((weak)) void uv__signal_global_once_init(void) {}
__attribute__((weak)) void uv__signal_cleanup(void) {}
__attribute__((weak)) void uv__signal_close(uv_signal_t* handle) {}

// Process stubs - use weak symbols to avoid conflicts
__attribute__((weak)) int uv__process_init(uv_loop_t* loop) {
  return 0;
}

__attribute__((weak)) void uv__process_close(uv_process_t* handle) {
  // No-op for WASI
}

__attribute__((weak)) void uv__wait_children(uv_loop_t* loop) {
  // No-op for WASI
}

__attribute__((weak)) int uv_spawn(uv_loop_t* loop,
                                   uv_process_t* handle,
                                   const uv_process_options_t* options) {
  (void) loop;
  (void) handle;
  (void) options;
  return UV_ENOSYS;
}

__attribute__((weak)) int uv_process_kill(uv_process_t* process, int signum) {
  (void) process;
  (void) signum;
  return UV_ENOSYS;
}

__attribute__((weak)) int uv_kill(int pid, int signum) {
  (void) pid;
  (void) signum;
  return UV_ENOSYS;
}

// Thread stubs - use weak symbols to avoid conflicts
__attribute__((weak)) int uv_thread_create(uv_thread_t* tid, void (*entry)(void *arg), void* arg) {
  return UV_ENOSYS;
}

__attribute__((weak)) int uv_thread_join(uv_thread_t* tid) {
  return UV_ENOSYS;
}

// Misc stubs - use weak symbols to avoid conflicts
__attribute__((weak)) int uv_cpu_info(uv_cpu_info_t** cpu_infos, int* count) {
  *cpu_infos = NULL;
  *count = 0;
  return 0;
}

__attribute__((weak)) void uv_free_cpu_info(uv_cpu_info_t* cpu_infos, int count) {
  // No-op for WASI
}

int uv_interface_addresses(uv_interface_address_t** addresses, int* count) {
  *addresses = NULL;
  *count = 0;
  return 0;
}

void uv_free_interface_addresses(uv_interface_address_t* addresses, int count) {
  // No-op for WASI
}

int uv_uptime(double* uptime) {
  *uptime = 0.0;
  return 0;
}

void uv_loadavg(double avg[3]) {
  avg[0] = avg[1] = avg[2] = 0.0;
}

// Network interface stubs for WASI
struct ifaddrs {
  struct ifaddrs *ifa_next;
  char *ifa_name;
  unsigned int ifa_flags;
  struct sockaddr *ifa_addr;
  struct sockaddr *ifa_netmask;
  struct sockaddr *ifa_dstaddr;
  void *ifa_data;
};

int getifaddrs(struct ifaddrs **ifap) {
  // WASI doesn't support network interface enumeration
  *ifap = NULL;
  return 0;
}

void freeifaddrs(struct ifaddrs *ifa) {
  // No-op since we don't allocate anything
}

// Process title stubs
char** uv_setup_args(int argc, char** argv) {
  return argv;
}

void uv__process_title_cleanup(void) {
  // No-op for WASI
}

int uv_set_process_title(const char* title) {
  return 0;
}

// Memory info stubs
uint64_t uv_get_constrained_memory(void) {
  // WASI doesn't support memory constraints
  return 0;
}

uint64_t uv_get_total_memory(void) {
  // Return a default value for WASI (256 MB)
  return 256 * 1024 * 1024;
}

uint64_t uv_get_available_memory(void) {
  // Return a default value for WASI
  return 256 * 1024 * 1024;
}

// Executable path stub
int uv_exepath(char* buffer, size_t* size) {
  if (buffer == NULL || size == NULL || *size == 0)
    return UV_EINVAL;
  // Return a placeholder path for WASI
  const char* path = "/wasi/node";
  size_t len = strlen(path);
  if (*size <= len) {
    *size = len + 1;
    return UV_ENOBUFS;
  }
  memcpy(buffer, path, len);
  buffer[len] = '\0';
  *size = len;
  return 0;
}

// Free memory stub
uint64_t uv_get_free_memory(void) {
  // Return a reasonable default for WASI
  return 128 * 1024 * 1024;  // 128 MB
}

// Resident set memory stub
int uv_resident_set_memory(size_t* rss) {
  if (rss != NULL) {
    *rss = 64 * 1024 * 1024;  // 64 MB default
  }
  return 0;
}

int uv_get_process_title(char* buffer, size_t size) {
  if (buffer == NULL || size == 0)
    return UV_EINVAL;
  buffer[0] = '\0';
  return 0;
}

// io_uring stubs - these are Linux-specific and not available on WASI
int uv__iou_fs_close(uv_loop_t* loop, uv_fs_t* req) {
  return 0;  // Return 0 to indicate io_uring not used, fallback to regular path
}

int uv__iou_fs_ftruncate(uv_loop_t* loop, uv_fs_t* req) {
  return 0;
}

int uv__iou_fs_fsync_or_fdatasync(uv_loop_t* loop, uv_fs_t* req, uint32_t fsync_flags) {
  return 0;
}

int uv__iou_fs_link(uv_loop_t* loop, uv_fs_t* req) {
  return 0;
}

int uv__iou_fs_mkdir(uv_loop_t* loop, uv_fs_t* req) {
  return 0;
}

int uv__iou_fs_open(uv_loop_t* loop, uv_fs_t* req) {
  return 0;
}

int uv__iou_fs_read_or_write(uv_loop_t* loop, uv_fs_t* req, int is_read) {
  return 0;
}

int uv__iou_fs_rename(uv_loop_t* loop, uv_fs_t* req) {
  return 0;
}

int uv__iou_fs_statx(uv_loop_t* loop, uv_fs_t* req, int is_fstat, int is_lstat) {
  return 0;
}

int uv__iou_fs_symlink(uv_loop_t* loop, uv_fs_t* req) {
  return 0;
}

int uv__iou_fs_unlink(uv_loop_t* loop, uv_fs_t* req) {
  return 0;
}

// Linux-specific statx stubs
int uv__statx(int dirfd,
              const char* path,
              int flags,
              unsigned int mask,
              struct uv__statx* statxbuf) {
  return UV_ENOSYS;  // Not supported on WASI
}

void uv__statx_to_stat(const struct uv__statx* statxbuf, uv_stat_t* buf) {
  // No-op - should not be called since uv__statx returns error
  memset(buf, 0, sizeof(*buf));
}

// Linux-specific kernel version stub
unsigned uv__kernel_version(void) {
  return 0;  // Return 0 to indicate unknown/minimal kernel
}

// Linux-specific copy_file_range stub
ssize_t uv__fs_copy_file_range(int fd_in,
                                off_t* off_in,
                                int fd_out,
                                off_t* off_out,
                                size_t len,
                                unsigned int flags) {
  errno = ENOSYS;
  return -1;  // Not supported on WASI
}

// Process title stub
void uv__set_process_title(const char* title) {
  // No-op for WASI
}

// CPU constraint stub (for cgroups on Linux)
int uv__get_constrained_cpu(long long* quota) {
  (void) quota;
  return UV_ENOENT;  // No CPU constraints on WASI
}

// getrandom stub
ssize_t uv__getrandom(void* buf, size_t buflen, unsigned flags) {
  errno = ENOSYS;
  return -1;  // Not supported on WASI via syscall
}

#endif /* __wasi__ || UV_WASI_STUBS */
