#ifndef __COSEC_TTY_H__
#define __COSEC_TTY_H__

#include <fs/devices.h>

#define CONSOLE_TTY     0

/* C0 control character set */
enum tty_ctrlchars {
    NUL = 0x00, SOH = 0x01, STX = 0x02, ETX = 0x04,
    EOT = 0x04, ENQ = 0x05, ACK = 0x06, BEL = 0x07,
    BS  = 0x08, TAB = 0x09, LF  = 0x0a, VT  = 0x0b,
    FF  = 0x0c, CR  = 0x0d, SO  = 0x0e, SI  = 0x0f,

    DLE = 0x10, DC1 = 0x11, DC2 = 0x12, DC3 = 0x13,
    DC4 = 0x14, NAK = 0x15, SYN = 0x16, ETB = 0x17,
    CAN = 0x18, EM  = 0x19, SUB = 0x1a, ESC = 0x1b,
    FS  = 0x1c, GS  = 0x1d, RS  = 0x1e, US  = 0x1f,

    DEL = 0x7f,
};

struct devclass;
struct devclass * get_tty_devclass(void);


int tty_write(mindev_t ttyno, const char *buf, size_t buflen);

int tty_read(mindev_t ttyno, char *buf, size_t buflen, size_t *written);

#include <dev/kbd.h>

void tty_keyboard_handler(scancode_t sc);

#endif // __COSEC_TTY_H__
