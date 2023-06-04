#include <interrupts.h>
#include <memoryManager.h>
#include <scheduler.h>

#define QUANTUM 5

// Pseudo PIDs for limit cases
#define PSEUDOPID_KERNEL -1
#define PSEUDOPID_NONE   -2

typedef struct {
    Priority priority;
    ProcessStatus status;
    void *currentRSP;
} ProcessControlBlock;

static void *mainRSP;

static ProcessControlBlock processTable[MAX_PROCESSES];
static Pid currentRunningPID;
static Pid forceRunNextPID;
static uint8_t currentQuantum;

extern void *createProcessStack(int argc, const char *const argv[], void *rsp, ProcessStart start);

static int
isValidPid(Pid pid) {
    return pid >= 0 && pid < MAX_PROCESSES;
}

static int
isActiveProcess(Pid pid) {
    return isValidPid(pid) && processTable[pid].currentRSP != NULL;
}

static int
isReadyProcess(Pid pid) {
    return isActiveProcess(pid) && processTable[pid].status == READY;
}

static int
getCurrentQuantum(Pid pid) {
    return PRIORITY_MIN - processTable[pid].priority;
}

static Pid
getNextReadyPid() {
    Pid first = currentRunningPID < 0 ? 0 : currentRunningPID;
    Pid next = first;

    do {
        next = (next + 1) % MAX_PROCESSES;
        if (isReadyProcess(next)) {
            return next;
        }
    } while (next != first);

    return PSEUDOPID_KERNEL;
}

static int
getProcessState(Pid pid, ProcessControlBlock **pcb) {
    if (!isActiveProcess(pid))
        return 0;

    *pcb = &processTable[pid];
    return 1;
}

void
initializeScheduler() {
    forceRunNextPID = PSEUDOPID_NONE;
    currentRunningPID = PSEUDOPID_KERNEL;
    currentQuantum = 0;
}

int
onProcessCreated(Pid pid, ProcessStart start, Priority priority, void *currentRSP, int argc, const char *const argv[]) {
    if (priority < PRIORITY_MAX || priority > PRIORITY_MIN)
        priority = PRIORITY_DEFAULT;

    processTable[pid].priority = priority;
    processTable[pid].status = READY;
    processTable[pid].currentRSP = createProcessStack(argc, argv, currentRSP, start);
    return 0;
}

int
onProcessKilled(Pid pid) {
    ProcessControlBlock *pcb;
    if (!getProcessState(pid, &pcb))
        return 1;

    if (pcb->status == KILLED)
        return 0;

    pcb->status = KILLED;
    pcb->currentRSP = NULL;

    if (currentRunningPID == pid)
        currentRunningPID = PSEUDOPID_NONE;

    return 0;
}

int
block(Pid pid) {
    ProcessControlBlock *pcb;
    if (!getProcessState(pid, &pcb))
        return 1;

    pcb->status = BLOCKED;

    if (currentRunningPID == pid)
        currentQuantum = 0;

    return 0;
}

int
unblock(Pid pid) {
    ProcessControlBlock *pcb;
    if (!getProcessState(pid, &pcb))
        return 1;

    if (pcb->status == READY || pcb->status == RUNNING)
        return 0;

    if (pcb->priority <= PRIORITY_IMPORTANT)
        forceRunNextPID = pid;

    pcb->status = READY;

    return 0;
}

Pid
getpid() {
    return currentRunningPID;
}

int
setPriority(Pid pid, Priority newPriority) {
    ProcessControlBlock *pcb;
    if (!getProcessState(pid, &pcb))
        return 1;

    if (newPriority < PRIORITY_MAX || newPriority > PRIORITY_MIN)
        return 1;

    pcb->priority = newPriority;

    return 0;
}

void
yield() {
    currentQuantum = 0;
    int81();
}

void *
switchProcess(void *currentRSP) {
    if (currentRunningPID >= 0) {
        processTable[currentRunningPID].currentRSP = currentRSP;
        if (processTable[currentRunningPID].status == RUNNING)
            processTable[currentRunningPID].status = READY;
    } else if (currentRunningPID == PSEUDOPID_KERNEL)
        mainRSP = currentRSP;

    if (isReadyProcess(forceRunNextPID)) {
        currentRunningPID = forceRunNextPID;
        forceRunNextPID = PSEUDOPID_NONE;
        currentQuantum = getCurrentQuantum(currentRunningPID);
    } else if (!isReadyProcess(currentRunningPID) || currentQuantum == 0) {
        currentRunningPID = getNextReadyPid();

        if (currentRunningPID == PSEUDOPID_KERNEL) {
            currentQuantum = 0;
            return mainRSP;
        }

        currentQuantum = getCurrentQuantum(currentRunningPID);
    } else {
        currentQuantum -= 1;
    }

    processTable[currentRunningPID].status = RUNNING;
    return processTable[currentRunningPID].currentRSP;
}

int
getProcessInfo(Pid pid, ProcessInfo *processInfo) {
    ProcessControlBlock *pcb;
    if (!getProcessState(pid, &pcb))
        return 1;

    processInfo->status = pcb->status;
    processInfo->priority = pcb->priority;
    processInfo->currentRSP = pcb->currentRSP;
    return 0;
}