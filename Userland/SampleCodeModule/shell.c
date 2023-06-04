#include <shell.h>
#include <userlib.h>
#include <programs.h>
#include <string.h>
#include <syscalls.h>
#include <test.h>

#define MAX_PARAMS 8

char *welcomeMessage = "Welcome to the Shell!\nType help for a list of available commands...\n";
char *noCommandMessage = "Command not found\nTry typing help for a list of available commands!\n";

typedef struct command {
    ProcessStart functionAddress;
    char *name;
} command;

command availableCommands[] = {
    {&testMM, "testMM"}, {&testProcesses, "testProcesses"}, {&testPrio, "testPrio"}, {&testSync, "testSync"}, {&ps, "ps"}};
size_t dimCommands = 5;

static Pid runningPrograms[MAX_PROCESSES - 1];  // C inicializa en 0 ... como shell es proceso 0 entonces es valido

void
runShell() {
    print(welcomeMessage);
    while (1) {
        char buf[128];
        char com[128];
        while (sys_read(STDIN, buf, 128)) {
            printf(buf);
            if (strcmp(buf, "\n") == 0) {
                commandDispatcher(com);
                strcpy(com, "");
            } else {
                strcat(com, buf);
            }
        }
    }
}

void
commandDispatcher(char *command) {
    int index = -1;
    for (int i = 0; i < MAX_PROCESSES - 1; i++) {
        if (runningPrograms[i] == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("No se pueden inicializar mas programas...\nPrueba terminar alguno\n");
        return;
    }

    char *commandName = my_strtok(command, ' ');
    char *args[MAX_PARAMS + 1];

    int argc = 0;
    while (commandName != NULL) {
        args[argc] = commandName;
        argc++;
        commandName = my_strtok(NULL, ' ');
    }

    commandName = args[0];

    for (int i = 0; i < argc; i++) {
        args[i] = args[i + 1];
    }
    argc--;
    args[argc] = NULL;

    for (int i = 0; i < dimCommands; i++) {
        if (strcmp(availableCommands[i].name, commandName) == 0) {
            ProcessCreateInfo processInfo = {
                availableCommands[i].name, (ProcessStart) availableCommands[i].functionAddress, 1, PRIORITY_DEFAULT, argc, args};
            runningPrograms[index] = sys_createProcess(KBDIN, STDOUT, STDERR, &processInfo);
            return 0;
        }
    }
    printf(noCommandMessage);
    return 1;
}
