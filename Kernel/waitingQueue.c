#include <defs.h>
#include <lib.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <waitingQueue.h>

#define BUFFER_CHUNK_SIZE 8

struct WaitingQueueData {
    Pid *buf;
    unsigned int offset;
    unsigned int count;
    unsigned int bufSize;
};

WaitingQueue
newQueue() {
    WaitingQueue queue;
    if ((queue = malloc(sizeof(struct WaitingQueueData))) == NULL)
        return NULL;

    queue->buf = NULL;
    queue->offset = 0;
    queue->count = 0;
    queue->bufSize = 0;
    return queue;
}

int
freeQueue(WaitingQueue queue) {
    return free(queue->buf) + free(queue);
}

int
addInQueue(WaitingQueue queue, Pid pid) {
    if (queue->count == queue->bufSize) {
        unsigned int newBufSize = queue->bufSize + BUFFER_CHUNK_SIZE;
        Pid *newBuf = malloc(newBufSize * sizeof(Pid));

        if (newBuf == NULL)
            return 1;

        unsigned int x = queue->bufSize - queue->offset;
        if (x <= queue->count) {
            memcpy(newBuf, &queue->buf[queue->offset], queue->count * sizeof(Pid));
        } else {
            memcpy(newBuf, &queue->buf[queue->offset], x * sizeof(Pid));
            memcpy(&newBuf[x], queue->buf, (queue->count - x) * sizeof(Pid));
        }

        queue->offset = 0;
        free(queue->buf);
        queue->buf = newBuf;
        queue->bufSize = newBufSize;
    }

    queue->buf[(queue->offset + queue->count++) % queue->bufSize] = pid;
    return 0;
}

int
entriesInQueue(WaitingQueue queue) {
    return queue->count;
}

int
containsInQueue(WaitingQueue queue, Pid pid) {
    for (unsigned int i = 0; i < queue->count; i++)
        if (queue->buf[(queue->offset + i) % queue->bufSize] == pid)
            return 1;
    return 0;
}

int
addIfNotExistsInQueue(WaitingQueue queue, Pid pid) {
    return containsInQueue(queue, pid) ? 0 : addInQueue(queue, pid);
}

int
removeInQueue(WaitingQueue queue, Pid pid) {
    unsigned int i;
    for (i = 0; i < queue->count && queue->buf[(queue->offset + i) % queue->bufSize] != pid; i++)
        ;

    if (i == queue->count)
        return 1;

    queue->count--;
    for (; i < queue->count; i++)
        queue->buf[(queue->offset + i) % queue->bufSize] = queue->buf[(queue->offset + i + 1) % queue->bufSize];
    return 0;
}

int
unblockInQueue(WaitingQueue queue) {
    int failed = 0;

    while (queue->count != 0) {
        Pid pid = queue->buf[queue->offset];
        queue->offset = (queue->offset + 1) % queue->bufSize;
        queue->count--;

        if (unblock(pid) == 0)
            return 0;
        failed++;
    }

    return failed;
}

int
unblockAllInQueue(WaitingQueue queue) {
    int failed = 0;

    for (unsigned int i = 0; i < queue->count; i++)
        if (unblock(queue->buf[(queue->offset + i) % queue->bufSize]) != 0)
            failed++;

    queue->count = 0;
    queue->offset = 0;
    return failed;
}

int
listPidsInQueue(WaitingQueue queue, Pid *storingInfo, int maxPids) {
    if (maxPids > queue->count)
        maxPids = queue->count;

    for (int i = 0; i < maxPids; i++)
        storingInfo[i] = queue->buf[(queue->offset + i) % queue->bufSize];

    return maxPids;
}