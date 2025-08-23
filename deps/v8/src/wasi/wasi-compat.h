// WASI Compatibility Layer
#ifndef V8_WASI_COMPAT_H_
#define V8_WASI_COMPAT_H_

#ifdef __wasi__

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <sys/types.h>

// Signal handling stubs (WASI has no signals)
#define SIGINT 2
#define SIGTERM 15
#define SIGKILL 9
#define SIGSEGV 11
#define SIGABRT 6

struct sigaction {
  void (*sa_handler)(int);
  int sa_flags;
};

inline int sigaction(int, const struct sigaction*, struct sigaction*) { 
  return 0; 
}
inline int raise(int) { return 0; }
inline void (*signal(int, void (*)(int)))(int) { return nullptr; }

// Thread stubs (WASI is single-threaded)
typedef int pthread_t;
typedef int pthread_mutex_t;
typedef int pthread_cond_t;
typedef int pthread_key_t;
typedef struct {} pthread_attr_t;
typedef struct {} pthread_mutexattr_t;

#define PTHREAD_MUTEX_INITIALIZER 0
#define PTHREAD_ONCE_INIT 0

inline int pthread_create(pthread_t*, const pthread_attr_t*, 
                          void* (*)(void*), void*) { return -1; }
inline int pthread_join(pthread_t, void**) { return 0; }
inline int pthread_mutex_init(pthread_mutex_t*, const pthread_mutexattr_t*) { 
  return 0; 
}
inline int pthread_mutex_lock(pthread_mutex_t*) { return 0; }
inline int pthread_mutex_unlock(pthread_mutex_t*) { return 0; }
inline int pthread_mutex_destroy(pthread_mutex_t*) { return 0; }
inline int pthread_key_create(pthread_key_t*, void (*)(void*)) { return 0; }
inline void* pthread_getspecific(pthread_key_t) { return nullptr; }
inline int pthread_setspecific(pthread_key_t, const void*) { return 0; }

// Memory mapping stubs
#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4
#define PROT_NONE 0
#define MAP_PRIVATE 2
#define MAP_ANONYMOUS 0x20
#define MAP_FAILED ((void*)-1)

inline void* mmap(void*, size_t size, int, int, int, off_t) {
  return malloc(size);
}
inline int munmap(void* addr, size_t) {
  free(addr);
  return 0;
}
inline int mprotect(void*, size_t, int) { return 0; }

// File system stubs
inline int access(const char*, int) { return -1; }
inline int chmod(const char*, mode_t) { return -1; }
inline int chown(const char*, uid_t, gid_t) { return -1; }

// Process stubs
inline pid_t fork() { return -1; }
inline int execvp(const char*, char* const[]) { return -1; }
inline pid_t waitpid(pid_t, int*, int) { return -1; }

// System info stubs
#ifndef _SC_PAGESIZE
#define _SC_PAGESIZE 30
#endif
#ifndef _SC_NPROCESSORS_ONLN
#define _SC_NPROCESSORS_ONLN 84
#endif

inline long sysconf(int name) {
  switch (name) {
    case _SC_PAGESIZE: return 65536;  // WASM page size
    case _SC_NPROCESSORS_ONLN: return 1;
    default: return -1;
  }
}

struct rlimit {
  unsigned long rlim_cur;
  unsigned long rlim_max;
};

inline int getrlimit(int, struct rlimit*) { return -1; }
inline int setrlimit(int, const struct rlimit*) { return -1; }

#endif // __wasi__

#endif // V8_WASI_COMPAT_H_