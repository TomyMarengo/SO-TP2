#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <stdint.h>

#define NULL 0

void* memset(void* destination, uint8_t c, uint64_t length);
void* memcpy(void* destination, const void* source, uint64_t length);

#endif