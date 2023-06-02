/* Local headers */
#include <shell.h>
#include <syscalls.h>
#include <userstdlib.h>

int
main(int argc, char *argv[]) {
    welcome_message();
    while (1) {
        fputChar(STDERR, '>');
        wait_command();
    }
}