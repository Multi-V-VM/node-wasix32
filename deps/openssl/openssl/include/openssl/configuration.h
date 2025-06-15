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
