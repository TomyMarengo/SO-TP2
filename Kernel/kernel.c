#include <idtLoader.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <moduleLoader.h>
#include <video.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <scheduler.h>
#include <keyboard.h>
#include <kernel.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const userCodeModuleAddress = (void *) 0x400000;
static void *const userDataModuleAddress = (void *) 0x500000;
static void *const startHeapAddress = (void *) 0xF00000;
static void *const endHeapAddress = (void *) 0x2000000;

void
clearBSS(void *bssAddress, uint64_t bssSize) {
    memset(bssAddress, 0, bssSize);
}

void *
getStackBase() {
    return (void *) ((uint64_t) &endOfKernel + PageSize * 8  // The size of the stack itself, 32KiB
                     - sizeof(uint64_t)                      // Begin at the top of the stack
    );
}

void *
initializeKernelBinary() {
    void *moduleAddresses[] = {userCodeModuleAddress, userDataModuleAddress};

    loadModules(&endOfKernelBinary, moduleAddresses);
    clearBSS(&bss, &endOfKernel - &bss);
    return getStackBase();
}

void initializeShell(){
    Color gray = {0x90, 0x90, 0x90};
    Color red = {0x00, 0x00, 0xFF};
    const char* args[] = {NULL};
    Pid pid = prc_create((ProcessStart)userCodeModuleAddress, 0, args);

    kbd_addFd(pid, STDIN);
    scr_addFd(pid, STDOUT, &gray);
    scr_addFd(pid, STDERR, &red);
}

int
main() {
    _cli();

    load_idt();
    
    mm_init(startHeapAddress, (size_t) (endHeapAddress - startHeapAddress));

    sch_init();

    initializeShell();

    _sti();

    while (1)
        sch_yield();
        _hlt(); 
    return 0;

}
