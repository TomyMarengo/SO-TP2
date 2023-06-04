#include <programlib.h>

void ps(int argc, char* argv[]){

    if (argc != 0 )
    {
        printf("Error, wrong usage of ps\n");
        return;
    }
    
    ProcessInfo processes[MAX_PROCESSES] = {};
    
    sys_listProcesses(processes,MAX_PROCESSES);

    for (int i = 0 ; i < MAX_PROCESSES ; i++)
    {
        if (processes[i].stackEnd == 0)
            continue;
        printf("PID: %d   name: %s status: %d\n", processes[i].pid, processes[i].name, processes[i].status);
    }

    // sys_kill(sys_getpid());
}