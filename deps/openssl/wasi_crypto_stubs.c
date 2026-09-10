/* Trap-stubs for OpenSSL functions referenced by Node/ncrypto/ssl but not
 * compiled in this partial WASI OpenSSL build (no libcrypto QUIC/EC/RSA
 * serialization pieces). They abort loudly when reached instead of silently
 * returning empty values — silent stubs have already caused hard-to-trace
 * crashes elsewhere in this port.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define WASI_CRYPTO_STUB(name)                                              \
  do {                                                                      \
    fprintf(stderr,                                                         \
            "FATAL: OpenSSL function '" #name                               \
            "' is not available in this WASI build\n");                     \
    abort();                                                                \
  } while (0)

int i2d_RSA_PUBKEY(const void *a, unsigned char **pp) {
  (void)a; (void)pp;
  WASI_CRYPTO_STUB(i2d_RSA_PUBKEY);
}

/* point_conversion_form_t is an enum (int) */
int EC_KEY_get_conv_form(const void *key) {
  (void)key;
  WASI_CRYPTO_STUB(EC_KEY_get_conv_form);
}

const char *EC_curve_nid2nist(int nid) {
  (void)nid;
  WASI_CRYPTO_STUB(EC_curve_nid2nist);
}

int BN_print(void *bio, const void *a) {
  (void)bio; (void)a;
  WASI_CRYPTO_STUB(BN_print);
}

void ossl_quic_set_connect_state(void *s) {
  (void)s;
  WASI_CRYPTO_STUB(ossl_quic_set_connect_state);
}

void ossl_quic_set_accept_state(void *s) {
  (void)s;
  WASI_CRYPTO_STUB(ossl_quic_set_accept_state);
}

int ossl_quic_conn_shutdown(void *s, uint64_t flags, const void *args,
                            size_t args_len) {
  (void)s; (void)flags; (void)args; (void)args_len;
  WASI_CRYPTO_STUB(ossl_quic_conn_shutdown);
}

void ossl_quic_conn_set0_net_rbio(void *s, void *b) {
  (void)s; (void)b;
  WASI_CRYPTO_STUB(ossl_quic_conn_set0_net_rbio);
}

void ossl_quic_conn_set0_net_wbio(void *s, void *b) {
  (void)s; (void)b;
  WASI_CRYPTO_STUB(ossl_quic_conn_set0_net_wbio);
}

void ERR_print_errors(void *bio) {
  (void)bio;
  WASI_CRYPTO_STUB(ERR_print_errors);
}

void *ENGINE_get_ssl_client_cert_function(const void *engine) {
  (void)engine;
  WASI_CRYPTO_STUB(ENGINE_get_ssl_client_cert_function);
}

int ENGINE_set_default(void *engine, unsigned int flags) {
  (void)engine; (void)flags;
  WASI_CRYPTO_STUB(ENGINE_set_default);
}

void *BN_GENCB_new(void) {
  WASI_CRYPTO_STUB(BN_GENCB_new);
}

void BN_GENCB_free(void *cb) {
  (void)cb;
  WASI_CRYPTO_STUB(BN_GENCB_free);
}

void BN_GENCB_set(void *cb, void *callback, void *arg) {
  (void)cb; (void)callback; (void)arg;
  WASI_CRYPTO_STUB(BN_GENCB_set);
}

void *BN_GENCB_get_arg(void *cb) {
  (void)cb;
  WASI_CRYPTO_STUB(BN_GENCB_get_arg);
}

int BN_generate_prime_ex(void *ret, int bits, int safe, const void *add,
                         const void *rem, void *cb) {
  (void)ret; (void)bits; (void)safe; (void)add; (void)rem; (void)cb;
  WASI_CRYPTO_STUB(BN_generate_prime_ex);
}

int BN_is_prime_ex(const void *candidate, int checks, void *ctx, void *cb) {
  (void)candidate; (void)checks; (void)ctx; (void)cb;
  WASI_CRYPTO_STUB(BN_is_prime_ex);
}

void *BN_get_rfc2409_prime_768(void *bn) {
  (void)bn;
  WASI_CRYPTO_STUB(BN_get_rfc2409_prime_768);
}

void *BN_get_rfc3526_prime_1536(void *bn) {
  (void)bn;
  WASI_CRYPTO_STUB(BN_get_rfc3526_prime_1536);
}

void *BN_get_rfc3526_prime_6144(void *bn) {
  (void)bn;
  WASI_CRYPTO_STUB(BN_get_rfc3526_prime_6144);
}

int EVP_PKEY_CTX_set_hkdf_md(void *ctx, const void *md) {
  (void)ctx; (void)md;
  WASI_CRYPTO_STUB(EVP_PKEY_CTX_set_hkdf_md);
}

int EVP_PKEY_CTX_set_hkdf_mode(void *ctx, int mode) {
  (void)ctx; (void)mode;
  WASI_CRYPTO_STUB(EVP_PKEY_CTX_set_hkdf_mode);
}

int EVP_PKEY_CTX_set1_hkdf_key(void *ctx, const unsigned char *key,
                               int key_len) {
  (void)ctx; (void)key; (void)key_len;
  WASI_CRYPTO_STUB(EVP_PKEY_CTX_set1_hkdf_key);
}

int EVP_PKEY_CTX_add1_hkdf_info(void *ctx, const unsigned char *info,
                                int info_len) {
  (void)ctx; (void)info; (void)info_len;
  WASI_CRYPTO_STUB(EVP_PKEY_CTX_add1_hkdf_info);
}

unsigned char *HMAC(const void *evp_md, const void *key, int key_len,
                    const unsigned char *data, size_t data_len,
                    unsigned char *md, unsigned int *md_len) {
  (void)evp_md; (void)key; (void)key_len; (void)data; (void)data_len;
  (void)md; (void)md_len;
  WASI_CRYPTO_STUB(HMAC);
}

int PKCS5_PBKDF2_HMAC(const char *pass, int pass_len,
                      const unsigned char *salt, int salt_len, int iterations,
                      const void *digest, int key_len, unsigned char *out) {
  (void)pass; (void)pass_len; (void)salt; (void)salt_len; (void)iterations;
  (void)digest; (void)key_len; (void)out;
  WASI_CRYPTO_STUB(PKCS5_PBKDF2_HMAC);
}

void *EVP_PKEY_new_raw_public_key(int type, void *engine,
                                  const unsigned char *key, size_t key_len) {
  (void)type; (void)engine; (void)key; (void)key_len;
  WASI_CRYPTO_STUB(EVP_PKEY_new_raw_public_key);
}

void *EVP_PKEY_new_raw_private_key(int type, void *engine,
                                   const unsigned char *key, size_t key_len) {
  (void)type; (void)engine; (void)key; (void)key_len;
  WASI_CRYPTO_STUB(EVP_PKEY_new_raw_private_key);
}

int EVP_PKEY_assign(void *pkey, int type, void *key) {
  (void)pkey; (void)type; (void)key;
  WASI_CRYPTO_STUB(EVP_PKEY_assign);
}

int EC_KEY_set_public_key_affine_coordinates(void *key, void *x, void *y) {
  (void)key; (void)x; (void)y;
  WASI_CRYPTO_STUB(EC_KEY_set_public_key_affine_coordinates);
}

int EC_curve_nist2nid(const char *name) {
  (void)name;
  WASI_CRYPTO_STUB(EC_curve_nist2nid);
}

#define WASI_PKEY_CTX_INT_STUB(name) \
  int name(void *ctx, int value) {   \
    (void)ctx; (void)value;           \
    WASI_CRYPTO_STUB(name);           \
  }

WASI_PKEY_CTX_INT_STUB(EVP_PKEY_CTX_set_dh_paramgen_prime_len)
WASI_PKEY_CTX_INT_STUB(EVP_PKEY_CTX_set_dh_paramgen_generator)
WASI_PKEY_CTX_INT_STUB(EVP_PKEY_CTX_set_dsa_paramgen_bits)
WASI_PKEY_CTX_INT_STUB(EVP_PKEY_CTX_set_dsa_paramgen_q_bits)
WASI_PKEY_CTX_INT_STUB(EVP_PKEY_CTX_set_ec_paramgen_curve_nid)
WASI_PKEY_CTX_INT_STUB(EVP_PKEY_CTX_set_ec_param_enc)
WASI_PKEY_CTX_INT_STUB(EVP_PKEY_CTX_set_rsa_keygen_bits)
WASI_PKEY_CTX_INT_STUB(EVP_PKEY_CTX_set_rsa_pss_keygen_saltlen)

int EVP_PKEY_CTX_set_rsa_keygen_pubexp(void *ctx, void *exponent) {
  (void)ctx; (void)exponent;
  WASI_CRYPTO_STUB(EVP_PKEY_CTX_set_rsa_keygen_pubexp);
}

int EVP_PKEY_CTX_set_rsa_pss_keygen_md(void *ctx, const void *md) {
  (void)ctx; (void)md;
  WASI_CRYPTO_STUB(EVP_PKEY_CTX_set_rsa_pss_keygen_md);
}

int EVP_PKEY_CTX_set_rsa_pss_keygen_mgf1_md(void *ctx, const void *md) {
  (void)ctx; (void)md;
  WASI_CRYPTO_STUB(EVP_PKEY_CTX_set_rsa_pss_keygen_mgf1_md);
}

int RSA_set0_key(void *rsa, void *n, void *e, void *d) {
  (void)rsa; (void)n; (void)e; (void)d;
  WASI_CRYPTO_STUB(RSA_set0_key);
}

int RSA_set0_factors(void *rsa, void *p, void *q) {
  (void)rsa; (void)p; (void)q;
  WASI_CRYPTO_STUB(RSA_set0_factors);
}

int RSA_set0_crt_params(void *rsa, void *dmp1, void *dmq1, void *iqmp) {
  (void)rsa; (void)dmp1; (void)dmq1; (void)iqmp;
  WASI_CRYPTO_STUB(RSA_set0_crt_params);
}

int EVP_DigestSignInit(void *ctx, void **pctx, const void *type,
                       void *engine, void *pkey) {
  (void)ctx; (void)pctx; (void)type; (void)engine; (void)pkey;
  WASI_CRYPTO_STUB(EVP_DigestSignInit);
}

int EVP_DigestVerifyInit(void *ctx, void **pctx, const void *type,
                         void *engine, void *pkey) {
  (void)ctx; (void)pctx; (void)type; (void)engine; (void)pkey;
  WASI_CRYPTO_STUB(EVP_DigestVerifyInit);
}
