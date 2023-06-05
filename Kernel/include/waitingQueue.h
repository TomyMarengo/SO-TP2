#ifndef _WAITING_QUEUE_ADT_
#define _WAITING_QUEUE_ADT_

#include <defs.h>

typedef struct WaitingQueueData *WaitingQueue;

/**
 * @brief Creates a new waiting queue instance.
 *
 * @returns - The created wait queue instance, or NULL if the operation fails.
 */
WaitingQueue newQueue();

/**
 * @brief Frees all resources used by a waiting queue.
 * 
 * @param queue The waiting queue to be freed.
 *
 * @returns - 0 if the operation is successful, 1 otherwise.
 */
int freeQueue(WaitingQueue queue);

/**
 * @brief Adds a process ID (PID) to the waiting queue. If the PID was already in the queue,
 * this operation will add a new entry, resulting in multiple occurrences of the PID in the queue.
 * 
 * @param queue The waiting queue.
 * @param pid PID of the process.
 * 
 * @returns - 0 if the operation successful, 1 otherwise.
 */
int addInQueue(WaitingQueue queue, Pid pid);

/**
 * @brief Gets the length of the queue.
 * 
 * @param queue The waiting queue.
 * 
 * @returns - The amount of entries (length).
 */
int entriesInQueue(WaitingQueue queue);

/**
 * @brief Gets if the pid is in the queue or not.
 *  
 * @param queue The waiting queue.
 * @param pid PID of the process.
 *
 * @returns - 1 if the given PID is contained in the queue, 0 otherwise.
 */
int containsInQueue(WaitingQueue queue, Pid pid);

/**
 * @brief Adds a PID to the waiting queue if it was not already in it.
 *  
 * @param queue The waiting queue.
 * @param pid PID of the process.
 *
 * @returns - 0 if the operation successful, 1 otherwise.
 */
int addIfNotExistsInQueue(WaitingQueue queue, Pid pid);

/**
 * @brief Removes a PID from the waiting queue.
 *
 * @param queue The waiting queue.
 * @param pid PID of the process.
 * 
 * @returns - 0 if the operation successful, 1 otherwise.
 */
int removeInQueue(WaitingQueue queue, Pid pid);

/**
 * @brief Unblocks a single process on the queue.
 *
 * @param queue The waiting queue.
 * 
 * @returns - 0 if the operation successful, 1 otherwise.
 */
int unblockInQueue(WaitingQueue queue);

/**
 * @brief Unblocks all processes on the queue.
 *
 * @param queue The waiting queue.
 * 
 * @returns - 0 if the operation successful, 1 otherwise.
 */
int unblockAllInQueue(WaitingQueue queue);

/**
 *
 * @brief Gets (maxPids)-amount of pids.
 * 
 * @param queue The waiting queue.
 * @param storingInfo Array of Pid to archive the data.
 * @param maxPids Limit.
 * 
 * @returns - The amount of PIDs written to the storingInfo array.
 */
int listPidsInQueue(WaitingQueue queue, Pid *storingInfo, int maxPids);

#endif