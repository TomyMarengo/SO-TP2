#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <stdint.h>
#include <defs.h>

typedef struct color {
    uint8_t R;
    uint8_t G;
    uint8_t B;
} Color;

extern const Color RED;
extern const Color WHITE;
extern const Color BLACK;

/**
 * @brief Initializes the screen display.
 */
void initializeScreen();

/**
 * @brief Clears the screen.
 */
void clearScreen();

/**
 * @brief Prints line into screen.
 */
void printLine();

/**
 * @brief Prints a char into screen.
 * 
 * @param character Char printed.
 */
void printChar(char character);

/**
 * @brief Prints a char with format into screen.
 * 
 * @param character Char printed.
 * @param charColor Color of the char.
 * @param bgColor Color of the background.
 */
void printCharFormat(char character, const Color* charColor, const Color* bgColor);

/**
 * @brief Prints a string into screen.
 * 
 * @param string String printed.
 */
void print(const char* string);

/**
 * @brief Prints a number in decimal format into screen.
 * 
 * @param number Number printed.
 */
void printDec(uint64_t number);

/**
 * @brief Prints a number in hexa format into screen.
 * 
 * @param number Number printed.
 */
void printHex(uint64_t number);

/**
 * @brief Prints a number in binary format into screen.
 * 
 * @param number Number printed.
 */
void printBin(uint64_t number);

/**
 * @brief Prints a number in the base format into screen.
 * 
 * @param number Number printed.
 * @param base Final base.
 */
void printBase(uint64_t number, uint32_t base);

/**
 * @brief Prints in register format into screen.
 * 
 * @param reg Value printed.
 */
void printRegisterFormat(uint64_t reg);

/**
 * @brief Restarts screen cursor.
 */
void restartCursor();

/**
 * @brief Add the screen into fd process table.
 * 
 * @param pid PID of the process.
 * @param fd File descriptor to add.
 * @param color Color resource to map to the screen.
 * 
 * @return - The screen file descriptor, -1 in error cases.
 */
int addFdScreen(Pid pid, int fd, const Color* color);

#endif
