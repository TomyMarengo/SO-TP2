#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <stdint.h>
#include <stddef.h>

void* memset(void* destination, uint8_t c, size_t length);
void* memcpy(void* destination, const void* source, size_t length);

#endif