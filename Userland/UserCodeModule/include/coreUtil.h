#ifndef _COREUTIL_H_
#define _COREUTIL_H_

#include <stdint.h>
#include <color.h>

#define UP 0x48
#define DOWN 0x50
#define LEFT 0x4B
#define RIGHT 0x4D
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

char scancodeToAscii(uint8_t scancode);

void getScreenSize(uint32_t* width, uint32_t* height);

void clearscreen(void);
void putchar(char c);
void print(const char* buf, uint64_t count, Color color);
char getChar(void);
void scanf(char* readbuf, uint64_t maxLen);

#endif
