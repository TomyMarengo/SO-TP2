#ifndef _KERNEL_TYPES_H_
#define _KERNEL_TYPES_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Represents the various categories of supported memory managers.
 */
typedef enum { LIST, BUDDY } TMemoryManagerType;

/**
 * @brief Reflects the condition of the system memory at a specific moment.
 */
typedef struct {
    size_t total;
    size_t used;
    TMemoryManagerType type;
    unsigned int chunks;
} TMemoryState;

#endif