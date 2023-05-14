#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

#define BIT_SCANCODE_UP 0b10000000
#define BIT_SCANCODE_DOWN 0b00000000

/* Converts a scancode value to it's ASCII representation. Returns '\0' if there's no ASCII equivalent. */
char scancodeToAscii(uint8_t scancode);

/* Gets the amount of scancodes in the internal buffer waiting to be used. */
unsigned int kbd_getBufferLength();

/* Clears the internal scancode buffer. */
void kbd_clearBuffer();

/* Reads up to n scancodes from the internal buffer. Returns how many scancodes were read. */
unsigned int kbd_readScancodes(uint8_t* buf, unsigned int n);

/* Reads up to n characters from the internal buffer. Returns how many characters were read. */
unsigned int kbd_readCharacters(char* buf, unsigned int n);

#endif
