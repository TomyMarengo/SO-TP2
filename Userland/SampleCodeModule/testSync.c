#include <testUtil.h>
#include <syscalls.h>
#include <userlib.h>

/* Constants */
#define SEM_ID               "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global;  // shared memory

void
slowInc(int64_t *p, int64_t inc) {
    uint64_t aux = *p;
    sys_yield();  // This makes the race condition highly probable
    aux += inc;
    *p = aux;
}

void
myProcessInc(int argc, char *argv[]) {
    uint64_t n;
    int8_t inc;
    int8_t use_sem;

    if (argc != 3) {
        fprintf(STDERR, "Must receive three arguments: n, inc, useSem\n");
        return;
    }

    if ((n = satoi(argv[0])) <= 0)
        return;
    if ((inc = satoi(argv[1])) == 0)
        return;
    if ((use_sem = satoi(argv[2])) < 0)
        return;

    Sem sem;

    if (use_sem) {
        if ((sem = sys_openSem(SEM_ID, 1)) < 0) {
            printf("test_sync: ERROR opening semaphore\n");
            return;
        }
    }

    uint64_t i;
    for (i = 0; i < n; i++) {
        if (use_sem)
            sys_wait(sem);
        slowInc(&global, inc);
        if (use_sem)
            sys_post(sem);
    }

    if (use_sem)
        sys_closeSem(sem);
}

void
testSync(int argc, char *argv[]) {
    uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

    if (argc != 2)
    {
        printf("Wrong usage...\nTest sync takes two arguments:\nn -Integer amount of times to Inc/Dec\nuse_sem -0 for False integer for True \n");
        return;
    }
        
    char *argvDec[] = {argv[0], "-1", argv[1], NULL};
    char *argvInc[] = {argv[0], "1", argv[1], NULL};

    ProcessCreateInfo decInfo = {.name = "processDec",
                                 .isForeground = 1,
                                 .priority = PRIORITY_DEFAULT,
                                 .start = (ProcessStart) myProcessInc,
                                 .argc = 3,
                                 .argv = (const char *const *) argvDec};

    ProcessCreateInfo incInfo = {.name = "processInc",
                                 .isForeground = 1,
                                 .priority = PRIORITY_DEFAULT,
                                 .start = (ProcessStart) myProcessInc,
                                 .argc = 3,
                                 .argv = (const char *const *) argvInc};

    global = 0;

    uint64_t i;
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
        pids[i] = sys_createProcess(-1, STDOUT, STDERR, &decInfo);
        pids[i + TOTAL_PAIR_PROCESSES] = sys_createProcess(-1, STDOUT, STDERR, &incInfo);
    }

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
        sys_waitpid(pids[i]);
        sys_waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
    }

    printf("Final value: %d\n", global);

    return;
}