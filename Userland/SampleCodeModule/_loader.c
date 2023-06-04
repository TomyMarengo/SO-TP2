#include <userlib.h>

extern char bss;
extern char endOfBinary;

int main(int argc, char *argv[]);

int
_start(int argc, char *argv[]) {
    // Clean BSS
    memset(&bss, 0, &endOfBinary - &bss);
    return main(0, 0);
}

