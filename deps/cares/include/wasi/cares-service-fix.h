#ifndef CARES_SERVICE_FIX_H
#define CARES_SERVICE_FIX_H

#include <netdb.h>

#ifdef __wasi__
// Service function stubs for WASI
struct servent {
  char *s_name;
  char **s_aliases;
  int s_port;
  char *s_proto;
};

static inline struct servent* getservbyname(const char *name, const char *proto) {
  return NULL;
}

static inline struct servent* getservbyport(int port, const char *proto) {
  return NULL;
}
#endif

#endif /* CARES_SERVICE_FIX_H */
