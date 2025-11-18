#ifndef _STDIO_H
#define _STDIO_H

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

// File structure
typedef struct {
    int _flags;
    char* _IO_read_ptr;
    char* _IO_read_end;
    char* _IO_read_base;
    char* _IO_write_base;
    char* _IO_write_ptr;
    char* _IO_write_end;
    char* _IO_buf_base;
    char* _IO_buf_end;
    char* _IO_save_base;
    char* _IO_backup_base;
    char* _IO_save_end;
    void* _markers;
    void* _chain;
    int _fileno;
    int _flags2;
    void* _old_offset;
    short _cur_column;
    signed char _vtable_offset;
    char* _shortbuf;
    void* _lock;
    int _offset;
    void* _codecvt;
    void* _wide_data;
    void* _freeres_list;
    size_t _freeres_buf;
    size_t __pad5;
    int _mode;
    char _unused2[15 * sizeof(void*) - 2 * sizeof(size_t)];
} FILE;

// EOF constant
#define EOF (-1)

// Standard file descriptors
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

// Standard FILE pointers
extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

// Basic function declarations
int printf(const char* format, ...);
int fprintf(FILE* stream, const char* format, ...);
int sprintf(char* str, const char* format, ...);
int snprintf(char* str, size_t size, const char* format, ...);

int scanf(const char* format, ...);
int fscanf(FILE* stream, const char* format, ...);
int sscanf(const char* str, const char* format, ...);

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);

int fgetc(FILE* stream);
int fputc(int c, FILE* stream);
char* fgets(char* s, int size, FILE* stream);
int fputs(const char* s, FILE* stream);

int getchar(void);
int putchar(int c);
char* gets(char* s);
int puts(const char* s);

int fseek(FILE* stream, long offset, int whence);
long ftell(FILE* stream);
void rewind(FILE* stream);

int fgetpos(FILE* stream, void* pos);
int fsetpos(FILE* stream, const void* pos);

int fflush(FILE* stream);
int fclose(FILE* stream);
FILE* fopen(const char* pathname, const char* mode);
FILE* freopen(const char* pathname, const char* mode, FILE* stream);
FILE* fdopen(int fd, const char* mode);

void clearerr(FILE* stream);
int feof(FILE* stream);
int ferror(FILE* stream);
void perror(const char* s);

char* strerror(int errnum);

int remove(const char* pathname);
int rename(const char* oldpath, const char* newpath);
FILE* tmpfile(void);
char* tmpnam(char* str);

#ifdef __cplusplus
}
#endif

#endif // _STDIO_H
