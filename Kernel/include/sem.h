#ifndef _SEM_H_
#define _SEM_H_

#include <defs.h>

/**
 * @brief Initialize the semaphore system.
 * 
 * @returns - 0 if the operation succeeded, -1 otherwise.
 *
 */
int initializeSem();

/**
 * @brief Creates a named semaphore, or opens if exists. 
 * 
 * @param name Semaphore's name.
 * @param initialValue Semaphore's initial value.
 * 
 * @returns - The semaphore, or -1 if the operation failed.
 */
Sem openSem(const char * name, uint8_t initialValue);

/**
 * @brief Deallocates a semaphore. The semaphore will be destroyed once all
 * the processes that were linked to it have closed it.
 * 
 * @param sem The semaphore (returned in openSem).
 * 
 * @returns - 0 if the operation is successful or -2 if the requested semaphore does not exist.
 */
int closeSem(Sem sem);

/**
 * @brief Decrements or locks the semaphore pointed to by sem parameter.
 * 
 * @param sem The semaphore (returned in openSem).
 * 
 * @returns - 0 if the operation is successful or -2 if the requested semaphore does not exist.
 */
int wait(Sem sem);

/**
 * @brief Increments or unlocks the semaphore pointed to by sem.
 * 
 * @param sem The semaphore (returned in openSem).
 * 
 * @returns - 0 if the operation is successful or -2 if the requested semaphore does not exist.
 */
int post(Sem sem);

/**
 * @brief Gets (maxSemaphores)-amount information of semaphores.
 * 
 * @param storingInfo Array of SemaphoreInfo to archive the data.
 * @param maxSemaphores Limit.
 * 
 * @returns - The amount of semaphores archived.
 */
int listSemaphores(SemaphoreInfo* storingInfo, int maxSemaphores);

#endif