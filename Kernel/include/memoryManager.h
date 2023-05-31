#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <stddef.h>
#include <defs.h>

/**
 * @brief Initializes the memory manager
 *
 * @param memoryStart The initial location of the memory segment utilized by the memory manager.
 * @param memorySize The total number of bytes allocated for the memory manager starting from the initial address.
 */
void initializeMemory(void *memoryStart, size_t memorySize);

/**
 * @brief Request the memory manager to reserve a chunk of memory.
 *
 * @param size - The desired amount of memory requested.
 *
 * @return - A pointer to the reserved memory, or NULL if the operation failed.
 */
void *malloc(size_t size);

/**
 * @brief Notifies the memory manager that a previously allocated memory segment, reserved by my_malloc(),
 *        is now available to be marked as free and used elsewhere.
 *
 * @param ptr Pointer to the memory segment to be released.
 *
 * @return 0 if the operation is successful, 1 otherwise.
 */
int free(void *ptr);

/**
 * @brief Instructs the memory manager to modify the size of a previously allocated block of memory.
 *
 * @param ptr Pointer to the memory block that was previously allocated and needs to be resized.
 *
 * @param size The new size for the memory block.
 *
 * @return A pointer to the resized memory block, or NULL if the operation failed.
 */
void *realloc(void *ptr, size_t size);

/**
 * @brief Retrieves the current status of the system memory.
 *
 * @param memoryState Out struct to save the data.
 * @returns 0 if the operation is successful.
 */
int getStateMemory(MemoryState *memoryState);

#endif