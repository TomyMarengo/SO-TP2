#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Concatenates the source string `src` to the destination string `dest` and returns a pointer to the resulting string.
 *
 * @param dest Pointer to the destination string.
 * @param src Pointer to the source string.
 * @return - Pointer to the resulting string (same as `dest`).
 */
char *strcat(char *dest, const char *src);

/**
 * @brief Concatenates at most `n` characters from the source string `src` to the destination string `dest` and returns a pointer to the resulting string.
 *
 * @param dest Pointer to the destination string.
 * @param src Pointer to the source string.
 * @param n Maximum number of characters to concatenate.
 * @return - Pointer to the resulting string.
 */
char *strncat(char *dest, const char *src, uint64_t n);

/**
 * @brief Finds the first occurrence of the character `c` in the string `str` and returns a pointer to that location.
 *
 * @param str Pointer to the input string.
 * @param c Character to be searched for.
 * @return - Pointer to the first occurrence of `c` in `str`, or NULL if `c` is not found.
 */
const char *strchr(const char *str, char c);

/**
 * @brief Compares two strings `str1` and `str2` and returns an integer indicating their relative order.
 *
 * @param str1 Pointer to the first string.
 * @param str2 Pointer to the second string.
 * @return - Integer value less than, equal to, or greater than zero if `str1` is found to be less than, equal to, or greater than `str2` respectively.
 */
int strcmp(const char *str1, const char *str2);

/**
 * @brief Compares at most `n` characters of two strings `str1` and `str2` and returns an integer indicating their relative order.
 *
 * @param str1 Pointer to the first string.
 * @param str2 Pointer to the second string.
 * @param n Maximum number of characters to compare.
 * @return - Integer value less than, equal to, or greater than zero if `str1` is found to be less than, equal to, or greater than `str2` respectively.
 */
int strncmp(const char *str1, const char *str2, uint64_t n);

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
 * @brief Converts a uint64_t into a 16-char hexadecimal number.
 *
 * @param n Integer to convert.
 * @param buf Buffer to store the string.
 */
void uint64ToHex(uint64_t n, char buf[16]);

/**
 * @brief Attempts to convert a hex string with 0x prefix into a memory address.
 *
 * @param s Hex string to convert.
 * @param result Buffer to store the memory address.
 * @return 0 if the operation failed, 1 otherwise.
 */
int tryReadHexAddress(char *s, uint8_t **result);

#endif
