#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <stdint.h>
#include <stddef.h>
#define WORD_ALIGN_DOWN(value) ((value) & (~(size_t)0x07))
#define WORD_ALIGN_UP(value) (WORD_ALIGN_DOWN((size_t)(value) + 7))

void* memset(void* destination, uint8_t c, size_t length);
void* memcpy(void* destination, const void* source, size_t length);

#endif