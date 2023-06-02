#include <graphics.h>
#include <keyboard.h>
#include <scheduler.h>
#include <process.h>
#include <kernel.h>
#include <interrupts.h>
#include <defs.h>

static const char* exceptionMessages[] = {
    /* 0x00 */ "Divide by Zero",
    /* 0x01 */ 0,
    /* 0x02 */ 0,
    /* 0x03 */ 0,
    /* 0x04 */ 0,
    /* 0x05 */ 0,
    /* 0x06 */ "Invalid Opcode",
    /* 0x07 */ 0,
    /* 0x08 */ 0,
    /* 0x09 */ 0,
    /* 0x0A */ 0,
    /* 0x0B */ 0,
    /* 0x0C */ 0,
    /* 0x0D */ "General Protection",
    /* 0x0E */ "Page Fault",
};

static const char* registerNames[18] = {
    "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15", "RIP", "RFLAGS"
};

void exceptionDispatcher(uint64_t exception, const uint64_t regdata[18]) {
    Pid pid = getpid();
    print("PID ");
    printDec(pid);
    print(" CRASHED! Unhandled exception: (0x");
    printHex(exception);
    print(") ");
    print(exception < (sizeof(exceptionMessages) / sizeof(exceptionMessages[0])) ? exceptionMessages[exception] : "Unknown");
    printLine();

    for (int i = 0; i < (sizeof(registerNames) / sizeof(registerNames[0])); i++) {
        print(registerNames[i]);
        print(": ");
        printRegisterFormat(regdata[i]);
        print(i % 4 == 3 ? "\n" : "    ");
    }

    if (pid == 0) {
        print("\nPress ENTER to restart the shell.");
        clearKeyboard();
        do {
            hlt(); 
            cli();
        } while (getChar() != '\n');

        kill(pid);
        clearScreen();
        initializeShell();
    } else {
        kill(pid);
    }

    yield();
}