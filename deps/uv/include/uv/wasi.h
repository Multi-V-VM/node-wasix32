#ifndef UV_WASI_H
#define UV_WASI_H

#ifdef __wasi__

// WASI-specific UV configuration
#define UV_PLATFORM_LOOP_FIELDS /* empty */
#define UV_PLATFORM_SEM_T void*
#define UV_PLATFORM_FS_FIELDS /* empty */

// Disable features not available in WASI
#define UV_HAVE_FORK 0
#define UV_HAVE_KQUEUE 0
#define UV_HAVE_EPOLL 0

// Use basic file descriptors
typedef int uv_os_fd_t;
typedef void* uv_os_sock_t;

// Thread types (minimal for WASI)
typedef struct {
  void* unused;
} uv_thread_t;

typedef void* uv_key_t;
typedef void* uv_once_t;
typedef void* uv_mutex_t;
typedef void* uv_rwlock_t;
typedef void* uv_sem_t;
typedef void* uv_cond_t;
typedef void* uv_barrier_t;

// Process types
typedef int uv_pid_t;
typedef int uv_uid_t;
typedef int uv_gid_t;

// Library handle
typedef void* uv_lib_t;

#define UV_ONCE_INIT 0

#endif /* __wasi__ */

#endif /* UV_WASI_H */
