#ifndef WASI_STDLIB_COMPAT_H_
#define WASI_STDLIB_COMPAT_H_

#ifdef __wasi__

// Missing POSIX C library compatibility layer for WASI
#include <stddef.h>
#include <stdint.h>

// errno.h compatibility
#ifndef errno
extern int errno;
#endif

// errno definitions
#define EPERM 1
#define ENOENT 2
#define ESRCH 3
#define EINTR 4
#define EIO 5
#define ENXIO 6
#define E2BIG 7
#define ENOEXEC 8
#define EBADF 9
#define ECHILD 10
#define EAGAIN 11
#define ENOMEM 12
#define EACCES 13
#define EFAULT 14
#define ENOTBLK 15
#define EBUSY 16
#define EEXIST 17
#define EXDEV 18
#define ENODEV 19
#define ENOTDIR 20
#define EISDIR 21
#define EINVAL 22
#define ENFILE 23
#define EMFILE 24
#define ENOTTY 25
#define ETXTBSY 26
#define EFBIG 27
#define ENOSPC 28
#define ESPIPE 29
#define EROFS 30
#define EMLINK 31
#define EPIPE 32
#define EDOM 33
#define ERANGE 34

// stdio.h compatibility
#ifndef FILE
typedef struct {
  int _flags;
  char* _IO_read_ptr;
  char* _IO_read_end;
  char* _IO_read_base;
  char* _IO_write_base;
  char* _IO_write_ptr;
  char* _IO_write_end;
  char* _IO_buf_base;
  char* _IO_buf_end;
  char* _IO_save_base;
  char* _IO_backup_base;
  char* _IO_save_end;
  void* _markers;
  void* _chain;
  int _fileno;
  int _flags2;
  void* _old_offset;
  short _cur_column;
  signed char _vtable_offset;
  char* _shortbuf;
  void* _lock;
  int _offset;
  void* _codecvt;
  void* _wide_data;
  void* _freeres_list;
  size_t _freeres_buf;
  size_t __pad5;
  int _mode;
  char _unused2[15 * sizeof(void*) - 2 * sizeof(size_t)];
} FILE;
#endif

// Standard file descriptors
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO  
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

// Standard FILE pointers
extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

// Basic stdio function stubs
#ifndef EOF
#define EOF (-1)
#endif

// Process IDs (stubs for WASI)
#ifndef getpid
#define getpid() 1
#endif

// File operations (limited in WASI)
#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif
#ifndef SEEK_END
#define SEEK_END 2
#endif

#endif // __wasi__

#endif // WASI_STDLIB_COMPAT_H_
