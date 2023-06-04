#ifndef _STRING_H_
#define _STRING_H_

#include <defs.h>

/**
 * @brief Calculates the length of the string `s` (excluding the null-terminating character) and returns the length as a `uint64_t` value.
 *
 * @param str Pointer to the input string.
 * 
 * @returns - Length of the string.
 */
size_t strlen(const char* str);

/**
 * @brief Converts integers to strings.
 *
 * @param number Integer to convert.
 * @param buffer Buffer to store the string.
 * 
 * @returns - Number of digits converted.
 */
uint64_t itoa(uint64_t number, char *buffer);

/**
 * @brief Copies the string `src` to `dest` including the null-terminating character and returns a pointer to `dest`.
 *
 * @param destination Pointer to the destination string.
 * @param source Pointer to the source string.
 * 
 * @returns - Pointer to the destination string.
 */
char* strcpy(char* destination, const char* source);

/**
 * @brief Copies at most `n` characters from the string `src` to `dest` and returns a pointer to `dest`.
 *
 * @param destination Pointer to the destination string.
 * @param source Pointer to the source string.
 * @param limit Maximum number of characters to copy.
 * 
 * @returns - Pointer to the destination string.
 */
char* strncpy(char* destination, const char* source, size_t limit);

/**
 * @brief Compares two strings character by character. 
 *
 * @param str1 A string.
 * @param str2 A string.
 * 
 * @returns - If the strings are equal, the function returns 0.
 */
int strcmp(const char* str1, const char* str2);

/**
 * @brief Appends the source string to the destination string.
 *  
 * @param destination Pointer to the destination string.
 * @param source Pointer to the source string.
 * 
 * @returns - A pointer to the resulting string.
 */   
char* strcat(char* destination, const char* source);

#endif