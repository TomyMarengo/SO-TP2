#include <stdint.h>
#include <idtLoader.h>
#include <defs.h>
#include <interrupts.h>

#pragma pack(push) // Current lineup push
#pragma pack(1)    // Align the following structures to 1 byte

// Interrupt descriptor
typedef struct {
    uint16_t offset_l, selector;
    uint8_t cero, access;
    uint16_t offset_m;
    uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop) // Reset the current alignment

DESCR_INT* idt = (DESCR_INT*)0; // 255 entry IDT

static void setupIDTEntry(int index, uint64_t offset);

void loadIDT() {

    // Exceptions
    setupIDTEntry(0x00, (uint64_t)&exception0Handler);
    setupIDTEntry(0x06, (uint64_t)&exception6Handler);
    setupIDTEntry(0x0D, (uint64_t)&exception0DHandler);
    setupIDTEntry(0x0E, (uint64_t)&exception0EHandler);

    // Hardware Interrupts
    setupIDTEntry(0x20, (uint64_t)&irq00Handler); // timer tick
    setupIDTEntry(0x21, (uint64_t)&irq01Handler); // keyboard

    // Software Interrupts
    setupIDTEntry(0x80, (uint64_t)&syscallHandler);
    setupIDTEntry(0x81, (uint64_t)&awakeScheduler);

    // 1111 1100 timer-tick and keyboard
    picMasterMask(0xFC);
    picSlaveMask(0xFF);

}

static void setupIDTEntry(int index, uint64_t offset) {
    idt[index].selector = 0x08;
    idt[index].offset_l = offset & 0xFFFF;
    idt[index].offset_m = (offset >> 16) & 0xFFFF;
    idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
    idt[index].access = ACS_INT;
    idt[index].cero = 0;
    idt[index].other_cero = (uint64_t)0;
}