// WASI netdb.h stub
#ifndef _WASI_NETDB_H
#define _WASI_NETDB_H

#define MAXHOSTNAMELEN 256

#ifndef HOSTENT_DEFINED
#define HOSTENT_DEFINED
struct hostent {
    char  *h_name;
    char **h_aliases;
    int    h_addrtype;
    int    h_length;
    char **h_addr_list;
};
#endif

#ifndef SERVENT_DEFINED
#define SERVENT_DEFINED
struct servent {
    char  *s_name;
    char **s_aliases;
    int    s_port;
    char  *s_proto;
};
#endif

// Stub implementations that return errors

#endif
