#include <stdint.h>
#include <sys/types.h>
#include <process.h>
#include <scheduler.h>
#include <memoryManager.h>
#include <pipe.h>
#include <string.h>

#define FD_TABLE_CHUNK_SIZE 8
#define FD_TABLE_MAX_ENTRIES 64

typedef struct {
    void* resource;
    TFdReadHandler readHandler;
    TFdWriteHandler writeHandler;
    TFdCloseHandler closeHandler;
} FDEntry;

typedef struct {
	int isForeground;
	void* stackStart;
    void* stackEnd;
    char* name;
    FDEntry* fdTable;
	unsigned int fdTableSize;
	char** argv;
    int argc;
} Process;

static Process processes[MAX_PROCESSES];

static int deleteFdUnchecked(Process* process, Pid pid, int fd);

static int getProcessByPid(Pid pid, Process** outProcess) {
    if (pid < 0 || pid >= MAX_PROCESSES || processes[pid].stackEnd == NULL)
        return 0;

    *outProcess = &processes[pid];
    return 1;
}

static int isNameValid(const char* name) {
    if (name == NULL)
        return 0;

    for (int i = 0; i <= MAX_NAME_LENGTH; i++) {
        char c = name[i];
        if (c == '\0')
            return 1;

        // The first character must be a letter. Subsequent characters may be a letter or a number.
        if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && c != '_') {
            if (i == 0 || c < '0' || c > '9')
                return 0;
        }
    }

    return 0;
}

Pid prc_create(const ProcessCreateInfo* createInfo) {
    Pid pid = 0;
    for (; pid < MAX_PROCESSES && processes[pid].stackEnd != NULL; pid++);

    if (createInfo->argc < 0 || pid == MAX_PROCESSES || !isNameValid(createInfo->name))
        return -1;

    void* stackEnd = NULL;
    char* nameCopy = NULL;
    char** argvCopy = NULL;
    if ((stackEnd = mm_malloc(PROCESS_STACK_SIZE)) == NULL
        || (nameCopy = mm_malloc(strlen(createInfo->name) + 1)) == NULL
        || (createInfo->argc != 0 && (argvCopy = mm_malloc(sizeof(char*) * createInfo->argc)) == NULL)) {
        mm_free(stackEnd);
        mm_free(nameCopy);
        return -1;
    }

    for (int i = 0; i < createInfo->argc; ++i) {
        size_t length = strlen(createInfo->argv[i]) + 1;

        if ((argvCopy[i] = mm_malloc(length)) == NULL) {
            mm_free(stackEnd);
            mm_free(nameCopy);
            while (i > 0) {
                i--;
                mm_free(argvCopy[i]);
            }
            mm_free(argvCopy);
            return -1;
        }

        memcpy(argvCopy[i], createInfo->argv[i], length);
    }

    strcpy(nameCopy, createInfo->name);

    Process* process = &processes[pid];

    memset(process, 0, sizeof(Process));
    process->stackEnd = stackEnd;
    process->stackStart = stackEnd + PROCESS_STACK_SIZE;
    process->isForeground = createInfo->isForeground;
    process->name = nameCopy;
    process->argv = argvCopy;
    process->argc = createInfo->argc;


	sch_onProcessCreated(pid, createInfo->start, createInfo->priority, process->stackStart, createInfo->argc, (const char* const*)argvCopy);

    return pid;
}

int prc_kill(Pid pid) {
    Process* process;
    if (!getProcessByPid(pid, &process))
        return 1;

    for (int fd = 0; fd < process->fdTableSize; fd++)
        if (process->fdTable[fd].resource != NULL)
            deleteFdUnchecked(process, pid, fd);

    sch_onProcessKilled(pid);

	for (int i = 0; i < process->argc; i++){
        mm_free(process->argv[i]);
    }
    mm_free(process->argv);
    mm_free(process->stackEnd);
    mm_free(process->fdTable);
    process->stackEnd = NULL;
    process->stackStart = NULL;
    process->fdTable = NULL;
	process->fdTableSize = 0;

    return 0;
}

int prc_isForeground(Pid pid) {
    Process* process;
    if (!getProcessByPid(pid, &process))
        return -1;

    return process->isForeground;
}

int prc_setIsForeground(Pid pid, int isForeground) {
    Process* process;
    if (!getProcessByPid(pid, &process))
        return -1;

    return process->isForeground = (isForeground != 0);
    return 0;
}

int prc_addFd(Pid pid, int fd, void* resource, TFdReadHandler readHandler, TFdWriteHandler writeHandler, TFdCloseHandler closeHandler) {
    Process* process;
    if (resource == NULL || !getProcessByPid(pid, &process))
        return -1;

	if (fd < 0) {
        // Look for the lowest available file descriptor.
        for (fd = 0; fd < process->fdTableSize && process->fdTable[fd].resource != NULL; fd++);

    } else {
        // Check that the requested fd is available.
        if (fd < process->fdTableSize && process->fdTable[fd].resource != NULL)
            return -1;
    }

    // If the table doesn't have enough space for this fd, expand it.
    if (fd >= process->fdTableSize) {
        size_t newFdTableSize = (process->fdTableSize + FD_TABLE_CHUNK_SIZE) / FD_TABLE_CHUNK_SIZE * FD_TABLE_CHUNK_SIZE;
        if (newFdTableSize > FD_TABLE_MAX_ENTRIES)
            newFdTableSize = FD_TABLE_MAX_ENTRIES;

        if (fd >= newFdTableSize)
            return -1;

        FDEntry* newFdTable = mm_realloc(process->fdTable, sizeof(FDEntry) * newFdTableSize);
        if (newFdTable == NULL)
            return -1;

        memset(&newFdTable[process->fdTableSize], 0, sizeof(FDEntry) * (newFdTableSize - process->fdTableSize));
        process->fdTable = newFdTable;
        process->fdTableSize = newFdTableSize;
    }

    process->fdTable[fd].resource = resource;
    process->fdTable[fd].readHandler = readHandler;
    process->fdTable[fd].writeHandler = writeHandler;
    process->fdTable[fd].closeHandler = closeHandler;

    return 1;
}

int prc_deleteFd(Pid pid, int fd) {
    Process* process;
    if (fd < 0 || !getProcessByPid(pid, &process) || process->fdTableSize <= fd || process->fdTable[fd].resource == NULL)
        return 1;

	return deleteFdUnchecked(process, pid, fd);
}

static int deleteFdUnchecked(Process* process, Pid pid, int fd) {
    FDEntry* entry = &process->fdTable[fd];
    int r;
    if (entry->closeHandler != NULL && (r = entry->closeHandler(pid, fd, entry->resource)) != 0)
        return r;

    entry->resource = NULL;
    entry->readHandler = NULL;
    entry->writeHandler = NULL;
    entry->closeHandler = NULL;
    return 0;
}

ssize_t prc_handleReadFd(Pid pid, int fd, char* buf, size_t count) {
    Process* process;
    FDEntry* entry;
    if (fd < 0 || !getProcessByPid(pid, &process) || process->fdTableSize <= fd
        || (entry = &process->fdTable[fd])->resource == NULL || entry->readHandler == NULL)
        return -1;

    return entry->readHandler(pid, fd, entry->resource, buf, count);
}

ssize_t prc_handleWriteFd(Pid pid, int fd, const char* buf, size_t count) {
    Process* process;
    FDEntry* entry;
    if (fd < 0 || !getProcessByPid(pid, &process) || process->fdTableSize <= fd
        || (entry = &process->fdTable[fd])->resource == NULL || entry->writeHandler == NULL)
        return -1;

    return entry->writeHandler(pid, fd, entry->resource, buf, count);
}

uint8_t prc_listProcesses(ProcessInfo* array, uint8_t maxProcesses) {
    int processCounter = 0;
    for (int i = 0; i < MAX_PROCESSES && processCounter < maxProcesses; ++i) {
        Process* process = &processes[i];
        if (process->stackEnd != NULL) {
            ProcessInfo* info = &array[processCounter++];
            info->pid = i;
            //strncpy(info->name, process->name, MAX_NAME_LENGTH);
            info->stackEnd = process->stackEnd;
            info->stackStart = process->stackStart;
            info->isForeground = process->isForeground;
            sch_getProcessInfo(i, info);
        }
    }

    return processCounter;
}