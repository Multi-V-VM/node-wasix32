/* Minimal WASI c-ares configuration */
#ifndef ARES_CONFIG_H
#define ARES_CONFIG_H

#define CARES_STATICLIB 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define PACKAGE "c-ares"
#define PACKAGE_VERSION "1.17.0"

/* WASI-specific defines to avoid network code */
#ifdef __wasi__
#define CARES_DISABLE_NETWORK 1
#endif

#endif
