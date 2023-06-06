#include <defs.h>
#include <graphics.h>
#include <keyboard.h>
#include <lib.h>
#include <memoryManager.h>
#include <pipe.h>
#include <process.h>
#include <scheduler.h>
#include <sem.h>
#include <time.h>

typedef size_t (*SyscallHandlerFunction)(size_t rdi, size_t rsi, size_t rdx, size_t r10, size_t r8);

static ssize_t
readHandler(int fd, char *buffer, size_t count) {
    ssize_t c = handleRead(getpid(), fd, buffer, count);
    if (buffer[0] && getCtrlState() == 1) {
        return -1;
    }
    return c;
}

static ssize_t
writeHandler(int fd, const char *buffer, size_t count) {
    return handleWrite(getpid(), fd, buffer, count);
}

static int
closeHandler(int fd) {
    return deleteFd(getpid(), fd);
}

static int
clearScreenHandler() {
    if (!isForeground(getpid()))
        return 1;

    clearScreen();
    return 0;
}

static unsigned long
millisHandler() {
    return TICKS_TO_MILLISECONDS(getElapsedTicks());
}

static int
timeHandler(char *buffer) {
    getCurrentTimeString(buffer);
    return 0;
}

static int
dateHandler(char *buffer) {
    getCurrentDateString(buffer);
    return 0;
}

static void *
mallocHandler(size_t size) {
    return handleMalloc(getpid(), size);
}

static int
freeHandler(void *ptr) {
    return handleFree(getpid(), ptr);
}

static void *
reallocHandler(void *ptr, size_t size) {
    return handleRealloc(getpid(), ptr, size);
}

static int
memoryStateHandler(MemoryState *memoryState) {
    return getStateMemory(memoryState);
}

static Pid
getpidHandler() {
    return getpid();
}

static Pid
createProcessHandler(int stdinMapFd, int stdoutMapFd, int stderrMapFd, const ProcessCreateInfo *createInfo) {
    Pid callerPid = getpid();
    Pid newPid = createProcess(createInfo);

    if (newPid < 0)
        return newPid;

    if (stdinMapFd < 0)
        addFdKeyboard(newPid, STDIN);
    else
        dupFd(callerPid, newPid, stdinMapFd, STDIN);

    if (stdoutMapFd < 0)
        addFdScreen(newPid, STDOUT, &WHITE);
    else
        dupFd(callerPid, newPid, stdoutMapFd, STDOUT);

    if (stderrMapFd < 0)
        addFdScreen(newPid, STDERR, &RED);
    else
        dupFd(callerPid, newPid, stderrMapFd, STDERR);

    return newPid;
}

static int
exitHandler() {
    kill(getpid());
    yield();
    return 1;
}

static int
blockHandler(Pid pid) {
    int result = block(pid);
    if (pid == getpid())
        yield();
    return result;
}

static int
unblockHandler(Pid pid) {
    return unblock(pid);
}

static void
yieldHandler() {
    yield();
}

static int
killHandler(Pid pid) {
    int result = kill(pid);
    if (pid == getpid())
        yield();
    return result;
}

static int
priorityHandler(Pid pid, Priority priority) {
    return setPriority(pid, priority);
}

static int
listProcessesHandler(ProcessInfo *array, int maxProcesses) {
    return listProcesses(array, maxProcesses);
}

static int
waitpidHandler(Pid pid) {
    Pid currentPid = getpid();
    if (pid == currentPid || unblockOnKilled(currentPid, pid))
        return 1;

    block(currentPid);
    yield();
    return 0;
}

static int
createPipeHandler(int pipefd[2]) {
    Pid pid = getpid();

    Pipe pipe;
    int readFd = -1, writeFd = -1;

    if ((pipe = createPipe()) < 0 || (readFd = addFdPipe(pid, -1, pipe, 1, 0)) < 0 ||
        (writeFd = addFdPipe(pid, -1, pipe, 0, 1)) < 0) {
        if (readFd >= 0)
            deleteFd(pid, readFd);
        if (pipe >= 0)
            freePipe(pipe);
        return 1;
    }

    pipefd[0] = readFd;
    pipefd[1] = writeFd;
    return 0;
}

static int
openPipeHandler(const char *name, int pipefd[2]) {
    Pid pid = getpid();

    Pipe pipe;
    int readFd = -1, writeFd = -1;

    if ((pipe = openPipe(name)) < 0 || (readFd = addFdPipe(pid, -1, pipe, 1, 0)) < 0 ||
        (writeFd = addFdPipe(pid, -1, pipe, 0, 1)) < 0) {
        if (readFd >= 0)
            deleteFd(pid, readFd);
        if (pipe >= 0)
            freePipe(pipe);
        return 1;
    }

    pipefd[0] = readFd;
    pipefd[1] = writeFd;
    return 0;
}

static int
unlinkPipeHandler(const char *name) {
    return unlinkPipe(name);
}

static int
listPipesHandler(PipeInfo *array, int maxPipes) {
    return listPipes(array, maxPipes);
}

static Sem
openSemHandler(const char *name, unsigned int value) {
    return openSem(name, value);
}

static int
closeSemHandler(Sem sem) {
    return closeSem(sem);
}

static int
postSemHandler(Sem sem) {
    return post(sem);
}

static int
waitSemHandler(Sem sem) {
    return wait(sem);
}

static int
listSemaphoresHandler(SemaphoreInfo *array, int maxSemaphores) {
    return listSemaphores(array, maxSemaphores);
}

static SyscallHandlerFunction syscallHandlers[] = {
    /* I/O syscalls */
    /* 0x00 */ (SyscallHandlerFunction) readHandler,
    /* 0x01 */ (SyscallHandlerFunction) writeHandler,
    /* 0x02 */ (SyscallHandlerFunction) closeHandler,
    /* 0x03 -> 0x0F*/ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,

    /* Graphics-related syscalls */
    /* 0x10 */ (SyscallHandlerFunction) clearScreenHandler,
    /* 0x11 -> 0x1F */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,

    /* Time & date syscalls*/
    /* 0x20 */ (SyscallHandlerFunction) millisHandler,
    /* 0x21 */ (SyscallHandlerFunction) timeHandler,
    /* 0x22 */ (SyscallHandlerFunction) dateHandler,
    /* 0x23 -> 0x2F */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,

    /* Memory management syscalls */
    /* 0x30 */ (SyscallHandlerFunction) mallocHandler,
    /* 0x31 */ (SyscallHandlerFunction) freeHandler,
    /* 0x32 */ (SyscallHandlerFunction) reallocHandler,
    /* 0x33 */ (SyscallHandlerFunction) memoryStateHandler,
    /* 0x34 -> 0x3F */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,

    /* Process-related syscalls */
    /* 0x40 */ (SyscallHandlerFunction) getpidHandler,
    /* 0x41 */ (SyscallHandlerFunction) createProcessHandler,
    /* 0x42 */ (SyscallHandlerFunction) exitHandler,
    /* 0x43 */ (SyscallHandlerFunction) blockHandler,
    /* 0x44 */ (SyscallHandlerFunction) unblockHandler,
    /* 0x45 */ (SyscallHandlerFunction) yieldHandler,
    /* 0x46 */ (SyscallHandlerFunction) killHandler,
    /* 0x47 */ (SyscallHandlerFunction) priorityHandler,
    /* 0x48 */ (SyscallHandlerFunction) listProcessesHandler,
    /* 0x49 */ (SyscallHandlerFunction) waitpidHandler,
    /* 0x4A -> 0x4F */ NULL, NULL, NULL, NULL, NULL, NULL,

    /* Pipe syscalls */
    /* 0x50 */ (SyscallHandlerFunction) createPipeHandler,
    /* 0x51 */ (SyscallHandlerFunction) openPipeHandler,
    /* 0x52 */ (SyscallHandlerFunction) unlinkPipeHandler,
    /* 0x53 */ (SyscallHandlerFunction) listPipesHandler,
    /* 0x54 -> 0x5F */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,

    /* Semaphore syscalls */
    /* 0x60 */ (SyscallHandlerFunction) openSemHandler,
    /* 0x61 */ (SyscallHandlerFunction) closeSemHandler,
    /* 0x62 */ NULL,
    /* 0x63 */ (SyscallHandlerFunction) postSemHandler,
    /* 0x64 */ (SyscallHandlerFunction) waitSemHandler,
    /* 0x65 */ (SyscallHandlerFunction) listSemaphoresHandler};

size_t
syscallDispatcher(size_t rdi, size_t rsi, size_t rdx, size_t r10, size_t r8, size_t rax) {
    SyscallHandlerFunction handler;
    if (rax < (sizeof(syscallHandlers) / sizeof(syscallHandlers[0])) && (handler = syscallHandlers[rax]) != NULL)
        return handler(rdi, rsi, rdx, r10, r8);
    return -1;
}