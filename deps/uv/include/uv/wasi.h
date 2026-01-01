#ifndef UV_WASI_H
#define UV_WASI_H

#ifdef __wasi__

#include <pthread.h>

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

// Thread types - use real pthread types for WASI threading
typedef pthread_t uv_thread_t;
typedef pthread_key_t uv_key_t;
typedef pthread_once_t uv_once_t;
typedef pthread_mutex_t uv_mutex_t;
typedef pthread_rwlock_t uv_rwlock_t;
typedef pthread_cond_t uv_cond_t;

// Semaphore - WASI doesn't have sem_t, use pointer to custom struct
// Actual struct is defined in thread.c
typedef void* uv_sem_t;

// Barrier - use pthread barrier
typedef pthread_barrier_t uv_barrier_t;

// Process types
typedef int uv_pid_t;
typedef int uv_uid_t;
typedef int uv_gid_t;

// Library handle
typedef void* uv_lib_t;

#define UV_ONCE_INIT PTHREAD_ONCE_INIT

#endif /* __wasi__ */

#endif /* UV_WASI_H */
