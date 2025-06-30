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

// Stub implementations for WASI

char** uv_setup_args(int argc, char** argv) {
  return argv;
}

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

int uv__io_poll(uv_loop_t* loop, int timeout) {
  // Basic implementation - just sleep if timeout is positive
  if (timeout > 0) {
    struct timespec ts;
    ts.tv_sec = timeout / 1000;
    ts.tv_nsec = (timeout % 1000) * 1000000;
    nanosleep(&ts, NULL);
  }
  return 0;
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

// Additional stubs that might be needed
int uv__tcp_bind(uv_tcp_t* handle, const struct sockaddr* addr, unsigned int addrlen, unsigned int flags) {
  return UV_ENOSYS;
}

int uv__tcp_connect(uv_connect_t* req, uv_tcp_t* handle, const struct sockaddr* addr, unsigned int addrlen, uv_connect_cb cb) {
  return UV_ENOSYS;
}

int uv__udp_bind(uv_udp_t* handle, const struct sockaddr* addr, unsigned int addrlen, unsigned int flags) {
  return UV_ENOSYS;
}

int uv__udp_connect(uv_udp_t* handle, const struct sockaddr* addr) {
  return UV_ENOSYS;
}

int uv__udp_disconnect(uv_udp_t* handle) {
  return UV_ENOSYS;
}

int uv__udp_is_connected(const uv_udp_t* handle) {
  return 0;
}

#endif /* __wasi__ */