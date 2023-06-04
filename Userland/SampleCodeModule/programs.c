#include <programs.h>
#include <userlib.h>

void
ps(int argc, char *argv[]) {

    if (argc != 0) {
        printf("Error, wrong usage of ps\n");
        return;
    }

    ProcessInfo processes[MAX_PROCESSES];

    sys_listProcesses(processes, MAX_PROCESSES);

    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == 0 && i != 0)
            return;
        printf("PID: %d   name: %s   status: %d\n", processes[i].pid, processes[i].name, processes[i].status);
    }

    sys_kill(sys_getpid());
}


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