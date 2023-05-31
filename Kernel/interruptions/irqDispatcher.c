#include <stdint.h>
#include <time.h>
#include <keyboard.h>

typedef void (*VoidFunction)(void);

static VoidFunction interruptions[255] = {&rtcInterruptHandler, &kbdInterruptHandler};

void irqDispatcher(uint64_t irq) {
    VoidFunction interruption = interruptions[irq];
    if (interruption != 0) {
        interruption();
    }
}