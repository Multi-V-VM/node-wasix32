/* WASI-specific c-ares configuration */
/* Based on Linux config but with WASI limitations */

/* a suitable file/device to read random data from */
/* #undef CARES_RANDOM_FILE */

/* Set to 1 if non-public shared library symbols are hidden */
#define CARES_SYMBOL_HIDING 1

/* Threading disabled for WASI */
/* #undef CARES_THREADS */

/* the signed version of size_t */
#define CARES_TYPEOF_ARES_SSIZE_T ssize_t

/* Use resolver library to configure cares */
/* #undef CARES_USE_LIBRESOLV */

/* if a /etc/inet dir is being used */
/* #undef ETC_INET */

/* gethostname() arg2 type */
#define GETHOSTNAME_TYPE_ARG2 size_t

/* getnameinfo() arg1 type */
#define GETNAMEINFO_TYPE_ARG1 struct sockaddr *

/* getnameinfo() arg2 type */
#define GETNAMEINFO_TYPE_ARG2 socklen_t

/* getnameinfo() arg4 and 6 type */
#define GETNAMEINFO_TYPE_ARG46 socklen_t

/* getnameinfo() arg7 type */
#define GETNAMEINFO_TYPE_ARG7 int

/* number of arguments for getservbyname_r() */
/* #undef GETSERVBYNAME_R_ARGS */

/* number of arguments for getservbyport_r() */
/* #undef GETSERVBYPORT_R_ARGS */

/* Define to 1 if you have AF_INET6 */
#define HAVE_AF_INET6 1

/* Define to 1 if you have `arc4random_buf` */
/* #undef HAVE_ARC4RANDOM_BUF */

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the <arpa/nameser_compat.h> header file. */
/* #undef HAVE_ARPA_NAMESER_COMPAT_H */

/* Define to 1 if you have the <arpa/nameser.h> header file. */
/* #undef HAVE_ARPA_NAMESER_H */

/* Define to 1 if you have the <assert.h> header file. */
#define HAVE_ASSERT_H 1

/* Define to 1 if you have `clock_gettime` */
#define HAVE_CLOCK_GETTIME 1

/* clock_gettime() with CLOCK_MONOTONIC support */
#define HAVE_CLOCK_GETTIME_MONOTONIC 1

/* Define to 1 if you have `connect` */
#define HAVE_CONNECT 1

/* define if the compiler supports basic C++14 syntax */
#define HAVE_CXX14 1

/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */

/* Define to 1 if you have `epoll_{create1,ctl,wait}` - not available in WASI */
/* #undef HAVE_EPOLL */

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have `fcntl` */
#define HAVE_FCNTL 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have `freeaddrinfo` */
#define HAVE_FREEADDRINFO 1

/* Define to 1 if you have `getaddrinfo` */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have `getenv` */
#define HAVE_GETENV 1

/* Define to 1 if you have `gethostname` */
#define HAVE_GETHOSTNAME 1

/* Define to 1 if you have `getifaddrs` */
/* #undef HAVE_GETIFADDRS */

/* Define to 1 if you have `getnameinfo` */
#define HAVE_GETNAMEINFO 1

/* Define to 1 if you have `getservbyport_r` */
/* #undef HAVE_GETSERVBYPORT_R */

/* Define to 1 if you have `gettimeofday` */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have `if_indextoname` */
/* #undef HAVE_IF_INDEXTONAME */

/* Define to 1 if you have `if_nametoindex` */
/* #undef HAVE_IF_NAMETOINDEX */

/* Define to 1 if you have `inet_net_pton` */
/* #undef HAVE_INET_NET_PTON */

/* Define to 1 if you have `inet_ntop` */
#define HAVE_INET_NTOP 1

/* Define to 1 if you have `inet_pton` */
#define HAVE_INET_PTON 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have `ioctl` */
/* #undef HAVE_IOCTL */

/* Define to 1 if you have `kqueue` */
/* #undef HAVE_KQUEUE */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if the compiler supports the 'long long' data type. */
#define HAVE_LONGLONG 1

/* Define to 1 if you have the <malloc.h> header file. */
/* #undef HAVE_MALLOC_H */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <netinet/tcp.h> header file. */
/* #undef HAVE_NETINET_TCP_H */

/* Define to 1 if you have the <net/if.h> header file. */
/* #undef HAVE_NET_IF_H */

/* Define to 1 if you have PF_INET6 */
#define HAVE_PF_INET6 1

/* Define to 1 if you have `pipe` */
#define HAVE_PIPE 1

/* Define to 1 if you have `pipe2` */
/* #undef HAVE_PIPE2 */

/* Define to 1 if you have `poll` */
#define HAVE_POLL 1

/* Define to 1 if you have the <poll.h> header file. */
#define HAVE_POLL_H 1

/* Define to 1 if you have a working POSIX-style strerror_r function. */
/* #undef HAVE_POSIX_STRERROR_R */

/* Define to 1 if you have `pthread_init` */
/* #undef HAVE_PTHREAD_INIT */

/* Define to 1 if you have `pthread_np.h` */
/* #undef HAVE_PTHREAD_NP_H */

/* Define to 1 if you have `recv` */
#define HAVE_RECV 1

/* Define to 1 if you have `recvfrom` */
#define HAVE_RECVFROM 1

/* Define to 1 if you have `send` */
#define HAVE_SEND 1

/* Define to 1 if you have `setsockopt` */
#define HAVE_SETSOCKOPT 1

/* Define to 1 if you have `socket` */
#define HAVE_SOCKET 1

/* Define to 1 if you have the <socket.h> header file. */
/* #undef HAVE_SOCKET_H */

/* socklen_t */
#define HAVE_SOCKLEN_T 1

/* Define to 1 if you have the <stdbool.h> header file. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have `strcasecmp` */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have `strdup` */
#define HAVE_STRDUP 1

/* Define to 1 if you have `stricmp` */
/* #undef HAVE_STRICMP */

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have `strncasecmp` */
#define HAVE_STRNCASECMP 1

/* Define to 1 if you have `strnicmp` */
/* #undef HAVE_STRNICMP */

/* Define to 1 if you have `struct sockaddr_in6` */
#define HAVE_STRUCT_SOCKADDR_IN6 1

/* Define to 1 if `sin6_scope_id` is a member of `struct sockaddr_in6`. */
#define HAVE_STRUCT_SOCKADDR_IN6_SIN6_SCOPE_ID 1

/* Define to 1 if you have `struct timeval` */
#define HAVE_STRUCT_TIMEVAL 1

/* Define to 1 if you have the <sys/epoll.h> header file. */
/* #undef HAVE_SYS_EPOLL_H */

/* Define to 1 if you have the <sys/event.h> header file. */
/* #undef HAVE_SYS_EVENT_H */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
/* #undef HAVE_SYS_IOCTL_H */

/* Define to 1 if you have the <sys/param.h> header file. */
/* #undef HAVE_SYS_PARAM_H */

/* Define to 1 if you have the <sys/random.h> header file. */
/* #undef HAVE_SYS_RANDOM_H */

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
/* #undef HAVE_SYS_UIO_H */

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have `writev` */
/* #undef HAVE_WRITEV */

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* recv() arg2 type */
#define RECV_TYPE_ARG2 void *

/* recv() arg3 type */
#define RECV_TYPE_ARG3 size_t

/* recv() arg4 type */
#define RECV_TYPE_ARG4 int

/* recv() return type */
#define RECV_TYPE_RETV ssize_t

/* recvfrom() arg2 type */
#define RECVFROM_TYPE_ARG2 void *

/* recvfrom() arg2-5 qualifier */
#define RECVFROM_TYPE_ARG2_IS_VOID 1

/* recvfrom() arg3 type */
#define RECVFROM_TYPE_ARG3 size_t

/* recvfrom() arg4 type */
#define RECVFROM_TYPE_ARG4 int

/* recvfrom() arg5 type */
#define RECVFROM_TYPE_ARG5 struct sockaddr *

/* recvfrom() arg6 type */
#define RECVFROM_TYPE_ARG6 socklen_t *

/* recvfrom() return type */
#define RECVFROM_TYPE_RETV ssize_t

/* send() arg2 type */
#define SEND_TYPE_ARG2 void *

/* send() arg2 qualifier */
#define SEND_QUAL_ARG2 const

/* send() arg3 type */
#define SEND_TYPE_ARG3 size_t

/* send() arg4 type */
#define SEND_TYPE_ARG4 int

/* send() return type */
#define SEND_TYPE_RETV ssize_t

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* struct hostent available */
#define HAVE_STRUCT_HOSTENT 1
