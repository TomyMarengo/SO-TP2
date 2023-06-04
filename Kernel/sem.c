#include <defs.h>
#include <sem.h>
#include <lib.h>
#include <memoryManager.h>
#include <namer.h>
#include <scheduler.h>
#include <string.h>
#include <waitingQueue.h>

typedef struct {
    uint8_t value;
    Lock lock;
    uint8_t linkedProcesses;
    const char* name;
    WaitingQueue processesWQ; 
} Semaphore;

static Semaphore* semaphores[MAX_SEMAPHORES] = {NULL};
static Namer namer;
static Lock generalLock;

extern int spinLock(Lock* lock);
extern void unlock(Lock* lock);
static int freeSem(Sem sem);
static int isValidSemId(Sem sem);
static int adquireSem(Sem sem);

static int freeSem(Sem sem) {
    int value = deleteResource(namer, semaphores[sem]->name) == NULL;
    value += freeQueue(semaphores[sem]->processesWQ);
    value += free(semaphores[sem]);
    semaphores[sem] = NULL;
    if (value != 0)
        return SEM_FAIL;
    return SEM_OK;
}

static int isValidSemId(Sem sem) {
    return sem > 0 && sem < MAX_SEMAPHORES && semaphores[sem] != NULL;
}

static int adquireSem(Sem sem) {
    spinLock(&generalLock);

    if (!isValidSemId(sem)) {
        unlock(&generalLock);
        return SEM_NOT_EXISTS;
    }

    spinLock(&(semaphores[sem]->lock));
    unlock(&generalLock);
    return SEM_OK;
}

int initializeSem() {
    namer = newNamer();
    generalLock = 0;
    if (namer != 0)
        return SEM_FAIL;
    return 0;
}

Sem openSem(const char* name, uint8_t initialValue) {

    spinLock(&generalLock);

    Sem sem = (Sem)(int64_t)getResource(namer, name);

    if (sem != 0) {
        semaphores[sem]->linkedProcesses += 1;
        unlock(&generalLock);
        return sem;
    }

    int i;
    for (i = 1; i < MAX_SEMAPHORES && semaphores[i]; ++i)
        ;

    if (i == MAX_SEMAPHORES) {
        unlock(&generalLock);
        return SEM_FAIL;
    }

    semaphores[i] = malloc(sizeof(Semaphore));
    if (semaphores[i] == NULL) {
        unlock(&generalLock);
        return SEM_FAIL;
    }
    semaphores[i]->value = initialValue;
    unlock(&semaphores[i]->lock);
    semaphores[i]->linkedProcesses = 1;
    semaphores[i]->processesWQ = newQueue();

    if (semaphores[i]->processesWQ == NULL) {
        free(semaphores[i]);
        unlock(&generalLock);
        return SEM_FAIL;
    }

    if (addResource(namer, (void*)(int64_t)i, name, &(semaphores[i]->name)) != 0) {
        freeQueue(semaphores[i]->processesWQ);
        free(semaphores[i]);
        semaphores[i] = NULL;
        unlock(&generalLock);
        return SEM_FAIL;
    }

    unlock(&generalLock);
    return (Sem)i;
}

int closeSem(Sem sem) {

    if (adquireSem(sem) == SEM_NOT_EXISTS) {
        return SEM_NOT_EXISTS;
    }

    if (semaphores[sem]->linkedProcesses == 1) {
        return freeSem(sem);
    }

    semaphores[sem]->linkedProcesses -= 1;
    unlock(&semaphores[sem]->lock);
    return SEM_OK;
}

int post(Sem sem) {

    if (adquireSem(sem) == SEM_NOT_EXISTS) {
        return SEM_NOT_EXISTS;
    }

    semaphores[sem]->value++;
    unblockInQueue(semaphores[sem]->processesWQ);

    unlock(&semaphores[sem]->lock);
    return SEM_OK;
}

int wait(Sem sem) {

    if (adquireSem(sem) == SEM_NOT_EXISTS) {
        return SEM_NOT_EXISTS;
    }

    Pid cpid = getpid();

    while (semaphores[sem]->value == 0) {
        addInQueue(semaphores[sem]->processesWQ, cpid);
        unlock(&semaphores[sem]->lock);
        block(cpid);
        yield();
        spinLock(&(semaphores[sem]->lock));
    }

    semaphores[sem]->value--;
    unlock(&semaphores[sem]->lock);
    return SEM_OK;
}

int listSemaphores(SemaphoreInfo* storingInfo, int maxSemaphores) {
    spinLock(&generalLock);
    int semCounter = 0;

    for (int i = 1; i < MAX_SEMAPHORES && semCounter < maxSemaphores; ++i) {
        Semaphore* sem = semaphores[i];
        if (sem != NULL) {
            SemaphoreInfo* info = &storingInfo[semCounter++];
            info->value = sem->value;
            info->linkedProcesses = sem->linkedProcesses;

            if (sem->name == NULL)
                info->name[0] = '\0';
            else
                strncpy(info->name, sem->name, MAX_NAME_LENGTH);

            int waitingPids = listPidsInQueue(sem->processesWQ, info->processesWQ, entriesInQueue(sem->processesWQ));
            info->processesWQ[waitingPids] = -1;
        }
    }
    unlock(&generalLock);
    return semCounter;
}