#ifndef CARES_SERVICE_FIX_H
#define CARES_SERVICE_FIX_H

#include <netdb.h>

#ifdef __wasi__
// Service function stubs for WASI
// servent struct already defined in cares-wasi-comprehensive-fix.h

// Just provide stub implementations as replacements
#define getservbyname(name, proto) ((struct servent*)NULL)
#define getservbyport(port, proto) ((struct servent*)NULL)

#endif

#endif /* CARES_SERVICE_FIX_H */
