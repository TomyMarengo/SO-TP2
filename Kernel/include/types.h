#ifndef _KERNEL_TYPES_H_
#define _KERNEL_TYPES_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Represents the different types of supported memory managers.
 */
typedef enum { NODE, BUDDY } TMemoryManagerType;

/**
 * @brief Represents the state of the system memory at a given point in time.
 */
typedef struct {
    size_t total;
    size_t used;
    TMemoryManagerType type;
    unsigned int chunks;
} TMemoryState;

#endif