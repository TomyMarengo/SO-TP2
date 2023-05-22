#include "string.h"
#include <color.h>
#include <coreUtil.h>
#include <stdarg.h>
#include <syscalls.h>

static uint32_t penpos = 0;

static char scancodeToAsciiTable[] = {0,   27,  '1', '2', '3', '4', '5', '6', '7',  '8', '9', '0',  '-',  '=', '\b', '\t',
                                      'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o',  'p', '[', ']',  '\n', 0,   'a',  's',
                                      'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\', 'z',  'x', 'c',  'v',
                                      'b', 'n', 'm', ',', '.', '/', 0,   ' ', 0,    ' ', 0,   0,    0,    0,   0,    0,
                                      0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   '-', 0,    0,    0,   '+'};

#define SCANCODE_ARR_LENGTH (sizeof(scancodeToAsciiTable) / sizeof(scancodeToAsciiTable[0]))

char
scancodeToAscii(uint8_t scancode) {
    return scancode < SCANCODE_ARR_LENGTH ? scancodeToAsciiTable[scancode] : '\0';
}

void
getScreenSize(uint32_t *width, uint32_t *height) {
    uint64_t size = sys_screensize();
    *width = (size & 0xFFFFFFFF);
    *height = (size >> 32);
}

void
clearscreen(void) {
    sys_clearscreen();
    penpos = 0;
}

void
putchar(char c) {
    print(&c, 1, gray);
}

void
print(const char *buf, uint64_t count, Color color) {
    penpos = sys_writeat(buf, count, (penpos & 0xFFFF), (penpos >> 16), color);
}

char
getChar() {
    char c;
    uint16_t rectX = penpos & 0x0000FFFF;
    uint16_t rectY = (penpos >> 16) + 13;

    uint8_t isBlinkerOn = 1;
    sys_drawrect(rectX, rectY, 9, 3, gray);

    while (sys_pollread(STDIN, &c, 1, 500) == 0) {
        if (isBlinkerOn) {
            sys_drawrect(rectX, rectY, 9, 3, black);
            isBlinkerOn = 0;
        } else {
            sys_drawrect(rectX, rectY, 9, 3, gray);
            isBlinkerOn = 1;
        }
    }

    if (isBlinkerOn)
        sys_drawrect(rectX, rectY, 9, 3, black);

    return c;
}

void
scanf(char *readbuf, uint64_t maxlen) {
    if (maxlen == 0)
        return;

    // We read up to maxlen-1 characters from the input.
    uint64_t count = 0;
    do {
        char c = getChar();

        if (c == '\n') {  // If a '\n' is found, we zero-terminate the string and return.
            readbuf[MIN(count, maxlen - 1)] = '\0';
            print(&c, 1, gray);
            return;
        }
        if (c == '\b') {  // If a '\b' character is found, we remove the last char from readbuf.
            if (count != 0) {
                count--;
                uint32_t penX = penpos & 0x0000FFFF;
                uint32_t penY = penpos >> 16;
                if (penX < 9) {
                    if (penY != (uint32_t) 0)
                        penY -= 16;
                    penX = ((sys_screensize() & 0xFFFFFFFF) / 9 - 1) * 9;
                } else {
                    penX -= 9;
                }
                penpos = penX | (penY << 16);
                sys_drawrect(penX, penY, 9, 16, black);
            }
        } else {
            // We add the read character to the buffer and continue.
            print(&c, 1, gray);
            if (count < maxlen - 1)
                readbuf[count] = c;
            count++;
        }
    } while (1);
}

///

char *
convert(unsigned int num, unsigned int base, char *buff) {
    const char *representation = "0123456789ABCDEF";
    char *ptr;

    ptr = &buff[sizeof(buff) - 1];
    *ptr = '\0';

    do {
        *--ptr = representation[num % base];
        num /= base;
    } while (num != 0);

    return ptr;
}

void
printf(const char *frmt, ...) {
    va_list arg;
    va_start(arg, frmt);

    const char *aux;

    int i;
    unsigned int u;
    char *s;
    char tmpBuff[33];

    for (aux = frmt; *aux != '\0'; aux++) {
        while (*aux != '%') {
            if (*aux == '\0') {
                va_end(arg);
                return;
            }
            putchar(*aux);
            aux++;
        }
        aux++;

        switch (*aux) {
        case 'c':
            i = va_arg(arg, int);
            putchar(i);
            break;

        case 'd':
            i = va_arg(arg, int);
            if (i < 0) {
                i = -i;
                putchar('-');
            }
            print(convert(i, 10, tmpBuff), strlen(convert(i, 10, tmpBuff)), gray);
            break;

        case 'o':
            u = va_arg(arg, unsigned int);
            print(convert(u, 8, tmpBuff), strlen(convert(u, 8, tmpBuff)), gray);
            break;

        case 's':
            s = va_arg(arg, char *);
            print(s, strlen(s), gray);
            break;

        case 'u':
            u = va_arg(arg, unsigned int);
            print(convert(u, 10, tmpBuff), strlen(convert(u, 10, tmpBuff)), gray);
            break;

        case 'x':
            u = va_arg(arg, unsigned int);
            print(convert(u, 16, tmpBuff), strlen(convert(u, 16, tmpBuff)), gray);
            break;

        case '%':
            putchar('%');
            break;
        }
    }

    va_end(arg);
}