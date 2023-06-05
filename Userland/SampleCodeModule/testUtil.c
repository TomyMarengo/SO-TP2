#include <testUtil.h>
#include <syscalls.h>
#include <userlib.h>

void
bussyWait(uint64_t n) {
    uint64_t i;
    for (i = 0; i < n; i++)
        ;
}

void
endlessLoop(int argc, char *argv[]) {
    while (1)
        ;
}

void
endlessLoopPrint(int argc, char *argv[]) {
    Pid pid = sys_getpid();

    while (1) {
        printf("%d ", pid);
        bussyWait(9000000);
    }
}

uint8_t
getMaxAvailableProcesses(){
    ProcessInfo array[MAX_PROCESSES];
    int count = sys_listProcesses(array, MAX_PROCESSES);

    return MAX_PROCESSES - count;
}
