#include <stdint.h>
#include <idtLoader.h>
#include <interrupts.h>
#include <defs.h>

#pragma pack(push)		/* Push de la alineación actual */
#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de interrupcion */
typedef struct {
	uint16_t offset_l;
	uint16_t selector;
	uint8_t cero;
	uint8_t access;
	uint16_t offset_m;
	uint32_t offset_h;
	uint32_t other_cero;
} DESCR_INT;

#pragma pack(pop)		/* Reestablece la alinceación actual */

DESCR_INT * idt = (DESCR_INT *) 0;	// IDT de 255 entradas

extern void timerIntRoutine(void);
extern void keyboardIntRoutine(void);
extern void divideByZeroIntRoutine(void);
extern void invalidOpcodeIntRoutine(void);
extern void generalprotIntRoutine(void);
extern void pagefaultIntRoutine(void);
extern void syscallIntRoutine(void);

static void setup_IDT_entry (int index, uint64_t offset);

void load_idt(void) {
	_cli();

    // Setup exception interrupts
	setup_IDT_entry(0x00, (uint64_t)&divideByZeroIntRoutine);
	setup_IDT_entry(0x06, (uint64_t)&invalidOpcodeIntRoutine);
	setup_IDT_entry(0x0D, (uint64_t)&generalprotIntRoutine);
	setup_IDT_entry(0x0E, (uint64_t)&pagefaultIntRoutine);

    // Setup hardware interrupts
	setup_IDT_entry(0x20, (uint64_t)&timerIntRoutine);
	setup_IDT_entry(0x21, (uint64_t)&keyboardIntRoutine);
    
    // Setup syscall interrupt
	setup_IDT_entry(0x80, (uint64_t)&syscallIntRoutine);
	
	// Enable timer tick and keyboard interruptions
	picMasterMask(0b11111100); 
	picSlaveMask(0b11111111);
	
	_sti();
}

static void setup_IDT_entry (int index, uint64_t offset) {
	idt[index].offset_l = offset & 0xFFFF;
	idt[index].selector = 0x08;
	idt[index].cero = 0;
	idt[index].access = ACS_INT;
	idt[index].offset_m = (offset >> 16) & 0xFFFF;
	idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
	idt[index].other_cero = (uint64_t) 0;
}
