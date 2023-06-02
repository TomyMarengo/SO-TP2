#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <defs.h>

#define PROCESS_STACK_SIZE 4096

/**
 * @brief Defines a function that will handle a file descriptor read operation.
 *
 * @returns The amount of bytes read, -1 in error cases.
 */
typedef ssize_t (*FdReadHandler)(Pid pid, int fd, void* resource, char* buf, size_t count);

/**
 * @brief Defines a function that will handle a file descriptor write operation.
 *
 * @returns The amount of bytes written, -1 in error cases.
 */
typedef ssize_t (*FdWriteHandler)(Pid pid, int fd, void* resource, const char* buf, size_t count);

/**
 * @brief Defines a function that will handle a file descriptor close operation.
 *
 * @returns 0 if the operation succeeded.
 */
typedef int (*FdCloseHandler)(Pid pid, int fd, void* resource);

/**
 * @brief Represents a function that will handle a file descriptor dup operation.
 * Handlers need not check that pidFrom or fdFrom are valid.
 * 
 * @returns The file descriptor on which the resource was mapped for the process,
 * or -1 if an error occurred.
 */
typedef int (*FdDupHandler)(Pid pidFrom, Pid pidTo, int fdFrom, int fdTo, void* resource);

/**
 * @brief Creates a new process from a given entry point and arguments list.
 * 
 * @returns The newly created TProcess, or -1 if process creation failed.
 */
Pid createProcess(const ProcessCreateInfo* createInfo);

/**
 * @brief Kills a process and frees associated resources. Using a process after
 * it has been killed results in undefined behaviour.
 */
int kill(Pid pid);

/**
 * @brief Handles a malloc operation for a process.
 */
void* handleMalloc(Pid pid, size_t size);

/**
 * @brief Handles a memory free operation for a process.
 */
int handleFree(Pid pid, void* ptr);

/**
 * @brief Handles a realloc operation for a process.
 */
void* handleRealloc(Pid pid, void* ptr, size_t size);

/**
 * @brief Maps a resource onto a process' I/O table.
 * 
 * @returns The file descriptor on which the resource was mapped for the process,
 * or -1 if an error occurred.
 */
int addFdProcess(Pid pid, int fd, void* resource, FdReadHandler readHandler, FdWriteHandler writeHandler, FdCloseHandler closeHandler
, FdDupHandler dupHandler);

/**
 * @brief Unmaps a resource from a process' I/O table.
 * 
 * @returns 0 if the operation succeeded.
 */
int deleteFdProcess(Pid pid, int fd);

/** 
 * @brief Handles a process' read operation from a file descriptor in its I/O table.
 *
 * @returns The amount of bytes read, or -1 if an error occurred.
 */
ssize_t handleReadFdProcess(Pid pid, int fd, char* buf, size_t count);

/**
 * @brief Handles a process' write operation to a file descriptor in its I/O table.
 * 
 * @returns The amount of bytes written, or -1 if an error occurred.
 */
ssize_t handleWriteFdProcess(Pid pid, int fd, const char* buf, size_t count);

/**
 * @brief Returns if a process is in the foreground or not.
 * 
 * @returns 1 if is in the foreground, 0 if is in the background
 */
int isForeground(Pid pid);

/**
 * @brief Sets a process to the foreground.
 * 
 * @returns 0 if the operation succeeded.
 */
int toForeground(Pid pid);

/**
 * @brief Sets a process to the background.
 * 
 * @returns 0 if the operation succeeded.
 */
int toBackground(Pid pid);

/**
 * @brief List (maxProcesses)-amount of processes.
 * 
 * @returns The amount of processes archived.
 */
uint8_t listProcesses(ProcessInfo* vec, uint8_t maxProcesses);

/**
 * @brief Maps the resource on a file descriptor from a process to a file descriptor
 * on another process. fdTo may be -1 to let the I/O table decide a file descriptor.
 * 
 * @returns The file descriptor on which the resource was mapped for the process,
 * or -1 if an error occurred.
 */
int dupFdProcess(Pid pidFrom, Pid pidTo, int fdFrom, int fdTo);

/**
 * @brief Adds a process to another process' "unblock on killed" list.
 * This will not change the state of any of the involved processes.
 * 
 * @returns 0 if the operation succeeded, != 0 if not.
 */
int unblockOnKilled(Pid pidToUnblock, Pid pidToWait);


#endif