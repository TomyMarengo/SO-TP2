#ifndef _COREUTIL_H_
#define _COREUTIL_H_

#include <color.h>
#include <stdint.h>

#define UP        0x48
#define DOWN      0x50
#define LEFT      0x4B
#define RIGHT     0x4D
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/**
 * @brief Converts scancode to ascii.
 *
 * @param scancode Scancode to convert.
 * @return - The corresponding ascii.
 */
char scancodeToAscii(uint8_t scancode);

/**
 * @brief Get the screen size.
 *
 * @param width Here the width will be stored (uint32_t *).
 * @param height Here the height will be stored (uint32_t *).
 */
void getScreenSize(uint32_t *width, uint32_t *height);

/**
 * @brief Clear screen to black.
 */
void clearscreen(void);

/**
 * @brief Print a character at the pen position.
 *
 * @param c Character to print.
 */
void putchar(char c);

/**
 * @brief Reads a single character from the standard input (stdin)
 *
 * @return - The ASCII value of the character read from the standard input.
 */
char getChar(void);

/**
 * @brief Print a string at the pen position.
 *
 * @param buf Buffer containing the string to print.
 * @param count Lenght of the string.
 * @param color Color used to print the string.
 */
void print(const char *buf, uint64_t count, Color color);

/**
 * @brief Prints formatted output at the pen position.
 *
 * @param frmt A pointer to a null-terminated format string.
 * @param ... Additional arguments corresponding to the format specifiers in the format string.
 */
void printf(const char *frmt, ...);

/**
 * @brief Reads input from the standard input.
 *
 * @param readbuf Buffer to store the read input.
 * @param maxLen Limit of characters to read.
 */
void scanf(char *readbuf, uint64_t maxLen);

/**
 * @brief Converts the given number num into a string representation in the specified base and stores the result in the buff buffer.
 *
 * @param num Given number.
 * @param base Specified base.
 * @param buff Temporary buffer.
 * @return - Pointer to the string representation.
 */
char *convert(unsigned int num, unsigned int base, char *buff);


#endif
