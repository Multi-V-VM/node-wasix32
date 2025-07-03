#ifndef ARES_AF_FIX_H
#define ARES_AF_FIX_H

// Fix AF_MAX for WASI
#ifndef AF_MAX
#define AF_MAX 42
#endif

// Undefine and redefine AF_INET6 to avoid conflicts
#ifdef AF_INET6
#undef AF_INET6
#endif
#define AF_INET6 10

#endif // ARES_AF_FIX_H
