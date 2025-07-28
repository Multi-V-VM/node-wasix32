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

#ifdef __wasi__

#include "uv.h"
#include "internal.h"
#include <stdint.h>
#include <time.h>

// Stub implementations for WASI - Only implement missing functions


unsigned int if_nametoindex(const char *ifname) {
  return 0;
}

uint64_t uv__hrtime(uv_clocktype_t type) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

void uv__fs_event_close(uv_fs_event_t* handle) {
  // No-op for WASI
}

void uv__io_poll(uv_loop_t* loop, int timeout) {
  // Basic implementation - just sleep if timeout is positive
  if (timeout > 0) {
    struct timespec ts;
    ts.tv_sec = timeout / 1000;
    ts.tv_nsec = (timeout % 1000) * 1000000;
    nanosleep(&ts, NULL);
  }
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

// Node.js specific stubs
char** uv_setup_args(int argc, char** argv) {
  // Return argv as-is for WASI
  return argv;
}

#endif /* __wasi__ */
