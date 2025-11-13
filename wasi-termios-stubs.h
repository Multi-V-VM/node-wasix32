#ifndef WASI_TERMIOS_STUBS_H_
#define WASI_TERMIOS_STUBS_H_

#ifdef __wasi__

// Terminal I/O compatibility stubs for WASI
// WASI doesn't support terminal I/O operations, so we provide stubs

#include <sys/types.h>

// Basic terminal structure
struct termios {
  unsigned long c_iflag;   // input mode flags
  unsigned long c_oflag;   // output mode flags
  unsigned long c_cflag;   // control mode flags
  unsigned long c_lflag;   // local mode flags
  unsigned char c_cc[32];  // control characters
  speed_t c_ispeed;        // input speed
  speed_t c_ospeed;        // output speed
};

// Terminal control flags
#define B0      0x00000000
#define B50     0x00000001
#define B75     0x00000002
#define B110    0x00000003
#define B134    0x00000004
#define B150    0x00000005
#define B200    0x00000006
#define B300    0x00000007
#define B600    0x00000008
#define B1200   0x00000009
#define B1800   0x0000000A
#define B2400   0x0000000B
#define B4800   0x0000000C
#define B9600   0x0000000D
#define B19200  0x0000000E
#define B38400  0x0000000F

#define CSIZE   0x00000010
#define CS5     0x00000010
#define CS6     0x00000020
#define CS7     0x00000030
#define CS8     0x00000040

#define CSTOPB  0x00000080
#define CREAD   0x00000100
#define PARENB  0x00000200
#define PARODD  0x00000400
#define HUPCL   0x00000800
#define CLOCAL  0x00001000

// Input mode flags
#define IGNBRK  0x00000001
#define BRKINT  0x00000002
#define IGNPAR  0x00000004
#define PARMRK  0x00000008
#define INPCK   0x00000010
#define ISTRIP  0x00000020
#define INLCR   0x00000040
#define IGNCR   0x00000080
#define ICRNL   0x00000100
#define IUCLC   0x00000200
#define IXON    0x00000400
#define IXANY   0x00000800
#define IXOFF   0x00001000
#define IMAXBEL 0x00002000
#define IUTF8   0x00004000

// Output mode flags
#define OPOST   0x00000001
#define OLCUC   0x00000002
#define ONLCR   0x00000004
#define OCRNL   0x00000008
#define ONOCR   0x00000010
#define ONLRET  0x00000020
#define OFILL   0x00000040
#define OFDEL   0x00000080
#define NLDLY   0x00000100
#define NL0     0x00000000
#define NL1     0x00000100
#define CRDLY   0x00000200
#define CR0     0x00000000
#define CR1     0x00000200
#define CR2     0x00000400
#define CR3     0x00000600
#define TABDLY  0x00000800
#define TAB0    0x00000000
#define TAB1    0x00000800
#define TAB2    0x00001000
#define TAB3    0x00001800
#define BSDLY   0x00002000
#define BS0     0x00000000
#define BS1     0x00002000
#define FFDLY   0x00008000
#define FF0     0x00000000
#define FF1     0x00008000

// Local mode flags
#define ISIG    0x00000001
#define ICANON  0x00000002
#define XCASE   0x00000004
#define ECHO    0x00000008
#define ECHOE   0x00000010
#define ECHOK   0x00000020
#define ECHONL  0x00000040
#define NOFLSH  0x00000080
#define TOSTOP  0x00000100
#define ECHOCTL 0x00000200
#define ECHOPRT 0x00000400
#define ECHOKE  0x00000800
#define FLUSHO  0x00001000
#define PENDIN  0x00002000
#define IEXTEN  0x00004000

// Control characters
#define VINTR    0
#define VQUIT    1
#define VERASE   2
#define VKILL    3
#define VEOF     4
#define VTIME    5
#define VMIN     6
#define VSWTC    7
#define VSTART   8
#define VSTOP    9
#define VSUSP    10
#define VEOL     11
#define VREPRINT 12
#define VDISCARD 13
#define VWERASE  14
#define VLNEXT   15
#define VEOL2    16

// ioctl commands
#define TCGETS     0x5401
#define TCSETS     0x5402
#define TCSETSW    0x5403
#define TCSETSF    0x5404
#define TCGETA     0x5405
#define TCSETA     0x5406
#define TCSETAW    0x5407
#define TCSETAF    0x5408
#define TCSBRK     0x5409
#define TCXONC     0x540A
#define TCFLSH     0x540B
#define TIOCSWINSZ 0x5413
#define TIOCGWINSZ 0x5414

// Window size structure
struct winsize {
  unsigned short ws_row;
  unsigned short ws_col;
  unsigned short ws_xpixel;
  unsigned short ws_ypixel;
};

// Function prototypes - all return error since WASI doesn't support terminals
static inline int tcgetattr(int fd, struct termios *termios_p) {
  (void)fd; (void)termios_p;
  return -1; // Not supported in WASI
}

static inline int tcsetattr(int fd, int optional_actions, const struct termios *termios_p) {
  (void)fd; (void)optional_actions; (void)termios_p;
  return -1; // Not supported in WASI
}

static inline int tcsendbreak(int fd, int duration) {
  (void)fd; (void)duration;
  return -1; // Not supported in WASI
}

static inline int tcdrain(int fd) {
  (void)fd;
  return -1; // Not supported in WASI
}

static inline int tcflush(int fd, int queue_selector) {
  (void)fd; (void)queue_selector;
  return -1; // Not supported in WASI
}

static inline int tcflow(int fd, int action) {
  (void)fd; (void)action;
  return -1; // Not supported in WASI
}

static inline pid_t tcgetsid(int fd) {
  (void)fd;
  return (pid_t)-1; // Not supported in WASI
}

static inline int cfgetispeed(const struct termios *termios_p) {
  (void)termios_p;
  return B9600; // Default speed
}

static inline int cfgetospeed(const struct termios *termios_p) {
  (void)termios_p;
  return B9600; // Default speed
}

static inline int cfsetispeed(struct termios *termios_p, speed_t speed) {
  (void)termios_p; (void)speed;
  return 0; // Success (no-op)
}

static inline int cfsetospeed(struct termios *termios_p, speed_t speed) {
  (void)termios_p; (void)speed;
  return 0; // Success (no-op)
}

#endif // __wasi__

#endif // WASI_TERMIOS_STUBS_H_
