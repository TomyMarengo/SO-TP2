#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <pipe.h>
#include <memoryManager.h>

#define MIN_BUFFER_SIZE 512
#define MAX_BUFFER_SIZE 4096
#define ROUND_BUFFER_SIZE(value) (((value) + 511) / 512 * 512)

struct PipeData {
    void* buffer;
    size_t bufferSize;
    size_t readOffset;
    size_t remainingBytes;
};

Pipe pipe_create() {
    Pipe pipe = mm_malloc(sizeof(struct PipeData));
    if (pipe == NULL)
        return NULL;

    pipe->buffer = NULL;
    pipe->bufferSize = 0;
    pipe->readOffset = 0;
    pipe->remainingBytes = 0;
    return pipe;
}

int pipe_free(Pipe pipe) {
    return mm_free(pipe->buffer) + mm_free(pipe);
}

ssize_t pipe_write(Pipe pipe, const void* buf, size_t count) {
    if (pipe->bufferSize < count && pipe->bufferSize < MAX_BUFFER_SIZE) {
        size_t newBufferSize = count;
        if (newBufferSize > MAX_BUFFER_SIZE)
            newBufferSize = MAX_BUFFER_SIZE;
        newBufferSize = ROUND_BUFFER_SIZE(newBufferSize);

        void* newBuf = mm_malloc(newBufferSize);
        if (newBuf != NULL) {
            memcpy(newBuf, pipe->buffer + pipe->readOffset, pipe->bufferSize - pipe->readOffset);
            memcpy(newBuf + pipe->readOffset, pipe->buffer, pipe->readOffset);
            pipe->readOffset = 0;
            mm_free(pipe->buffer);
            pipe->buffer = newBuf;
            pipe->bufferSize = newBufferSize;
        } else if (pipe->buffer == NULL)
            return -1; 
    } else if (pipe->remainingBytes == 0)
        pipe->readOffset = 0;

    size_t writeOffset = (pipe->readOffset + pipe->remainingBytes) % pipe->bufferSize;
    size_t spaceAvailable = pipe->bufferSize - pipe->remainingBytes;

    size_t bytesToWrite = count < spaceAvailable ? count : spaceAvailable;

    if (bytesToWrite == 0)
        return 0;

    size_t firstWriteSize = pipe->bufferSize - writeOffset;
    if (firstWriteSize > bytesToWrite)
        firstWriteSize = bytesToWrite;
    memcpy(pipe->buffer + writeOffset, buf, firstWriteSize);

    if (firstWriteSize < bytesToWrite)
        memcpy(pipe->buffer, buf + firstWriteSize, bytesToWrite - firstWriteSize);

    pipe->remainingBytes += bytesToWrite;
    return bytesToWrite;
}

ssize_t pipe_read(Pipe pipe, void* buf, size_t count) {
    size_t bytesToRead = pipe->remainingBytes;
    if (bytesToRead > count)
        bytesToRead = count;

    if (bytesToRead == 0)
        return 0;

    size_t firstReadSize = pipe->bufferSize - pipe->readOffset;
    if (firstReadSize > bytesToRead)
        firstReadSize = bytesToRead;
    memcpy(buf, pipe->buffer + pipe->readOffset, firstReadSize);

    if (firstReadSize < bytesToRead)
        memcpy(buf + firstReadSize, pipe->buffer, bytesToRead - firstReadSize);

    pipe->remainingBytes -= bytesToRead;
    pipe->readOffset = (pipe->readOffset + bytesToRead) % pipe->bufferSize;
    return bytesToRead;
}