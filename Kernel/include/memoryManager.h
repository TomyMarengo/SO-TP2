#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

/* Standard library */
#include <stddef.h>
#include <stdint.h>
#include <types.h>
#include <video.h>

/* Local headers */
#include "string.h"

void my_init(void *memoryStart, size_t memorySize);

void *my_malloc(size_t size);

void *my_realloc(void *ptr, size_t size);

int my_free(void *ptr);

int my_getState(TMemoryState *memoryState);

#endif