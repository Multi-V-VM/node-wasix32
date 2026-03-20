#ifdef __wasi__

#include "wasi-stdlib-compat.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

// Global errno variable
int errno = 0;

// Standard FILE objects
FILE _stdin_file;
FILE _stdout_file;
FILE _stderr_file;

FILE* stdin = &_stdin_file;
FILE* stdout = &_stdout_file;
FILE* stderr = &_stderr_file;

// Basic function stubs for WASI compatibility
extern "C" {

// Stub implementations for missing functions
int fprintf(FILE* stream, const char* format, ...) {
    // Minimal implementation - just return 0 for now
    return 0;
}

int printf(const char* format, ...) {
    // Minimal implementation - just return 0 for now
    return 0;  
}

int fflush(FILE* stream) {
    return 0;
}

int fclose(FILE* stream) {
    return 0;
}

void perror(const char* s) {
    // Minimal implementation - no-op
}

char* strerror(int errnum) {
    return (char*)"Unknown error";
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return 0;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return 0;
}

int fgetc(FILE* stream) {
    return EOF;
}

int ungetc(int c, FILE* stream) {
    return EOF;
}

int fputs(const char* s, FILE* stream) {
    return 0;
}

int putc(int c, FILE* stream) {
    return c;
}

int putchar(int c) {
    return c;
}

void clearerr(FILE* stream) {
}

int feof(FILE* stream) {
    return 0;
}

int ferror(FILE* stream) {
    return 0;
}

FILE* fopen(const char* pathname, const char* mode) {
    return NULL;
}

}

#endif // __wasi__
