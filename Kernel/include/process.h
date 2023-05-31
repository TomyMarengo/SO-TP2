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
 * @brief Creates a new process from a given entry point and arguments list.
 * 
 * @returns The newly created TProcess, or -1 if process creation failed.
 */
Pid prcCreate(const ProcessCreateInfo* createInfo);

/**
 * @brief Kills a process and frees associated resources. Using a process after
 * it has been killed results in undefined behaviour.
 */
int kill(Pid pid);

/**
 * @brief Maps a resource onto a process' I/O table.
 * 
 * @returns The file descriptor on which the resource was mapped for the process,
 * or -1 if an error occurred.
 */
int prcAddFd(Pid pid, int fd, void* resource, FdReadHandler readHandler, FdWriteHandler writeHandler, FdCloseHandler closeHandler);

/**
 * @brief Unmaps a resource from a process' I/O table.
 * 
 * @returns 0 if the operation succeeded.
 */
int prcDeleteFd(Pid pid, int fd);

/** 
 * @brief Handles a process' read operation from a file descriptor in its I/O table.
 *
 * @returns The amount of bytes read, or -1 if an error occurred.
 */
ssize_t prcHandleReadFd(Pid pid, int fd, char* buf, size_t count);

/**
 * @brief Handles a process' write operation to a file descriptor in its I/O table.
 * 
 * @returns The amount of bytes written, or -1 if an error occurred.
 */
ssize_t prcHandleWriteFd(Pid pid, int fd, const char* buf, size_t count);

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

#endif