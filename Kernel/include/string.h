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
 * @brief Copies the string pointed to by src, including the terminating null byte ('\0'), to the buffer  
 * pointed to by dest. The strings may not overlap, and the destination string dest must be large 
 * enough to receive the copy.
 *
 * @returns A pointer to the destination string dest.
 */    
char* strcpy(char* dest, const char* src);

/**
 * @brief Copies at most size bytes from the string pointed to by src, including the terminating null byte ('\0'), 
 * to the buffer pointed to by dest. The strings may not overlap, and the destination string dest must be large 
 * enough to receive the copy.
 *
 * @returns A pointer to the destination string dest.
 */   
char* strncpy(char* dest, const char* src, size_t size);

/**
 * @brief Compares the two strings s1 and s2.
 *
 * @returns An integer less than, equal to, or greater than zero if s1 is found, respectively, to be less than,
 * to match, or be greater than s2.
 */   
int strcmp(const char* s1, const char* s2);

/**
 * @brief Appends the src string to the dest string, overwriting the terminating null byte ('\0') at the end of dest,
 * and then adds a terminating null byte.
 *
 * @returns  A pointer to the resulting string dest.
 */   
char* strcat(char* dest, const char* src);

#endif