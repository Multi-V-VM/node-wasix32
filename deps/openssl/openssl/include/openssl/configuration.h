/*
 * {- join("\n * ", @autowarntext) -}
 *
 * Copyright 2016-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OPENSSL_CONFIGURATION_H
#define OPENSSL_CONFIGURATION_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OPENSSL_ALGORITHM_DEFINES
#error OPENSSL_ALGORITHM_DEFINES no longer supported
#endif

/*
 * OpenSSL was configured with the following options:
 */

#define RC4_INT unsigned int

#ifdef __wasi__
# define OPENSSL_NO_ASYNC
# define OPENSSL_NO_COMP
# define OPENSSL_NO_DEFAULT_THREAD_POOL
# define OPENSSL_NO_DGRAM
# define OPENSSL_NO_DSO
# define OPENSSL_NO_DTLS
# define OPENSSL_NO_DYNAMIC_ENGINE
# define OPENSSL_NO_ENGINE
# define OPENSSL_NO_MD2
# define OPENSSL_NO_POSIX_IO
# define OPENSSL_NO_QUIC
# define OPENSSL_NO_RC5
# define OPENSSL_NO_SCTP
# define OPENSSL_NO_SECURE_MEMORY
# define OPENSSL_NO_SM2
# define OPENSSL_NO_SOCK
# define OPENSSL_NO_STDIO
# define OPENSSL_NO_THREAD_POOL
# define OPENSSL_NO_THREADS
# define OPENSSL_NO_TS
# define OPENSSL_NO_UI
# define OPENSSL_NO_WINSTORE
# define OPENSSL_NO_ZLIB
# define OPENSSL_RAND_SEED_GETRANDOM
# define ASYNC_NULL
# define DSO_NONE
#endif

#if defined(OPENSSL_NO_COMP) ||                                                \
    (defined(OPENSSL_NO_BROTLI) && defined(OPENSSL_NO_ZSTD) &&                 \
     defined(OPENSSL_NO_ZLIB))
#define OPENSSL_NO_COMP_ALG
#else
#undef OPENSSL_NO_COMP_ALG
#endif

#ifdef __cplusplus
}
#endif

#endif /* OPENSSL_CONFIGURATION_H */
