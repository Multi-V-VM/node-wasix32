/*
 * Special header for WASI platform compatibility
 * This defines platform-specific macros for WebAssembly/WASI
 */

#ifndef HEADER_WASI_CRYPTO_H
#define HEADER_WASI_CRYPTO_H

/* Define architecture as WASI */
#define OPENSSL_SYS_WASI

/* Disable features that are not supported in WASI */
#define OPENSSL_NO_ASYNC
#define OPENSSL_NO_POSIX_IO
#define OPENSSL_NO_SOCK
#define OPENSSL_NO_DGRAM
#define OPENSSL_NO_AFALGENG
#define OPENSSL_NO_ENGINE
#define OPENSSL_NO_DYNAMIC_ENGINE
#define OPENSSL_NO_STATIC_ENGINE
#define OPENSSL_NO_DEVCRYPTOENG
#define OPENSSL_NO_SCTP
#define OPENSSL_NO_STDIO
#define OPENSSL_NO_OCSP
#define OPENSSL_NO_THREADS
#define OPENSSL_NO_DSO
#define OPENSSL_NO_DEPRECATED

/* Define X509 certificate directories for WASI */
#define X509_CERT_DIR "/etc/ssl/certs"
#define X509_CERT_FILE "/etc/ssl/cert.pem"
#define X509_PRIVATE_DIR "/etc/ssl/private"
#define X509_CERT_AREA "/etc/ssl"
#define X509_CERT_URI ""
#define X509_CERT_FILE_EVP "SSL_CERT_FILE"
#define X509_CERT_DIR_EVP "SSL_CERT_DIR"
#define X509_CERT_PATH_EVP "SSL_CERT_PATH"
#define X509_CERT_URI_EVP "SSL_CERT_URI"

/* Define compiler-specific attributes */
#if defined(__GNUC__) && __GNUC__ >= 3
#define OPENSSL_EXPORT __attribute__((visibility("default")))
#else
#define OPENSSL_EXPORT
#endif

/* WebAssembly/WASI specific configurations */
#define OPENSSLDIR "/etc/ssl"
#define ENGINESDIR "/dev/null"
#define MODULESDIR "/dev/null"

#endif /* HEADER_WASI_CRYPTO_H */