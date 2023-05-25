#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <string.h>
#include <memoryManager.h>
#include <process.h>
#include <pipe.h>


#define MIN_BUFFER_SIZE 512
#define MAX_BUFFER_SIZE 4096
#define ROUND_BUFFER_SIZE(value) (((value) + 511) / 512 * 512)

struct PipeData {
    void* buffer;
    size_t bufferSize;
    size_t readOffset;
    size_t remainingBytes;
    unsigned int readerFdCount;
    unsigned int writerFdCount;
};

typedef struct {
    Pipe pipe;
    int allowRead, allowWrite;
} PipeFdMapping;

static ssize_t fdReadHandler(Pid pid, int fd, void* resource, char* buf, size_t count);
static ssize_t fdWriteHandler(Pid pid, int fd, void* resource, const char* buf, size_t count);
static int fdCloseHandler(Pid pid, int fd, void* resource);

Pipe pipe_create() {
    Pipe pipe = mm_malloc(sizeof(struct PipeData));
    if (pipe == NULL)
        return NULL;

    pipe->buffer = NULL;
    pipe->bufferSize = 0;
    pipe->readOffset = 0;
    pipe->remainingBytes = 0;
    pipe->readerFdCount = 0;
    pipe->writerFdCount = 0;
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

int pipe_addFd(Pid pid, int fd, Pipe pipe, int allowRead, int allowWrite) {
    PipeFdMapping* mapping = mm_malloc(sizeof(PipeFdMapping));
    if (mapping == NULL)
        return -1;

    int r = prc_addFd(pid, fd, mapping, allowRead ? &fdReadHandler : NULL, allowWrite ? &fdWriteHandler : NULL, &fdCloseHandler);
    if (r < 0) {
        mm_free(mapping);
        return r;
    }

    // Ensure these are a value of either 0 or 1.
    allowRead = (allowRead != 0);
    allowWrite = (allowWrite != 0);

    // Fill in the mapping info to the struct.
    mapping->pipe = pipe;
    mapping->allowRead = allowRead;
    mapping->allowWrite = allowWrite;

    // Resource tracking.
    // TODO: lockPipe(); // Should this be done in this function? Who's the calling PID? What if it's the kernel?
    pipe->readerFdCount += allowRead;
    pipe->writerFdCount += allowWrite;
    // TODO: unlockPipe();

    return r;
}

static ssize_t fdReadHandler(Pid pid, int fd, void* resource, char* buf, size_t count) {
    PipeFdMapping* mapping = (PipeFdMapping*) resource;
    Pipe pipe = mapping->pipe;

    if (count == 0)
        return 0;

    // TODO: Implement blocking read.
    // TODO: Make access to the TPipe struct mutually exclusive between processes.
    // lockPipe(); // With a semaphore or somethin
    ssize_t r;
    while ((r = pipe_read(pipe, buf, count)) == 0 && pipe->writerFdCount != 0) {
        // Add to queue of blocked
        // unlockPipe();
        // sch_block(pid);
        // sch_ready(pidOtro);
        // sch_yield(pid);
        // lockPipe();
    }

    // If no bytes remain for reading and there are no writers, then "end of file" was reached.
    // We can free the internal pipe's buffer to save memory.
    if (pipe->buffer != NULL && pipe->writerFdCount == 0 && pipe->remainingBytes == 0) {
        mm_free(pipe->buffer);
        pipe->buffer = NULL;
        pipe->bufferSize = 0;
        // TODO: wake up all readers?
    }

    // unlockPipe();
    return r;
}

static ssize_t fdWriteHandler(Pid pid, int fd, void* resource, const char* buf, size_t count) {
    PipeFdMapping* mapping = (PipeFdMapping*) resource;
    Pipe pipe = mapping->pipe;

    if (count == 0)
        return 0;

    // TODO: Implement blocking write.
    // TODO: Make access to the TPipe struct mutually exclusive between processes.
    // lockPipe();
    ssize_t r;
    while ((r = pipe_write(pipe, buf, count)) == 0 && pipe->readerFdCount != 0) {
        // Add to queue of blocked
        // unlockPipe();
        // sch_block(pid);
        // sch_ready(pidOtro);
        // sch_yield(pid);
        // lockPipe();
    }

    // unlockPipe();
    return r == 0 ? -1 : r;
}

static int fdCloseHandler(Pid pid, int fd, void* resource) {
    PipeFdMapping* mapping = (PipeFdMapping*) resource;
    Pipe pipe = mapping->pipe;
    // TODO: Make access to the TPipe struct mutually exclusive between processes.
    // lockPipe();
    pipe->readerFdCount -= mapping->allowRead;
    pipe->writerFdCount -= mapping->allowWrite;
    int result = mm_free(mapping);

    if (pipe->readerFdCount == 0) {
        if (pipe->writerFdCount == 0) {
            // No more readers nor writers, fully dispose of the pipe.
            return result + pipe_free(pipe);
        } else {
            // No more readers, we don't need to keep any data inside the pipe. Free the internal buffer.
            result += mm_free(pipe->buffer);
            pipe->buffer = NULL;
            pipe->bufferSize = 0;
        }
    }

    // unlockPipe();
    return result;
}