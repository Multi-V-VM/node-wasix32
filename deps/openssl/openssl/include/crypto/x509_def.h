/*
 * Copyright 2022 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef HEADER_X509_DEF_H
#define HEADER_X509_DEF_H

/* Define the X509 macros that are needed by x509_def.c */
#ifndef X509_PRIVATE_DIR
#define X509_PRIVATE_DIR "/etc/ssl/private"
#endif

#ifndef X509_CERT_AREA
#define X509_CERT_AREA "/etc/ssl"
#endif

#ifndef X509_CERT_DIR
#define X509_CERT_DIR "/etc/ssl/certs"
#endif

#ifndef X509_CERT_FILE
#define X509_CERT_FILE "/etc/ssl/cert.pem"
#endif

#ifndef X509_CERT_URI
#define X509_CERT_URI ""
#endif

/* Environment variable suffixes */
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

#endif /* HEADER_X509_DEF_H */