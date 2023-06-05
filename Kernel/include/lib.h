#ifndef _LIB_H_
#define _LIB_H_

#include <defs.h>

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
 * @param value The value to be set (interpreted as an unsigned char).
 * @param length Number of bytes to be set.
 *
 * @returns - A pointer to the destination memory block (destination).
 */
void *memset(void *destination, int32_t value, size_t length);

/**
 * @brief Copies a block of memory from a source location to a destination location.
 *
 * @param destination Pointer to the destination memory block.
 * @param source Pointer to the source memory block.
 * @param length Number of bytes to be copied.
 *
 * @returns - A pointer to the destination memory block (dest).
 */
void *memcpy(void *destination, const void *source, size_t length);

/**
 * @brief Converts the given value into a number in the specified base and stores the result in the buffer.
 *
 * @param value The number to be converted.
 * @param buffer The destination memory area where the converted number will be stored.
 * @param base The base to which the number will be converted.
 *
 * @returns - The number of digits in the converted number.
 */
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

/**
 * @brief Converts a binary-coded decimal number into its decimal representation.
 * 
 * @param value The number to be converted.
 * 
 * @returns - The decimal representation of value.
 */
uint8_t bcdToDec(uint8_t value);

#endif
