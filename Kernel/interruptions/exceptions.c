#include <graphics.h>
#include <keyboard.h>
#include <exceptions.h>
#include <scheduler.h>
#include <process.h>
#include <kernel.h>
#include <interrupts.h>
#include <defs.h>

typedef void (*Exception)(void);

static void zeroDivision();
static void invalidOpcode();
static void generalProtection();
static void pageFault();
static void exceptionHandler(int exception, const char* msg);

static Exception exceptions[] = {
    /* 0x00 */ &zeroDivision, 0, 0, 0, 0, 0,
    /* 0x06 */ &invalidOpcode, 0, 0, 0, 0, 0, 0,
    /* 0x0D */ &generalProtection,
    /* 0x0E */ &pageFault
};

void exceptionDispatcher(int exception) {
    Exception ex = exceptions[exception];
    if (ex == 0) {
        exceptionHandler(exception, "Unknown exception");
    } else {
        ex();
    }
}

static void exceptionHandler(int exception, const char* msg) {
    Pid pid = getpid();
    if (isForeground(pid)) {
        print(msg);
        print(" (0x");
        printDec(exception);
        printChar(')');
        printLine();
        print("Presione enter para continuar");

        clearKeyboard();
        int c;
        do {
            hlt(); // halts the central processing unit until the next external interrupt is fired.
        } while ((c = getChar()) != '\n');
        cli();
    }

    kill(pid);

    if (pid == 0) { // TO DO: check if shell PID is ALWAYS 0.
        clearScreen();
        initializeShell();
    }

    yield();
}

static void zeroDivision() {
    exceptionHandler(0x00, "Zero Division Exception");
}

static void invalidOpcode() {
    exceptionHandler(0x06, "Invalid Opcode Exception");
}

static void generalProtection() {
    exceptionHandler(0x0D, "General Protection Exception");
}

static void pageFault() {
    exceptionHandler(0x0E, "Page Fault Exception");
}