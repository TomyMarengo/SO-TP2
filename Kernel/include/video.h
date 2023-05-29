#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <stdint.h>
#include <process.h>
#include <defs.h>

/**
 * @brief The width of a character in pixels.
 */
#define CHAR_WIDTH 9

/**
 * @brief The height of a character in pixels.
 */
#define CHAR_HEIGHT 16


/**
 * @brief Structure of a color in RGB (b,g,r).
 */
typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} Color;

/**
 * @brief Gets the width of the screen in pixels.
 *
 * @return The width of the screen in pixels.
 */
uint16_t scr_getWidth(void);

/**
 * @brief Gets the height of the screen in pixels.
 *
 * @return The height of the screen in pixels.
 */
uint16_t scr_getHeight(void);

/**
 * @brief Gets the X position of the writting pen.
 *
 * @return The X position of the writting pen.
 */
uint16_t scr_getPenX(void);

/**
 * @brief Gets the Y position of the writting pen.
 *
 * @return The Y position of the writting pen.
 */
uint16_t scr_getPenY(void);

/**
 * @brief Clears the whole screen to black.
 */
void scr_clear(void);

/**
 * @brief Sets the color of a specific pixel on the screen.
 *
 * @param x X coordinate.
 * @param y Y coordinate
 * @param color Color to draw.
 */
void scr_setPixel(uint16_t x, uint16_t y, Color color);

/**
 * @brief Sets a specified rectangle of pixels on the screen to the specified color.
 *
 * @param x X coordinate of the top left corner.
 * @param y Y coordinate of the top left corner.
 * @param width Width from the top left corner to top right corner.
 * @param height Height from the top left corner to bottom left corner.
 * @param color Color to draw.
 */
void scr_drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);

/**
 * @brief Sets a specified rectangle of pixels on the screen to the specified color.
 *
 * @param fromX X coordinate of the start of the line.
 * @param fromY Y coordinate of the start of the line
 * @param toX X coordinate of the end of the line.
 * @param toY Y coordinate of the end of the line.
 * @param color Color to draw.
 */
void scr_drawLine(uint16_t fromX, uint16_t fromY, uint16_t toX, uint16_t toY, Color color);

/**
 * @brief Sets the pen position for drawing characters on the screen as a console.
 *
 * @param x X coordinate to set.
 * @param y Y coordinate to set.
 */
void scr_setPenPosition(uint16_t x, uint16_t y);

/**
 * @brief Sets the pen color for drawing characters on the screen as a console.
 *
 * @param color Color to set.
 */
void scr_setPenColor(Color color);

/**
 * @brief Advances the pen to the beginning of the next line.
 */
void scr_printNewline(void);

/**
 * @brief Prints a single character with the pen, wrapping around the end of the screen and pushing old lines up if necessary.
 *
 * @param c Character to print
 */
void scr_printChar(char c);

/**
 * @brief Prints a string of characters with the pen, wrapping around the end of the screen and pushing old lines up if necessary.
 *
 * @param s String to print
 * @return - The new pen position as a 32 bit number, where the 16 lowest bits are the x and the upper 16 bits are the y.
 */
uint32_t scr_print(const char *s);

/**
 * @brief Prints a string of characters with scr_print(s) followed by a newline.
 *
 * @param s String to print
 * @return - The new pen position as a 32 bit number, where the 16 lowest bits are the x and the upper 16 bits are the y.
 */
uint32_t scr_println(const char *s);

/**
 * @brief Add the screen into fd process table.
 * 
 * @param pid PID of the process.
 * @param fd File descriptor to add.
 * @param color Color resource to map to the screen.
 * @returns The screen file descriptor, -1 in error cases.
 */
int scr_addFd(TPid pid, int fd, const Color* color);

#endif
