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
