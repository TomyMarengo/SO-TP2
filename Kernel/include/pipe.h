#ifndef _PIPE_H_
#define _PIPE_H_

#include <defs.h>

/**
 * @brief Generates a new pipe.
 *
 * @returns - The newly created pipe, otherwise -1.
 */
Pipe createPipe();

/**
 * @brief Retrieves the pipe with the specified name, or creates it if it doesn't exist.
 *
 * @param name Name of the pipe.
 * 
 * @returns - The named pipe, or -1 if the operation failed.
 */
Pipe openPipe(const char *name);

/**
 * @brief Removes the name associated with a named pipe, allowing the name to be used
 * for future pipes. The pipe itself is not disposed of until no processes are using it.
 * 
 * @param name Name of the pipe.
 * 
 * @returns - 0 if the operation is successful, or a non-zero value if not.
 */
int unlinkPipe(const char *name);

/**
 * @brief Releases all resources used by a pipe.
 *
 * @param pipe Pipe to be freed, returned in createPipe().
 * 
 * @returns - 0 if the operation succeeded, or greater than 0 if not.
 */
int freePipe(Pipe pipe);

/**
 * @brief Writes up to count bytes from the provided buffer into a pipe.
 *
 * @param pipe Pipe to be written, returned in createPipe().
 * @param buffer Buffer containing the data to be written.
 * @param count Amount of bytes to write.
 * 
 * @returns - The number of bytes written, -1 in error cases.
 */
ssize_t writePipe(Pipe pipe, const void *buffer, size_t count);

/**
 * @brief Reads up to count bytes from the provided pipe into the buffer.
 *
 * @param pipe Pipe to be read, returned in createPipe().
 * @param buffer Buffer containing the after read data.
 * @param count Amount of bytes to read.
 * 
 * @returns The number of bytes read, -1 in error cases.
 */
ssize_t readPipe(Pipe pipe, void *buffer, size_t count);

/**
 * @brief Add the pipe into fd process table.
 *
 * @param pid Process' PID.
 * @param fd FD of the pipe.
 * @param pipe Pipe itself, returned in createPipe().
 * @param allowRead Allow the pipe to be read.
 * @param allowWrite Allow the pipe to be written.
 * 
 * @returns - The pipe file descriptor, -1 in error cases.
 */
int addFdPipe(Pid pid, int fd, Pipe pipe, int allowRead, int allowWrite);

/**
 * @brief Gets the information of up to maxPipes pipes.
 *
 * @returns - The amount of pipes read.
 */
int listPipes(PipeInfo *array, int limit);

#endif