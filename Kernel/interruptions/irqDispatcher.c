#include <stdint.h>
#include <time.h>
#include <keyboard.h>

typedef void (*VoidFunction)(void);

static VoidFunction interruptions[255] = {&interruptHandlerRTC, &interruptHandlerKeyboard};

void irqDispatcher(uint64_t irq) {
    VoidFunction intFunction;
    if (irq < (sizeof(interruptions) / sizeof(interruptions[0])) && (intFunction = interruptions[irq]) != NULL) {
        intFunction();
    }
}