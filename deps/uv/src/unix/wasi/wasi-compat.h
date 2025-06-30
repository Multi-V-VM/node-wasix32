#ifndef UV_WASI_COMPAT_H
#define UV_WASI_COMPAT_H

#ifdef __wasi__

/* WASI compatibility definitions */
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 1
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif

/* Stub out signals for WASI */
#define SIGINT 2
#define SIGTERM 15
#define SIGKILL 9

/* File descriptor limits */
#ifndef FD_SETSIZE
#define FD_SETSIZE 1024
#endif

#endif /* __wasi__ */

#endif /* UV_WASI_COMPAT_H */
