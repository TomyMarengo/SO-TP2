#ifndef USE_BUDDY

#include <defs.h>
#include <lib.h>
#include <memoryManager.h>

typedef struct memoryListNode {
    size_t size;
    size_t leftoverSize;
    struct memoryListNode *previous;
    struct memoryListNode *next;
    size_t checksum;
} MemoryListNode;

static size_t totalMemory;
static size_t usedMemory;
static unsigned int memoryChunks;

static MemoryListNode *firstBlock = NULL;

static void
calcNodeChecksum(const MemoryListNode *node, size_t *result) {
    *result = node->size ^ node->leftoverSize ^ (size_t) node->previous ^ (size_t) node->next;
}

void
initializeMemory(void *memoryStart, size_t memorySize) {
    void *actualStart = (void *) WORD_ALIGN_UP(memoryStart);
    memorySize -= (actualStart - memoryStart);
    memorySize = WORD_ALIGN_DOWN(memorySize);

    totalMemory = memorySize;
    usedMemory = sizeof(MemoryListNode);
    memoryChunks = 1;

    firstBlock = (MemoryListNode *) actualStart;
    memorySize -= sizeof(MemoryListNode);

    firstBlock->size = 0;
    firstBlock->leftoverSize = memorySize;
    firstBlock->previous = NULL;
    firstBlock->next = NULL;
    calcNodeChecksum(firstBlock, &firstBlock->checksum);
}

void *
malloc(size_t size) {
    if (firstBlock == NULL || size == 0)
        return NULL;

    size = WORD_ALIGN_UP(size);

    MemoryListNode *node = firstBlock;
    size_t totalSizeWithNode = size + sizeof(MemoryListNode);

    while ((node->size != 0 || node->leftoverSize < size) && node->leftoverSize < totalSizeWithNode) {
        node = node->next;

        if (node == NULL)
            return NULL;
    }

    if (node->size == 0) {
        node->size = size;
        node->leftoverSize -= size;
        calcNodeChecksum(node, &node->checksum);
        usedMemory += size;
        return (void *) node + sizeof(MemoryListNode);
    }

    MemoryListNode *newNode = (MemoryListNode *) ((void *) node + sizeof(MemoryListNode) + node->size);
    newNode->size = size;
    newNode->leftoverSize = node->leftoverSize - sizeof(MemoryListNode) - newNode->size;
    newNode->previous = node;
    newNode->next = node->next;
    node->leftoverSize = 0;
    node->next = newNode;

    if (newNode->next != NULL) {
        newNode->next->previous = newNode;
        calcNodeChecksum(newNode->next, &newNode->next->checksum);
    }

    calcNodeChecksum(newNode, &newNode->checksum);
    calcNodeChecksum(node, &node->checksum);

    memoryChunks++;
    usedMemory += totalSizeWithNode;
    return (void *) newNode + sizeof(MemoryListNode);
}

int
free(void *memorySegment) {
    if (memorySegment == NULL)
        return 0;

    MemoryListNode *node = (MemoryListNode *) (memorySegment - sizeof(MemoryListNode));

    size_t checksum;
    calcNodeChecksum(node, &checksum);
    if (checksum != node->checksum)
        return 1;

    if (node->previous == NULL) {
        node->leftoverSize += node->size;
        node->size = 0;
        usedMemory -= node->size;
        calcNodeChecksum(node, &node->checksum);
    } else {
        node->previous->leftoverSize += node->size + node->leftoverSize + sizeof(MemoryListNode);
        node->previous->next = node->next;
        usedMemory -= node->size + sizeof(MemoryListNode);
        memoryChunks--;
        calcNodeChecksum(node->previous, &node->previous->checksum);

        if (node->next != NULL) {
            node->next->previous = node->previous;
            calcNodeChecksum(node->next, &node->next->checksum);
        }
    }

    return 0;
}

void *
realloc(void *memorySegment, size_t size) {
    size = WORD_ALIGN_UP(size);

    if (memorySegment == NULL)
        return malloc(size);

    if (size == 0) {
        free(memorySegment);
        return NULL;
    }

    MemoryListNode *node = (MemoryListNode *) (memorySegment - sizeof(MemoryListNode));

    size_t checksum;
    calcNodeChecksum(node, &checksum);
    if (checksum != node->checksum)
        return NULL;

    if (node->size == size)
        return memorySegment;

    if (size < node->size) {
        node->leftoverSize += node->size - size;
        usedMemory -= node->size - size;
        node->size = size;
        calcNodeChecksum(node, &node->checksum);
        return memorySegment;
    }

    size_t extraRequiredSize = size - node->size;
    if (node->leftoverSize >= extraRequiredSize) {
        node->leftoverSize -= extraRequiredSize;
        node->size = size;
        usedMemory += extraRequiredSize;
        calcNodeChecksum(node, &node->checksum);
        return memorySegment;
    }

    void *newPtr = malloc(size);
    if (newPtr != NULL) {
        memcpy(newPtr, memorySegment, node->size);
        free(memorySegment);
    }
    return newPtr;
}

int
getStateMemory(MemoryState *memoryState) {
    memoryState->total = totalMemory;
    memoryState->used = usedMemory;
    memoryState->type = LIST;
    memoryState->chunks = memoryChunks;
    return 0;
}

#endif