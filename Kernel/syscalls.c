#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <graphics.h>
#include <syscalls.h>
#include <keyboard.h>
#include <time.h>
#include <memoryManager.h>
#include <lib.h>
#include <process.h>
#include <scheduler.h>
#include <pipe.h>
#include <defs.h>

static char store[900];
typedef size_t (*SyscallHandlerFunction)(size_t rdi, size_t rsi, size_t rdx, size_t r10, size_t r8);

ssize_t writeHandler(int fd, const char* buffer, size_t count) {
    return prcHandleWriteFd(getpid(), fd, buffer, count);
}

ssize_t readHandler(int fd, char* buffer, size_t count) {
    return prcHandleReadFd(getpid(), fd, buffer, count);
}

int dateHandler(char* buffer) {
    getCurrentDateString(buffer);
    return 0;
}

int timeHandler(char* buffer) {
    getCurrentTimeString(buffer);
    return 0;
}

int closeHandler(int fd) {
    return prcDeleteFd(getpid(), fd);
}

int clearScreenHandler() {
    clearScreen();
    return 0;
}

int printmemHandler(void* address) {
    if ((uint64_t)address > (0x20000000 - 32))
        return -1;

    for (int i = 0; i < 32; ++i) {
        printHex((uint64_t)address);
        print(" = ");
        printHex(*((uint8_t*)address));
        printLine();
        address++;
    }

    return 0;
}

int inforegHandler() {
    if (!store[0])
        print("Debes presionar '-' para guardar el estado de los registros en un momento especifico \n");

    print(store);
    return 0;
}

void* mallocHandler(size_t size) {
    return malloc(size);
}

int freeHandler(void* ptr) {
    return free(ptr);
}

void* reallocHandler(void* ptr, size_t size) {
    return realloc(ptr, size);
}

int pipeHandler(int pipefd[2]) {
    Pid pid = getpid();

    Pipe pipe;
    int readFd = -1, writeFd = -1;

    if ((pipe = pipeCreate()) == NULL || (readFd = pipeAddFd(pid, -1, pipe, 1, 0)) < 0 || (writeFd = pipeAddFd(pid, -1, pipe, 0, 1)) < 0) {
        if (pipe != NULL)
            pipeFree(pipe);
        if (readFd >= 0)
            prcDeleteFd(pid, readFd);
        return 1;
    }

    pipefd[0] = readFd;
    pipefd[1] = writeFd;
    return 0;
}

int killHandler(Pid pid) {
    int result = kill(pid);
    if (pid == getpid())
        yield();
    return result;
}

int blockHandler(Pid pid) {
    int result = block(pid);
    if (pid == getpid())
        yield();
    return result;
}

int unblockHandler(Pid pid) {
    return unblock(pid);
}

Pid createHandler(ProcessCreateInfo* createInfo) {
    Pid pid = prcCreate(createInfo);

    //TODO
    if (pid >= 0) {
        kbdAddFd(pid, STDIN);          // Map STDIN
        scrAddFd(pid, STDOUT, &GREEN); // Map STDOUT
        scrAddFd(pid, STDERR, &BLUE);   // Map STDERR
    }

    return pid;
}

void yieldHandler() {
    yield();
}

Pid getpidHandler() {
    return getpid();
}

int priorityHandler(Pid pid, Priority priority) {
    return setPriority(pid, priority);
}

int exitHandler() {
    kill(getpid());
    yield();
    return 1;
}

static SyscallHandlerFunction syscallHandlers[] = {
    /* 0x00 */ (SyscallHandlerFunction)readHandler,
    /* 0x01 */ (SyscallHandlerFunction)writeHandler,
    /* 0x02 */ (SyscallHandlerFunction)timeHandler,
    /* 0x03 */ (SyscallHandlerFunction)closeHandler,
    /* 0x04 */ (SyscallHandlerFunction)clearScreenHandler,
    /* 0x05 */ (SyscallHandlerFunction)killHandler,
    /* 0x06 */ (SyscallHandlerFunction)blockHandler,
    /* 0x07 */ (SyscallHandlerFunction)unblockHandler,
    /* 0x08 */ (SyscallHandlerFunction)printmemHandler,
    /* 0x09 */ (SyscallHandlerFunction)createHandler,
    /* 0x0A */ (SyscallHandlerFunction)dateHandler,
    /* 0x0B */ (SyscallHandlerFunction)inforegHandler,
    /* 0x0C */ (SyscallHandlerFunction)mallocHandler,
    /* 0x0D */ (SyscallHandlerFunction)freeHandler,
    /* 0x0E */ (SyscallHandlerFunction)reallocHandler,
    /* 0x0F */ (SyscallHandlerFunction)yieldHandler,
    /* 0x10 */ (SyscallHandlerFunction)getpidHandler,
    /* 0x11 */ (SyscallHandlerFunction)priorityHandler,
    /* 0x12 */ (SyscallHandlerFunction)exitHandler,
    /* 0x13 */ (SyscallHandlerFunction)NULL,
    /* 0x14 */ (SyscallHandlerFunction)NULL,
    /* 0x15 */ (SyscallHandlerFunction)NULL,
    /* 0x16 */ (SyscallHandlerFunction)pipeHandler
};

size_t syscallDispatcher(size_t rdi, size_t rsi, size_t rdx, size_t r10, size_t r8, size_t rax) {
    SyscallHandlerFunction handler;
    if (rax < (sizeof(syscallHandlers) / sizeof(syscallHandlers[0])) && (handler = syscallHandlers[rax]) != NULL)
        return handler(rdi, rsi, rdx, r10, r8);
    return -1;
}

void storeRegisters(uint64_t* start) {

    char* reg_text[] = {"RAX: 0x", "R15: 0x", "R14: 0x", "R13: 0x", "R12: 0x", "R11: 0x", "R10: 0x", "R9:  0x",
                        "R8:  0x", "RSI: 0x", "RDI: 0x", "RBP: 0x", "RDX: 0x", "RCX: 0x", "RBX: 0x", "RSP: 0x", 0};
    uint32_t j = 0; // store counter

    for (int i = 0; reg_text[i]; ++i) {
        // Agregamos el string al store
        int m = 0;
        while (reg_text[i][m])
            store[j++] = reg_text[i][m++];

        // Agregamos el nro al store
        uint64_t aux = start[i];
        int64_t count = 16;
        while (aux) {
            aux = aux >> 4;
            --count;
        }
        for (int k = 0; k < count; k++) {
            store[j++] = '0';
        }

        if (start[i]) {
            j += uintToBase(start[i], store + j, 16);
        }
        store[j++] = '\n';
    }
    store[j] = 0;
}