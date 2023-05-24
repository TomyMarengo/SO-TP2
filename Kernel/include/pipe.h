#ifndef _PIPE_H_
#define _PIPE_H_

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

typedef struct PipeData* Pipe;

/**
 * @brief Generates a new pipe.
 * 
 * @returns The newly generated pipe, otherwise NULL.
 */
Pipe pipe_create();

/**
 * @brief Releases all resources used by a pipe.
 * 
 * @returns 0 if the operation is successful.
 */
int pipe_free(Pipe pipe);

/**
 * @brief Writes up to count bytes from the provided buffer into a pipe.
 * 
 * @returns The number of bytes written, -1 in error cases.
 */
ssize_t pipe_write(Pipe pipe, const void* buf, size_t count);

/**
 * @brief Reads up to count bytes from the provided pipe into the buffer.
 * 
 * @returns The number of bytes read, -1 in error cases.
 */
ssize_t pipe_read(Pipe pipe, void* buf, size_t count);

#endif