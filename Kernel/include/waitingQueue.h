#ifndef _WAITING_QUEUE_ADT_
#define _WAITING_QUEUE_ADT_

#include <defs.h>

typedef struct WaitingQueueData* WaitingQueue;

/**
 * @brief Creates a new wait queue instance.
 * 
 * @returns The created wait queue instance, or NULL of the operation failed.
 */
WaitingQueue newWQ();

/**
 * @brief Frees all resources used by a wait queue. Attempting to use a
 * wait queue after it has been freed results in undefined behavior.
 * 
 * @returns 0 if the operation succeeded, !=0 if not.
 */
int freeWQ(WaitingQueue wq);

/**
 * @brief Adds a PID to the waiting queue. If the PID was already in the queue,
 * this operation will add a new entry and have the PID multiple times on the queue.
 *  
 * @returns 0 if the operation succeeded, !=0 if not.
 */
int addWQ(WaitingQueue wq, Pid pid);

/**
 * @brief Gets the amount of entries on the queue.
 */
int entriesWQ(WaitingQueue wq);

/**
 * @brief Gets whether a given PID has an entry in the queue.
 * 
 * @returns 1 if the given PID is contained in the queue, 0 otherwise.
 */
int containsWQ(WaitingQueue wq, Pid pid);

/**
 * @brief Adds a PID to the waiting queue, only if it was not already in it.
 * 
 * @returns 0 if the operation succeeded. The PID already being on the queue is considered success.
 */
int addIfNotExistsWQ(WaitingQueue wq, Pid pid);

/**
 * @brief Removes a PID from the waiting queue.
 * 
 * @returns 0 if the operation succeeded. The PID not being on the queue is considered a failure.
 */
int removeWQ(WaitingQueue wq, Pid pid);

/**
 * @brief Unblocks a single process on the queue.
 * 
 * @returns 0 if the operation succeded. The queue being empty is not considered a failure.
 */
int unblockWQ(WaitingQueue wq);

/**
 * @brief Unblocks all the processes on the queue.
 * 
 * @returns 0 if the operation succeded. The queue being empty is not considered a failure.
 */
int unblockAllWQ(WaitingQueue wq);

/**
 * @brief Gets up to maxPids PIDs stored in this queue, in the order in which they were queued.
 * (array[0] will be the next PID to be unblocked).
 * 
 * @returns The amount of PIDs written to the array.
 */
int getpidsWQ(WaitingQueue wq, Pid* array, int maxPids);

#endif