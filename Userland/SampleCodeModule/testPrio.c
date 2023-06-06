#include <syscalls.h>
#include <testUtil.h>
#include <userlib.h>

#define MINOR_WAIT 1000000  // TODO: Change this value to prevent a process from flooding the screen
#define WAIT       100000000  // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST          PRIORITY_MIN  // TODO: Change as required
#define MEDIUM          0             // TODO: Change as required
#define HIGHEST         PRIORITY_MAX  // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void
testPrio(int argc, char *argv[]) {
    int64_t pids[TOTAL_PROCESSES];
    char *argvAux[] = {0};
    uint64_t i;

    ProcessCreateInfo endlessInfo = {.name = "endless",
                                     .isForeground = 1,
                                     .priority = PRIORITY_DEFAULT,
                                     .start = (ProcessStart) endlessLoopPrint,
                                     .argc = 0,
                                     .argv = (const char *const *) argvAux};

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = sys_createProcess(-1, -1, -1, &endlessInfo);

    sleep(1000);
    printf("\nCHANGING PRIORITIES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sys_priority(pids[i], prio[i]);

    sleep(1000);
    printf("\nBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sys_block(pids[i]);

    printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sys_priority(pids[i], MEDIUM);

    printf("UNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sys_unblock(pids[i]);

    sleep(1000);
    printf("\nKILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sys_kill(pids[i]);
}