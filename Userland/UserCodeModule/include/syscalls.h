#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>
#include <color.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define KBDIN 3

/* The width of a character in pixels. */
#define CHAR_WIDTH 9
/* The height of a character in pixels. */
#define CHAR_HEIGHT 16

uint64_t sys_read(uint64_t fd, char* buf, uint64_t count);
uint64_t sys_write(uint64_t fd, const char* buf, uint64_t count);
uint64_t sys_time(void);
uint64_t sys_date(void);
uint64_t sys_millis(void);
void sys_clearscreen(void);
uint32_t sys_writeat(const char* buf, uint64_t count, uint16_t x, uint16_t y, Color color);
uint64_t sys_screensize();
uint64_t sys_pollread(uint64_t fd, char* buf, uint64_t count, uint64_t timeout_ms);
void sys_drawpoint(uint16_t x, uint16_t y, Color color);
void sys_drawrect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
void sys_drawline(uint16_t fromX, uint16_t fromY, uint16_t toX, uint16_t toY, Color color);
uint8_t sys_inforeg(uint64_t reg[17]);

#endif
