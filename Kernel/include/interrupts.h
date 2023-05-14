#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <stdint.h>

void _cli(void);
void _sti(void);
void _hlt(void);

void haltcpu(void);

void picMasterMask(uint8_t mask);
void picSlaveMask(uint8_t mask);

#endif