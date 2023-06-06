#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <defs.h>

/**
 * @brief Creates a new process from a ProcessCreateInfo struct.
 *
 * @param createInfo Structure containing all necessary creation information.
 *
 * @returns - The newly created TProcess, or -1 if process creation failed.
 */
Pid createProcess(const ProcessCreateInfo *createInfo);

/**
 * @brief Kills a process and frees associated resources.
 *
 * @param pid PID of the process.
 *
 * @returns - 0 if the operation succeeded, or greater than 0 if not.
 */
int kill(Pid pid);

/**
 * @brief Returns if a process is in the foreground or not.
 *
 * @param pid PID of the process.
 *
 * @returns - 1 if is in the foreground, 0 if is in the background
 */
int isForeground(Pid pid);

/**
 * @brief Sets a process to the foreground.
 *
 * @param pid PID of the process.
 *
 * @returns - 0 if the operation succeeded.
 */
int toForeground(Pid pid);

/**
 * @brief Sets a process to the background.
 *
 * @param pid PID of the process.
 *
 * @returns - 0 if the operation succeeded.
 */
int toBackground(Pid pid);

/**
 * @brief Handles a process malloc operation.
 *
 * @param pid PID of the process.
 * @param size - The desired amount of memory requested.
 *
 * @returns - A pointer to the reserved memory, or NULL if the operation failed.
 */
void *handleMalloc(Pid pid, size_t size);

/**
 * @brief Handles a process free operation.
 *
 * @param pid PID of the process.
 * @param memorySegment Pointer to the memory segment to be released.
 *
 * @returns - 0 if the operation is successful, 1 otherwise.
 */
int handleFree(Pid pid, void *memorySegment);

/**
 * @brief Handles a process realloc operation.
 *
 * @param pid PID of the process.
 * @param memorySegment Pointer to the memory block that was previously allocated and needs to be resized.
 * @param size The new size for the memory block.
 *
 * @returns - A pointer to the resized memory block, or NULL if the operation failed
 */
void *handleRealloc(Pid pid, void *memorySegment, size_t size);

/**
 * @brief Adds a resource with file descriptor and handlers onto a process.
 *
 * @param pid PID of the process.
 * @param fd File descriptor desired or < 0 to let the kernel decide.
 * @param readHandler Function of what you want to do when reading the file descriptor.
 * @param writeHandler Function of what you want to do when writing the file descriptor.
 * @param closeHandler Function of what you want to do when closing the file descriptor.
 * @param dupHandler Function of what you want to do when duplicating the file descriptor.
 *
 * @returns The file descriptor of the resource or -1 if an error occurred.
 */
int addFd(Pid pid, int fd, void *resource, ReadHandler readHandler, WriteHandler writeHandler, CloseHandler closeHandler,
          DupHandler dupHandler);

/**
 * @brief Delete a resource previously added onto a process.
 *
 * @param pid PID of the process.
 * @param fd File descriptor asociated to the resource.
 *
 * @returns - 0 if the operation is successful, 1 otherwise.
 */
int deleteFd(Pid pid, int fd);

/**
 * @brief Maps the resource on a file descriptor from a process to a file descriptor on another process.
 *
 * @param pidFrom PID of the source process.
 * @param pidTo PID of the destination process.
 * @param fdFrom File descriptor associated to the resource in the source process.
 * @param fdTo File descriptor desired or < 0 to let the kernel decide.
 *
 * @returns - The file descriptor on which the resource was associated for the process,
 * or -1 if an error occurred.
 */
int dupFd(Pid pidFrom, Pid pidTo, int fdFrom, int fdTo);

/**
 * @brief Handles a process read operation.
 *
 * @param pid PID of the process.
 * @param fd File descriptor to be read.
 * @param buffer Buffer to store the read data.
 * @param count Limit to be read.
 *
 * @returns - The amount of bytes read, or -1 if an error occurred.
 */
ssize_t handleRead(Pid pid, int fd, char *buffer, size_t count);

/**
 * @brief Handles a process write operation.
 *
 * @param pid PID of the process.
 * @param fd File descriptor to be written.
 * @param buf Buffer where the data to be written is stored.
 * @param count Limit to be written.
 *
 * @returns - The amount of bytes written, or -1 if an error occurred.
 */
ssize_t handleWrite(Pid pid, int fd, const char *buffer, size_t count);

/**
 * @brief Adds a process to another process' "unblock on killed" list.
 *
 * @param pidToUnblock "Child".
 * @param pidToWait "Parent".
 *
 * @returns - 0 if the operation is successful, 1 otherwise.
 */
int unblockOnKilled(Pid pidToUnblock, Pid pidToWait);

/**
 * @brief Gets (maxProcesses)-amount information of processes.
 *
 * @param storingInfo Array of ProcessInfo to archive the data.
 * @param maxProcesses Limit.
 *
 * @returns - The amount of processes archived.
 */
uint8_t listProcesses(ProcessInfo *storingInfo, uint8_t maxProcesses);

#endif