/*
 * Copyright 1995-2022 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef HEADER_CRYPTLIB_H
#define HEADER_CRYPTLIB_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include "internal/common.h"

#ifdef OPENSSL_SYS_VMS
#pragma names save
#pragma names as_is, shortened
#endif

/* Define the X509 macros if they're not defined elsewhere */
#ifndef X509_PRIVATE_DIR
#define X509_PRIVATE_DIR OPENSSLDIR "/private"
#endif

#ifndef X509_CERT_AREA
#define X509_CERT_AREA OPENSSLDIR
#endif

#ifndef X509_CERT_DIR
#define X509_CERT_DIR OPENSSLDIR "/certs"
#endif

#ifndef X509_CERT_FILE
#define X509_CERT_FILE OPENSSLDIR "/cert.pem"
#endif

#ifndef X509_CERT_URI
#define X509_CERT_URI ""
#endif

#ifndef X509_CERT_URI_EVP
#define X509_CERT_URI_EVP "SSL_CERT_URI"
#endif

#ifndef X509_CERT_PATH_EVP
#define X509_CERT_PATH_EVP "SSL_CERT_PATH"
#endif

#ifndef X509_CERT_DIR_EVP
#define X509_CERT_DIR_EVP "SSL_CERT_DIR"
#endif

#ifndef X509_CERT_FILE_EVP
#define X509_CERT_FILE_EVP "SSL_CERT_FILE"
#endif

#ifndef OPENSSLDIR
#define OPENSSLDIR "/etc/ssl"
#endif

#ifndef ENGINESDIR
#define ENGINESDIR "/dev/null"
#endif

#ifndef MODULESDIR
#define MODULESDIR "/usr/lib/openssl-modules"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OPENSSL_SYS_VMS
#pragma names restore
#endif

#endif /* HEADER_CRYPTLIB_H */
