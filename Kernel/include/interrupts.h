#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <stdint.h>

/**
 * @brief Clear Interrupt Flag.
 */
void cli(void);

/**
 * @brief Set Interrupt Flag.
 */
void sti(void);

/**
 * @brief Halts the central processing unit (CPU) until the next external interrupt is fired.
 */
void hlt(void);

/**
 * @brief Clear Interrupt Flag and Halt.
 */
void haltCPU(void);

/**
 * @brief Pic Master Mask.
 */
void picMasterMask(uint8_t mask);

/**
 * @brief Pic Slave Mask.
 */
void picSlaveMask(uint8_t mask);

/* --- Interrupt and Exception Handlers --- */

void irq00Handler(void);
void irq01Handler(void);
void exception0Handler(void);
void exception6Handler(void);
void exception0DHandler(void);
void exception0EHandler(void);

/* --- Scheduler Handlers --- */

void awakeScheduler(void);
void int81(void);

/* --- Syscall Handlers --- */

void syscallHandler(void);

#endif
