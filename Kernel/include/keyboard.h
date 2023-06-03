#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <defs.h>

/**
 * @brief Initializes the keyboard.
 */
void initializeKeyboard();

/**
 * @brief Invoked by the interrupt dispatcher when a keyboard interrupt is detected.
 */
void interruptHandlerKeyboard();

/**
 * @brief Tries to retrieve up to count characters from the keyboard buffer and copies them to the provided buffer.
 * 
 * @param bufferDestination The buffer that will store the characters read.
 * @param count Maximum characters to be read.
 * 
 * @returns - The number of characters successfully read.
 */
unsigned int readChars(char* bufferDestination, unsigned int count);

/**
 * @brief Tries to read a single character from the keyboard buffer.
 * 
 * @returns - The character read from the buffer, or -1 if the buffer was empty.
 */
int getChar();

/**
 * @brief Resets the keyboard buffer, making it empty until new keys are received.
 */
void clearKeyboard();

/**
 * @brief Add the keyboard into fd process table.
 * 
 * @param pid PID of the process.
 * @param fd File descriptor to add to the table.
 * 
 * @returns - The keyboard file descriptor, -1 in error cases.
 */
int addFdKeyboard(Pid pid, int fd);

#endif
