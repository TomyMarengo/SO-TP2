#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Aligns the provided integer value downwards to the closest multiple of 8.
 */
#define WORD_ALIGN_DOWN(value) ((value) & (~(size_t) 0x07))

/**
 * @brief Aligns the provided integer value upwards to the closest multiple of 8.
 */
#define WORD_ALIGN_UP(value) (WORD_ALIGN_DOWN((size_t) (value) + 7))

/**
 * @brief Fills a block of memory with a specified value.
 *
 * @param destination Pointer to the destination memory block.
 * @param c The value to be set (interpreted as an unsigned char).
 * @param length Number of bytes to be set.
 *
 * @return A pointer to the destination memory block (dest).
 */
void *memset(void *destination, uint8_t c, size_t length);

/**
 * @brief Copies a block of memory from a source location to a destination location.
 *
 * @param destination Pointer to the destination memory block.
 * @param source Pointer to the source memory block.
 * @param length Number of bytes to be copied.
 *
 * @return A pointer to the destination memory block (dest).
 */
void *memcpy(void *destination, const void *source, size_t length);

/**
 * @brief Calculates the length of the string `s` (excluding the null-terminating character) and returns the length as a `uint64_t` value.
 *
 * @param s Pointer to the input string.
 * @return - Length of the string.
 */
uint64_t strlen(const char *s);

/**
 * @brief Converts integers to strings.
 *
 * @param number Integer to convert.
 * @param s Buffer to store the string.
 * @return - Number of digits converted.
 */
uint64_t itoa(uint64_t number, char *s);

/**
 * @brief Copies the string `src` to `dest` including the null-terminating character and returns a pointer to `dest`.
 *
 * @param dest Pointer to the destination string.
 * @param src Pointer to the source string.
 * @return - Pointer to the destination string.
 */
char *strcpy(char *dest, const char *src);

/**
 * @brief Copies at most `n` characters from the string `src` to `dest` and returns a pointer to `dest`.
 *
 * @param dest Pointer to the destination string.
 * @param src Pointer to the source string.
 * @param n Maximum number of characters to copy.
 * @return - Pointer to the destination string.
 */
char *strncpy(char *dest, const char *src, uint64_t n);

#endif