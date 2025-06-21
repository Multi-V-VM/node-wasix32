// WASI netdb.h stub
#ifndef _WASI_NETDB_H
#define _WASI_NETDB_H

#define MAXHOSTNAMELEN 256

struct hostent {
    char  *h_name;
    char **h_aliases;
    int    h_addrtype;
    int    h_length;
    char **h_addr_list;
};

struct servent {
    char  *s_name;
    char **s_aliases;
    int    s_port;
    char  *s_proto;
};

// Stub implementations that return errors
static inline struct hostent* gethostbyname(const char* name) { return NULL; }
static inline struct servent* getservbyname(const char* name, const char* proto) { return NULL; }
static inline struct servent* getservbyport(int port, const char* proto) { return NULL; }

#endif
