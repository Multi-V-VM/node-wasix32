#ifndef UV_WASI_NETWORKING_H_
#define UV_WASI_NETWORKING_H_

#ifdef __wasi__

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// WASI might be missing some networking constants
#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

#ifndef NI_MAXSERV
#define NI_MAXSERV 32
#endif

#ifndef NI_NUMERICHOST
#define NI_NUMERICHOST 1
#endif

#ifndef NI_NUMERICSERV
#define NI_NUMERICSERV 2
#endif

#ifndef NI_NOFQDN
#define NI_NOFQDN 4
#endif

#ifndef NI_NAMEREQD
#define NI_NAMEREQD 8
#endif

#ifndef NI_DGRAM
#define NI_DGRAM 16
#endif

// Ensure standard address families are defined
#ifndef AF_INET
#define AF_INET 2
#endif

#ifndef AF_INET6
#define AF_INET6 10
#endif

#ifndef AF_UNSPEC
#define AF_UNSPEC 0
#endif

#endif /* __wasi__ */

#endif /* UV_WASI_NETWORKING_H_ */
