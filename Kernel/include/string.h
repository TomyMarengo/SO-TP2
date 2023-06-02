#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Calculates the length of the string `s` (excluding the null-terminating character) and returns the length as a `uint64_t` value.
 *
 * @param s Pointer to the input string.
 * @return - Length of the string.
 */
size_t strlen(const char* s);

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
char* strcpy(char* dest, const char* src);

/**
 * @brief Copies at most `n` characters from the string `src` to `dest` and returns a pointer to `dest`.
 *
 * @param dest Pointer to the destination string.
 * @param src Pointer to the source string.
 * @param n Maximum number of characters to copy.
 * @return - Pointer to the destination string.
 */
char* strncpy(char* dest, const char* src, size_t n);

/**
 * @brief Compares two strings character by character. 
 *
 * @param s1 A string.
 * @param s2 A string.
 * @return - If the strings are equal, the function returns 0.
 */
int strcmp(const char* s1, const char* s2);

/**
 * @brief Appends the s1 string to the s2 string.
 *  
 * @param s1 A string.
 * @param s2 A string.
 * @return - A pointer to the resulting string dest.
 */   
char* strcat(char* s1, const char* s2);

#endif