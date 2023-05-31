#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <stdint.h>
#include <defs.h>

/**
 * @struct color 
 * @brief Stores the color of a pixel in RGB format.
 */
typedef struct color {
    uint8_t R; /// < Red
    uint8_t G; /// < Green
    uint8_t B; /// < Black
} Color;

/* From graphicMode.c */
extern const Color RED;
extern const Color GREEN;
extern const Color BLUE;
extern const Color WHITE;
extern const Color GRAY;
extern const Color BLACK;

void initializeScreen();
void clearScreen();
void printLine();
void printChar(char c);
void printCharFormat(char c, const Color* charColor, const Color* bgColor);
void print(const char* string);
void printDec(uint64_t value);
void printHex(uint64_t value);
void printBin(uint64_t value);
void printBase(uint64_t value, uint32_t base);
void printRegisterFormat(uint64_t reg);
void restartCursor();

/**
 * @brief Add the screen into fd process table.
 * 
 * @param pid PID of the process.
 * @param fd File descriptor to add.
 * @param color Color resource to map to the screen.
 * @returns The screen file descriptor, -1 in error cases.
 */
int scrAddFd(Pid pid, int fd, const Color* color);

#endif
