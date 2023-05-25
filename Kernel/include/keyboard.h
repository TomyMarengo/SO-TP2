#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>
#include <defs.h>

#define BIT_SCANCODE_UP   0b10000000
#define BIT_SCANCODE_DOWN 0b00000000

/**
 * @brief Converts a scancode value to it's ASCII representation.
 *
 * @param scancode The scancode to convert.
 * @return - '\0' if there's no ASCII equivalent.
 */
char scancodeToAscii(uint8_t scancode);

/**
 * @brief Gets the amount of scancodes in the internal buffer waiting to be used.
 *
 * @return - The amount of scancodes in the internal buffer waiting to be used.
 */
unsigned int kbd_getBufferLength();

/**
 * @brief Clears the internal scancode buffer.
 */
void kbd_clearBuffer();

/**
 * @brief Reads up to n scancodes from the internal buffer.
 *
 * @param buf The internal buffer.
 * @param n Limit to be read.
 * @return - How many scancodes were read.
 */
unsigned int kbd_readScancodes(uint8_t *buf, unsigned int n);

/**
 * @brief Reads up to n characters from the internal buffer.
 *
 * @param buf The internal buffer.
 * @param n Limit to be read.
 * @return - How many characters were read.
 */
unsigned int kbd_readCharacters(char *buf, unsigned int n);

/**
 * @brief Add the keyboard into fd process table.
 * 
 * @param pid PID of the process.
 * @param fd File descriptor to add to the table.
 * @returns The keyboard file descriptor, -1 in error cases.
 */
int kbd_addFd(Pid pid, int fd);

#endif
