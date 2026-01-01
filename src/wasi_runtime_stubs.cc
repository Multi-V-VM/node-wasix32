// WASI C++ runtime stubs
// These stubs provide minimal implementations for C++ runtime features
// that are not available or not fully supported in WASI

#ifdef __wasi__

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>

// C++ exception handling stubs for WASI
extern "C" {

void* __cxa_allocate_exception(size_t size) {
  fprintf(stderr, "C++ exceptions not supported in WASI\n");
  abort();
}

[[noreturn]] void __cxa_throw(void* thrown_exception, void* tinfo, void (*dest)(void*)) {
  fprintf(stderr, "C++ exceptions not supported in WASI\n");
  abort();
}

void* __cxa_begin_catch(void* exception) {
  return nullptr;
}

void __cxa_end_catch() {}

[[noreturn]] void __cxa_rethrow() {
  fprintf(stderr, "C++ rethrow not supported in WASI\n");
  abort();
}

void* __cxa_current_exception_type() {
  return nullptr;
}

// OpenSSL stubs - use weak linkage in case OpenSSL is properly built
__attribute__((weak)) void EVP_MD_free(void* md) {}
__attribute__((weak)) void* OPENSSL_INIT_new(void) { return nullptr; }
__attribute__((weak)) void OPENSSL_INIT_free(void* settings) {}
__attribute__((weak)) int OPENSSL_INIT_set_config_filename(void* settings, const char* filename) { return 1; }
__attribute__((weak)) int OPENSSL_INIT_set_config_appname(void* settings, const char* appname) { return 1; }
__attribute__((weak)) void OPENSSL_INIT_set_config_file_flags(void* settings, unsigned long flags) {}
__attribute__((weak)) int OPENSSL_init_crypto(uint64_t opts, const void* settings) { return 1; }
__attribute__((weak)) unsigned long ERR_peek_error(void) { return 0; }
__attribute__((weak)) void ERR_print_errors_cb(int (*cb)(const char*, size_t, void*), void* u) {}
__attribute__((weak)) const char* OpenSSL_version(int type) { return "OpenSSL stub"; }
__attribute__((weak)) unsigned char* SHA1(const unsigned char* d, size_t n, unsigned char* md) { return md; }

// OpenSSL BIO stubs
__attribute__((weak)) void* BIO_get_data(void* bio) { return nullptr; }
__attribute__((weak)) void BIO_set_data(void* bio, void* ptr) {}
__attribute__((weak)) void BIO_set_init(void* bio, int init) {}
__attribute__((weak)) int BIO_get_init(void* bio) { return 0; }
__attribute__((weak)) void BIO_set_shutdown(void* bio, int shut) {}
__attribute__((weak)) int BIO_get_shutdown(void* bio) { return 0; }
__attribute__((weak)) void BIO_set_flags(void* bio, int flags) {}
__attribute__((weak)) void BIO_clear_flags(void* bio, int flags) {}
__attribute__((weak)) long BIO_ctrl(void* bio, int cmd, long larg, void* parg) { return 0; }
__attribute__((weak)) void* BIO_meth_new(int type, const char* name) { return nullptr; }
__attribute__((weak)) int BIO_meth_set_write(void* biom, int (*write)(void*, const char*, int)) { return 1; }
__attribute__((weak)) int BIO_meth_set_read(void* biom, int (*read)(void*, char*, int)) { return 1; }
__attribute__((weak)) int BIO_meth_set_puts(void* biom, int (*puts)(void*, const char*)) { return 1; }
__attribute__((weak)) int BIO_meth_set_gets(void* biom, int (*gets)(void*, char*, int)) { return 1; }
__attribute__((weak)) int BIO_meth_set_ctrl(void* biom, long (*ctrl)(void*, int, long, void*)) { return 1; }
__attribute__((weak)) int BIO_meth_set_create(void* biom, int (*create)(void*)) { return 1; }
__attribute__((weak)) int BIO_meth_set_destroy(void* biom, int (*destroy)(void*)) { return 1; }

// OpenSSL X509 stubs
__attribute__((weak)) void* X509_STORE_new(void) { return nullptr; }
__attribute__((weak)) int X509_STORE_set_default_paths(void* ctx) { return 0; }
__attribute__((weak)) int X509_STORE_add_cert(void* ctx, void* x) { return 0; }
__attribute__((weak)) void* PEM_read_bio_X509(void* bp, void** x, void* cb, void* u) { return nullptr; }

// OpenSSL ERR stubs
__attribute__((weak)) void ERR_error_string_n(unsigned long e, char* buf, size_t len) { if (buf && len > 0) buf[0] = '\0'; }
__attribute__((weak)) void ERR_clear_error(void) {}

// OpenSSL EC stubs
__attribute__((weak)) size_t EC_POINT_point2oct(void* group, void* p, int form, unsigned char* buf, size_t len, void* ctx) { return 0; }

// OpenSSL stack stubs
__attribute__((weak)) int OPENSSL_sk_num(void* st) { return 0; }
__attribute__((weak)) void* OPENSSL_sk_value(void* st, int i) { return nullptr; }
__attribute__((weak)) void* OPENSSL_sk_new(void* cmp) { return nullptr; }
__attribute__((weak)) int OPENSSL_sk_push(void* st, void* data) { return 0; }
__attribute__((weak)) void* OPENSSL_sk_pop(void* st) { return nullptr; }
__attribute__((weak)) void OPENSSL_sk_free(void* st) {}
__attribute__((weak)) void OPENSSL_sk_pop_free(void* st, void (*func)(void*)) {}

// More OpenSSL stubs for crypto subsystem
__attribute__((weak)) void* SSL_CTX_new(void* method) { return nullptr; }
__attribute__((weak)) void SSL_CTX_free(void* ctx) {}
__attribute__((weak)) void* SSL_new(void* ctx) { return nullptr; }
__attribute__((weak)) void SSL_free(void* ssl) {}
__attribute__((weak)) int SSL_read(void* ssl, void* buf, int num) { return -1; }
__attribute__((weak)) int SSL_write(void* ssl, const void* buf, int num) { return -1; }
__attribute__((weak)) int SSL_get_error(void* ssl, int ret) { return 0; }
__attribute__((weak)) void* TLS_method(void) { return nullptr; }
__attribute__((weak)) void* TLS_client_method(void) { return nullptr; }
__attribute__((weak)) void* TLS_server_method(void) { return nullptr; }

__attribute__((weak)) void* EVP_sha256(void) { return nullptr; }
__attribute__((weak)) void* EVP_sha512(void) { return nullptr; }
__attribute__((weak)) void* EVP_md5(void) { return nullptr; }
__attribute__((weak)) void* EVP_PKEY_new(void) { return nullptr; }
__attribute__((weak)) void EVP_PKEY_free(void* pkey) {}
__attribute__((weak)) void* EVP_MD_CTX_new(void) { return nullptr; }
__attribute__((weak)) void EVP_MD_CTX_free(void* ctx) {}
__attribute__((weak)) int EVP_DigestInit_ex(void* ctx, void* type, void* impl) { return 0; }
__attribute__((weak)) int EVP_DigestUpdate(void* ctx, const void* d, size_t cnt) { return 0; }
__attribute__((weak)) int EVP_DigestFinal_ex(void* ctx, unsigned char* md, unsigned int* s) { return 0; }

__attribute__((weak)) int RAND_bytes(unsigned char* buf, int num) {
  // Fill with pseudo-random bytes using a simple PRNG for WASI
  // This is NOT cryptographically secure, just for basic functionality
  static unsigned int seed = 12345;
  for (int i = 0; i < num; i++) {
    seed = seed * 1103515245 + 12345;
    buf[i] = (unsigned char)(seed >> 16);
  }
  return 1;  // Success
}
__attribute__((weak)) void RAND_seed(const void* buf, int num) {}

__attribute__((weak)) void OPENSSL_cleanse(void* ptr, size_t len) { if (ptr) memset(ptr, 0, len); }
__attribute__((weak)) void* OPENSSL_malloc(size_t num) { return malloc(num); }
__attribute__((weak)) void OPENSSL_free(void* addr) { free(addr); }

__attribute__((weak)) unsigned long ERR_get_error(void) { return 0; }
__attribute__((weak)) const char* ERR_reason_error_string(unsigned long e) { return ""; }
__attribute__((weak)) const char* ERR_lib_error_string(unsigned long e) { return ""; }
__attribute__((weak)) const char* ERR_func_error_string(unsigned long e) { return ""; }

__attribute__((weak)) void* X509_new(void) { return nullptr; }
__attribute__((weak)) void X509_free(void* x) {}
__attribute__((weak)) void* X509_get_subject_name(void* x) { return nullptr; }
__attribute__((weak)) void* X509_get_issuer_name(void* x) { return nullptr; }
__attribute__((weak)) int X509_NAME_print_ex(void* out, void* nm, int indent, unsigned long flags) { return 0; }

__attribute__((weak)) void* BIO_new(void* type) { return nullptr; }
__attribute__((weak)) void* BIO_new_mem_buf(const void* buf, int len) { return nullptr; }
__attribute__((weak)) int BIO_free(void* a) { return 1; }
__attribute__((weak)) int BIO_read(void* b, void* data, int dlen) { return -1; }
__attribute__((weak)) int BIO_write(void* b, const void* data, int dlen) { return -1; }
__attribute__((weak)) void* BIO_s_mem(void) { return nullptr; }

__attribute__((weak)) void* EC_KEY_new(void) { return nullptr; }
__attribute__((weak)) void EC_KEY_free(void* key) {}
__attribute__((weak)) void* EC_KEY_get0_group(void* key) { return nullptr; }
__attribute__((weak)) void* EC_KEY_get0_public_key(void* key) { return nullptr; }

__attribute__((weak)) void* RSA_new(void) { return nullptr; }
__attribute__((weak)) void RSA_free(void* rsa) {}

__attribute__((weak)) void* DSA_new(void) { return nullptr; }
__attribute__((weak)) void DSA_free(void* dsa) {}

__attribute__((weak)) void* DH_new(void) { return nullptr; }
__attribute__((weak)) void DH_free(void* dh) {}

__attribute__((weak)) int CRYPTO_memcmp(const void* a, const void* b, size_t len) { return memcmp(a, b, len); }
__attribute__((weak)) void* CRYPTO_malloc(size_t num, const char* file, int line) { return malloc(num); }
__attribute__((weak)) void CRYPTO_free(void* ptr, const char* file, int line) { free(ptr); }

// zlib stubs
__attribute__((weak)) int deflateParams(void* strm, int level, int strategy) { return 0; }
__attribute__((weak)) int deflateEnd(void* strm) { return 0; }
__attribute__((weak)) int deflateInit2_(void* strm, int level, int method, int windowBits, int memLevel, int strategy, const char* version, int stream_size) { return 0; }
__attribute__((weak)) int deflateReset(void* strm) { return 0; }
__attribute__((weak)) int deflateSetDictionary(void* strm, const unsigned char* dictionary, unsigned int dictLength) { return 0; }
__attribute__((weak)) int inflateParams(void* strm, int level, int strategy) { return 0; }
__attribute__((weak)) int inflateEnd(void* strm) { return 0; }
__attribute__((weak)) int inflateInit2_(void* strm, int windowBits, const char* version, int stream_size) { return 0; }
__attribute__((weak)) int inflateReset(void* strm) { return 0; }
__attribute__((weak)) int inflateSetDictionary(void* strm, const unsigned char* dictionary, unsigned int dictLength) { return 0; }
__attribute__((weak)) int deflate(void* strm, int flush) { return 0; }
__attribute__((weak)) int inflate(void* strm, int flush) { return 0; }

// Additional OpenSSL X509 stubs
__attribute__((weak)) void X509_STORE_free(void* v) {}
__attribute__((weak)) void* PEM_read_bio_X509_AUX(void* bp, void** x, void* cb, void* u) { return nullptr; }
__attribute__((weak)) void* OPENSSL_sk_new_null(void) { return nullptr; }
__attribute__((weak)) unsigned long ERR_peek_last_error(void) { return 0; }
__attribute__((weak)) void* X509_dup(void* x509) { return nullptr; }
__attribute__((weak)) const char* X509_get_default_cert_file_env(void) { return "SSL_CERT_FILE"; }
__attribute__((weak)) const char* X509_get_default_cert_file(void) { return "/etc/ssl/certs/ca-certificates.crt"; }
__attribute__((weak)) const char* X509_get_default_cert_dir_env(void) { return "SSL_CERT_DIR"; }
__attribute__((weak)) const char* X509_get_default_cert_dir(void) { return "/etc/ssl/certs"; }
__attribute__((weak)) void* PEM_read_bio_PrivateKey(void* bp, void** x, void* cb, void* u) { return nullptr; }
__attribute__((weak)) int X509_STORE_set_flags(void* ctx, unsigned long flags) { return 0; }
__attribute__((weak)) void* PEM_read_bio_DHparams(void* bp, void** x, void* cb, void* u) { return nullptr; }
__attribute__((weak)) void DH_get0_pqg(void* dh, const void** p, const void** q, const void** g) {}
__attribute__((weak)) void* d2i_PKCS12_bio(void* bp, void** p12) { return nullptr; }
__attribute__((weak)) int PKCS12_parse(void* p12, const char* pass, void** pkey, void** cert, void** ca) { return 0; }
__attribute__((weak)) void PKCS12_free(void* p12) {}
__attribute__((weak)) int i2d_X509(void* x, unsigned char** out) { return 0; }
__attribute__((weak)) int EVP_EncryptInit_ex(void* ctx, void* type, void* impl, const unsigned char* key, const unsigned char* iv) { return 0; }
__attribute__((weak)) int EVP_DecryptInit_ex(void* ctx, void* type, void* impl, const unsigned char* key, const unsigned char* iv) { return 0; }
__attribute__((weak)) int EVP_EncryptUpdate(void* ctx, unsigned char* out, int* outl, const unsigned char* in, int inl) { return 0; }
__attribute__((weak)) int EVP_DecryptUpdate(void* ctx, unsigned char* out, int* outl, const unsigned char* in, int inl) { return 0; }
__attribute__((weak)) int EVP_EncryptFinal_ex(void* ctx, unsigned char* out, int* outl) { return 0; }
__attribute__((weak)) int EVP_DecryptFinal_ex(void* ctx, unsigned char* out, int* outl) { return 0; }
__attribute__((weak)) void* EVP_CIPHER_CTX_new(void) { return nullptr; }
__attribute__((weak)) void EVP_CIPHER_CTX_free(void* ctx) {}
__attribute__((weak)) int EVP_CIPHER_CTX_set_padding(void* ctx, int pad) { return 0; }
__attribute__((weak)) void* EVP_aes_128_gcm(void) { return nullptr; }
__attribute__((weak)) void* EVP_aes_256_gcm(void) { return nullptr; }
__attribute__((weak)) void* EVP_aes_128_cbc(void) { return nullptr; }
__attribute__((weak)) void* EVP_aes_256_cbc(void) { return nullptr; }

// More EVP stubs
__attribute__((weak)) int EVP_PKEY_CTX_ctrl(void* ctx, int keytype, int optype, int cmd, int p1, void* p2) { return 0; }
__attribute__((weak)) void* EVP_PKEY_CTX_new(void* pkey, void* e) { return nullptr; }
__attribute__((weak)) void* EVP_PKEY_CTX_new_id(int id, void* e) { return nullptr; }
__attribute__((weak)) void EVP_PKEY_CTX_free(void* ctx) {}
__attribute__((weak)) int EVP_PKEY_keygen_init(void* ctx) { return 0; }
__attribute__((weak)) int EVP_PKEY_keygen(void* ctx, void** ppkey) { return 0; }
__attribute__((weak)) int EVP_PKEY_derive_init(void* ctx) { return 0; }
__attribute__((weak)) int EVP_PKEY_derive_set_peer(void* ctx, void* peer) { return 0; }
__attribute__((weak)) int EVP_PKEY_derive(void* ctx, unsigned char* key, size_t* keylen) { return 0; }
__attribute__((weak)) int EVP_PKEY_sign_init(void* ctx) { return 0; }
__attribute__((weak)) int EVP_PKEY_sign(void* ctx, unsigned char* sig, size_t* siglen, const unsigned char* tbs, size_t tbslen) { return 0; }
__attribute__((weak)) int EVP_PKEY_verify_init(void* ctx) { return 0; }
__attribute__((weak)) int EVP_PKEY_verify(void* ctx, const unsigned char* sig, size_t siglen, const unsigned char* tbs, size_t tbslen) { return 0; }

// BIGNUM stubs
__attribute__((weak)) void* BN_new(void) { return nullptr; }
__attribute__((weak)) void BN_free(void* bn) {}
__attribute__((weak)) void BN_clear_free(void* bn) {}
__attribute__((weak)) int BN_set_word(void* bn, unsigned long w) { return 0; }
__attribute__((weak)) void* BN_bin2bn(const unsigned char* s, int len, void* ret) { return nullptr; }
__attribute__((weak)) int BN_bn2bin(void* bn, unsigned char* to) { return 0; }
__attribute__((weak)) int BN_num_bits(void* bn) { return 0; }
__attribute__((weak)) char* BN_bn2hex(void* bn) { return nullptr; }
__attribute__((weak)) char* BN_bn2dec(void* bn) { return nullptr; }

// More X509 stubs
__attribute__((weak)) void* X509_get_pubkey(void* x) { return nullptr; }
__attribute__((weak)) int X509_verify(void* x, void* pkey) { return 0; }
__attribute__((weak)) void* X509_get_serialNumber(void* x) { return nullptr; }
__attribute__((weak)) void* X509_get0_notBefore(void* x) { return nullptr; }
__attribute__((weak)) void* X509_get0_notAfter(void* x) { return nullptr; }
__attribute__((weak)) int X509_cmp(void* a, void* b) { return 0; }
__attribute__((weak)) int X509_check_ca(void* x) { return 0; }
__attribute__((weak)) void* X509_get_ext_d2i(void* x, int nid, int* crit, int* idx) { return nullptr; }
__attribute__((weak)) int X509_add_ext(void* x, void* ex, int loc) { return 0; }
__attribute__((weak)) void* X509_EXTENSION_dup(void* ex) { return nullptr; }
__attribute__((weak)) void X509_EXTENSION_free(void* ex) {}
__attribute__((weak)) int X509_EXTENSION_get_critical(void* ex) { return 0; }
__attribute__((weak)) void* X509_EXTENSION_get_object(void* ex) { return nullptr; }
__attribute__((weak)) void* X509_EXTENSION_get_data(void* ex) { return nullptr; }

// ASN1 stubs
__attribute__((weak)) void* ASN1_INTEGER_to_BN(void* ai, void* bn) { return nullptr; }
__attribute__((weak)) void* BN_to_ASN1_INTEGER(void* bn, void* ai) { return nullptr; }
__attribute__((weak)) void ASN1_INTEGER_free(void* a) {}
__attribute__((weak)) long ASN1_INTEGER_get(void* a) { return 0; }
__attribute__((weak)) int ASN1_STRING_length(void* x) { return 0; }
__attribute__((weak)) const unsigned char* ASN1_STRING_get0_data(void* x) { return nullptr; }
__attribute__((weak)) int ASN1_STRING_type(void* x) { return 0; }
__attribute__((weak)) void ASN1_STRING_free(void* a) {}
__attribute__((weak)) int ASN1_TIME_print(void* bp, void* tm) { return 0; }
__attribute__((weak)) void* ASN1_TIME_new(void) { return nullptr; }
__attribute__((weak)) void ASN1_TIME_free(void* a) {}

// OBJ stubs
__attribute__((weak)) int OBJ_obj2nid(void* o) { return 0; }
__attribute__((weak)) const char* OBJ_nid2sn(int n) { return ""; }
__attribute__((weak)) const char* OBJ_nid2ln(int n) { return ""; }
__attribute__((weak)) void* OBJ_nid2obj(int n) { return nullptr; }
__attribute__((weak)) int OBJ_txt2nid(const char* s) { return 0; }
__attribute__((weak)) int OBJ_obj2txt(char* buf, int buf_len, void* a, int no_name) { return 0; }

// HMAC stubs
__attribute__((weak)) void* HMAC_CTX_new(void) { return nullptr; }
__attribute__((weak)) void HMAC_CTX_free(void* ctx) {}
__attribute__((weak)) int HMAC_Init_ex(void* ctx, const void* key, int len, void* md, void* impl) { return 0; }
__attribute__((weak)) int HMAC_Update(void* ctx, const unsigned char* data, size_t len) { return 0; }
__attribute__((weak)) int HMAC_Final(void* ctx, unsigned char* md, unsigned int* len) { return 0; }

// SSL/TLS additional stubs
__attribute__((weak)) int SSL_set_fd(void* s, int fd) { return 0; }
__attribute__((weak)) int SSL_connect(void* ssl) { return -1; }
__attribute__((weak)) int SSL_accept(void* ssl) { return -1; }
__attribute__((weak)) int SSL_shutdown(void* ssl) { return 0; }
__attribute__((weak)) void* SSL_get_peer_certificate(void* ssl) { return nullptr; }
__attribute__((weak)) long SSL_get_verify_result(void* ssl) { return 0; }
__attribute__((weak)) void SSL_set_verify(void* s, int mode, void* callback) {}
__attribute__((weak)) int SSL_CTX_use_certificate(void* ctx, void* x) { return 0; }
__attribute__((weak)) int SSL_CTX_use_PrivateKey(void* ctx, void* pkey) { return 0; }
__attribute__((weak)) int SSL_CTX_check_private_key(void* ctx) { return 0; }
__attribute__((weak)) int SSL_CTX_set_cipher_list(void* ctx, const char* str) { return 0; }
__attribute__((weak)) void SSL_CTX_set_verify(void* ctx, int mode, void* cb) {}
__attribute__((weak)) int SSL_CTX_load_verify_locations(void* ctx, const char* file, const char* path) { return 0; }
__attribute__((weak)) void* SSL_CTX_get_cert_store(void* ctx) { return nullptr; }
__attribute__((weak)) void SSL_CTX_set_cert_store(void* ctx, void* store) {}
__attribute__((weak)) long SSL_CTX_ctrl(void* ctx, int cmd, long larg, void* parg) { return 0; }
__attribute__((weak)) long SSL_ctrl(void* ssl, int cmd, long larg, void* parg) { return 0; }

// PEM stubs
__attribute__((weak)) void* PEM_read_bio_RSAPrivateKey(void* bp, void** x, void* cb, void* u) { return nullptr; }
__attribute__((weak)) void* PEM_read_bio_RSA_PUBKEY(void* bp, void** x, void* cb, void* u) { return nullptr; }
__attribute__((weak)) int PEM_write_bio_X509(void* bp, void* x) { return 0; }
__attribute__((weak)) int PEM_write_bio_PrivateKey(void* bp, void* x, void* enc, unsigned char* kstr, int klen, void* cb, void* u) { return 0; }
__attribute__((weak)) int PEM_write_bio_PUBKEY(void* bp, void* x) { return 0; }

// RSA stubs
__attribute__((weak)) int RSA_size(void* rsa) { return 0; }
__attribute__((weak)) int RSA_public_encrypt(int flen, const unsigned char* from, unsigned char* to, void* rsa, int padding) { return -1; }
__attribute__((weak)) int RSA_private_decrypt(int flen, const unsigned char* from, unsigned char* to, void* rsa, int padding) { return -1; }
__attribute__((weak)) int RSA_sign(int type, const unsigned char* m, unsigned int m_len, unsigned char* sigret, unsigned int* siglen, void* rsa) { return 0; }
__attribute__((weak)) int RSA_verify(int type, const unsigned char* m, unsigned int m_len, const unsigned char* sigbuf, unsigned int siglen, void* rsa) { return 0; }
__attribute__((weak)) void RSA_get0_key(void* r, const void** n, const void** e, const void** d) {}

// EC stubs
__attribute__((weak)) void* EC_KEY_new_by_curve_name(int nid) { return nullptr; }
__attribute__((weak)) int EC_KEY_generate_key(void* key) { return 0; }
__attribute__((weak)) int EC_KEY_set_private_key(void* key, void* priv_key) { return 0; }
__attribute__((weak)) int EC_KEY_set_public_key(void* key, void* pub) { return 0; }
__attribute__((weak)) const void* EC_KEY_get0_private_key(void* key) { return nullptr; }
__attribute__((weak)) void* EC_GROUP_new_by_curve_name(int nid) { return nullptr; }
__attribute__((weak)) void EC_GROUP_free(void* group) {}
__attribute__((weak)) int EC_GROUP_get_curve_name(void* group) { return 0; }
__attribute__((weak)) void* EC_POINT_new(void* group) { return nullptr; }
__attribute__((weak)) void EC_POINT_free(void* point) {}
__attribute__((weak)) int EC_POINT_mul(void* group, void* r, void* n, void* q, void* m, void* ctx) { return 0; }
__attribute__((weak)) int EC_GROUP_get_degree(void* group) { return 0; }
__attribute__((weak)) int EC_GROUP_get_order(void* group, void* order, void* ctx) { return 0; }
__attribute__((weak)) int EC_POINT_get_affine_coordinates(void* group, void* p, void* x, void* y, void* ctx) { return 0; }
__attribute__((weak)) int ECDH_compute_key(void* out, size_t outlen, void* pub_key, void* ecdh, void* kdf) { return 0; }

// CRL stubs
__attribute__((weak)) void* PEM_read_bio_X509_CRL(void* bp, void** x, void* cb, void* u) { return nullptr; }
__attribute__((weak)) int X509_STORE_add_crl(void* ctx, void* x) { return 0; }
__attribute__((weak)) void X509_CRL_free(void* crl) {}
__attribute__((weak)) int X509_STORE_up_ref(void* v) { return 1; }

// OpenSSL stack additional stubs
__attribute__((weak)) void OPENSSL_sk_zero(void* st) {}

// CRYPTO stubs
__attribute__((weak)) void CRYPTO_clear_free(void* ptr, size_t len, const char* file, int line) { if (ptr) free(ptr); }

// ERR additional stubs
__attribute__((weak)) void ERR_new(void) {}
__attribute__((weak)) void ERR_set_debug(const char* file, int line, const char* func) {}
__attribute__((weak)) void ERR_set_error(int lib, int reason, const char* fmt, ...) {}

// OBJ additional stubs
__attribute__((weak)) int OBJ_sn2nid(const char* s) { return 0; }

// DH additional stubs
__attribute__((weak)) int DH_size(void* dh) { return 0; }
__attribute__((weak)) int DH_compute_key(unsigned char* key, void* pub_key, void* dh) { return 0; }
__attribute__((weak)) void* DH_get0_priv_key(void* dh) { return nullptr; }
__attribute__((weak)) void* DH_get0_pub_key(void* dh) { return nullptr; }
__attribute__((weak)) int DH_generate_parameters_ex(void* dh, int prime_len, int generator, void* cb) { return 0; }
__attribute__((weak)) int DH_generate_key(void* dh) { return 0; }
__attribute__((weak)) int DH_set0_pqg(void* dh, void* p, void* q, void* g) { return 0; }
__attribute__((weak)) int DH_set0_key(void* dh, void* pub_key, void* priv_key) { return 0; }

// BN additional stubs
__attribute__((weak)) void* BN_CTX_new(void) { return nullptr; }
__attribute__((weak)) void BN_CTX_free(void* c) {}
__attribute__((weak)) void* BN_dup(void* from) { return nullptr; }
__attribute__((weak)) int BN_cmp(void* a, void* b) { return 0; }
__attribute__((weak)) int BN_is_zero(void* a) { return 0; }
__attribute__((weak)) int BN_is_one(void* a) { return 0; }
__attribute__((weak)) int BN_is_negative(void* a) { return 0; }
__attribute__((weak)) int BN_num_bytes(void* a) { return 0; }
__attribute__((weak)) void* BN_copy(void* to, void* from) { return nullptr; }
__attribute__((weak)) int BN_add(void* r, void* a, void* b) { return 0; }
__attribute__((weak)) int BN_sub(void* r, void* a, void* b) { return 0; }
__attribute__((weak)) int BN_mul(void* r, void* a, void* b, void* ctx) { return 0; }
__attribute__((weak)) int BN_div(void* dv, void* rem, void* a, void* d, void* ctx) { return 0; }
__attribute__((weak)) int BN_mod(void* rem, void* a, void* m, void* ctx) { return 0; }

// EVP additional stubs
__attribute__((weak)) int EVP_PKEY_eq(void* a, void* b) { return 0; }
__attribute__((weak)) void* EVP_PKEY2PKCS8(void* pkey) { return nullptr; }
__attribute__((weak)) int i2d_PKCS8_PRIV_KEY_INFO_bio(void* bp, void* p8) { return 0; }

// OpenSSL sk additional stubs
__attribute__((weak)) void* OPENSSL_sk_delete(void* st, int loc) { return nullptr; }

// EVP_MD stubs
__attribute__((weak)) const char* EVP_MD_get0_name(void* md) { return ""; }
__attribute__((weak)) void* EVP_MD_fetch(void* ctx, const char* algorithm, const char* properties) { return nullptr; }
__attribute__((weak)) int EVP_MD_names_do_all(void* md, void (*fn)(const char*, void*), void* data) { return 0; }
__attribute__((weak)) void EVP_MD_do_all_sorted(void (*fn)(const void*, const char*, const char*, void*), void* arg) {}

// ENGINE stubs
__attribute__((weak)) int ENGINE_finish(void* e) { return 1; }
__attribute__((weak)) int ENGINE_free(void* e) { return 1; }
__attribute__((weak)) void* ENGINE_by_id(const char* id) { return nullptr; }
__attribute__((weak)) int ENGINE_ctrl_cmd_string(void* e, const char* cmd_name, const char* arg, int cmd_optional) { return 0; }
__attribute__((weak)) int ENGINE_init(void* e) { return 0; }
__attribute__((weak)) void* ENGINE_load_private_key(void* e, const char* key_id, void* ui_method, void* callback_data) { return nullptr; }
__attribute__((weak)) void ENGINE_load_builtin_engines(void) {}
__attribute__((weak)) void* ENGINE_get_first(void) { return nullptr; }
__attribute__((weak)) void* ENGINE_get_next(void* e) { return nullptr; }
__attribute__((weak)) const char* ENGINE_get_id(void* e) { return ""; }
__attribute__((weak)) const char* ENGINE_get_name(void* e) { return ""; }
__attribute__((weak)) int ENGINE_register_all_complete(void) { return 1; }

// ERR mark stubs
__attribute__((weak)) int ERR_set_mark(void) { return 0; }
__attribute__((weak)) int ERR_pop_to_mark(void) { return 0; }

// CRYPTO memory stubs
__attribute__((weak)) void* CRYPTO_zalloc(size_t num, const char* file, int line) { return calloc(1, num); }
__attribute__((weak)) void* CRYPTO_secure_zalloc(size_t num, const char* file, int line) { return calloc(1, num); }
__attribute__((weak)) int CRYPTO_secure_malloc_initialized(void) { return 0; }
__attribute__((weak)) size_t CRYPTO_secure_used(void) { return 0; }
__attribute__((weak)) int CRYPTO_secure_malloc_init(size_t sz, size_t minsize) { return 0; }
__attribute__((weak)) void CRYPTO_secure_clear_free(void* ptr, size_t len, const char* file, int line) { if (ptr) { memset(ptr, 0, len); free(ptr); } }
__attribute__((weak)) void* CRYPTO_realloc(void* addr, size_t num, const char* file, int line) { return realloc(addr, num); }

// EVP FIPS stubs
__attribute__((weak)) int EVP_default_properties_is_fips_enabled(void* ctx) { return 0; }
__attribute__((weak)) int EVP_default_properties_enable_fips(void* ctx, int enable) { return 0; }

// BN secure stubs
__attribute__((weak)) void* BN_secure_new(void) { return nullptr; }
__attribute__((weak)) int BN_bn2binpad(void* a, unsigned char* to, int tolen) { return 0; }
__attribute__((weak)) unsigned long BN_get_word(void* a) { return 0; }
__attribute__((weak)) const void* BN_value_one(void) { return nullptr; }
__attribute__((weak)) int BN_lshift(void* r, void* a, int n) { return 0; }

// RAND stubs
__attribute__((weak)) int RAND_status(void) { return 1; }
__attribute__((weak)) int RAND_bytes_ex(void* ctx, unsigned char* buf, size_t num, unsigned int strength) {
  // Fill with pseudo-random bytes for WASI
  static unsigned int seed = 67890;
  for (size_t i = 0; i < num; i++) {
    seed = seed * 1103515245 + 12345;
    buf[i] = (unsigned char)(seed >> 16);
  }
  return 1;  // Success
}
__attribute__((weak)) int RAND_poll(void) { return 1; }

// NETSCAPE_SPKI stubs
__attribute__((weak)) void* NETSCAPE_SPKI_b64_decode(const char* str, int len) { return nullptr; }
__attribute__((weak)) void* NETSCAPE_SPKI_get_pubkey(void* spki) { return nullptr; }
__attribute__((weak)) int NETSCAPE_SPKI_verify(void* spki, void* pkey) { return 0; }
__attribute__((weak)) void NETSCAPE_SPKI_free(void* spki) {}
__attribute__((weak)) char* NETSCAPE_SPKI_b64_encode(void* spki) { return nullptr; }
__attribute__((weak)) void* NETSCAPE_SPKI_new(void) { return nullptr; }
__attribute__((weak)) int NETSCAPE_SPKI_set_pubkey(void* spki, void* pkey) { return 0; }
__attribute__((weak)) int NETSCAPE_SPKI_sign(void* spki, void* pkey, void* md) { return 0; }

// X509_PUBKEY stubs
__attribute__((weak)) void* X509_PUBKEY_get(void* key) { return nullptr; }
__attribute__((weak)) void* X509_PUBKEY_get0(void* key) { return nullptr; }

// BIO additional stubs
__attribute__((weak)) void BIO_free_all(void* a) {}
__attribute__((weak)) int i2d_X509_bio(void* bp, void* x509) { return 0; }
__attribute__((weak)) void* d2i_X509_bio(void* bp, void** x509) { return nullptr; }

// OSSL_PARAM stubs - moved to correct location with proper signatures below

// EVP_MAC stubs
__attribute__((weak)) void* EVP_MAC_fetch(void* ctx, const char* algorithm, const char* properties) { return nullptr; }
__attribute__((weak)) void EVP_MAC_free(void* mac) {}
__attribute__((weak)) void* EVP_MAC_CTX_new(void* mac) { return nullptr; }
__attribute__((weak)) void EVP_MAC_CTX_free(void* ctx) {}
__attribute__((weak)) int EVP_MAC_init(void* ctx, const unsigned char* key, size_t keylen, void* params) { return 0; }
__attribute__((weak)) int EVP_MAC_update(void* ctx, const unsigned char* data, size_t datalen) { return 0; }
__attribute__((weak)) int EVP_MAC_final(void* ctx, unsigned char* out, size_t* outl, size_t outsize) { return 0; }

// EVP_KDF stubs
__attribute__((weak)) void* EVP_KDF_fetch(void* ctx, const char* algorithm, const char* properties) { return nullptr; }
__attribute__((weak)) void EVP_KDF_free(void* kdf) {}
__attribute__((weak)) void* EVP_KDF_CTX_new(void* kdf) { return nullptr; }
__attribute__((weak)) void EVP_KDF_CTX_free(void* ctx) {}
__attribute__((weak)) int EVP_KDF_derive(void* ctx, unsigned char* key, size_t keylen, void* params) { return 0; }

// OCSP stubs
__attribute__((weak)) void* OCSP_RESPONSE_new(void) { return nullptr; }
__attribute__((weak)) void OCSP_RESPONSE_free(void* r) {}
__attribute__((weak)) void* d2i_OCSP_RESPONSE(void** a, const unsigned char** in, long len) { return nullptr; }
__attribute__((weak)) int i2d_OCSP_RESPONSE(void* a, unsigned char** out) { return 0; }

// More X509 stubs
__attribute__((weak)) int X509_get_version(void* x) { return 0; }
__attribute__((weak)) int X509_set_version(void* x, long version) { return 0; }
__attribute__((weak)) int X509_sign(void* x, void* pkey, void* md) { return 0; }
__attribute__((weak)) int X509_set_pubkey(void* x, void* pkey) { return 0; }
__attribute__((weak)) int X509_set_subject_name(void* x, void* name) { return 0; }
__attribute__((weak)) int X509_set_issuer_name(void* x, void* name) { return 0; }
__attribute__((weak)) int X509_set_serialNumber(void* x, void* serial) { return 0; }
__attribute__((weak)) int X509_set1_notBefore(void* x, void* tm) { return 0; }
__attribute__((weak)) int X509_set1_notAfter(void* x, void* tm) { return 0; }
__attribute__((weak)) int X509_print_ex(void* bp, void* x, unsigned long nmflag, unsigned long cflag) { return 0; }

// More SSL stubs
__attribute__((weak)) void* SSL_get_current_cipher(void* ssl) { return nullptr; }
__attribute__((weak)) const char* SSL_CIPHER_get_name(void* cipher) { return ""; }
__attribute__((weak)) int SSL_CIPHER_get_bits(void* cipher, int* alg_bits) { return 0; }
__attribute__((weak)) const char* SSL_CIPHER_get_version(void* cipher) { return ""; }
__attribute__((weak)) int SSL_pending(void* ssl) { return 0; }
__attribute__((weak)) int SSL_has_pending(void* ssl) { return 0; }
__attribute__((weak)) const char* SSL_get_version(void* ssl) { return ""; }
__attribute__((weak)) void* SSL_get_session(void* ssl) { return nullptr; }
__attribute__((weak)) void* SSL_SESSION_get0_peer(void* ses) { return nullptr; }

// EVP_PKEY_fromdata stubs
__attribute__((weak)) int EVP_PKEY_fromdata_init(void* ctx) { return 0; }
__attribute__((weak)) int EVP_PKEY_fromdata(void* ctx, void** ppkey, int selection, void* params) { return 0; }

// EVP_PKEY_get_bn_param stub
__attribute__((weak)) int EVP_PKEY_get_bn_param(void* pkey, const char* key_name, void** bn) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_octet_string_param(void* pkey, const char* key_name, unsigned char* buf, size_t max_buf_sz, size_t* out_sz) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_utf8_string_param(void* pkey, const char* key_name, char* str, size_t max_buf_sz, size_t* out_sz) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_int_param(void* pkey, const char* key_name, int* out) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_size_t_param(void* pkey, const char* key_name, size_t* out) { return 0; }

// PKCS8 stubs
__attribute__((weak)) void PKCS8_PRIV_KEY_INFO_free(void* p8) {}
__attribute__((weak)) void* PKCS8_PRIV_KEY_INFO_new(void) { return nullptr; }

// More EVP cipher stubs
__attribute__((weak)) int EVP_CIPHER_CTX_ctrl(void* ctx, int type, int arg, void* ptr) { return 0; }
__attribute__((weak)) int EVP_CIPHER_get_iv_length(void* cipher) { return 0; }
__attribute__((weak)) int EVP_CIPHER_get_key_length(void* cipher) { return 0; }
__attribute__((weak)) int EVP_CIPHER_get_block_size(void* cipher) { return 0; }
__attribute__((weak)) int EVP_CIPHER_get_nid(void* cipher) { return 0; }
__attribute__((weak)) void* EVP_CIPHER_fetch(void* ctx, const char* algorithm, const char* properties) { return nullptr; }
__attribute__((weak)) void EVP_CIPHER_free(void* cipher) {}
__attribute__((weak)) int EVP_CIPHER_names_do_all(void* cipher, void (*fn)(const char*, void*), void* data) { return 0; }
__attribute__((weak)) void EVP_CIPHER_do_all_sorted(void (*fn)(const void*, const char*, const char*, void*), void* arg) {}

// OSSL_PROVIDER stubs
__attribute__((weak)) void* OSSL_PROVIDER_load(void* ctx, const char* name) { return nullptr; }
__attribute__((weak)) int OSSL_PROVIDER_unload(void* prov) { return 1; }

// BN_MONT_CTX stubs
__attribute__((weak)) void* BN_MONT_CTX_new(void) { return nullptr; }
__attribute__((weak)) void BN_MONT_CTX_free(void* mont) {}
__attribute__((weak)) int BN_MONT_CTX_set(void* mont, void* mod, void* ctx) { return 0; }
__attribute__((weak)) int BN_mod_exp_mont(void* r, void* a, void* p, void* m, void* ctx, void* m_ctx) { return 0; }
__attribute__((weak)) int BN_mod_exp(void* r, void* a, void* p, void* m, void* ctx) { return 0; }

// DSA stubs
__attribute__((weak)) int DSA_size(void* dsa) { return 0; }
__attribute__((weak)) int DSA_sign(int type, const unsigned char* dgst, int dlen, unsigned char* sig, unsigned int* siglen, void* dsa) { return 0; }
__attribute__((weak)) int DSA_verify(int type, const unsigned char* dgst, int dlen, const unsigned char* sig, int siglen, void* dsa) { return 0; }
__attribute__((weak)) void DSA_get0_pqg(void* d, const void** p, const void** q, const void** g) {}
__attribute__((weak)) void DSA_get0_key(void* d, const void** pub_key, const void** priv_key) {}

// BIO additional stubs
__attribute__((weak)) int BIO_puts(void* bp, const char* buf) { return 0; }
__attribute__((weak)) int BIO_gets(void* bp, char* buf, int size) { return 0; }
__attribute__((weak)) long BIO_get_mem_data(void* bp, char** pp) { return 0; }
__attribute__((weak)) void* BIO_push(void* b, void* append) { return nullptr; }
__attribute__((weak)) void* BIO_pop(void* b) { return nullptr; }
__attribute__((weak)) void* BIO_find_type(void* b, int type) { return nullptr; }
__attribute__((weak)) void* BIO_next(void* b) { return nullptr; }
__attribute__((weak)) int BIO_pending(void* b) { return 0; }
__attribute__((weak)) int BIO_wpending(void* b) { return 0; }
__attribute__((weak)) int BIO_flush(void* b) { return 1; }

// X509_NAME stubs
__attribute__((weak)) int X509_NAME_get_index_by_NID(void* name, int nid, int lastpos) { return -1; }
__attribute__((weak)) void* X509_NAME_get_entry(void* name, int loc) { return nullptr; }
__attribute__((weak)) void* X509_NAME_ENTRY_get_data(void* ne) { return nullptr; }
__attribute__((weak)) void* X509_NAME_ENTRY_get_object(void* ne) { return nullptr; }
__attribute__((weak)) int X509_NAME_entry_count(void* name) { return 0; }
__attribute__((weak)) void* X509_NAME_dup(void* xn) { return nullptr; }
__attribute__((weak)) void X509_NAME_free(void* name) {}

// GENERAL_NAME stubs (for SAN)
__attribute__((weak)) void GENERAL_NAME_free(void* a) {}
__attribute__((weak)) void GENERAL_NAMES_free(void* a) {}

// X509V3 stubs
__attribute__((weak)) void* X509V3_EXT_nconf(void* conf, void* ctx, const char* name, const char* value) { return nullptr; }
__attribute__((weak)) int X509_add1_ext_i2d(void* x, int nid, void* value, int crit, unsigned long flags) { return 0; }

// EVP_PKEY additional
__attribute__((weak)) int EVP_PKEY_get_id(void* pkey) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_bits(void* pkey) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_size(void* pkey) { return 0; }
__attribute__((weak)) void* EVP_PKEY_get0_RSA(void* pkey) { return nullptr; }
__attribute__((weak)) void* EVP_PKEY_get0_DSA(void* pkey) { return nullptr; }
__attribute__((weak)) void* EVP_PKEY_get0_EC_KEY(void* pkey) { return nullptr; }
__attribute__((weak)) void* EVP_PKEY_get0_DH(void* pkey) { return nullptr; }
__attribute__((weak)) int EVP_PKEY_set1_RSA(void* pkey, void* key) { return 0; }
__attribute__((weak)) int EVP_PKEY_set1_DSA(void* pkey, void* key) { return 0; }
__attribute__((weak)) int EVP_PKEY_set1_EC_KEY(void* pkey, void* key) { return 0; }
__attribute__((weak)) int EVP_PKEY_set1_DH(void* pkey, void* key) { return 0; }
__attribute__((weak)) int i2d_PrivateKey(void* pkey, unsigned char** pp) { return 0; }
__attribute__((weak)) int i2d_PUBKEY(void* pkey, unsigned char** pp) { return 0; }
__attribute__((weak)) void* d2i_PrivateKey(int type, void** a, const unsigned char** pp, long length) { return nullptr; }
__attribute__((weak)) void* d2i_PUBKEY(void** a, const unsigned char** pp, long length) { return nullptr; }

// X509 V3 extensions
__attribute__((weak)) int X509_get_ext_count(void* x) { return 0; }
__attribute__((weak)) void* X509_get_ext(void* x, int loc) { return nullptr; }
__attribute__((weak)) int X509_get_ext_by_NID(void* x, int nid, int lastpos) { return -1; }
__attribute__((weak)) void* X509_delete_ext(void* x, int loc) { return nullptr; }

// X509_REQ stubs
__attribute__((weak)) void* X509_REQ_new(void) { return nullptr; }
__attribute__((weak)) void X509_REQ_free(void* a) {}
__attribute__((weak)) int i2d_X509_REQ(void* a, unsigned char** out) { return 0; }
__attribute__((weak)) void* d2i_X509_REQ(void** a, const unsigned char** in, long len) { return nullptr; }
__attribute__((weak)) void* PEM_read_bio_X509_REQ(void* bp, void** x, void* cb, void* u) { return nullptr; }
__attribute__((weak)) int PEM_write_bio_X509_REQ(void* bp, void* x) { return 0; }

// libuv stubs - use weak linkage in case libuv provides them
__attribute__((weak)) int uv_set_process_title(const char* title) { return 0; }
__attribute__((weak)) int uv_fs_event_init(void* loop, void* handle) { return -1; }
__attribute__((weak)) int uv_fs_event_start(void* handle, void* cb, const char* path, unsigned int flags) { return -1; }

// Node.js postmortem debug metadata - required for test_node_postmortem_metadata.cc
__attribute__((weak)) int nodedbg_offset_Environment_HandleWrapQueue__head___ListNode_HandleWrap = 0;
__attribute__((weak)) int nodedbg_offset_ListNode_HandleWrap__prev___uintptr_t = 0;
__attribute__((weak)) int nodedbg_offset_ListNode_HandleWrap__next___uintptr_t = 0;
__attribute__((weak)) int nodedbg_offset_Environment_ReqWrapQueue__head___ListNode_ReqWrapQueue = 0;
__attribute__((weak)) int nodedbg_offset_ListNode_ReqWrap__prev___uintptr_t = 0;
__attribute__((weak)) int nodedbg_offset_ListNode_ReqWrap__next___uintptr_t = 0;

// llhttp WASM callback stubs - needed when llhttp is compiled for WASM mode
// Note: wasm_on_headers_complete takes 4 params: parser, method, http_major, http_minor
__attribute__((weak)) int wasm_on_headers_complete(void* p, int method, int http_major, int http_minor) { return 0; }
__attribute__((weak)) int wasm_on_message_begin(void* p) { return 0; }
__attribute__((weak)) int wasm_on_url(void* p, const char* at, size_t length) { return 0; }
__attribute__((weak)) int wasm_on_status(void* p, const char* at, size_t length) { return 0; }
__attribute__((weak)) int wasm_on_header_field(void* p, const char* at, size_t length) { return 0; }
__attribute__((weak)) int wasm_on_header_value(void* p, const char* at, size_t length) { return 0; }
__attribute__((weak)) int wasm_on_body(void* p, const char* at, size_t length) { return 0; }
__attribute__((weak)) int wasm_on_message_complete(void* p) { return 0; }

// More OpenSSL EVP_MD stubs
__attribute__((weak)) int EVP_MD_get_size(void* md) { return 0; }
__attribute__((weak)) int EVP_MD_get_type(void* md) { return 0; }
__attribute__((weak)) void* EVP_MD_CTX_get0_md(void* ctx) { return nullptr; }
__attribute__((weak)) int EVP_MD_CTX_copy_ex(void* out, void* in) { return 0; }
__attribute__((weak)) int EVP_MD_CTX_set_params(void* ctx, void* params) { return 0; }
__attribute__((weak)) void* EVP_sha1(void) { return nullptr; }
__attribute__((weak)) void* EVP_get_digestbyname(const char* name) { return nullptr; }

// OSSL_PARAM stubs with correct OpenSSL 3.x signatures
// These functions write to an output OSSL_PARAM structure (first param)
typedef struct ossl_param_st {
    const char *key;
    unsigned int data_type;
    void *data;
    size_t data_size;
    size_t return_size;
} OSSL_PARAM;

__attribute__((weak)) void OSSL_PARAM_construct_octet_string(OSSL_PARAM* p, const char* key, void* buf, size_t bsize) {
  if (p) {
    p->key = key;
    p->data = buf;
    p->data_size = bsize;
  }
}
__attribute__((weak)) void OSSL_PARAM_construct_utf8_string(OSSL_PARAM* p, const char* key, char* buf, size_t bsize) {
  if (p) {
    p->key = key;
    p->data = buf;
    p->data_size = bsize;
  }
}
__attribute__((weak)) void OSSL_PARAM_construct_end(OSSL_PARAM* p) {
  if (p) {
    p->key = nullptr;
    p->data = nullptr;
    p->data_size = 0;
  }
}

// OPENSSL_die stub
__attribute__((weak)) void OPENSSL_die(const char* message, const char* file, int line) { abort(); }

// WPACKET stubs (OpenSSL internal packet writing) - note: val is uint64_t in OpenSSL
__attribute__((weak)) int WPACKET_put_bytes__(void* pkt, uint64_t val, size_t size) { return 0; }
__attribute__((weak)) int WPACKET_start_sub_packet_len__(void* pkt, size_t lenbytes) { return 0; }

// More OpenSSL stubs needed by ncrypto and openssl internals
__attribute__((weak)) int EVP_PBE_scrypt(const char* pass, size_t passlen, const unsigned char* salt, size_t saltlen, uint64_t N, uint64_t r, uint64_t p, uint64_t maxmem, unsigned char* key, size_t keylen) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_raw_public_key(void* pkey, unsigned char* pub, size_t* len) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_raw_private_key(void* pkey, unsigned char* priv, size_t* len) { return 0; }
__attribute__((weak)) int i2d_PUBKEY_bio(void* bp, void* pkey) { return 0; }
__attribute__((weak)) int PEM_bytes_read_bio(unsigned char** pdata, long* plen, char** pnm, const char* name, void* bp, void* cb, void* u) { return 0; }
__attribute__((weak)) void* d2i_X509(void** a, const unsigned char** in, long len) { return nullptr; }
__attribute__((weak)) void* d2i_PrivateKey_bio(void* bp, void** a) { return nullptr; }
__attribute__((weak)) void* d2i_PKCS8PrivateKey_bio(void* bp, void** x, void* cb, void* u) { return nullptr; }
__attribute__((weak)) void* d2i_PKCS8_PRIV_KEY_INFO_bio(void* bp, void** p8inf) { return nullptr; }
__attribute__((weak)) void* EVP_PKCS82PKEY(void* p8) { return nullptr; }
__attribute__((weak)) int PEM_write_bio_RSAPrivateKey(void* bp, void* x, void* enc, unsigned char* kstr, int klen, void* cb, void* u) { return 0; }
__attribute__((weak)) int i2d_RSAPrivateKey_bio(void* bp, void* rsa) { return 0; }
__attribute__((weak)) int i2d_PKCS8PrivateKey_bio(void* bp, void* x, void* enc, const char* kstr, int klen, void* cb, void* u) { return 0; }
__attribute__((weak)) char* CRYPTO_strdup(const char* str, const char* file, int line) { return str ? strdup(str) : nullptr; }
__attribute__((weak)) int EVP_PKEY_up_ref(void* pkey) { return 1; }
__attribute__((weak)) void* CRYPTO_memdup(const void* data, size_t siz, const char* file, int line) { if (!data) return nullptr; void* ret = malloc(siz); if (ret) memcpy(ret, data, siz); return ret; }
__attribute__((weak)) void OSSL_STACK_OF_X509_free(void* certs) {}
__attribute__((weak)) void* OSSL_trace_begin(int category) { return nullptr; }
__attribute__((weak)) void OSSL_trace_end(int category, void* bio) {}
__attribute__((weak)) int BIO_printf(void* bio, const char* format, ...) { return 0; }
__attribute__((weak)) void* OPENSSL_sk_new_reserve(void* cmp, int n) { return nullptr; }

// More X509 stubs
__attribute__((weak)) void* X509_getm_notBefore(void* x) { return nullptr; }
__attribute__((weak)) void* X509_getm_notAfter(void* x) { return nullptr; }
__attribute__((weak)) int X509_check_private_key(void* x509, void* pkey) { return 0; }
__attribute__((weak)) int X509_digest(void* data, void* type, unsigned char* md, unsigned int* len) { return 0; }
__attribute__((weak)) void* X509_STORE_CTX_new(void) { return nullptr; }
__attribute__((weak)) int X509_STORE_CTX_init(void* ctx, void* store, void* x509, void* chain) { return 0; }
__attribute__((weak)) int X509_STORE_CTX_get1_issuer(void** issuer, void* ctx, void* x) { return 0; }
__attribute__((weak)) void X509_STORE_CTX_free(void* ctx) {}
__attribute__((weak)) const char* X509_verify_cert_error_string(long n) { return ""; }

// BIO additional stubs
__attribute__((weak)) void* BIO_s_secmem(void) { return nullptr; }
__attribute__((weak)) void* BIO_new_file(const char* filename, const char* mode) { return nullptr; }
__attribute__((weak)) void* BIO_new_fp(void* stream, int close_flag) { return nullptr; }

// DH additional stubs
__attribute__((weak)) int DH_check(void* dh, int* codes) { return 0; }
__attribute__((weak)) int DH_check_pub_key(void* dh, void* pub_key, int* codes) { return 0; }
__attribute__((weak)) void DH_get0_key(void* dh, const void** pub_key, const void** priv_key) {}

// ASYNC stubs
__attribute__((weak)) void* ASYNC_get_current_job(void) { return nullptr; }
__attribute__((weak)) int ASYNC_start_job(void** job, void* ctx, int* ret, int (*func)(void*), void* args, size_t size) { return 0; }
__attribute__((weak)) void ASYNC_WAIT_CTX_free(void* ctx) {}
__attribute__((weak)) void* ASYNC_WAIT_CTX_new(void) { return nullptr; }
__attribute__((weak)) int ASYNC_WAIT_CTX_set_callback(void* ctx, void* callback, void* callback_arg) { return 0; }

// BIO dgram stubs
__attribute__((weak)) int BIO_dgram_is_sctp(void* bio) { return 0; }
__attribute__((weak)) int BIO_dgram_sctp_msg_waiting(void* bio) { return 0; }
__attribute__((weak)) int BIO_dgram_sctp_wait_for_dry(void* bio) { return 0; }
__attribute__((weak)) int BIO_dump(void* b, const void* bytes, int len) { return 0; }
__attribute__((weak)) int BIO_dump_indent(void* b, const void* bytes, int len, int indent) { return 0; }
__attribute__((weak)) void* BIO_f_brotli(void) { return nullptr; }
__attribute__((weak)) void* BIO_f_buffer(void) { return nullptr; }
__attribute__((weak)) void* BIO_f_zlib(void) { return nullptr; }
__attribute__((weak)) void* BIO_f_zstd(void) { return nullptr; }
__attribute__((weak)) int BIO_get_retry_reason(void* bio) { return 0; }
__attribute__((weak)) int BIO_indent(void* bio, int indent, int max) { return 0; }
__attribute__((weak)) long BIO_int_ctrl(void* bp, int cmd, long larg, int iarg) { return 0; }
__attribute__((weak)) void* BIO_s_dgram_mem(void) { return nullptr; }
__attribute__((weak)) void* BIO_s_file(void) { return nullptr; }
__attribute__((weak)) int BIO_snprintf(char* buf, size_t n, const char* format, ...) { return 0; }
__attribute__((weak)) int BIO_test_flags(void* b, int flags) { return 0; }
__attribute__((weak)) int BIO_up_ref(void* bio) { return 1; }
__attribute__((weak)) int BIO_write_ex(void* b, const void* data, size_t dlen, size_t* written) { return 0; }

// BN prime stubs
__attribute__((weak)) void* BN_get_rfc2409_prime_1024(void* bn) { return nullptr; }
__attribute__((weak)) void* BN_get_rfc3526_prime_2048(void* bn) { return nullptr; }
__attribute__((weak)) void* BN_get_rfc3526_prime_3072(void* bn) { return nullptr; }
__attribute__((weak)) void* BN_get_rfc3526_prime_4096(void* bn) { return nullptr; }
__attribute__((weak)) void* BN_get_rfc3526_prime_8192(void* bn) { return nullptr; }
__attribute__((weak)) void BN_set_negative(void* bn, int n) {}
__attribute__((weak)) int BN_ucmp(void* a, void* b) { return 0; }

// BUF_MEM stubs
__attribute__((weak)) void BUF_MEM_free(void* a) {}
__attribute__((weak)) int BUF_MEM_grow(void* str, size_t len) { return 0; }
__attribute__((weak)) int BUF_MEM_grow_clean(void* str, size_t len) { return 0; }
__attribute__((weak)) void* BUF_MEM_new(void) { return nullptr; }
__attribute__((weak)) void BUF_reverse(unsigned char* out, const unsigned char* in, size_t size) {}

// COMP stubs
// Note: COMP_*_oneshot functions return compression method pointers (no params, return int for availability)
__attribute__((weak)) int COMP_brotli_oneshot(void) { return 0; }
__attribute__((weak)) int COMP_compress_block(void* ctx, unsigned char* out, int olen, unsigned char* in, int ilen) { return 0; }
__attribute__((weak)) void COMP_CTX_free(void* ctx) {}
__attribute__((weak)) void* COMP_CTX_get_method(void* ctx) { return nullptr; }
__attribute__((weak)) void* COMP_CTX_new(void* meth) { return nullptr; }
__attribute__((weak)) int COMP_expand_block(void* ctx, unsigned char* out, int olen, unsigned char* in, int ilen) { return 0; }
__attribute__((weak)) const char* COMP_get_name(void* comp) { return ""; }
__attribute__((weak)) int COMP_get_type(void* comp) { return 0; }
__attribute__((weak)) void* COMP_zlib(void) { return nullptr; }
__attribute__((weak)) int COMP_zlib_oneshot(void) { return 0; }
__attribute__((weak)) int COMP_zstd_oneshot(void) { return 0; }

// CONF stubs
__attribute__((weak)) int CONF_parse_list(const char* list, int sep, int nospc, int (*list_cb)(const char*, int, void*), void* arg) { return 0; }
__attribute__((weak)) void* conf_ssl_get(void* conf, const char* name, size_t* cnt) { return nullptr; }
// conf_ssl_get_cmd: signature (ssl_conf_cmd_st*, conf, name, cnt) -> void
__attribute__((weak)) void conf_ssl_get_cmd(void* cmd, void* conf, const char* name, size_t* cnt) {}
__attribute__((weak)) int conf_ssl_name_find(const char* name, size_t* idx) { return 0; }

// CRYPTO additional stubs
__attribute__((weak)) int CRYPTO_dup_ex_data(int class_index, void* to, void* from) { return 0; }
__attribute__((weak)) void CRYPTO_free_ex_data(int class_index, void* obj, void* ad) {}
__attribute__((weak)) void* CRYPTO_get_ex_data(void* ad, int idx) { return nullptr; }
__attribute__((weak)) int CRYPTO_get_ex_new_index(int class_index, long argl, void* argp, void* new_func, void* dup_func, void* free_func) { return 0; }
__attribute__((weak)) int CRYPTO_new_ex_data(int class_index, void* obj, void* ad) { return 0; }
__attribute__((weak)) void CRYPTO_secure_free(void* ptr, const char* file, int line) { free(ptr); }
__attribute__((weak)) int CRYPTO_set_ex_data(void* ad, int idx, void* val) { return 0; }
__attribute__((weak)) char* CRYPTO_strndup(const char* str, size_t s, const char* file, int line) { return str ? strndup(str, s) : nullptr; }
__attribute__((weak)) void CRYPTO_THREAD_lock_free(void* lock) {}
__attribute__((weak)) void* CRYPTO_THREAD_lock_new(void) { return nullptr; }
__attribute__((weak)) int CRYPTO_THREAD_read_lock(void* lock) { return 1; }
__attribute__((weak)) int CRYPTO_THREAD_run_once(void* once, void (*init)(void)) { return 0; }
__attribute__((weak)) int CRYPTO_THREAD_unlock(void* lock) { return 1; }
__attribute__((weak)) int CRYPTO_THREAD_write_lock(void* lock) { return 1; }

// CTLOG stubs
__attribute__((weak)) void CTLOG_STORE_free(void* store) {}
__attribute__((weak)) void* CTLOG_STORE_new_ex(void* libctx, const char* propq) { return nullptr; }

// CT_POLICY stubs
__attribute__((weak)) void CT_POLICY_EVAL_CTX_free(void* ctx) {}
__attribute__((weak)) void* CT_POLICY_EVAL_CTX_new_ex(void* libctx, const char* propq) { return nullptr; }
__attribute__((weak)) int CT_POLICY_EVAL_CTX_set1_cert(void* ctx, void* cert) { return 0; }
__attribute__((weak)) int CT_POLICY_EVAL_CTX_set1_issuer(void* ctx, void* issuer) { return 0; }
__attribute__((weak)) void CT_POLICY_EVAL_CTX_set_shared_CTLOG_STORE(void* ctx, void* store) {}
__attribute__((weak)) void CT_POLICY_EVAL_CTX_set_time(void* ctx, uint64_t time_in_ms) {}

// d2i stubs
__attribute__((weak)) void* d2i_DSAPublicKey(void** a, const unsigned char** pp, long length) { return nullptr; }
__attribute__((weak)) void* d2i_ECDSA_SIG(void** sig, const unsigned char** pp, long len) { return nullptr; }
__attribute__((weak)) void* d2i_OCSP_RESPID(void** a, const unsigned char** pp, long length) { return nullptr; }
__attribute__((weak)) void* d2i_PrivateKey_ex_bio(void* bp, void** a, void* libctx, const char* propq) { return nullptr; }
__attribute__((weak)) void* d2i_PUBKEY_ex(void** a, const unsigned char** pp, long length, void* libctx, const char* propq) { return nullptr; }
__attribute__((weak)) void* d2i_RSAPublicKey(void** a, const unsigned char** pp, long length) { return nullptr; }
__attribute__((weak)) void* d2i_X509_EXTENSIONS(void** a, const unsigned char** pp, long length) { return nullptr; }
__attribute__((weak)) void* d2i_X509_NAME(void** a, const unsigned char** pp, long length) { return nullptr; }

// DSA additional stubs
__attribute__((weak)) void* DSA_get0_q(void* d) { return nullptr; }

// ECDSA_SIG stubs
__attribute__((weak)) void ECDSA_SIG_free(void* sig) {}
__attribute__((weak)) void ECDSA_SIG_get0(void* sig, const void** pr, const void** ps) {}
__attribute__((weak)) void* ECDSA_SIG_new(void) { return nullptr; }
__attribute__((weak)) int ECDSA_SIG_set0(void* sig, void* r, void* s) { return 0; }

// EC additional stubs
__attribute__((weak)) size_t EC_get_builtin_curves(void* r, size_t nitems) { return 0; }
__attribute__((weak)) int EC_GROUP_order_bits(void* group) { return 0; }
__attribute__((weak)) int EC_KEY_check_key(void* key) { return 0; }
__attribute__((weak)) void* EC_KEY_dup(void* key) { return nullptr; }
__attribute__((weak)) int EC_KEY_set_group(void* key, void* group) { return 0; }
__attribute__((weak)) int EC_POINT_oct2point(void* group, void* p, const unsigned char* buf, size_t len, void* ctx) { return 0; }

// ENGINE additional stubs
__attribute__((weak)) void* ENGINE_get_cipher(void* e, int nid) { return nullptr; }
__attribute__((weak)) void* ENGINE_get_cipher_engine(int nid) { return nullptr; }
__attribute__((weak)) void* ENGINE_get_digest(void* e, int nid) { return nullptr; }
__attribute__((weak)) void* ENGINE_get_digest_engine(int nid) { return nullptr; }
__attribute__((weak)) int ENGINE_load_ssl_client_cert(void* e, void* s, void* ca_dn, void** pcert, void** ppkey, void** pother, void* ui_method, void* callback_data) { return 0; }
__attribute__((weak)) void* ENGINE_pkey_asn1_find_str(void** pe, const char* str, int len) { return nullptr; }

// ERR additional stubs
__attribute__((weak)) void ERR_add_error_data(int num, ...) {}
__attribute__((weak)) int ERR_clear_last_mark(void) { return 0; }
__attribute__((weak)) int ERR_load_strings_const(void* str) { return 0; }
__attribute__((weak)) void ERR_vset_error(int lib, int reason, const char* fmt, va_list args) {}

// EVP additional stubs from sb1 list
__attribute__((weak)) int EVP_Cipher(void* c, unsigned char* out, const unsigned char* in, unsigned int inl) { return 0; }
__attribute__((weak)) void* EVP_CIPHER_CTX_cipher(void* ctx) { return nullptr; }
__attribute__((weak)) void* EVP_CIPHER_CTX_get0_cipher(void* ctx) { return nullptr; }
__attribute__((weak)) int EVP_CIPHER_CTX_get_block_size(void* ctx) { return 0; }
__attribute__((weak)) int EVP_CIPHER_CTX_get_iv_length(void* ctx) { return 0; }
__attribute__((weak)) int EVP_CIPHER_CTX_get_nid(void* ctx) { return 0; }
__attribute__((weak)) int EVP_CIPHER_CTX_get_params(void* ctx, void* params) { return 0; }
__attribute__((weak)) int EVP_CIPHER_CTX_reset(void* ctx) { return 0; }
__attribute__((weak)) void EVP_CIPHER_CTX_set_flags(void* ctx, int flags) {}
__attribute__((weak)) int EVP_CIPHER_CTX_set_key_length(void* ctx, int keylen) { return 0; }
__attribute__((weak)) int EVP_CIPHER_CTX_set_params(void* ctx, void* params) { return 0; }
__attribute__((weak)) int EVP_CipherFinal_ex(void* ctx, unsigned char* outm, int* outl) { return 0; }
__attribute__((weak)) void* EVP_CIPHER_get0_provider(void* cipher) { return nullptr; }
__attribute__((weak)) unsigned long EVP_CIPHER_get_flags(void* cipher) { return 0; }
__attribute__((weak)) int EVP_CIPHER_get_mode(void* cipher) { return 0; }
__attribute__((weak)) int EVP_CipherInit_ex(void* ctx, void* type, void* impl, const unsigned char* key, const unsigned char* iv, int enc) { return 0; }
__attribute__((weak)) int EVP_CIPHER_is_a(void* cipher, const char* name) { return 0; }
__attribute__((weak)) int EVP_CipherUpdate(void* ctx, unsigned char* out, int* outl, const unsigned char* in, int inl) { return 0; }
__attribute__((weak)) int EVP_CIPHER_up_ref(void* cipher) { return 0; }
__attribute__((weak)) int EVP_DecryptFinal(void* ctx, unsigned char* outm, int* outl) { return 0; }
__attribute__((weak)) int EVP_Digest(const void* data, size_t count, unsigned char* md, unsigned int* size, void* type, void* impl) { return 0; }
__attribute__((weak)) int EVP_DigestFinal(void* ctx, unsigned char* md, unsigned int* s) { return 0; }
__attribute__((weak)) int EVP_DigestFinalXOF(void* ctx, unsigned char* md, size_t len) { return 0; }
__attribute__((weak)) int EVP_DigestInit(void* ctx, void* type) { return 0; }
__attribute__((weak)) int EVP_DigestSign(void* ctx, unsigned char* sigret, size_t* siglen, const unsigned char* tbs, size_t tbslen) { return 0; }
__attribute__((weak)) int EVP_DigestSignFinal(void* ctx, unsigned char* sigret, size_t* siglen) { return 0; }
__attribute__((weak)) int EVP_DigestSignInit_ex(void* ctx, void** pctx, const char* mdname, void* libctx, const char* props, void* pkey, void* params) { return 0; }
__attribute__((weak)) int EVP_DigestSignUpdate(void* ctx, const void* data, size_t dsize) { return 0; }
__attribute__((weak)) int EVP_DigestVerify(void* ctx, const unsigned char* sigret, size_t siglen, const unsigned char* tbs, size_t tbslen) { return 0; }
__attribute__((weak)) int EVP_DigestVerifyFinal(void* ctx, const unsigned char* sig, size_t siglen) { return 0; }
__attribute__((weak)) int EVP_DigestVerifyInit_ex(void* ctx, void** pctx, const char* mdname, void* libctx, const char* props, void* pkey, void* params) { return 0; }
__attribute__((weak)) int EVP_DigestVerifyUpdate(void* ctx, const void* data, size_t dsize) { return 0; }
__attribute__((weak)) int EVP_EncryptFinal(void* ctx, unsigned char* out, int* outl) { return 0; }
__attribute__((weak)) void* EVP_KEYEXCH_fetch(void* libctx, const char* algorithm, const char* properties) { return nullptr; }
__attribute__((weak)) void EVP_KEYEXCH_free(void* exchange) {}
__attribute__((weak)) void* EVP_KEYMGMT_fetch(void* libctx, const char* algorithm, const char* properties) { return nullptr; }
__attribute__((weak)) void EVP_KEYMGMT_free(void* keymgmt) {}
__attribute__((weak)) void* EVP_KEYMGMT_get0_provider(void* keymgmt) { return nullptr; }
__attribute__((weak)) size_t EVP_MAC_CTX_get_mac_size(void* ctx) { return 0; }
__attribute__((weak)) int EVP_MD_CTX_copy(void* out, void* in) { return 0; }
__attribute__((weak)) int EVP_MD_CTX_ctrl(void* ctx, int type, int arg, void* ptr) { return 0; }
__attribute__((weak)) void* EVP_MD_CTX_get_pkey_ctx(void* ctx) { return nullptr; }
__attribute__((weak)) void* EVP_MD_CTX_md(void* ctx) { return nullptr; }
__attribute__((weak)) void* EVP_MD_get0_provider(void* md) { return nullptr; }
__attribute__((weak)) unsigned long EVP_MD_get_flags(void* md) { return 0; }
__attribute__((weak)) int EVP_MD_is_a(void* md, const char* name) { return 0; }
__attribute__((weak)) int EVP_MD_up_ref(void* md) { return 0; }
__attribute__((weak)) int EVP_PKEY_can_sign(void* pkey) { return 0; }
__attribute__((weak)) int EVP_PKEY_check(void* ctx) { return 0; }
__attribute__((weak)) int evp_pkey_copy_downgraded(void** dest, void* src) { return 0; }
__attribute__((weak)) int EVP_PKEY_copy_parameters(void* to, void* from) { return 0; }
__attribute__((weak)) void* EVP_PKEY_CTX_new_from_name(void* libctx, const char* name, const char* propquery) { return nullptr; }
__attribute__((weak)) void* EVP_PKEY_CTX_new_from_pkey(void* libctx, void* pkey, const char* propquery) { return nullptr; }
__attribute__((weak)) int EVP_PKEY_CTX_set0_rsa_oaep_label(void* ctx, void* label, int llen) { return 0; }
__attribute__((weak)) int EVP_PKEY_CTX_set_dh_pad(void* ctx, int pad) { return 0; }
__attribute__((weak)) int EVP_PKEY_CTX_set_group_name(void* ctx, const char* name) { return 0; }
__attribute__((weak)) int EVP_PKEY_CTX_set_params(void* ctx, void* params) { return 0; }
__attribute__((weak)) int EVP_PKEY_CTX_set_rsa_mgf1_md(void* ctx, void* md) { return 0; }
__attribute__((weak)) int EVP_PKEY_CTX_set_rsa_oaep_md(void* ctx, void* md) { return 0; }
__attribute__((weak)) int EVP_PKEY_CTX_set_rsa_padding(void* ctx, int pad) { return 0; }
__attribute__((weak)) int EVP_PKEY_CTX_set_rsa_pss_saltlen(void* ctx, int len) { return 0; }
__attribute__((weak)) int EVP_PKEY_CTX_set_signature_md(void* ctx, void* md) { return 0; }
__attribute__((weak)) int EVP_PKEY_decapsulate(void* ctx, unsigned char* unwrapped, size_t* unwrappedlen, const unsigned char* wrapped, size_t wrappedlen) { return 0; }
__attribute__((weak)) int EVP_PKEY_decapsulate_init(void* ctx, void* params) { return 0; }
__attribute__((weak)) int EVP_PKEY_decrypt(void* ctx, unsigned char* out, size_t* outlen, const unsigned char* in, size_t inlen) { return 0; }
__attribute__((weak)) int EVP_PKEY_decrypt_init(void* ctx) { return 0; }
__attribute__((weak)) int EVP_PKEY_digestsign_supports_digest(void* pkey, void* libctx, const char* name, const char* propq) { return 0; }
__attribute__((weak)) int EVP_PKEY_encapsulate(void* ctx, unsigned char* wrappedkey, size_t* wrappedkeylen, unsigned char* genkey, size_t* genkeylen) { return 0; }
__attribute__((weak)) int EVP_PKEY_encapsulate_init(void* ctx, void* params) { return 0; }
__attribute__((weak)) int EVP_PKEY_encrypt(void* ctx, unsigned char* out, size_t* outlen, const unsigned char* in, size_t inlen) { return 0; }
__attribute__((weak)) int EVP_PKEY_encrypt_init(void* ctx) { return 0; }
__attribute__((weak)) int EVP_PKEY_get1_encoded_public_key(void* pkey, unsigned char** ppub) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_base_id(void* pkey) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_ec_point_conv_form(void* pkey) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_field_type(void* pkey) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_group_name(void* pkey, char* name, size_t name_sz, size_t* gname_len) { return 0; }
__attribute__((weak)) int EVP_PKEY_get_security_bits(void* pkey) { return 0; }
__attribute__((weak)) int EVP_PKEY_is_a(void* pkey, const char* name) { return 0; }
__attribute__((weak)) int EVP_PKEY_missing_parameters(void* pkey) { return 0; }
__attribute__((weak)) void* EVP_PKEY_new_mac_key(int type, void* e, const unsigned char* key, int keylen) { return nullptr; }
__attribute__((weak)) void* EVP_PKEY_new_raw_private_key_ex(void* libctx, const char* keytype, const char* propq, const unsigned char* priv, size_t len) { return nullptr; }
__attribute__((weak)) int EVP_PKEY_param_check_quick(void* ctx) { return 0; }
__attribute__((weak)) int EVP_PKEY_paramgen(void* ctx, void** ppkey) { return 0; }
__attribute__((weak)) int EVP_PKEY_paramgen_init(void* ctx) { return 0; }
__attribute__((weak)) int EVP_PKEY_print_public(void* out, void* pkey, int indent, void* pctx) { return 0; }
__attribute__((weak)) int EVP_PKEY_public_check(void* ctx) { return 0; }
__attribute__((weak)) int EVP_PKEY_set1_encoded_public_key(void* pkey, const unsigned char* pub, size_t publen) { return 0; }
__attribute__((weak)) int EVP_PKEY_set_type(void* pkey, int type) { return 0; }
__attribute__((weak)) int EVP_PKEY_verify_recover(void* ctx, unsigned char* rout, size_t* routlen, const unsigned char* sig, size_t siglen) { return 0; }
__attribute__((weak)) int EVP_PKEY_verify_recover_init(void* ctx) { return 0; }
__attribute__((weak)) void* EVP_Q_mac(void* libctx, const char* name, const char* propq, const char* subalg, void* params, const void* key, size_t keylen, const unsigned char* data, size_t datalen, unsigned char* out, size_t outsize, size_t* outlen) { return nullptr; }
__attribute__((weak)) void* EVP_sha384(void) { return nullptr; }
__attribute__((weak)) void* EVP_SIGNATURE_fetch(void* libctx, const char* algorithm, const char* properties) { return nullptr; }
__attribute__((weak)) void EVP_SIGNATURE_free(void* signature) {}

// HMAC additional stubs
__attribute__((weak)) size_t HMAC_size(void* ctx) { return 0; }

// i2d stubs
__attribute__((weak)) int i2d_ECDSA_SIG(void* sig, unsigned char** pp) { return 0; }
__attribute__((weak)) int i2d_ECPrivateKey_bio(void* bp, void* ec) { return 0; }
__attribute__((weak)) int i2d_OCSP_RESPID(void* a, unsigned char** pp) { return 0; }
__attribute__((weak)) int i2d_RSAPublicKey_bio(void* bp, void* rsa) { return 0; }
__attribute__((weak)) int i2d_X509_EXTENSIONS(void* x, unsigned char** out) { return 0; }
__attribute__((weak)) int i2d_X509_NAME(void* a, unsigned char** out) { return 0; }
__attribute__((weak)) int i2d_X509_PUBKEY(void* a, unsigned char** pp) { return 0; }

// ASN1 item symbols are functions in OpenSSL that return const ASN1_ITEM*
// They are created by ASN1_ITEM_TEMPLATE macros
typedef struct ASN1_ITEM_st ASN1_ITEM;
__attribute__((weak)) const ASN1_ITEM* INT32_it(void) { return nullptr; }
__attribute__((weak)) const ASN1_ITEM* UINT32_it(void) { return nullptr; }
__attribute__((weak)) const ASN1_ITEM* ZINT32_it(void) { return nullptr; }
__attribute__((weak)) const ASN1_ITEM* ZINT64_it(void) { return nullptr; }
__attribute__((weak)) const ASN1_ITEM* ZUINT32_it(void) { return nullptr; }
__attribute__((weak)) const ASN1_ITEM* ZUINT64_it(void) { return nullptr; }
__attribute__((weak)) const ASN1_ITEM* X509_it(void) { return nullptr; }
__attribute__((weak)) const ASN1_ITEM* X509_ATTRIBUTE_it(void) { return nullptr; }

// MD5 stubs
__attribute__((weak)) int MD5_Init(void* c) { return 0; }
__attribute__((weak)) void MD5_Transform(void* c, const unsigned char* data) {}

// o2i stubs
__attribute__((weak)) void* o2i_ECPublicKey(void** key, const unsigned char** in, long len) { return nullptr; }
__attribute__((weak)) void* o2i_SCT_LIST(void** scts, const unsigned char** in, size_t len) { return nullptr; }

// OBJ additional stubs
__attribute__((weak)) int OBJ_add_sigid(int signid, int dig_id, int pkey_id) { return 0; }
__attribute__((weak)) void* OBJ_bsearch_(const void* key, const void* base, int num, int size, int (*cmp)(const void*, const void*)) { return nullptr; }
__attribute__((weak)) int OBJ_create(const char* oid, const char* sn, const char* ln) { return 0; }
__attribute__((weak)) int OBJ_ln2nid(const char* s) { return 0; }

// OCSP additional stubs
__attribute__((weak)) void OCSP_BASICRESP_free(void* bs) {}
__attribute__((weak)) int OCSP_resp_count(void* bs) { return 0; }
__attribute__((weak)) void* OCSP_resp_get0(void* bs, int idx) { return nullptr; }
__attribute__((weak)) void OCSP_RESPID_free(void* id) {}
__attribute__((weak)) void* OCSP_response_get1_basic(void* resp) { return nullptr; }
__attribute__((weak)) void* OCSP_SINGLERESP_get1_ext_d2i(void* single, int nid, int* crit, int* idx) { return nullptr; }

// OPENSSL additional stubs
__attribute__((weak)) int OPENSSL_atexit(void (*handler)(void)) { return 0; }
__attribute__((weak)) int OPENSSL_DIR_end(void** ctx) { return 0; }
__attribute__((weak)) const char* OPENSSL_DIR_read(void** ctx, const char* directory) { return nullptr; }
__attribute__((weak)) void* OPENSSL_gmtime(const void* timer, void* result) { return nullptr; }
__attribute__((weak)) int OPENSSL_gmtime_adj(void* tm, int off_day, long offset_sec) { return 0; }
__attribute__((weak)) void* OPENSSL_LH_delete(void* lh, void* data) { return nullptr; }
__attribute__((weak)) void OPENSSL_LH_free(void* lh) {}
__attribute__((weak)) unsigned long OPENSSL_LH_get_down_load(void* lh) { return 0; }
__attribute__((weak)) void* OPENSSL_LH_insert(void* lh, void* data) { return nullptr; }
__attribute__((weak)) void* OPENSSL_LH_new(unsigned long (*hfn)(const void*), int (*cfn)(const void*, const void*)) { return nullptr; }
__attribute__((weak)) unsigned long OPENSSL_LH_num_items(void* lh) { return 0; }
__attribute__((weak)) void* OPENSSL_LH_retrieve(void* lh, const void* data) { return nullptr; }
__attribute__((weak)) void OPENSSL_LH_set_down_load(void* lh, unsigned long down_load) {}
__attribute__((weak)) void* OPENSSL_sk_dup(void* st) { return nullptr; }
__attribute__((weak)) int OPENSSL_sk_find(void* st, const void* data) { return -1; }
__attribute__((weak)) void* OPENSSL_sk_set(void* st, int i, const void* data) { return nullptr; }
__attribute__((weak)) void* OPENSSL_sk_set_cmp_func(void* st, void* cmp) { return nullptr; }
__attribute__((weak)) void* OPENSSL_sk_shift(void* st) { return nullptr; }
__attribute__((weak)) void OPENSSL_sk_sort(void* st) {}
__attribute__((weak)) int OPENSSL_sk_unshift(void* st, const void* data) { return 0; }
__attribute__((weak)) int OPENSSL_strcasecmp(const char* s1, const char* s2) { return strcasecmp(s1, s2); }
__attribute__((weak)) int OPENSSL_strncasecmp(const char* s1, const char* s2, size_t n) { return strncasecmp(s1, s2, n); }

// ossl ctype stubs
__attribute__((weak)) int ossl_ascii_isdigit(int c) { return c >= '0' && c <= '9'; }
__attribute__((weak)) int ossl_ctype_check(int c, unsigned int mask) { return 0; }
__attribute__((weak)) int ossl_isdigit(int c) { return c >= '0' && c <= '9'; }

// OSSL_DECODER stubs
__attribute__((weak)) void OSSL_DECODER_CTX_free(void* ctx) {}
__attribute__((weak)) void* OSSL_DECODER_CTX_new_for_pkey(void** pkey, const char* input_type, const char* input_struct, const char* keytype, int selection, void* libctx, const char* propq) { return nullptr; }
__attribute__((weak)) int OSSL_DECODER_from_bio(void* ctx, void* in) { return 0; }

// ossl asn1 meth stubs
__attribute__((weak)) const void* ossl_dh_asn1_meth = nullptr;
__attribute__((weak)) const void* ossl_dhx_asn1_meth = nullptr;
__attribute__((weak)) const void* const* ossl_dsa_asn1_meths = nullptr;
__attribute__((weak)) const void* ossl_eckey_asn1_meth = nullptr;
__attribute__((weak)) const void* ossl_ecx25519_asn1_meth = nullptr;
__attribute__((weak)) const void* ossl_ecx448_asn1_meth = nullptr;
__attribute__((weak)) const void* ossl_ed25519_asn1_meth = nullptr;
__attribute__((weak)) const void* ossl_ed448_asn1_meth = nullptr;
__attribute__((weak)) const void* const* ossl_rsa_asn1_meths = nullptr;
__attribute__((weak)) const void* ossl_rsa_pss_asn1_meth = nullptr;
__attribute__((weak)) const void* ossl_sm2_asn1_meth = nullptr;

// OSSL_LIB_CTX stubs
__attribute__((weak)) void* OSSL_LIB_CTX_set0_default(void* libctx) { return nullptr; }

// OSSL_PARAM_BLD stubs
__attribute__((weak)) void OSSL_PARAM_BLD_free(void* bld) {}
__attribute__((weak)) void* OSSL_PARAM_BLD_new(void) { return nullptr; }
__attribute__((weak)) int OSSL_PARAM_BLD_push_BN(void* bld, const char* key, void* bn) { return 0; }
__attribute__((weak)) int OSSL_PARAM_BLD_push_uint(void* bld, const char* key, unsigned int val) { return 0; }
__attribute__((weak)) void* OSSL_PARAM_BLD_to_param(void* bld) { return nullptr; }

// OSSL_PARAM additional construct stubs
__attribute__((weak)) void OSSL_PARAM_construct_int(OSSL_PARAM* p, const char* key, int* buf) {
  if (p) { p->key = key; p->data = buf; p->data_size = sizeof(int); }
}
__attribute__((weak)) void OSSL_PARAM_construct_octet_ptr(OSSL_PARAM* p, const char* key, void** buf, size_t bsize) {
  if (p) { p->key = key; p->data = buf; p->data_size = bsize; }
}
__attribute__((weak)) void OSSL_PARAM_construct_size_t(OSSL_PARAM* p, const char* key, size_t* buf) {
  if (p) { p->key = key; p->data = buf; p->data_size = sizeof(size_t); }
}
__attribute__((weak)) void OSSL_PARAM_construct_uint(OSSL_PARAM* p, const char* key, unsigned int* buf) {
  if (p) { p->key = key; p->data = buf; p->data_size = sizeof(unsigned int); }
}
__attribute__((weak)) void OSSL_PARAM_construct_uint32(OSSL_PARAM* p, const char* key, uint32_t* buf) {
  if (p) { p->key = key; p->data = buf; p->data_size = sizeof(uint32_t); }
}
__attribute__((weak)) void OSSL_PARAM_construct_uint64(OSSL_PARAM* p, const char* key, uint64_t* buf) {
  if (p) { p->key = key; p->data = buf; p->data_size = sizeof(uint64_t); }
}
__attribute__((weak)) void OSSL_PARAM_free(void* p) {}
__attribute__((weak)) int OSSL_PARAM_get_int(void* p, int* val) { return 0; }
__attribute__((weak)) int OSSL_PARAM_get_size_t(void* p, size_t* val) { return 0; }
__attribute__((weak)) int OSSL_PARAM_get_uint(void* p, unsigned int* val) { return 0; }
__attribute__((weak)) int OSSL_PARAM_get_uint32(void* p, uint32_t* val) { return 0; }
__attribute__((weak)) int OSSL_PARAM_get_uint64(void* p, uint64_t* val) { return 0; }
__attribute__((weak)) void* OSSL_PARAM_locate_const(void* p, const char* key) { return nullptr; }

// OSSL_PROVIDER additional stubs
__attribute__((weak)) int OSSL_PROVIDER_do_all(void* ctx, int (*cb)(void* provider, void* cbdata), void* cbdata) { return 0; }
__attribute__((weak)) int OSSL_PROVIDER_get_capabilities(void* prov, const char* capability, void* cb, void* arg) { return 0; }

// ossl_quic stubs
__attribute__((weak)) void* ossl_quic_conn_get_net_rbio(void* qc) { return nullptr; }
__attribute__((weak)) void* ossl_quic_conn_get_net_wbio(void* qc) { return nullptr; }
__attribute__((weak)) int ossl_quic_get_error(void* qc, int i) { return 0; }
__attribute__((weak)) int ossl_quic_trace(int write_p, int version, int content_type, const void* buf, size_t len, void* ssl, void* arg) { return 0; }

// OSSL_STORE stubs
__attribute__((weak)) int OSSL_STORE_close(void* ctx) { return 0; }
__attribute__((weak)) int OSSL_STORE_eof(void* ctx) { return 1; }
__attribute__((weak)) int OSSL_STORE_error(void* ctx) { return 0; }
__attribute__((weak)) void OSSL_STORE_INFO_free(void* info) {}
__attribute__((weak)) void* OSSL_STORE_INFO_get0_CERT(void* info) { return nullptr; }
__attribute__((weak)) const char* OSSL_STORE_INFO_get0_NAME(void* info) { return nullptr; }
__attribute__((weak)) int OSSL_STORE_INFO_get_type(void* info) { return 0; }
__attribute__((weak)) void* OSSL_STORE_load(void* ctx) { return nullptr; }
__attribute__((weak)) void* OSSL_STORE_open(const char* uri, void* ui_method, void* ui_data, void* post_process, void* post_process_data) { return nullptr; }

// ossl_time stubs
__attribute__((weak)) uint64_t ossl_time_now(void) { return 0; }

// PEM additional stubs
__attribute__((weak)) int PEM_read_bio(void* bp, char** name, char** header, unsigned char** data, long* len) { return 0; }
__attribute__((weak)) void* PEM_read_bio_PrivateKey_ex(void* bp, void** x, void* cb, void* u, void* libctx, const char* propq) { return nullptr; }
__attribute__((weak)) int PEM_write_bio_ECPrivateKey(void* bp, void* ec, void* enc, unsigned char* kstr, int klen, void* cb, void* u) { return 0; }
__attribute__((weak)) int PEM_write_bio_PKCS8PrivateKey(void* bp, void* x, void* enc, const char* kstr, int klen, void* cb, void* u) { return 0; }
__attribute__((weak)) int PEM_write_bio_RSAPublicKey(void* bp, void* rsa) { return 0; }

// RAND additional stubs
__attribute__((weak)) int RAND_priv_bytes_ex(void* ctx, unsigned char* buf, size_t num, unsigned int strength) {
  // Fill with pseudo-random bytes for WASI
  static unsigned int seed = 54321;
  for (size_t i = 0; i < num; i++) {
    seed = seed * 1103515245 + 12345;
    buf[i] = (unsigned char)(seed >> 16);
  }
  return 1;  // Success
}

// RSA additional stubs
__attribute__((weak)) void RSA_get0_crt_params(void* r, const void** dmp1, const void** dmq1, const void** iqmp) {}
__attribute__((weak)) void RSA_get0_factors(void* r, const void** p, const void** q) {}
__attribute__((weak)) void* RSA_get0_pss_params(void* r) { return nullptr; }

// SCT stubs
__attribute__((weak)) void SCT_LIST_free(void* scts) {}
__attribute__((weak)) int SCT_LIST_validate(void* scts, void* ctx) { return 0; }
__attribute__((weak)) int SCT_set_source(void* sct, int source) { return 0; }

// SHA stubs
__attribute__((weak)) int SHA1_Init(void* c) { return 0; }
__attribute__((weak)) void SHA1_Transform(void* c, const unsigned char* data) {}
__attribute__((weak)) int SHA224_Init(void* c) { return 0; }
__attribute__((weak)) int SHA256_Init(void* c) { return 0; }
__attribute__((weak)) void SHA256_Transform(void* c, const unsigned char* data) {}
__attribute__((weak)) int SHA384_Init(void* c) { return 0; }
__attribute__((weak)) int SHA512_Init(void* c) { return 0; }
__attribute__((weak)) void SHA512_Transform(void* c, const unsigned char* data) {}

// SRP stubs
__attribute__((weak)) void* SRP_Calc_A(void* a, void* N, void* g) { return nullptr; }
__attribute__((weak)) void* SRP_Calc_B_ex(void* b, void* N, void* g, void* v, void* libctx, const char* propq) { return nullptr; }
__attribute__((weak)) void* SRP_Calc_client_key_ex(void* N, void* B, void* g, void* x, void* a, void* u, void* libctx, const char* propq) { return nullptr; }
__attribute__((weak)) void* SRP_Calc_server_key(void* A, void* v, void* u, void* b, void* N) { return nullptr; }
__attribute__((weak)) void* SRP_Calc_u_ex(void* A, void* B, void* N, void* libctx, const char* propq) { return nullptr; }
__attribute__((weak)) void* SRP_Calc_x_ex(void* s, void* user, void* pass, void* libctx, const char* propq) { return nullptr; }
__attribute__((weak)) char* SRP_check_known_gN_param(void* g, void* N) { return nullptr; }
__attribute__((weak)) int SRP_Verify_A_mod_N(void* A, void* N) { return 0; }
__attribute__((weak)) int SRP_Verify_B_mod_N(void* B, void* N) { return 0; }

// WPACKET additional stubs
__attribute__((weak)) int WPACKET_allocate_bytes(void* pkt, size_t len, unsigned char** allocbytes) { return 0; }
__attribute__((weak)) void WPACKET_cleanup(void* pkt) {}
__attribute__((weak)) int WPACKET_close(void* pkt) { return 0; }
__attribute__((weak)) int WPACKET_fill_lengths(void* pkt) { return 0; }
__attribute__((weak)) int WPACKET_finish(void* pkt) { return 0; }
__attribute__((weak)) unsigned char* WPACKET_get_curr(void* pkt) { return nullptr; }
__attribute__((weak)) int WPACKET_get_length(void* pkt, size_t* len) { return 0; }
__attribute__((weak)) int WPACKET_get_total_written(void* pkt, size_t* len) { return 0; }
__attribute__((weak)) int WPACKET_init(void* pkt, void* buf) { return 0; }
__attribute__((weak)) int WPACKET_init_static_len(void* pkt, unsigned char* buf, size_t len, size_t lenbytes) { return 0; }
__attribute__((weak)) int WPACKET_memcpy(void* pkt, const void* src, size_t len) { return 0; }
__attribute__((weak)) int WPACKET_memset(void* pkt, int ch, size_t len) { return 0; }
__attribute__((weak)) int WPACKET_reserve_bytes(void* pkt, size_t len, unsigned char** allocbytes) { return 0; }
__attribute__((weak)) int WPACKET_set_flags(void* pkt, unsigned int flags) { return 0; }
__attribute__((weak)) int WPACKET_start_sub_packet(void* pkt) { return 0; }
__attribute__((weak)) int WPACKET_sub_allocate_bytes__(void* pkt, size_t len, unsigned char** allocbytes, size_t lenbytes) { return 0; }
__attribute__((weak)) int WPACKET_sub_memcpy__(void* pkt, const void* src, size_t len, size_t lenbytes) { return 0; }
__attribute__((weak)) int WPACKET_sub_reserve_bytes__(void* pkt, size_t len, unsigned char** allocbytes, size_t lenbytes) { return 0; }

// X509 additional stubs from sb1
__attribute__((weak)) int X509_chain_check_suiteb(int* perror_depth, void* x, void* chain, unsigned long flags) { return 0; }
__attribute__((weak)) void* X509_chain_up_ref(void* chain) { return nullptr; }
__attribute__((weak)) void* X509_get0_pubkey(void* x) { return nullptr; }
__attribute__((weak)) uint32_t X509_get_extension_flags(void* x) { return 0; }
__attribute__((weak)) uint32_t X509_get_key_usage(void* x) { return 0; }
__attribute__((weak)) int X509_get_signature_info(void* x, int* mdnid, int* pknid, int* secbits, uint32_t* flags) { return 0; }
__attribute__((weak)) int X509_get_signature_nid(void* x) { return 0; }
__attribute__((weak)) void* X509_get_X509_PUBKEY(void* x) { return nullptr; }
__attribute__((weak)) int X509_NAME_cmp(void* a, void* b) { return 0; }
__attribute__((weak)) int X509_NAME_ENTRY_set(void* ne) { return 0; }
__attribute__((weak)) int X509_NAME_print(void* bp, void* name, int obase) { return 0; }
__attribute__((weak)) void* X509_new_ex(void* libctx, const char* propq) { return nullptr; }
__attribute__((weak)) void* X509_STORE_CTX_get0_chain(void* ctx) { return nullptr; }
__attribute__((weak)) void* X509_STORE_CTX_get0_param(void* ctx) { return nullptr; }
__attribute__((weak)) void* X509_STORE_CTX_get1_chain(void* ctx) { return nullptr; }
__attribute__((weak)) int X509_STORE_CTX_get_error(void* ctx) { return 0; }
__attribute__((weak)) int X509_STORE_CTX_init_rpk(void* ctx, void* store, void* rpk) { return 0; }
__attribute__((weak)) void* X509_STORE_CTX_new_ex(void* libctx, const char* propq) { return nullptr; }
__attribute__((weak)) void X509_STORE_CTX_set0_dane(void* ctx, void* dane) {}
__attribute__((weak)) int X509_STORE_CTX_set_default(void* ctx, const char* name) { return 0; }
__attribute__((weak)) int X509_STORE_CTX_set_ex_data(void* ctx, int idx, void* data) { return 0; }
__attribute__((weak)) void X509_STORE_CTX_set_flags(void* ctx, unsigned long flags) {}
__attribute__((weak)) void X509_STORE_CTX_set_verify_cb(void* ctx, void* verify_cb) {}
__attribute__((weak)) int X509_STORE_load_file_ex(void* ctx, const char* file, void* libctx, const char* propq) { return 0; }
__attribute__((weak)) int X509_STORE_load_path(void* ctx, const char* path) { return 0; }
__attribute__((weak)) int X509_STORE_load_store_ex(void* ctx, const char* uri, void* libctx, const char* propq) { return 0; }
__attribute__((weak)) int X509_up_ref(void* x) { return 1; }
__attribute__((weak)) int X509_verify_cert(void* ctx) { return 0; }
__attribute__((weak)) void X509_VERIFY_PARAM_free(void* param) {}
__attribute__((weak)) int X509_VERIFY_PARAM_inherit(void* to, void* from) { return 0; }
__attribute__((weak)) void X509_VERIFY_PARAM_move_peername(void* to, void* from) {}
__attribute__((weak)) void* X509_VERIFY_PARAM_new(void) { return nullptr; }
__attribute__((weak)) int X509_VERIFY_PARAM_set1(void* to, void* from) { return 0; }
__attribute__((weak)) void X509_VERIFY_PARAM_set_auth_level(void* param, int auth_level) {}

}  // extern "C"

// ncrypto stubs (C++ namespace)
namespace ncrypto {
class BIOPointer;
}

// Forward declare X509_extension_st
struct X509_extension_st;

namespace ncrypto {
__attribute__((weak)) bool SafeX509SubjectAltNamePrint(const BIOPointer& bio, X509_extension_st* ext) {
  return false;
}
__attribute__((weak)) bool SafeX509InfoAccessPrint(const BIOPointer& bio, X509_extension_st* ext) {
  return false;
}
}  // namespace ncrypto

#endif  // __wasi__
