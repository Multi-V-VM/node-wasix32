#ifndef	_TERMIOS_H
#define	_TERMIOS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_pid_t
#define __NEED_struct_winsize

#include <bits/alltypes.h>

typedef unsigned char cc_t;
typedef unsigned int speed_t;
typedef unsigned int tcflag_t;

#define NCCS 32

struct termios {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t c_line;
    cc_t c_cc[NCCS];
    speed_t c_ispeed;
    speed_t c_ospeed;
};

#include <bits/termios.h>

speed_t cfgetospeed (const struct termios *);
speed_t cfgetispeed (const struct termios *);
int cfsetospeed (struct termios *, speed_t);
int cfsetispeed (struct termios *, speed_t);

int tcgetattr (int, struct termios *);
int tcsetattr (int, int, const struct termios *);

int tcgetwinsize (int, struct winsize *);
int tcsetwinsize (int, const struct winsize *);

int tcsendbreak (int, int);
int tcdrain (int);
int tcflush (int, int);
int tcflow (int, int);

pid_t tcgetsid (int);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
void cfmakeraw(struct termios *);
int cfsetspeed(struct termios *, speed_t);
#endif

// Function stubs for WASI compatibility
int tcgetattr(int fd, struct termios* termios_p) {
    return -1; // Not supported in WASI
}

int tcsetattr(int fd, int actions, const struct termios* termios_p) {
    return -1; // Not supported in WASI
}

int tcsendbreak(int fd, int duration) {
    return -1; // Not supported in WASI
}

int tcdrain(int fd) {
    return -1; // Not supported in WASI
}

int tcflush(int fd, int queue_selector) {
    return -1; // Not supported in WASI
}

int tcflow(int fd, int action) {
    return -1; // Not supported in WASI
}

void cfmakeraw(struct termios* termios_p) {
    // Stub implementation
}

speed_t cfgetospeed(const struct termios* termios_p) {
    return B9600; // Default stub value
}

speed_t cfgetispeed(const struct termios* termios_p) {
    return B9600; // Default stub value
}

int cfsetospeed(struct termios* termios_p, speed_t speed) {
    return -1; // Not supported in WASI
}

int cfsetispeed(struct termios* termios_p, speed_t speed) {
    return -1; // Not supported in WASI
}

int cfsetspeed(struct termios* termios_p, speed_t speed) {
    return -1; // Not supported in WASI
}

#ifdef __cplusplus
}
#endif

#endif
