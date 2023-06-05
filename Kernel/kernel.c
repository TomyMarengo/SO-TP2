#include <defs.h>
#include <kernel.h>
#include <graphics.h>
#include <idtLoader.h>
#include <interrupts.h>
#include <kernel.h>
#include <keyboard.h>
#include <memoryManager.h>
#include <lib.h>
#include <moduleLoader.h>
#include <process.h>
#include <scheduler.h>
#include <sem.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;
static void *const sampleCodeModuleAddress = (void *) 0x400000;
static void *const sampleDataModuleAddress = (void *) 0x500000;
static void *const startHeapAddres = (void *) 0xF00000;
static void *const endHeapAddres = (void *) 0x2000000;

void
clearBSS(void *bssAddress, uint64_t bssSize) {
    memset(bssAddress, 0, bssSize);
}

void *
getStackBase() {
    return (void *) ((uint64_t) &endOfKernel + PageSize * 8 - sizeof(uint64_t));
}

void *
initializeKernelBinary() {
    void *moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};

    loadModules(&endOfKernelBinary, moduleAddresses);

    clearBSS(&bss, &endOfKernel - &bss);

    return getStackBase();
}

void
initializeShell() {
    ProcessCreateInfo shellInfo = {"shell", (ProcessStart) sampleCodeModuleAddress, 1, PRIORITY_MAX, 0, NULL};
    Pid pid = createProcess(&shellInfo);

    addFdKeyboard(pid, STDIN);
    addFdScreen(pid, STDOUT, &WHITE);
    addFdScreen(pid, STDERR, &RED);
}

int
main() {

    // Disable interrupts
    cli();

    loadIDT();
    initializeScreen();
    initializeMemory(startHeapAddres, (size_t) (endHeapAddres - startHeapAddres));
    initializeKeyboard();
    initializeScheduler();
    initializeSem();

    initializeShell();

    // Enable interrupts
    sti();

    while (1) {
        yield();
        hlt();
    }

    return 0;
}