#ifdef CUSTOM

#include <memoryManager.h>

static void* next;
static size_t remainingSize;

void my_init(void* memoryStart, size_t memorySize) {
    next = (void*)(((size_t)memoryStart + 7) & (~(size_t)0x07));
    remainingSize = memorySize - (next - memoryStart);
}

void * my_malloc(size_t size) {
    if (remainingSize < size)
        return NULL;

    // word-allign size by rounding up to a multiple of 8.
    size = (void*)(((size_t)size + 7) & (~(size_t)0x07));

    void* p = next;
    next += size;
    remainingSize -= size;

    return p;
}

int my_free(void* ptr) {
    return 1;
}

#endif