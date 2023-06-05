#ifndef _USERLIB_H_
#define _USERLIB_H_

#include <defs.h>

uint32_t getUint();
uint32_t getUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
void *memset(void *destination, int32_t c, size_t length);
int64_t satoi(char *str);

/**
 * @brief Delay for a specified amount of milliseconds
 */
void sleep(unsigned long millis);

/**
 * @brief Reads the next character from STDIN
 */
int getChar();

/**
 * @brief Writes the character into STODUT
 */
int putChar(char c);

/**
 * @brief Reads the next character from the given output stream
 */
int fgetChar(int fd);

/**
 * @brief Writes the character to the given output stream
 */
int fputChar(int fd, char c);

/**
 * @brief Writes a string into STDOUT
 */
int print(const char *str);

/**
 * @brief Writes a string to the given output stream
 */
int fprint(int fd, const char *str);

/**
 * @brief Produces output according to a format
 */
void printf(const char *frmt, ...);

/**
 * @brief Writes output to the given output stream
 */
void fprintf(int fd, const char *frmt, ...);

/**
 * @brief Convert a string to an integer
 */
int atoi(const char *str);

/**
 * @brief Reads an entire line from STDIN. The buffer is null-terminated.
 */
int getLine(char *buffer, int maxSize);

/**
 * @brief Reads an entire line from the given output stream. The buffer is null-terminated.
 */
int fgetLine(int fd, char *buffer, int maxSize);

uint8_t processExists( Pid pid );

void intToStr(int num, char str[]);

#endif
