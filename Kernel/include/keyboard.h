#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <defs.h>

/**
 * @brief Called by the interrupt dispatcher whenever a keyboard interrupt occurs.
 */
void kbdInterruptHandler();

/**
 * @brief Attempts to read up to count chars from the keyboard buffer, and write
 * them to the given buffer.
 * 
 * @returns The amount of chars read.
 */
unsigned int readChars(char* buf, unsigned int count);

/**
 * @brief Attempts to read a single char from the keyboard buffer.
 * 
 * @returns The char read from the buffer, or -1 if the buffer was empty.
 */
int getChar();

/**
 * @brief Clears the keyboard buffer, setting it to empty until new keys arrive.
 */
void clearKeyboard();

/**
 * @brief Add the keyboard into fd process table.
 * 
 * @param pid PID of the process.
 * @param fd File descriptor to add to the table.
 * @returns The keyboard file descriptor, -1 in error cases.
 */
int kbdAddFd(Pid pid, int fd);

#endif
