#include "stdio.h"
#include "errno.h"

// Global FILE objects
FILE _stdin_file = {._flags = 0, ._fileno = 0};
FILE _stdout_file = {._flags = 0, ._fileno = 1};
FILE _stderr_file = {._flags = 0, ._fileno = 2};

FILE* stdin = &_stdin_file;
FILE* stdout = &_stdout_file;
FILE* stderr = &_stderr_file;

// Minimal implementations - for WASI compatibility
extern "C" {

int printf(const char* format, ...) {
    // stub - would need va_list processing
    return 1;
}

int fprintf(FILE* stream, const char* format, ...) {
    // stub - would need va_list processing  
    return 1;
}

int sprintf(char* str, const char* format, ...) {
    // stub - would need va_list processing
    return 0;
}

int scanf(const char* format, ...) {
    // stub
    return 0;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    // stub - WASI file I/O would go here
    return 0;
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream) {
    // stub - WASI file I/O would go here
    return 0;
}

int fgetc(FILE* stream) {
    // stub
    return EOF;
}

int fputc(int c, FILE* stream) {
    return c;
}

char* fgets(char* s, int size, FILE* stream) {
    if (size <= 0) return nullptr;
    s[0] = '\0';
    return s;
}

int fputs(const char* s, FILE* stream) {
    return 0;
}

int getchar(void) {
    return EOF;
}

int putchar(int c) {
    return c;
}

int puts(const char* s) {
    return 1;
}

int fseek(FILE* stream, long offset, int whence) {
    return 0;
}

long ftell(FILE* stream) {
    return 0;
}

void rewind(FILE* stream) {
    fseek(stream, 0, 0);
}

int fflush(FILE* stream) {
    return 0;
}

int fclose(FILE* stream) {
    return 0;
}

FILE* fopen(const char* pathname, const char* mode) {
    return nullptr;
}

void clearerr(FILE* stream) {
}

int feof(FILE* stream) {
    return 0;
}

int ferror(FILE* stream) {
    return 0;
}

void perror(const char* s) {
    // stub - would print to stderr
}

char* strerror(int errnum) {
    return (char*)"Error";
}

int remove(const char* pathname) {
    return -1;
}

int rename(const char* oldpath, const char* newpath) {
    return -1;
}

FILE* tmpfile(void) {
    return nullptr;
}

char* tmpnam(char* str) {
    static char buffer[] = "tmp";
    return buffer;
}

} // extern "C"
