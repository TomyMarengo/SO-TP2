#ifndef _PIPE_H_
#define _PIPE_H_

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <defs.h>

typedef int Pipe;

/**
 * @brief Generates a new pipe.
 * 
 * @returns The newly generated pipe, otherwise NULL.
 */
Pipe createPipe();

/**
 * @brief Gets the pipe with the given name, or creates it if it doesn't exist.
 * 
 * @returns The named pipe, or -1 if the operation failed.
 */
Pipe openPipe(const char* name);

/**
 * @brief Unnames a named pipe, making the name available for future pipes.
 * The pipe is not disposed until no more processes are using it.
 * 
 * @returns 0 if the operation succeeded, != 0 if not.
 */
int unlinkPipe(const char* name);

/**
 * @brief Releases all resources used by a pipe.
 * 
 * @param pipe Pipe to be freed, returned in pipe_create().
 * @returns 0 if the operation is successful.
 */
int freePipe(Pipe pipe);

/**
 * @brief Writes up to count bytes from the provided buffer into a pipe.
 * 
 * @param pipe Pipe to be written, returned in pipe_create().
 * @param buf Buffer containing the data to be written.
 * @param count Amount of bytes to write.
 * @returns The number of bytes written, -1 in error cases.
 */
ssize_t writePipe(Pipe pipe, const void* buf, size_t count);

/**
 * @brief Reads up to count bytes from the provided pipe into the buffer.
 * 
 * @param pipe Pipe to be read, returned in pipe_create().
 * @param buf Buffer containing the after read data.
 * @param count Amount of bytes to read.
 * @returns The number of bytes read, -1 in error cases.
 */
ssize_t readPipe(Pipe pipe, void* buf, size_t count);

/**
 * @brief Add the pipe into fd process table.
 * 
 * @param pid Process' PID.
 * @param fd FD of the pipe.
 * @param pipe Pipe itself, returned in pipe_create().
 * @param allowRead Allow the pipe to be read.
 * @param allowWrite Allow the pipe to be written.
 * @returns The pipe file descriptor, -1 in error cases.
 */
int addFdPipe(Pid pid, int fd, Pipe pipe, int allowRead, int allowWrite);

/**
 * @brief Gets the information of up to maxPipes pipes.
 * 
 * @returns The amount of pipes read.
 */
int listPipes(PipeInfo* array, int maxPipes);

#endif