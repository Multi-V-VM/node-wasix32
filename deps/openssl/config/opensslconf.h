#ifndef OPENSSL_OPENSSLCONF_H
#define OPENSSL_OPENSSLCONF_H

/* WASI-specific configuration for OpenSSL */

#ifdef __cplusplus
extern "C" {
#endif

#define OPENSSL_NO_AFALGENG 1
#define OPENSSL_NO_ASM 1
#define OPENSSL_NO_ASYNC 1
#define OPENSSL_NO_CAPIENG 1
#define OPENSSL_NO_COMP 1
#define OPENSSL_NO_CRYPTO_MDEBUG 1
#define OPENSSL_NO_DEPRECATED 1
#define OPENSSL_NO_DGRAM 1
#define OPENSSL_NO_DYNAMIC_ENGINE 1
#define OPENSSL_NO_EC_NISTP_64_GCC_128 1
#define OPENSSL_NO_EC2M 1
#define OPENSSL_NO_ENGINE 1
#define OPENSSL_NO_ERR 1
#define OPENSSL_NO_GOST 1
#define OPENSSL_NO_HEARTBEATS 1
#define OPENSSL_NO_HW 1
#define OPENSSL_NO_MSAN 1
#define OPENSSL_NO_NEXTPROTONEG 1
#define OPENSSL_NO_OCB 1
#define OPENSSL_NO_POSIX_IO 1
#define OPENSSL_NO_SOCK 1
#define OPENSSL_NO_STDIO 1
#define OPENSSL_NO_TESTS 1
#define OPENSSL_NO_THREADS 1
#define OPENSSL_NO_TS 1
#define OPENSSL_NO_UI 1
#define OPENSSL_NO_UBSAN 1
#define OPENSSL_NO_UNIT_TEST 1
#define OPENSSL_NO_WEAK_SSL_CIPHERS 1

/* X509 certificate directory definitions */
#define X509_CERT_DIR "/etc/ssl/certs"
#define X509_PRIVATE_DIR "/etc/ssl/private"
#define X509_CERT_AREA "/etc/ssl"
#define X509_CERT_FILE "/etc/ssl/cert.pem"
#define X509_CERT_DIR_EVP X509_CERT_DIR
#define OPENSSLDIR "/etc/ssl"
#define RC4_INT unsigned int

#ifdef __cplusplus
}
#endif
#endif /* OPENSSL_OPENSSLCONF_H */