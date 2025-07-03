#ifndef CARES_AF_FIX_H
#define CARES_AF_FIX_H

// Define AF_MAX if not defined
#ifndef AF_MAX
#define AF_MAX 42
#endif

// Ensure AF_INET6 is properly defined
#ifdef AF_INET6
#undef AF_INET6
#endif
#define AF_INET6 10

#endif
