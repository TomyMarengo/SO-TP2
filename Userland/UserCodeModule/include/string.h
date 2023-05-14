#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <stdint.h>
#define NULL 0

char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, uint64_t n);
const char* strchr(const char* str, char c);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, uint64_t n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, uint64_t n);
uint64_t strlen(const char* s);

uint64_t itoa(uint64_t number, char* s);

/* Converts a uint64_t into a 16-char hexadecimal number. */
void uint64ToHex(uint64_t n, char buf[16]);

/* Attempts to convert a hex string with 0x prefix into a memory address. Returns 0 if the operation failed, 1 otherwise. */
int tryReadHexAddress(char* s, uint8_t** result);

#endif
