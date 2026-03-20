#ifndef _PWD_H
#define _PWD_H

#ifdef __cplusplus
extern "C" {
#endif

// Password file structure - stubbed for WASI
struct passwd {
    char* pw_name;   // username
    char* pw_passwd; // user password
    uid_t pw_uid;    // user ID
    gid_t pw_gid;    // group ID
    char* pw_gecos;  // user information
    char* pw_dir;    // home directory
    char* pw_shell;  // shell program
};

// Function stubs for WASI compatibility
struct passwd* getpwuid(uid_t uid) {
    return NULL; // Not supported in WASI
}

struct passwd* getpwnam(const char* name) {
    return NULL; // Not supported in WASI
}

// Reentrant versions
int getpwnam_r(const char* name, struct passwd* pwd, char* buf, size_t buflen, struct passwd** result) {
    if (result) *result = NULL;
    return -1; // Not supported in WASI
}

int getpwuid_r(uid_t uid, struct passwd* pwd, char* buf, size_t buflen, struct passwd** result) {
    if (result) *result = NULL;
    return -1; // Not supported in WASI
}

// Set for scanning all password entries
void setpwent(void) {
    // No-op for WASI
}

struct passwd* getpwent(void) {
    return NULL; // Not supported in WASI
}

void endpwent(void) {
    // No-op for WASI
}

#ifdef __cplusplus
}
#endif

#endif // _PWD_H
