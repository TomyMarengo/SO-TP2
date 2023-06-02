#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <memoryManager.h>
#include <process.h>
#include <pipe.h>
#include <lib.h>
#include <defs.h>
#include <waitingQueue.h>
#include <namer.h>
#include <string.h>
#include <scheduler.h>

#define MAX_PIPES 64
#define MIN_BUFFER_SIZE 512
#define MAX_BUFFER_SIZE 4096
#define ROUND_BUFFER_SIZE(value) (((value) + 511) / 512 * 512)

typedef struct {
    void* buffer;
    size_t bufferSize;
    size_t readOffset;
    size_t remainingBytes;
    unsigned int readerFdCount;
    unsigned int writerFdCount;
    const char* name;
    WaitingQueue readProcessWQ;
    WaitingQueue writeProcessWQ;
} PipeData;

typedef struct {
    Pipe pipe;
    int allowRead, allowWrite;
} PipeFdMapping;

static PipeData* pipes[MAX_PIPES];
static int nextCandidate = 0;
static Namer namedPipes = NULL;

static ssize_t fdReadHandler(Pid pid, int fd, void* resource, char* buf, size_t count);
static ssize_t fdWriteHandler(Pid pid, int fd, void* resource, const char* buf, size_t count);
static int fdCloseHandler(Pid pid, int fd, void* resource);
static int fdDupHandler(Pid pidFrom, Pid pidTo, int fdFrom, int fdTo, void* resource);

static PipeData* getPipeData(Pipe pipe) {
    return (pipe < 0 || pipe >= MAX_PIPES) ? NULL : pipes[pipe];
}

Pipe createPipe() {
    int id = -1;
    for (int i = 0; i < MAX_PIPES; i++) {
        int idCandidate = (nextCandidate + i) % MAX_PIPES;
        if (pipes[idCandidate] == NULL) {
            id = idCandidate;
            break;
        }
    }

    if (id == -1)
        return -1;

    PipeData* pipeData;
    WaitingQueue readQueue = NULL;
    WaitingQueue writeQueue = NULL;
    if ((pipeData = malloc(sizeof(PipeData))) == NULL || (readQueue = newWQ()) == NULL || (writeQueue = newWQ()) == NULL) {
        free(pipeData);
        if (readQueue != NULL)
            free(readQueue);
        return -1;
    }

    memset(pipeData, 0, sizeof(PipeData));
    pipeData->readProcessWQ = readQueue;
    pipeData->writeProcessWQ = writeQueue;
    pipes[id] = pipeData;

    nextCandidate = (id + 1) % MAX_PIPES;
    return id;
}

Pipe openPipe(const char* name) {
    if (namedPipes == NULL && (namedPipes = newWQ()) == NULL)
        return -1;

    Pipe pipe = (Pipe)(size_t)getResource(namedPipes, name) - 1;

    if (pipe < 0) {
        if ((pipe = createPipe()) < 0)
            return -1;
        if (addResource(namedPipes, (void*)(size_t)(pipe + 1), name, &pipes[pipe]->name) != 0) {
            freePipe(pipe);
            return -1;
        }
    }

    return pipe;
}


int unlinkPipe(const char* name) {
    Pipe pipe = (Pipe)(size_t)deleteResource(namedPipes, name) - 1;
    if (pipe < 0)
        return 1;

    PipeData* pipeData = pipes[pipe];
    pipeData->name = NULL;

    if (pipeData->readerFdCount == 0) {
        if (pipeData->writerFdCount == 0) {
            return freePipe(pipe);
        }

        int result = free(pipeData->buffer);
        pipeData->buffer = NULL;
        pipeData->bufferSize = 0;
        unblockAllWQ(pipeData->writeProcessWQ);
        return result;
    } else if (pipeData->writerFdCount == 0)
        unblockAllWQ(pipeData->readProcessWQ);

    return 0;
}

int freePipe(Pipe pipe) {
    PipeData* pipeData = getPipeData(pipe);
    if (pipeData == NULL)
        return 1;

    pipes[pipe] = NULL;
    return free(pipeData->buffer) + freeWQ(pipeData->readProcessWQ) + freeWQ(pipeData->writeProcessWQ) + free(pipeData);
}


static ssize_t writeData(PipeData* pipe, const void* buf, size_t count) {
    size_t requiredBufferSize = pipe->remainingBytes + count;
    if (pipe->bufferSize < requiredBufferSize && pipe->bufferSize < MAX_BUFFER_SIZE) {
        size_t newBufferSize = requiredBufferSize < MAX_BUFFER_SIZE ? requiredBufferSize : MAX_BUFFER_SIZE;
        newBufferSize = ROUND_BUFFER_SIZE(newBufferSize);
        void* newBuf = malloc(newBufferSize);
        if (newBuf != NULL) {
            size_t x = pipe->bufferSize - pipe->readOffset;
            if (x >= pipe->remainingBytes) {
                memcpy(newBuf, pipe->buffer + pipe->readOffset, pipe->remainingBytes);
            } else {
                memcpy(newBuf, pipe->buffer + pipe->readOffset, x);
                memcpy(newBuf + x, pipe->buffer, pipe->remainingBytes - x);
            }
            pipe->readOffset = 0;
            free(pipe->buffer);
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

    unblockAllWQ(pipe->readProcessWQ);
    return bytesToWrite;
}


ssize_t readData(PipeData* pipe, void* buf, size_t count) {
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

    unblockAllWQ(pipe->writeProcessWQ);

    if (pipe->buffer != NULL && pipe->writerFdCount == 0 && pipe->remainingBytes == 0 && pipe->name == NULL) {
        free(pipe->buffer);
        pipe->buffer = NULL;
        pipe->bufferSize = 0;
        unblockAllWQ(pipe->readProcessWQ);
    }

    return bytesToRead;
}

ssize_t readPipe(Pipe pipe, void* buf, size_t count) {
    PipeData* pipeData = getPipeData(pipe);
    return pipeData == NULL ? -1 : readData(pipeData, buf, count);
}

ssize_t writePipe(Pipe pipe, const void* buf, size_t count) {
    PipeData* pipeData = getPipeData(pipe);
    return pipeData == NULL ? -1 : writeData(pipeData, buf, count);
}

int addFdPipe(Pid pid, int fd, Pipe pipe, int allowRead, int allowWrite) {
    PipeData* pipeData = getPipeData(pipe);
    if (pipeData == NULL)
        return -1;

    PipeFdMapping* mapping = malloc(sizeof(PipeFdMapping));
    if (mapping == NULL)
        return -1;

    int r = addFdProcess(pid, fd, mapping, allowRead ? &fdReadHandler : NULL, allowWrite ? &fdWriteHandler : NULL, &fdCloseHandler, &fdDupHandler);
    if (r < 0) {
        free(mapping);
        return r;
    }

    allowRead = (allowRead != 0);
    allowWrite = (allowWrite != 0);

    mapping->pipe = pipe;
    mapping->allowRead = allowRead;
    mapping->allowWrite = allowWrite;
    pipeData->readerFdCount += allowRead;
    pipeData->writerFdCount += allowWrite;

    return r;
}

static ssize_t fdReadHandler(Pid pid, int fd, void* resource, char* buf, size_t count) {
    PipeFdMapping* mapping = (PipeFdMapping*)resource;
    PipeData* pipe = pipes[mapping->pipe];

    if (count == 0)
        return 0;

    ssize_t r;
    while ((r = readData(pipe, buf, count)) == 0 && (pipe->name != NULL || pipe->writerFdCount != 0)) {
        addWQ(pipe->readProcessWQ, pid);
        block(pid);
        yield();
    }

    return r;
}

static ssize_t fdWriteHandler(Pid pid, int fd, void* resource, const char* buf, size_t count) {
    PipeFdMapping* mapping = (PipeFdMapping*)resource;
    PipeData* pipe = pipes[mapping->pipe];

    if (count == 0)
        return 0;

    ssize_t r;
    while ((r = writeData(pipe, buf, count)) == 0 && (pipe->name != NULL || pipe->readerFdCount != 0)) {
        addWQ(pipe->writeProcessWQ, pid);
        block(pid);
        yield();
    }

    return r == 0 ? -1 : r;
}

static int fdCloseHandler(Pid pid, int fd, void* resource) {
    PipeFdMapping* mapping = (PipeFdMapping*)resource;
    PipeData* pipe = pipes[mapping->pipe];

    pipe->readerFdCount -= mapping->allowRead;
    pipe->writerFdCount -= mapping->allowWrite;
    int result = free(mapping);

    if (pipe->name == NULL) {
        if (pipe->readerFdCount == 0) {
            if (pipe->writerFdCount == 0) {
                return result + freePipe(mapping->pipe);
            }

            result += free(pipe->buffer);
            pipe->buffer = NULL;
            pipe->bufferSize = 0;
            unblockAllWQ(pipe->writeProcessWQ);
        } else if (pipe->writerFdCount == 0)
            unblockAllWQ(pipe->readProcessWQ);
    }

    return result;
}

static int fdDupHandler(Pid pidFrom, Pid pidTo, int fdFrom, int fdTo, void* resource) {
    PipeFdMapping* mapping = (PipeFdMapping*)resource;
    return addFdPipe(pidTo, fdTo, mapping->pipe, mapping->allowRead, mapping->allowWrite);
}

int listPipes(PipeInfo* array, int maxPipes) {
    int pipeCounter = 0;
    for (int i = 0; i < MAX_PIPES && pipeCounter < maxPipes; i++) {
        PipeData* pipe = pipes[i];
        if (pipe != NULL) {
            PipeInfo* info = &array[pipeCounter++];
            info->remainingBytes = pipe->remainingBytes;
            info->readerFdCount = pipe->readerFdCount;
            info->writerFdCount = pipe->writerFdCount;

            int readPids = getpidsWQ(pipe->readProcessWQ, info->readBlockedPids, MAX_PID_ARRAY_LENGTH);
            info->readBlockedPids[readPids] = -1;

            int writePids = getpidsWQ(pipe->writeProcessWQ, info->writeBlockedPids, MAX_PID_ARRAY_LENGTH);
            info->writeBlockedPids[writePids] = -1;

            if (pipe->name == NULL)
                info->name[0] = '\0';
            else
                strncpy(info->name, pipe->name, MAX_NAME_LENGTH);
        }
    }

    return pipeCounter;
}