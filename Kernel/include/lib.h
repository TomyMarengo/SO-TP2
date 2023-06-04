#ifndef _LIB_H_
#define _LIB_H_

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
void *memset(void *destination, int32_t c, size_t length);

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
 * @brief Returns the output from executing the cpuid instruction.
 */
char *cpuVendor(char *result);

/**
 * @brief Transforms the value to a number in the base recived as parameter, and stores the result in the buffer.
 *
 * @param value the number to transform.
 * @param buffer memory area destination, where the number will be stored.
 * @param base base to convert the numbre to.
 *
 * @return The digits of the number.
 */
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

/**
 * @brief Converts a binary-coded decimal number into a decimal number in terms of representation.
 */
uint8_t bcdToDec(uint8_t time);

#endif
